//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h>
#include <math.h>
//------------------------------------------------------------------------
#include "app\app.h"
#include "Gameplay/TowerDefenseArena.h"
//------------------------------------------------------------------------

TowerDefenseArena* arena;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
    Float3 map[11]{
        Float3(10, 11),
        Float3(10, 8),
        Float3(6, 8),
        Float3(6, 5),
        Float3(4, 5),
        Float3(4, 10),
        Float3(1, 10),
        Float3(1, 3),
        Float3(10, 3),
        Float3(10, 1),
        Float3(1, 1)
    };
    arena = new TowerDefenseArena(12, 12, map);
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
    deltaTime /= 1000.0f;
    
    arena->Update(deltaTime);
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
    arena->Render();
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
}
