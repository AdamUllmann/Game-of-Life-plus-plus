// game of life.cpp
// by Adam Ullmann

#include "raylib.h"

// Constants
const int screenWidth = 1280;
const int screenHeight = 720;
const int cellSize = 5;
const int gridWidth = screenWidth / cellSize;
const int gridHeight = screenHeight / cellSize;

// Global Variables
bool grid[gridWidth][gridHeight] = { 0 };
bool gridBuffer[gridWidth][gridHeight] = { 0 };
bool gamePaused = true;
int zoomedCellSize = 5;
int frameCounter = 0;
int updateEveryNFrames = 8;

void randomizeGrid() {
    for (int i = 0; i < gridWidth; i++) {
        for (int j = 0; j < gridHeight; j++) {
            grid[i][j] = (GetRandomValue(0, 1) == 1);
        }
    }
}

void clearGrid() {
    for (int i = 0; i < gridWidth; i++) {
        for (int j = 0; j < gridHeight; j++) {
            grid[i][j] = 0;
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

void DrawSim() {
    BeginDrawing();
    ClearBackground(BLACK);

    for (int i = 0; i < gridWidth; i++) {
        for (int j = 0; j < gridHeight; j++) {
            if (grid[i][j]) {
                DrawRectangle(i * zoomedCellSize, j * zoomedCellSize, zoomedCellSize, zoomedCellSize, WHITE);
            }
        }
    }
    if (gamePaused) {
        for (int i = 0; i <= gridWidth; i++) {
            DrawLine(i * zoomedCellSize, 0, i * zoomedCellSize, screenHeight, { 35,35,35,255 });
        }
        for (int j = 0; j <= gridHeight; j++) {
            DrawLine(0, j * zoomedCellSize, screenWidth, j * zoomedCellSize, { 35,35,35,255 });
        }
    }
    EndDrawing();
}

void controls() {
    if (gamePaused) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            int mouseX = GetMouseX() / zoomedCellSize;
            int mouseY = GetMouseY() / zoomedCellSize;
            grid[mouseX][mouseY] = true;
        }
        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
            int mouseX = GetMouseX() / zoomedCellSize;
            int mouseY = GetMouseY() / zoomedCellSize;
            grid[mouseX][mouseY] = false;
        }
    }
    int mouseWheelMove = GetMouseWheelMove();
    if (mouseWheelMove != 0) {
        zoomedCellSize += mouseWheelMove;
        if (zoomedCellSize < 5) zoomedCellSize = 5;
        if (zoomedCellSize > 50) zoomedCellSize = 50;
    }
    if (IsKeyPressed(KEY_SPACE)) {
        gamePaused = !gamePaused;
    }
    if (IsKeyPressed(KEY_UP) && updateEveryNFrames > 1) {   // frame control
        updateEveryNFrames--; 
    }
    if (IsKeyPressed(KEY_DOWN)) {
        updateEveryNFrames++;
    }

    if (IsKeyPressed(KEY_R)) randomizeGrid();
    if (IsKeyPressed(KEY_C)) clearGrid();
}

void runSim() {
    while (!WindowShouldClose()) {
        controls();
           frameCounter++;
        if (!gamePaused && frameCounter % updateEveryNFrames == 0) {
            UpdateGrid();
        }
        DrawSim();
    }
}

int main() {
    InitWindow(screenWidth, screenHeight, "Game of Life");
    SetTargetFPS(480);
    runSim();
    CloseWindow();
    return 0;
}
