#pragma once
#include <dsound.h>
#include <string>


#include "../Gameplay/Buildings/IBuilding.h"
#include "../Math/Float3.h"

class UIButton
{
public:
    virtual ~UIButton() = default;

    enum ButtonState
    {
        Enabled = 0,
        Disabled = 1,
        Hovered = 2,
        Selected = 3
    };

    UIButton(Float3 position, Float3 size) : position{position}, size{size}
    {
    }

    ButtonState GetState() const
    {
        return buttonState;
    }

    void SetState(ButtonState newState)
    {
        buttonState = newState;
    }

    virtual void Render()
    {
        // Render borders
        const Float3* borderColor;
        float yOffset = 0;

        switch (buttonState)
        {
        case Disabled:
            borderColor = &borderColorDisabled;
            break;
        case Hovered:
            yOffset = size.y / 40;
        case Selected:
            borderColor = &borderColorSelected;
            break;
        default:
            borderColor = &borderColorNormal;
            break;
        }

        App::DrawLine(position.x - size.x / 2, position.y - size.y / 2 + yOffset,
                      position.x - size.x / 2, position.y + size.y / 2 + yOffset,
                      borderColor->x, borderColor->y, borderColor->z);
        App::DrawLine(position.x - size.x / 2, position.y + size.y / 2 + yOffset,
                      position.x + size.x / 2, position.y + size.y / 2 + yOffset,
                      borderColor->x, borderColor->y, borderColor->z);
        App::DrawLine(position.x + size.x / 2, position.y + size.y / 2 + yOffset,
                      position.x + size.x / 2, position.y - size.y / 2 + yOffset,
                      borderColor->x, borderColor->y, borderColor->z);
        App::DrawLine(position.x + size.x / 2, position.y - size.y / 2 + yOffset,
                      position.x - size.x / 2, position.y - size.y / 2 + yOffset,
                      borderColor->x, borderColor->y, borderColor->z);
    }

    boolean PixelInBounds(Float3 pixelPosition) const
    {
        return pixelPosition.x > position.x - size.y / 2 && pixelPosition.x < position.x + size.x / 2
            && pixelPosition.y > position.y - size.y / 2 && pixelPosition.y < position.y + size.y / 2;
    }

protected:
    const Float3 borderColorNormal{0, 0, 0};
    const Float3 borderColorSelected{0.102f, 0.553f, 0.561f};
    const Float3 borderColorDisabled{0.821f, 0.372f, 0.372f};

    Float3 position;
    Float3 size;

    ButtonState buttonState{Enabled};
};

class BuildingButton : public UIButton
{
public:
    BuildingButton(const Float3& position, const Float3& size, float renderScale)
        : UIButton(position, size), renderScale{renderScale}
    {
    }

    virtual IBuilding* GetBuilding() = 0;
    virtual IBuilding* GetPrototype() = 0;

    virtual void RenderPreviews(LineRenderer* lineRenderer, Camera* camera) = 0;
private:
    float renderScale = 1.f;
};

// Templated build button that will get cost and previews from type prototype
template <typename T = IBuilding>
class TypedBuildButton : public BuildingButton
{
public:
    TypedBuildButton(const Float3& position, const Float3& size, float renderScale)
        : BuildingButton(position, size, renderScale)
    {
        prototype = TypedBuildButton::GetBuilding();
        prototype->SetRenderScale(renderScale);
        prototype->Update(0);
    }

    ~TypedBuildButton()
    {
        delete prototype;
    }

    T* GetPrototype() override
    {
        return prototype;
    }

    T* GetBuilding() override
    {
        return new T();
    }

    void Render() override
    {
        UIButton::Render();

        const Float3* textColor = &textColorNormal;
        textBuffer = "Cost: " + std::to_string(prototype->GetCost());

        if (buttonState == Disabled)
        {
            textColor = &textColorDisabled;
        }
        App::Print(position.x - size.x / 3.5f, position.y - size.y / 2.6f, textBuffer.c_str(),
                   textColor->x, textColor->y, textColor->z,
                   GLUT_BITMAP_HELVETICA_18);
    }

    void RenderPreviews(LineRenderer* lineRenderer, Camera* camera) override
    {
        const Ray3 screenSpaceRay = camera->GetRayFromUV(position.x / static_cast<float>(WINDOW_WIDTH),
                                                         position.y / static_cast<float>(WINDOW_HEIGHT));
        prototype->MoveToLocal(screenSpaceRay.origin + screenSpaceRay.direction * 55);
        prototype->UpdateTransform();

        prototype->Render(lineRenderer, camera);
    }

private:
    const Float3 textColorNormal{0, 0, 0};
    const Float3 textColorDisabled{0.66f, 0, 0};

    T* prototype;
    std::string textBuffer;
};
