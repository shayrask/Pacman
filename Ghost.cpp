#include "Ghost.h"
#include "GameBoard.h"
#include <iostream>

using namespace std;

Ghost::Ghost() : ghostPosition{ 0, 0 }, R(255), G(255), B(255) {}
Ghost::~Ghost() {}

void Ghost::init(int row, int col, int R, int G, int B) {
    this->ghostPosition.row = row;
    this->ghostPosition.col = col;
    this->R = R;
    this->G = G;
    this->B = B;
}

void Ghost::renderGhost(int row, int col) {
    int cellSizeGX = GameBoard::getWidth() / GameBoard::getMazeCols();
    int cellSizeGY = GameBoard::getHeight() / GameBoard::getMazeRows();

    int pixelSizeGX = cellSizeGX / this->GHOST_SIZE;
    int pixelSizeGY = cellSizeGY / this->GHOST_SIZE;

    int (*ghostShape)[this->GHOST_SIZE] = ghost;

    SDL_SetRenderDrawColor(renderer, getR(), getG(), getB(), 255);

    int startX = col * cellSizeGX + (cellSizeGX - this->GHOST_SIZE * pixelSizeGX) / 2;
    int startY = row * cellSizeGY + (cellSizeGY - this->GHOST_SIZE * pixelSizeGY) / 2;

    for (int r = 0; r < this->GHOST_SIZE; r++) {
        for (int c = 0; c < this->GHOST_SIZE; c++) {
            if (ghostShape[r][c] == 1) {
                SDL_FRect pixel = { startX + c * pixelSizeGX, startY + r * pixelSizeGY, pixelSizeGX, pixelSizeGY };
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }
}

void GameBoard::updateRGhost() {
    if (ghostCourse.empty())
    {
        ghostCourse = ghostPath;
        Position ghostRdest = newDestination();
    }

    Position ghostRdest = newDestination();

    vector<Direction> possibleDirs;

    if (isValidMove(getGhostRLocRow(), getGhostRLocCol() - 1) && dirR != LEFT) possibleDirs.push_back(LEFT);
    if (isValidMove(getGhostRLocRow(), getGhostRLocCol() + 1) && dirR != RIGHT) possibleDirs.push_back(RIGHT);
    if (isValidMove(getGhostRLocRow() - 1, getGhostRLocCol()) && dirR != UP) possibleDirs.push_back(UP);
    if (isValidMove(getGhostRLocRow() + 1, getGhostRLocCol()) && dirR != DOWN) possibleDirs.push_back(DOWN);

    if (!possibleDirs.empty()) {
        dirR = possibleDirs[rand() % possibleDirs.size()];
    }

    switch (dirR) {
    case UP:    setGhostRLocRow(getGhostRLocRow() - 1); break;
    case DOWN:  setGhostRLocRow(getGhostRLocRow() + 1); break;
    case LEFT:  setGhostRLocCol(getGhostRLocCol() - 1); break;
    case RIGHT: setGhostRLocCol(getGhostRLocCol() + 1); break;
    default: break;
    }
}

void GameBoard::updatePGhost() {
    static bool initialized = false;
    if (!initialized) {
        srand(time(nullptr));
        initialized = true;
    }

    vector<Direction> possibleDirs;

    if (isValidMove(getGhostPLocRow(), getGhostPLocCol() - 1) && dirP != LEFT) possibleDirs.push_back(LEFT);
    if (isValidMove(getGhostPLocRow(), getGhostPLocCol() + 1) && dirP != RIGHT) possibleDirs.push_back(RIGHT);
    if (isValidMove(getGhostPLocRow() - 1, getGhostPLocCol()) && dirP != UP) possibleDirs.push_back(UP);
    if (isValidMove(getGhostPLocRow() + 1, getGhostPLocCol()) && dirP != DOWN) possibleDirs.push_back(DOWN);

    if (!possibleDirs.empty()) {
        dirP = possibleDirs[rand() % possibleDirs.size()];
    }

    switch (dirP) {
    case UP:    setGhostPLocRow(getGhostPLocRow() - 1); break;
    case DOWN:  setGhostPLocRow(getGhostPLocRow() + 1); break;
    case LEFT:  setGhostPLocCol(getGhostPLocCol() - 1); break;
    case RIGHT: setGhostPLocCol(getGhostPLocCol() + 1); break;
    default: break;
    }
}

Ghost::Position Ghost::newDestination() {
    static bool initialized = false;
    if (!initialized) {
        srand(time(nullptr));
        initialized = true;
    }

    Position ghostdest = destination[rand() % destination.size()];
    return ghostdest;
}
