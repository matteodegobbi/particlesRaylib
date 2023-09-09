#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "perlin.h"
#include "raylib.h"

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 1200
#define CELLS_PER_ROW 1200
#define CELLS_PER_COLUMN CELLS_PER_ROW
#define CELL_LENGTH (float)SCREEN_WIDTH / CELLS_PER_ROW
Color arrayColors[] = {WHITE, GREEN, RED, SKYBLUE, GOLD};
// size_t gol[150][150];
float alphaLevels[CELLS_PER_ROW][CELLS_PER_COLUMN];
float redLevels[CELLS_PER_ROW][CELLS_PER_COLUMN];
float greenLevels[CELLS_PER_ROW][CELLS_PER_COLUMN];
float blueLevels[CELLS_PER_ROW][CELLS_PER_COLUMN];

static void UpdateDrawFrame(float);  // Update and draw one frame

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = SCREEN_WIDTH;
    const int screenHeight = SCREEN_HEIGHT;
    InitWindow(screenWidth, screenHeight, "raylib");
    SetTargetFPS(169);
    // DisableCursor();
    //  Main game loop
    int seed_red = GetRandomValue(0, INT_MAX);
    int seed_blue = GetRandomValue(0, INT_MAX);
    int seed_green = GetRandomValue(0, INT_MAX);
    int seed_alpha = GetRandomValue(0, INT_MAX);
    for (size_t i = 0; i < CELLS_PER_ROW; i++) {
        for (size_t j = 0; j < CELLS_PER_COLUMN; j++) {
            // gol[i][j] = GetRandomValue(0, sizeof(arrayColors) / sizeof(arrayColors[0]));
            // gol[i][j] = (GetRandomValue(0, 1)&&GetRandomValue(0,1)&&GetRandomValue(0,1));
            float freq = 0.01;
            float depth = 1;
            float whitestreak = 0;

            redLevels[i][j] = perlin2d(i * CELL_LENGTH, j * CELL_LENGTH, freq, depth, seed_red) * 0.2 + 0.2;
            greenLevels[i][j] = perlin2d(i * CELL_LENGTH, j * CELL_LENGTH, freq, depth, seed_green) * 0.1;
            blueLevels[i][j] = perlin2d(i, j, freq, depth, seed_red) ;

            redLevels[i][j] = log2(blueLevels[i][j]*0.2);
            blueLevels[i][j] = log2(blueLevels[i][j]);

            /*float x, y;
            if (i > CELLS_PER_ROW / 2) {
                x = SCREEN_WIDTH - i * CELL_LENGTH;
            } else {
                x = i * CELL_LENGTH;
            }

            if (j > CELLS_PER_COLUMN / 2) {
                y = SCREEN_HEIGHT - j * CELL_LENGTH;
            } else {
                y = j * CELL_LENGTH;
            }
            float streak = perlin2d(i * CELL_LENGTH, j * CELL_LENGTH, freq, depth, seed_alpha)*0.35f ;
            float WeightedNearCenter = (1200.0f - (fabs(x - y))) / 800.0f;
            if ((WeightedNearCenter * streak) > 0.3) {
                greenLevels[i][j] = redLevels[i][j] = blueLevels[i][j];
            }
*/

            /*float whitebase = perlin2d(i*CELL_LENGTH,j*CELL_LENGTH,0.001,15,0)*0.6;
            blueLevels[i][j] = perlin2d(i, j, freq, depth, seed_blue)+whitebase;
            redLevels[i][j] = perlin2d(i, j, freq, depth, seed_blue)*0.55+whitebase;*/

            /*blueLevels[i][j] = perlin2d(i, j, freq, depth, seed_blue);
            redLevels[i][j] = blueLevels[i][j] * 0.55;*/

            /*greenLevels[i][j] = perlin2d(i, j, freq, depth, seed_green) * 0.1+whitebase;*/

            alphaLevels[i][j] = 1;  // perlin2d(i, j, freq, depth, seed_alpha)*0.1+0.9;
        }
    }

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
    if (IsKeyPressed(KEY_SPACE)) {
        Image screenshot = LoadImageFromScreen();  // Load image from screen buffer and (screenshot)
        char filename[500];
        char number1String[100];
        char number2String[100];
        srand(time(NULL));
        int number1 = rand();
        int number2 = time(NULL);

        strcpy(filename, "screenshots/screen");
        itoa(number1, number1String, 16);
        itoa(number2, number2String, 16);
        strcat(number2String, ".png");
        strcat(filename, number1String);
        strcat(filename, number2String);
        bool success = ExportImage(screenshot, filename);
        int x = 8;
    }

    // Draw
    //----------------------------------------------------------------------------------

    BeginDrawing();
    ClearBackground(RAYWHITE);
    for (size_t i = 0; i < CELLS_PER_ROW; i++) {
        for (size_t j = 0; j < CELLS_PER_COLUMN; j++) {
            // DrawRectangle(i * CELL_LENGTH, j * CELL_LENGTH, CELL_LENGTH, CELL_LENGTH,
            // arrayColors[gol[i][j]]);
            int alpha = (int)(alphaLevels[i][j] * 255);
            int red = (int)(redLevels[i][j] * 255);
            int green = (int)(greenLevels[i][j] * 255);
            int blue = (int)(blueLevels[i][j] * 255);
            Color noiseRGB = (Color){red, green, blue, alpha};
            DrawRectangle(i * CELL_LENGTH, j * CELL_LENGTH, CELL_LENGTH, CELL_LENGTH, noiseRGB);
        }
    }
    /*for (float x = 0; x < SCREEN_WIDTH; x += ((float)SCREEN_WIDTH) / CELLS_PER_ROW) {
        DrawLine(x, 0, x, SCREEN_HEIGHT, GRAY);
    }
    for (float y = 0; y < SCREEN_HEIGHT; y += ((float)SCREEN_HEIGHT) / CELLS_PER_COLUMN) {
        DrawLine(0, y, SCREEN_WIDTH, y, GRAY);
    }*/
    // DrawFPS(10, 10);

    EndDrawing();
    //----------------------------------------------------------------------------------
}
