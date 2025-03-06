#include "GameBoard.h"
#include "Ghost.h"

/**
 * @brief Maze layout array representing the game board
 *
 * The 2D array represents the maze layout with the following values:
 * - 0: Empty space/corridor (Pacman can move through, contains pellets)
 * - 1: Wall (Pacman and ghosts cannot pass through)
 * - 2: Ghost house door (only ghosts can pass through)
 * - 3: Power pellet (gives Pacman ability to eat ghosts when consumed)
 * - 4: Ghost house interior (where ghosts start)
 *
 * The maze is structured with walls (1) around the perimeter and interior walls
 * creating the classic Pacman maze pattern. The ghost house is in the center of the maze.
 */
int GameBoard::maze[GameBoard::MAZE_ROWS][GameBoard::MAZE_COLS] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1},
        {1, 3, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 2, 2, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 4, 4, 4, 4, 4, 4, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4, 4, 4, 4, 4, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 4, 4, 4, 4, 4, 4, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
        {1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1},
        {1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1},
        {1, 0, 0, 3, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 3, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

/**
 * @brief Pacman sprite with closed mouth
 *
 * This 2D array represents Pacman with a closed mouth (full circle).
 * The array describes a 10x10 pixel pattern where:
 * - 0: Transparent/no pixel
 * - 1: Filled pixel (yellow for Pacman)
 *
 * Creates a round shape with slightly trimmed corners
 */
int GameBoard::pacmanClosed[GameBoard::PACMAN_SIZE][GameBoard::PACMAN_SIZE] = {
        {0,0,1,1,1,1,1,1,0,0},
        {0,1,1,1,1,1,1,1,1,0},
        {1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1},
        {0,1,1,1,1,1,1,1,1,0},
        {0,0,1,1,1,1,1,1,0,0}
};

/**
 * @brief Pacman sprite with open mouth facing right
 *
 * This 2D array represents Pacman with his mouth open, facing right.
 * The array describes a 10x10 pixel pattern where:
 * - 0: Transparent/no pixel
 * - 1: Filled pixel (yellow for Pacman)
 *
 * Creates the classic "pizza slice" shape pointing right
 */
int GameBoard::pacmanOpenRight[GameBoard::PACMAN_SIZE][GameBoard::PACMAN_SIZE] = {
    {0,0,1,1,1,1,1,1,0,0},
    {0,1,1,1,1,1,1,1,1,0},
    {1,1,1,1,1,1,1,1,0,0},
    {1,1,1,1,1,1,0,0,0,0},
    {1,1,1,1,1,0,0,0,0,0},
    {1,1,1,1,1,0,0,0,0,0},
    {1,1,1,1,1,1,0,0,0,0},
    {1,1,1,1,1,1,1,1,0,0},
    {0,1,1,1,1,1,1,1,1,0},
    {0,0,1,1,1,1,1,1,0,0}
};

/**
 * @brief Pacman sprite with open mouth facing left
 *
 * This 2D array represents Pacman with his mouth open, facing left.
 * The array describes a 10x10 pixel pattern where:
 * - 0: Transparent/no pixel
 * - 1: Filled pixel (yellow for Pacman)
 *
 * Creates the classic "pizza slice" shape pointing left
 */
int GameBoard::pacmanOpenLeft[GameBoard::PACMAN_SIZE][GameBoard::PACMAN_SIZE] = {
    {0,0,1,1,1,1,1,1,0,0},
    {0,1,1,1,1,1,1,1,1,0},
    {0,0,1,1,1,1,1,1,1,1},
    {0,0,0,0,1,1,1,1,1,1},
    {0,0,0,0,0,1,1,1,1,1},
    {0,0,0,0,0,1,1,1,1,1},
    {0,0,0,0,1,1,1,1,1,1},
    {0,0,1,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,1,1,0},
    {0,0,1,1,1,1,1,1,0,0}
};

/**
 * @brief Pacman sprite with open mouth facing up
 *
 * This 2D array represents Pacman with his mouth open, facing upward.
 * The array describes a 10x10 pixel pattern where:
 * - 0: Transparent/no pixel
 * - 1: Filled pixel (yellow for Pacman)
 *
 * Creates the classic "pizza slice" shape pointing upward
 */
int GameBoard::pacmanOpenUp[GameBoard::PACMAN_SIZE][GameBoard::PACMAN_SIZE] = {
    {0,0,0,0,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0,1,0},
    {1,1,1,0,0,0,0,1,1,1},
    {1,1,1,0,0,0,0,1,1,1},
    {1,1,1,1,0,0,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,1,1,0},
    {0,0,1,1,1,1,1,1,0,0}
};

/**
 * @brief Pacman sprite with open mouth facing down
 *
 * This 2D array represents Pacman with his mouth open, facing downward.
 * The array describes a 10x10 pixel pattern where:
 * - 0: Transparent/no pixel
 * - 1: Filled pixel (yellow for Pacman)
 *
 * Creates the classic "pizza slice" shape pointing downward
 */
int GameBoard::pacmanOpenDown[GameBoard::PACMAN_SIZE][GameBoard::PACMAN_SIZE] = {
    {0,0,1,1,1,1,1,1,0,0},
    {0,1,1,1,1,1,1,1,1,0},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,0,0,1,1,1,1},
    {1,1,1,0,0,0,0,1,1,1},
    {1,1,1,0,0,0,0,1,1,1},
    {0,1,0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,0,0}
};

/**
 * @brief Ghost sprite pattern
 *
 * This 2D array represents the ghost character appearance.
 * The array describes a 10x10 pixel pattern where:
 * - 0: Transparent/no pixel
 * - 1: Filled pixel (colored according to ghost type)
 *
 * Creates the classic ghost shape with the characteristic wavy bottom
 * and two eyes. The specific ghost colors are applied during rendering.
 */
int Ghost::ghost[Ghost::GHOST_SIZE][Ghost::GHOST_SIZE] = {
    {0,0,1,1,1,1,1,1,0,0},
    {0,1,1,1,1,1,1,1,1,0},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,0,0,1,1,0,0,1},
    {1,1,1,0,0,1,1,0,0,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,0,1,1,0,1,1,0,1},
    {0,0,0,1,1,0,0,1,0,0}
};