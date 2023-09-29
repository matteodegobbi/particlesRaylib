#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "perlin.h"
#include "raylib.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HvEIGHT 1200
#define CELLS_PER_ROW 1200
#define CELLS_PER_COLUMN CELLS_PER_ROW
#define CELL_LENGTH (float)SCREEN_WIDTH / CELLS_PER_ROW

typedef struct Effect {
    Color color;
    int seed;
    bool directionVert;
    float freq;
    float depth;
    // Color** matrix;
    size_t nRows;
    size_t nColumns;
    float mix;
    float blockSize;
} Effect;
Effect effectArray[10];
Color arrayColors[] = {WHITE, GREEN, RED, SKYBLUE, GOLD};
// size_t gol[150][150];
/*float alphaLevels2[CELLS_PER_ROW][CELLS_PER_COLUMN];
float alphaLevels[CELLS_PER_ROW][CELLS_PER_COLUMN];
float redLevels[CELLS_PER_ROW][CELLS_PER_COLUMN];
float greenLevels[CELLS_PER_ROW][CELLS_PER_COLUMN];
float blueLevels[CELLS_PER_ROW][CELLS_PER_COLUMN];*/

Color colorMatrix1[CELLS_PER_ROW][CELLS_PER_COLUMN];
Color colorMatrix2[CELLS_PER_ROW][CELLS_PER_COLUMN];
Color blended[CELLS_PER_ROW][CELLS_PER_COLUMN];
Image screenImage;
Texture2D screenTexture;
#define alphaFreq 0.0055f
//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------

static void UpdateDrawFrame(float, long);  // Update and draw one frame
void screenshotWithTimestamp(void);
void generateInitialSmoke(void);
void updateSmoke(long);
void applyEffect(const Effect* e, long iteration, Color[1200][1200]);
//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
Effect effetto1 = {.color = { 102, 191, 255, 120 },
                   .blockSize = CELL_LENGTH,
                   .depth = 10,
                   .directionVert = true,
                   .freq = 0.01,
                   .mix = 1,
                   .nColumns = 1200,
                   .nRows = 1200,
                   .seed = 0};
Effect effetto2 = {.color = {.r=0,.g=0,.b=0,.a=255},
                   .blockSize = CELL_LENGTH,
                   .depth = 5,
                   .directionVert = false,
                   .freq = 0.001,
                   .mix = 2,
                   .nColumns = 1200,
                   .nRows = 1200,
                   .seed = 1};
