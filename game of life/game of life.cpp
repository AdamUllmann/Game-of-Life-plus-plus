// game of life.cpp
// by Adam Ullmann

#include "raylib.h"
#include <math.h>
#include <iostream>

const int screenWidth = 1700;
const int screenHeight = 900;
const int cellSize = 5;
const int gridWidth = screenWidth / cellSize;
const int gridHeight = screenHeight / cellSize;

bool grid[gridWidth][gridHeight] = { 0 };
bool gridBuffer[gridWidth][gridHeight] = { 0 };

void InitGrid() {
    for (int i = 0; i < gridWidth; i++) {
        for (int j = 0; j < gridHeight; j++) {
            grid[i][j] = (GetRandomValue(0, 1) == 1);
        }
    }
}

void UpdateGrid() {
    for (int i = 0; i < gridWidth; i++) {
        for (int j = 0; j < gridHeight; j++) {
            int neighbors = 0;
            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    if (x == 0 && y == 0) {
                        continue;
                    }
                    int posX = i + x;
                    int posY = j + y;
                    if (posX >= 0 && posX < gridWidth && posY >= 0 && posY < gridHeight) {
                        neighbors += grid[posX][posY];
                    }
                }
            }
            if (grid[i][j] == true) {
                if (neighbors < 2 || neighbors > 3) {
                    gridBuffer[i][j] = false;
                }
                else {
                    gridBuffer[i][j] = true;
                }
            }
            else {
                if (neighbors == 3) {
                    gridBuffer[i][j] = true;
                }
                else {
                    gridBuffer[i][j] = false;
                }
            }
        }
    }

    // copy buffer back to the grid
    for (int i = 0; i < gridWidth; i++) {
        for (int j = 0; j < gridHeight; j++) {
            grid[i][j] = gridBuffer[i][j];
        }
    }
}

void DrawGrid() {
    for (int i = 0; i < gridWidth; i++) {
        for (int j = 0; j < gridHeight; j++) {
            if (grid[i][j]) {
                DrawRectangle(i * cellSize, j * cellSize, cellSize, cellSize, WHITE);
            }
        }
    }
}

int main()
{
    InitWindow(screenWidth, screenHeight, "Game of Life");
    SetTargetFPS(30);

    InitGrid();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        UpdateGrid();
        DrawGrid();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
