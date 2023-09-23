// game of life.cpp
// by Adam Ullmann

#include "raylib.h"
#include <vector>

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
bool showRulesPanel = false;
int zoomedCellSize = 5;
int frameCounter = 0;
int updateEveryNFrames = 8;

// CA Rules
std::vector<int> sRules = { 2, 3 }; // Conway's Game of Life rules as the defualt
std::vector<int> bRules = { 3 };

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

void UpdateRulesPanel() {
    if (IsKeyPressed(KEY_ENTER)) {
        showRulesPanel = !showRulesPanel;
    }

    if (showRulesPanel) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int mouseX = GetMouseX();
            int mouseY = GetMouseY();
            // S Rules
            for (int i = 0; i < sRules.size(); i++) {
                if (CheckCollisionPointRec({ (float)mouseX, (float)mouseY }, { float(175 + i * 20), 40, 10, 20 })) {
                    sRules.erase(sRules.begin() + i); // delete the rule
                    break;
                }
                if (CheckCollisionPointRec({ (float)mouseX, (float)mouseY }, { float(175 + i * 20), 20, 20, 20 })) {
                    int newValue = (sRules[i] + 1) % 9; // 9 possible neighbor states
                    sRules[i] = newValue;
                    break;
                }
            }
            // B Rules
            for (int i = 0; i < bRules.size(); i++) {
                if (CheckCollisionPointRec({ (float)mouseX, (float)mouseY }, { float(175 + i * 20), 90, 10, 20 })) {
                    bRules.erase(bRules.begin() + i); // delete the rule
                    break;
                }
                if (CheckCollisionPointRec({ (float)mouseX, (float)mouseY }, { float(175 + i * 20), 70, 20, 20 })) {
                    int newValue = (bRules[i] + 1) % 9; // 9 possible neighbor states
                    bRules[i] = newValue;
                    break;
                }
            }
            if (CheckCollisionPointRec({ (float)mouseX, (float)mouseY }, { 360, 20, 20, 20 }) && sRules.size() < 9) {
                sRules.push_back(0);
            }
            else if (CheckCollisionPointRec({ (float)mouseX, (float)mouseY }, { 360, 70, 20, 20 }) && bRules.size() < 9) {
                bRules.push_back(0);
            }
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
            if (grid[i][j]) {
                bool survives = false;
                for (int k = 0; k < sRules.size(); k++) {
                    if (neighbors == sRules[k]) {
                        survives = true;
                        break;
                    }
                }
                gridBuffer[i][j] = survives;
            }
            else {
                bool reproduces = false;
                for (int k = 0; k < bRules.size(); k++) {
                    if (neighbors == bRules[k]) {
                        reproduces = true;
                        break;
                    }
                }
                gridBuffer[i][j] = reproduces;
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

void DrawRulesPanel() {
    DrawRectangle(10, 10, 380, 110, GRAY);
    DrawRectangleLines(10, 10, 380, 110, DARKGRAY);
    DrawText("Survive Rules:", 20, 20, 20, BLACK);
    DrawText("Birth Rules:", 20, 70, 20, BLACK);

    // S rules input
    for (int i = 0; i < sRules.size(); i++) {
        char ruleText[8];
        sprintf_s(ruleText, "%d", sRules[i]);
        DrawText(ruleText, 175 + i * 20, 20, 20, BLACK);
        DrawText("-", 175 + i * 20, 40, 30, RED);
    }

    // B rules input
    for (int i = 0; i < bRules.size(); i++) {
        char ruleText[8];
        sprintf_s(ruleText, "%d", bRules[i]);
        DrawText(ruleText, 175 + i * 20, 70, 20, BLACK);
        DrawText("-", 175 + i * 20, 90, 30, RED);
    }

    DrawText("+", 360, 20, 20, DARKGRAY);
    DrawText("+", 360, 70, 20, DARKGRAY);
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
    if (showRulesPanel) {
        DrawRulesPanel();
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

    UpdateRulesPanel();
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
    InitWindow(screenWidth, screenHeight, "Game of Life++");
    SetTargetFPS(480);
    runSim();
    CloseWindow();
    return 0;
}