int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = SCREEN_WIDTH;
    const int screenHeight = SCREEN_HEIGHT;
    InitWindow(screenWidth, screenHeight, "raylib");
    SetTargetFPS(169);
    // DisableCursor();
    //  Main game loop
    // generateInitialSmoke();
    /*effetto1.matrix = calloc(1200, sizeof(Color*));
    assert(effetto1.matrix);
    for (size_t i = 0; i < 1200; i++) {
        effetto1.matrix[i] = calloc(1200, sizeof(Color));
        assert(effetto1.matrix[i]);
    }*/
    screenImage = (Image){.data = blended,
                          .width = CELLS_PER_ROW,
                          .height = CELLS_PER_COLUMN,
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

// Update and draw game frame

static void UpdateDrawFrame(float delta_time, long iteration) {
    // Update
    if (IsKeyPressed(KEY_SPACE)) {
        screenshotWithTimestamp();
    }

    updateSmoke(iteration);
    // Draw
    //----------------------------------------------------------------------------------

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexture(screenTexture, 0, 0, RAYWHITE);

    /*for (size_t i = 0; i < CELLS_PER_ROW; i++) {
        for (size_t j = 0; j < CELLS_PER_COLUMN; j++) {
            // DrawRectangle(i * CELL_LENGTH, j * CELL_LENGTH, CELL_LENGTH, CELL_LENGTH,
            // arrayColors[gol[i][j]]);

            DrawRectangle(i * CELL_LENGTH, j * CELL_LENGTH, CELL_LENGTH, CELL_LENGTH, noiseRGB);
        }
    }*/
    /*for (float x = 0; x < SCREEN_WIDTH; x += ((float)SCREEN_WIDTH) / CELLS_PER_ROW) {
        DrawLine(x, 0, x, SCREEN_HEIGHT, GRAY);
    }
    for (float y = 0; y < SCREEN_HEIGHT; y += ((float)SCREEN_HEIGHT) / CELLS_PER_COLUMN) {
        DrawLine(0, y, SCREEN_WIDTH, y, GRAY);
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

/*void generateInitialSmoke() {
    seed_red = GetRandomValue(0, INT_MAX);
    seed_blue = GetRandomValue(0, INT_MAX);
    seed_green = GetRandomValue(0, INT_MAX);
    seed_alpha = GetRandomValue(0, INT_MAX);
    seed_alpha2 = GetRandomValue(0, INT_MAX);
    for (size_t i = 0; i < CELLS_PER_COLUMN; i++) {
        for (size_t j = 0; j < CELLS_PER_ROW; j++) {
            redLevels[i][j] = perlin2d(i * CELL_LENGTH, j * CELL_LENGTH, 0.005, 10, seed_red);
            greenLevels[i][j] = perlin2d(i * CELL_LENGTH, j * CELL_LENGTH, 0.005, 10, seed_green);
            blueLevels[i][j] = perlin2d(i * CELL_LENGTH, j * CELL_LENGTH, 0.005, 10, seed_blue);
            alphaLevels[i][j] = perlin2d((i)*CELL_LENGTH, (j)*CELL_LENGTH, alphaFreq, 10, seed_alpha);

            // greenLevels[i][j] = blueLevels[i][j] = redLevels[i][j];
            int alpha = (int)(alphaLevels[i][j] * 255);
            int red = (int)(redLevels[i][j] * 255);
            int green = (int)(greenLevels[i][j] * 255);
            int blue = (int)(blueLevels[i][j] * 255);
            Color noiseRGB = (Color){red, green, blue, alpha};
            colorMatrix[i][j] = noiseRGB;
        }
    }
    screenImage = (Image){.data = colorMatrix,
                          .width = CELLS_PER_ROW,
                          .height = CELLS_PER_COLUMN,
                          .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
                          .mipmaps = 1};

    screenTexture = LoadTextureFromImage(screenImage);
}*/
void updateSmoke(long iteration) {
    /*for (size_t i = 0; i < CELLS_PER_COLUMN - 1; i++) {
        for (size_t j = 0; j < CELLS_PER_ROW; j++) {
            redLevels[i][j] = redLevels[i + 1][j];
            greenLevels[i][j] = greenLevels[i + 1][j];
            blueLevels[i][j] = blueLevels[i + 1][j];
            alphaLevels[i][j] = alphaLevels[i + 1][j];

            int alpha = (int)(alphaLevels[i][j] * 255);
            int red = (int)(redLevels[i][j] * 255);
            int green = (int)(greenLevels[i][j] * 255);
            int blue = (int)(blueLevels[i][j] * 255);
            Color noiseRGB = (Color){red, green, blue, alpha};
            colorMatrix[i][j] = noiseRGB;
        }
    }

    for (size_t j = 0; j < CELLS_PER_ROW; j++) {
        redLevels[CELLS_PER_COLUMN - 1][j] =
            perlin2d((CELLS_PER_COLUMN - 1 + iteration) * CELL_LENGTH, (j)*CELL_LENGTH, 0.005, 10, seed_red);
        greenLevels[CELLS_PER_COLUMN - 1][j] = perlin2d((CELLS_PER_COLUMN - 1 + iteration) * CELL_LENGTH,
                                                        (j)*CELL_LENGTH, 0.005, 10, seed_green);
        blueLevels[CELLS_PER_COLUMN - 1][j] =
            perlin2d((CELLS_PER_COLUMN - 1 + iteration) * CELL_LENGTH, (j)*CELL_LENGTH, 0.005, 10, seed_blue);
        alphaLevels[CELLS_PER_COLUMN - 1][j] = perlin2d((CELLS_PER_COLUMN - 1 + iteration) * CELL_LENGTH,
                                                        (j)*CELL_LENGTH, alphaFreq, 10, seed_alpha);

        int alpha = (int)(alphaLevels[CELLS_PER_COLUMN - 1][j] * 255);
        int red = (int)(redLevels[CELLS_PER_COLUMN - 1][j] * 255);
        int green = (int)(greenLevels[CELLS_PER_COLUMN - 1][j] * 255);
        int blue = (int)(blueLevels[CELLS_PER_COLUMN - 1][j] * 255);
        Color noiseRGB = (Color){red, green, blue, alpha};
        colorMatrix[CELLS_PER_COLUMN - 1][j] = noiseRGB;
    }
    for (size_t i = 0; i < CELLS_PER_COLUMN; i++) {
        alphaLevels[i][CELLS_PER_ROW - 1] = perlin2d(
            i * CELL_LENGTH, (CELLS_PER_ROW - 1 + iteration) * CELL_LENGTH, alphaFreq, 10, seed_alpha);
        colorMatrix[i][CELLS_PER_ROW - 1].a = (int)(alphaLevels[i][CELLS_PER_ROW - 1] * 255);
    }
    for (size_t i = 0; i < CELLS_PER_COLUMN; i++) {
        for (size_t j = 0; j < CELLS_PER_ROW - 1; j++) {
            alphaLevels2[i][j] = alphaLevels2[i][j + 1];

            int alpha = (int)(alphaLevels2[i][j] * 255);
            colorMatrix[i][j].a += alpha * 0.2;
        }
    }
    for (size_t i = 0; i < CELLS_PER_COLUMN; i++) {
        alphaLevels2[i][CELLS_PER_ROW - 1] = perlin2d(
            i * CELL_LENGTH, (CELLS_PER_ROW - 1 + iteration) * CELL_LENGTH, alphaFreq, 10, seed_alpha2);
        colorMatrix[i][CELLS_PER_ROW - 1].a += (int)(alphaLevels2[i][CELLS_PER_ROW - 1] * 255) * 0.5;
    }*/
    applyEffect(&effetto1, iteration, colorMatrix1);
    applyEffect(&effetto2, iteration, colorMatrix2);
    for (size_t i = 0; i < CELLS_PER_COLUMN; i++) {
        for (size_t j = 0; j < CELLS_PER_ROW; j++) {
            blended[i][j] = (Color){
                .r = (effetto1.mix * colorMatrix1[i][j].r + effetto2.mix * colorMatrix2[i][j].r),
                .g = effetto1.mix * colorMatrix1[i][j].g + effetto2.mix * colorMatrix2[i][j].g,
                .b = effetto1.mix * colorMatrix1[i][j].b + effetto2.mix * colorMatrix2[i][j].b,
                .a = effetto1.mix * colorMatrix1[i][j].a + effetto2.mix * colorMatrix2[i][j].a,
            };
        }
    }

    UpdateTexture(screenTexture, blended);
}
void applyEffect(const Effect* e, long iteration, Color colorMatrix[1200][1200]) {
    if (e->directionVert) {
        for (size_t i = 0; i < e->nRows - 1; i++) {
            for (size_t j = 0; j < e->nColumns; j++) {
                colorMatrix[i][j] = colorMatrix[i + 1][j];
            }
        }
        for (size_t j = 0; j < CELLS_PER_ROW; j++) {
            float p = perlin2d((CELLS_PER_COLUMN - 1 + iteration) * CELL_LENGTH, j * CELL_LENGTH, e->freq,
                               e->depth, e->seed);

            int red = (int)(p * e->color.r);
            int green = (int)(p * e->color.g);
            int blue = (int)(p * e->color.b);
            int alpha = (int)(p * e->color.a);
            colorMatrix[CELLS_PER_COLUMN - 1][j] = (Color){red, green, blue, alpha};
        }
    } else {
        for (size_t i = 0; i < e->nRows; i++) {
            for (size_t j = 0; j < e->nColumns-1; j++) {
                colorMatrix[i][j] = colorMatrix[i][j+1];
            }
        }
        for (size_t i = 0; i < CELLS_PER_COLUMN; i++) {
            float p = perlin2d( i* CELL_LENGTH, (CELLS_PER_ROW - 1 + iteration) * CELL_LENGTH, e->freq,
                               e->depth, e->seed);

            int red = (int)(p * e->color.r);
            int green = (int)(p * e->color.g);
            int blue = (int)(p * e->color.b);
            int alpha = (int)(p * e->color.a);
            colorMatrix[i][CELLS_PER_ROW - 1] = (Color){red, green, blue, alpha};
        }
    }
}