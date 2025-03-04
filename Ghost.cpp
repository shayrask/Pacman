#include "Ghost.h"
#include "GameBoard.h"
#include <iostream>
#include <queue>

using namespace std;

SDL_Window* Ghost::win;
SDL_Renderer* Ghost::renderer;
vector<Ghost::Position> Ghost::Possibledestinations;

Ghost::Ghost() : ghostPosition{ 0, 0 }, R(255), G(255), B(255) {}
Ghost::~Ghost() {}

void Ghost::init(int row, int col, int R, int G, int B) {
    this->ghostPosition.row = row;
    this->ghostPosition.col = col;
    this->R = R;
    this->G = G;
    this->B = B;

    this->renderer = GameBoard::getRenderer();
    this->win = GameBoard::getWindow();
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

void Ghost::updateGhost() {
    if (ghostCourse.empty())
    {
        this->target = this->newDestination();
		this->ghostCourse = findPath(this->ghostPosition, this->target);
    }

    if (!this->ghostCourse.empty()) {
        Position nextStep = this->ghostCourse.front(); 
        this->ghostCourse.erase(this->ghostCourse.begin()); 

        if (GameBoard::isValidMove(nextStep.row, nextStep.col)) {
            this->ghostPosition = nextStep;
        }
        else {
            this->ghostCourse.clear();
        }
    }
}

vector<Ghost::Position> Ghost::findPath(Position start, Position end) {
	int rows = GameBoard::getMazeRows();
	int cols = GameBoard::getMazeCols();
    
    int dRow[] = { -1, 1, 0, 0 };
    int dCol[] = { 0, 0, -1, 1 };

    vector<vector<bool>> visited(rows, vector<bool>(cols, false));

    queue<Node> q;
    q.push({ start.row, start.col, {} });
    visited[start.row][start.col] = true;

    while (!q.empty()) {
        Node current = q.front();
        q.pop();

        int r = current.row, c = current.col;

        if (r == target.row && c == target.col) {
            return current.path;
        }

        for (int i = 0; i < 4; i++) {
            int newRow = r + dRow[i];
            int newCol = c + dCol[i];

            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols &&
                GameBoard::maze[newRow][newCol] != 1 && !visited[newRow][newCol]) {

                visited[newRow][newCol] = true;
                vector<Position> newPath = current.path;
                newPath.push_back({ newRow, newCol });

                q.push({ newRow, newCol, newPath });
            }
        }
    }

    return {};
}

Ghost::Position Ghost::newDestination() {
    static bool initialized = false;
    if (!initialized) {
        srand(time(nullptr));
        initialized = true;
    }

    Position ghostdest = Possibledestinations[rand() % Possibledestinations.size()];
    return ghostdest;
}
