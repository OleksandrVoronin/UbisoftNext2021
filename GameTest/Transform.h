#pragma once
#include <dsound.h>
#include <vector>

#include "Math/Float3.h"
#include "Math/Matrix3.h"

class Transform
{
public:
    Transform() : Transform(Float3(0, 0, 0), nullptr)
    {
    }

    Transform(Float3 position) : Transform(position, nullptr)
    {
    }

    Transform(Float3 position, Transform* parent) : children(0), localPosition{position}
    {
        SetParent(parent);
        Transform::RecalculateTransform();
        RecalculateWorldPRS();
    }

    virtual ~Transform() = default;

    Float3 GetWorldPosition() const
    {
        return worldPosition;
    }

    Float3 GetForward() const
    {
        return forward;
    }

    Float3 GetUp() const
    {
        return up;
    }

    Float3 GetRight() const
    {
        return right;
    }

    void UpdateTransform()
    {
        if (isDirtySelf)
        {
            RecalculateTransform();
            RecalculateWorldPRS();
        }
        else if (isDirtyHierarchy)
        {
            RecalculateWorldPRS();
        }

        for (int i = 0; i < children.size(); i++)
        {
            children[i]->UpdateTransform();
        }
    }

    void SetParent(Transform* parent)
    {
        this->parent = parent;

        if (this->parent != nullptr)
        {
            this->parent->children.push_back(this);
        }

        DirtySelf();
    }

    Transform* GetParent() const
    {
        return this->parent;
    }

    size_t ChildCount() const
    {
        return children.size();
    }

    Transform* GetChild(int i)
    {
        return children[i];
    }

    void SetScale(float scaleUniform)
    {
        localScale.x = scaleUniform;
        localScale.y = scaleUniform;
        localScale.z = scaleUniform;

        DirtySelf();
    }

    void RotateAbsolute(float newX, float newY, float newZ)
    {
        localRotation.x = newX;
        localRotation.y = newY;
        localRotation.z = newZ;

        DirtySelf();
    }

    void RotateBy(float byX, float byY, float byZ)
    {
        localRotation.x += byX;
        localRotation.y += byY;
        localRotation.z += byZ;

        DirtySelf();
    }

    void MoveToLocal(Float3 localPosition)
    {
        this->localPosition = localPosition;

        DirtySelf();
    }

    void MoveByLocal(Float3 localOffset)
    {
        this->localPosition = this->localPosition + localOffset;

        DirtySelf();
    }

protected:
    bool isDirtySelf = false;
    bool isDirtyHierarchy = false;

    Transform* parent = nullptr;
    std::vector<Transform*> children;

    // local
    Float3 localPosition{0, 0, 0};
    Float3 localRotation{0, 0, 0}; // euler
    Float3 localScale{1, 1, 1};

    Matrix3 localTransformationMatrix;
    Matrix3 localTransformationMatrixInverse;

    // world cached, skipping world rotation because I'm not sure what world rotation is supposed to mean..
    Float3 worldPosition{0, 0, 0};
    Float3 worldScale{0, 0, 0};

    Matrix3 globalTransformationMatrixInverse;

    // cached helpers
    Float3 forward{0, 0, 1};
    Float3 up{0, 1, 0};
    Float3 right{1, 0, 0};

    void DirtySelf()
    {
        isDirtySelf = true;
        DirtyChildren();
    }

    void DirtyChildren()
    {
        for (int i = 0; i < children.size(); i++)
        {
            children[i]->isDirtyHierarchy = true;
            children[i]->DirtyChildren();
        }
    }

    virtual void RecalculateTransform()
    {
        localTransformationMatrix = Matrix3::CombinedRotationMatrix(localRotation.x, localRotation.y, localRotation.z);

        forward = Float3(localTransformationMatrix[2][0], localTransformationMatrix[2][1],
                         localTransformationMatrix[2][2]);
        up = Float3(localTransformationMatrix[1][0], localTransformationMatrix[1][1], localTransformationMatrix[1][2]);
        right = Float3(localTransformationMatrix[0][0], localTransformationMatrix[0][1],
                       localTransformationMatrix[0][2]);

        localTransformationMatrixInverse = localTransformationMatrix.Inverse();

        isDirtySelf = false;
    }

    void RecalculateWorldPRS()
    {
        globalTransformationMatrixInverse = CalculateCombinedTransformationMatrixInverse();
        worldPosition = CalculateWorldPosition();
        worldScale = CalculateHierarchyScale();

        isDirtyHierarchy = false;
    }

    Float3 CalculateWorldPosition() const
    {
        if (parent == nullptr)
        {
            return localPosition;
        }

        return parent->worldPosition + parent->globalTransformationMatrixInverse * (localPosition * parent->worldScale);
    }

    Float3 CalculateHierarchyScale() const
    {
        if (parent == nullptr)
        {
            return localScale;
        }

        return localScale * parent->CalculateHierarchyScale();
    }

    Matrix3 CalculateCombinedTransformationMatrixInverse() const
    {
        if (parent == nullptr)
        {
            return localTransformationMatrixInverse;
        }

        return parent->globalTransformationMatrixInverse * localTransformationMatrixInverse;
    }
};
