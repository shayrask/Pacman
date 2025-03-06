#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_rect.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <utility>
#include <algorithm> 
#include <random>

using namespace std;

class GameBoard; // Forward declaration of GameBoard class

/**
 * @class Ghost
 * @brief Represents a ghost character in the Pac-Man game.
 */
class Ghost {
private:
    static SDL_Window* win; ///< SDL window pointer
    static SDL_Renderer* renderer; ///< SDL renderer pointer

    /**
     * @struct Position
     * @brief Represents a position in the game grid.
     */
    struct Position {
        int row; ///< Row index
        int col; ///< Column index
    };

    /**
     * @struct Node
     * @brief Represents a node in the pathfinding algorithm.
     */
    struct Node {
        int row, col; ///< Row and column indices
        vector<Position> path; ///< Path to this node
    };

    Position ghostPosition; ///< Current position of the ghost
    int R, G, B; ///< Color components of the ghost

public:
    /**
     * @enum GhostState
     * @brief Represents the possible states of a ghost.
     */
    enum class GhostState {
        ALIVE, ///< The ghost is alive and moving normally
        DEAD, ///< The ghost has been eaten and needs to return to spawn
        EATABLE ///< The ghost is vulnerable after a power pellet is eaten
    };

    Ghost(); ///< Constructor
    ~Ghost(); ///< Destructor

    /**
     * @brief Initializes the ghost with a specific position and color.
     * @param row Initial row position
     * @param col Initial column position
     * @param R Red color value
     * @param G Green color value
     * @param B Blue color value
     */
    void init(int row, int col, int R, int G, int B);

    static const int GHOST_SIZE = 10; ///< Size of the ghost sprite
    static int ghost[GHOST_SIZE][GHOST_SIZE]; ///< 2D array defining ghost shape
    static vector<Position> Possibledestinations; ///< Possible target positions

    Position target; ///< Current target position for movement
    Position deadPosition; ///< Respawn position when dead
    vector<Position> ghostCourse; ///< Current path to target
    GhostState currentState; ///< Current state of the ghost

    /**
     * @brief Updates the ghost's position and state.
     */
    void updateGhost();

    /**
     * @brief Determines a new random destination for the ghost.
     * @return The new target position.
     */
    Position newDestination();

    /**
     * @brief Renders the ghost at a specified position on the board.
     * @param row Row index to render the ghost
     * @param col Column index to render the ghost
     */
    void renderGhost(int row, int col);

    /**
     * @brief Finds a path from a start position to an end position.
     * @param start The starting position
     * @param end The destination position
     * @return A vector representing the path from start to end
     */
    vector<Position> findPath(Position start, Position end);

    // Getters
    static int getGhostSize() { return GHOST_SIZE; }
    int getGhostLocRow() { return this->ghostPosition.row; }
    int getGhostLocCol() { return this->ghostPosition.col; }
    int getR() { return this->R; }
    int getG() { return this->G; }
    int getB() { return this->B; }
    GhostState getcurrentState() { return currentState; }

    // Setters
    void setLoc(int row, int col) { this->ghostPosition.row = row; this->ghostPosition.col = col; }
    void setGhostLocRow(int row) { this->ghostPosition.row = row; }
    void setGhostLocCol(int col) { this->ghostPosition.col = col; }
    void setR(int R) { this->R = R; }
    void setG(int G) { this->G = G; }
    void setB(int B) { this->B = B; }
    void setRGB(int R, int G, int B) { this->R = R; this->G = G; this->B = B; }
    void setcurrentState(GhostState newState) { currentState = newState; }
};
