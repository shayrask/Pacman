#include "Ghost.h"
#include "GameBoard.h"
#include <iostream>
#include <queue>

using namespace std;

// Static members initialization
SDL_Window* Ghost::win;
SDL_Renderer* Ghost::renderer;
vector<Ghost::Position> Ghost::Possibledestinations;
Ghost::GhostState currentState;

/**
 * @brief Default constructor initializes ghost's position and color.
 */
Ghost::Ghost() : ghostPosition{ 0, 0 }, R(255), G(255), B(255) {}

/**
 * @brief Destructor
 */
Ghost::~Ghost() {}

/**
 * @brief Initializes the ghost with position and color.
 * @param row Initial row position
 * @param col Initial column position
 * @param R Red color value
 * @param G Green color value
 * @param B Blue color value
 */
void Ghost::init(int row, int col, int R, int G, int B) {
    this->ghostPosition.row = row;
    this->ghostPosition.col = col;
    this->R = R;
    this->G = G;
    this->B = B;
    this->currentState = GhostState::ALIVE;

    // Get SDL renderer and window from GameBoard
    this->renderer = GameBoard::getRenderer();
    this->win = GameBoard::getWindow();
}

/**
 * @brief Renders the ghost at a given grid position.
 * @param row Row index to render the ghost
 * @param col Column index to render the ghost
 */
void Ghost::renderGhost(int row, int col) {
    // Calculate cell size based on game board dimensions
    int cellSizeGX = GameBoard::getWidth() / GameBoard::getMazeCols();
    int cellSizeGY = GameBoard::getHeight() / GameBoard::getMazeRows();

    int pixelSizeGX = cellSizeGX / this->GHOST_SIZE;
    int pixelSizeGY = cellSizeGY / this->GHOST_SIZE;

    int (*ghostShape)[this->GHOST_SIZE] = ghost;

    SDL_SetRenderDrawColor(renderer, getR(), getG(), getB(), 255);

    // Center ghost in its cell
    int startX = col * cellSizeGX + (cellSizeGX - this->GHOST_SIZE * pixelSizeGX) / 2;
    int startY = row * cellSizeGY + (cellSizeGY - this->GHOST_SIZE * pixelSizeGY) / 2;

    // Draw ghost pixel by pixel
    for (int r = 0; r < this->GHOST_SIZE; r++) {
        for (int c = 0; c < this->GHOST_SIZE; c++) {
            if (ghostShape[r][c] == 1) {
                SDL_FRect pixel = { startX + c * pixelSizeGX, startY + r * pixelSizeGY, pixelSizeGX, pixelSizeGY };
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }
}

/**
 * @brief Updates the ghost's movement based on its current state.
 */
void Ghost::updateGhost() {
    if (currentState == GhostState::ALIVE || currentState == GhostState::EATABLE) {
        if (ghostCourse.empty()) {
            this->target = this->newDestination();
            this->ghostCourse = findPath(this->ghostPosition, this->target);
        }

        if (!this->ghostCourse.empty()) {
            Position nextStep = this->ghostCourse.front();
            this->ghostCourse.erase(this->ghostCourse.begin());

            // Move ghost if the next step is valid
            if (GameBoard::isValidMove(nextStep.row, nextStep.col)) {
                this->ghostPosition = nextStep;
            }
            else {
                this->ghostCourse.clear();
            }
        }
    }
    else {
        // If ghost is dead, move it to the respawn position
        if (this->ghostPosition.row != this->deadPosition.row || this->ghostPosition.col != this->deadPosition.col) {
            this->ghostPosition = this->deadPosition;
        }
    }
}

/**
 * @brief Implements BFS to find the shortest path from start to end.
 * @param start The starting position
 * @param end The destination position
 * @return A vector representing the shortest path
 */
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

        // If target is reached, return the path
        if (current.row == target.row && current.col == target.col) {
            return current.path;
        }

        // Try all four possible movements
        for (int i = 0; i < 4; i++) {
            int newRow = current.row + dRow[i];
            int newCol = current.col + dCol[i];

            // Ensure move is within bounds and not visited
            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols &&
                GameBoard::maze[newRow][newCol] != 1 && !visited[newRow][newCol]) {

                visited[newRow][newCol] = true;
                vector<Position> newPath = current.path;
                newPath.push_back({ newRow, newCol });

                q.push({ newRow, newCol, newPath });
            }
        }
    }

    return {}; // Return empty path if no valid path is found
}

/**
 * @brief Selects a random destination for the ghost from possible destinations.
 * @return A randomly chosen valid position.
 */
Ghost::Position Ghost::newDestination() {
    static bool initialized = false;
    if (!initialized) {
        srand(time(nullptr));
        initialized = true;
    }

    // Choose a random destination from the list of possible destinations
    Position ghostdest = Possibledestinations[rand() % Possibledestinations.size()];
    return ghostdest;
}
