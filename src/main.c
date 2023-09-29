#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "perlin.h"
#include "raylib.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000
#define CELLS_PER_ROW 75
#define CELLS_PER_COLUMN 75
#define CELL_LENGTH SCREEN_WIDTH / CELLS_PER_ROW
#define CELL_HEIGHT SCREEN_HEIGHT / CELLS_PER_COLUMN

// Color arrayColors[] = {WHITE, GREEN, RED, SKYBLUE, GOLD};
Color arrayColors[] = {GOLD};

// Color cellMatrix[CELLS_PER_ROW][CELLS_PER_COLUMN];
bool cellMatrix[CELLS_PER_ROW][CELLS_PER_COLUMN];
bool newCellMatrix[CELLS_PER_ROW][CELLS_PER_COLUMN];
Color rawPixels[SCREEN_WIDTH][SCREEN_HEIGHT];
Image screenImage;
Texture2D screenTexture;
//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------

static void UpdateDrawFrame(float, long);  // Update and draw one frame
void screenshotWithTimestamp(void);
short countAliveNeighborsConway(size_t, size_t);
void updateConway(void);

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = SCREEN_WIDTH;
    const int screenHeight = SCREEN_HEIGHT;
    InitWindow(screenWidth, screenHeight, "raylib");
    SetTargetFPS(10);
    for (size_t i = 0; i < CELLS_PER_ROW; i++) {
        for (size_t j = 0; j < CELLS_PER_COLUMN; j++) {
            /*Color c = GetRandomValue(0, 1) == 1
                          ? BLACK
                          : arrayColors[GetRandomValue(0, sizeof(arrayColors) / sizeof(arrayColors[0]))];*/
            cellMatrix[i][j] = perlin2d(i * CELL_LENGTH, j * CELL_HEIGHT, 0.02, 1, time(NULL)) < 0.2;

            for (size_t k = i * CELL_LENGTH; k < (i + 1) * CELL_LENGTH; k++) {
                for (size_t l = j * CELL_HEIGHT; l < (j + 1) * CELL_HEIGHT; l++) {
                    rawPixels[k][l] = cellMatrix[i][j] ? GOLD : BLACK;
                }
            }
        }
    }

    screenImage = (Image){.data = rawPixels,
                          .width = SCREEN_WIDTH,
                          .height = SCREEN_HEIGHT,
                          .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
                          .mipmaps = 1};

    screenTexture = LoadTextureFromImage(screenImage);
    long iteration = 1;
    while (!WindowShouldClose()) {  // Detect window close button or ESC key
        double delta_time = GetFrameTime();
        UpdateDrawFrame(delta_time, iteration++);
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}

inline short countAliveNeighborsConway(size_t i, size_t j) {
    return cellMatrix[i - 1][j - 1] + cellMatrix[i - 1][j] + cellMatrix[i - 1][j + 1] + cellMatrix[i][j - 1] +
           cellMatrix[i][j + 1] + cellMatrix[i + 1][j - 1] + cellMatrix[i + 1][j] + cellMatrix[i + 1][j + 1];
}
void updateConway() {
    for (size_t i = 1; i < CELLS_PER_ROW - 1; i++) {
        for (size_t j = 1; j < CELLS_PER_COLUMN - 1; j++) {
            short aliveNeigh = countAliveNeighborsConway(i, j);
            if (cellMatrix[i][j]) {
                if (aliveNeigh < 2 || aliveNeigh > 3) {
                    newCellMatrix[i][j] = false;
                }
            } else if (aliveNeigh == 3) {
                newCellMatrix[i][j] = true;
            }
        }
    }

    for (size_t i = 1; i < CELLS_PER_ROW - 1; i++) {
        for (size_t j = 1; j < CELLS_PER_COLUMN - 1; j++) {
            cellMatrix[i][j] = newCellMatrix[i][j];
            for (size_t k = i * CELL_LENGTH; k < (i + 1) * CELL_LENGTH; k++) {
                for (size_t l = j * CELL_HEIGHT; l < (j + 1) * CELL_HEIGHT; l++) {
                    rawPixels[k][l] = cellMatrix[i][j] ? GOLD : BLACK;
                }
            }
        }
    }
}
// Update and draw game frame

static void UpdateDrawFrame(float delta_time, long iteration) {
    // Update
    if (IsKeyPressed(KEY_SPACE)) {
        screenshotWithTimestamp();
    }
    updateConway();
    UpdateTexture(screenTexture, rawPixels);
    // Draw
    //----------------------------------------------------------------------------------

    BeginDrawing();
    ClearBackground(BLACK);

    DrawTexture(screenTexture, 0, 0, RAYWHITE);
    /*float lineThickness = 1.0f;
    for (float x = 0; x < SCREEN_WIDTH; x += CELL_LENGTH) {
        DrawLineEx((Vector2){x, 0}, (Vector2){x, SCREEN_HEIGHT}, lineThickness, GRAY);
    }
    for (float y = 0; y < SCREEN_HEIGHT; y += CELL_HEIGHT) {
        DrawLineEx((Vector2){0, y}, (Vector2){SCREEN_WIDTH, y}, lineThickness, GRAY);
    }*/
    DrawFPS(10, 10);

    EndDrawing();
    //----------------------------------------------------------------------------------
}

void screenshotWithTimestamp() {
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
}
