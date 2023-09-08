#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(float);  // Update and draw one frame

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 100;
    const int screenHeight = 100;
    InitWindow(screenWidth, screenHeight, "raylib");

    // DisableCursor();
    //  Main game loop
    while (!WindowShouldClose()) {  // Detect window close button or ESC key
        double delta_time = GetFrameTime();
        UpdateDrawFrame(delta_time);
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}
// Update and draw game frame
static void UpdateDrawFrame(float delta_time) {
    // Update

    // Draw
    //----------------------------------------------------------------------------------

    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawFPS(10, 10);

    EndDrawing();
    //----------------------------------------------------------------------------------
}
