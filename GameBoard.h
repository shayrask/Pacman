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
#include "Ghost.h"


using namespace std;

/**
 * @class GameBoard
 * @brief Main class for managing the Pacman game board and game logic
 *
 * GameBoard handles the game window, rendering, game state management, and coordinates
 * the movement and interaction between Pacman and the ghosts. It controls the game flow
 * from main menu to gameplay to game over states.
 */
class GameBoard
{
private:
    // SDL rendering components
    static SDL_Window* window;          // Main game window
    static SDL_Renderer* renderer;      // Renderer for drawing game elements
    SDL_Texture* texture;               // Game texture
    SDL_FRect startButton;              // Button for starting the game
    SDL_FRect gameOverButton;           // Button for game over screen

    // Animation control
    Uint32 lastToggleTime = 0;          // Last time Pacman's mouth state was toggled
    const Uint32 TOGGLE_INTERVAL = 250; // Time between Pacman mouth open/close toggles (ms)
    bool pacmanOpen = true;             // Whether Pacman's mouth is currently open

    // Game state variables
    bool isRunning;                     // Whether the game is currently running
    int x, y;                           // General coordinates for positioning
    static float width, height;         // Width and height of the game window
    int randNum;                        // Random number for various uses

    // Pacman position and state
    int packmanLocRow, packmanLocCol;   // Pacman's position in the maze grid
    int packmanState = 1;               // Pacman's direction (1=Right, 2=Left, 3=Up, 4=Down)

    // Game timing and update rates
    int pacmanUpdateRate = 4;           // How often to update Pacman's position
    int eventHandleRate = 4;            // How often to handle events
    int ghostUpdateRate = 16;           // How often to update ghost positions
    int frameCount = 1;                 // Current frame count

    // Game metrics
    int score = 0;                      // Player's current score
    int numGhosts = 5;                  // Number of ghosts in the game

    /**
     * @enum GameState
     * @brief Represents the current state of the game
     */
    enum class GameState {
        MAIN_MENU,                      // Game is showing the main menu
        GAMEPLAY,                       // Game is in active play
        GAMEOVER                        // Game is in the game over state
    };

    /**
     * @enum GameLevel
     * @brief Represents the difficulty level of the game
     */
    enum class GameLevel {
        EASY,                           // Easy difficulty level
        MEDIUM,                         // Medium difficulty level
        HARD                            // Hard difficulty level
    };

    /**
     * @enum Direction
     * @brief Represents movement directions in the game
     */
    enum Direction {
        UP,                             // Movement upwards
        DOWN,                           // Movement downwards
        LEFT,                           // Movement to the left
        RIGHT                           // Movement to the right
    };

    /**
     * @struct Position
     * @brief Represents a position in the maze grid
     */
    struct Position {
        int row;                        // Row in the maze
        int col;                        // Column in the maze
    };

    // Current game state
    GameState currentState;             // Current state of the game
    GameLevel currentLevel;             // Current difficulty level

    /**
     * @brief Renders the main menu screen
     */
    void renderMenu();

    /**
     * @brief Renders the active gameplay
     */
    void renderGame();

    /**
     * @brief Renders the game over screen
     */
    void renderGameOver();

    /**
     * @brief Renders Pacman at the specified position with given state
     * @param x X-coordinate to render Pacman
     * @param y Y-coordinate to render Pacman
     * @param mouthOpen Whether Pacman's mouth is open
     * @param pacState Direction state of Pacman
     */
    void renderPacman(int x, int y, bool mouthOpen, int pacState);



public:
    /**
     * @brief Constructor for the GameBoard class
     */
    GameBoard();

    /**
     * @brief Destructor for the GameBoard class
     */
    ~GameBoard();

    // Game board and Pacman constants and static data
    static const int MAZE_ROWS = 31;    // Number of rows in the maze
    static const int MAZE_COLS = 28;    // Number of columns in the maze
    static int maze[MAZE_ROWS][MAZE_COLS]; // 2D array representing the maze layout
    static const int PACMAN_SIZE = 10;  // Size of Pacman in pixels

    // Pacman sprite patterns for different states
    static int pacmanClosed[PACMAN_SIZE][PACMAN_SIZE];       // Pacman with closed mouth
    static int pacmanOpenRight[PACMAN_SIZE][PACMAN_SIZE];    // Pacman with open mouth facing right
    static int pacmanOpenLeft[PACMAN_SIZE][PACMAN_SIZE];     // Pacman with open mouth facing left
    static int pacmanOpenUp[PACMAN_SIZE][PACMAN_SIZE];       // Pacman with open mouth facing up
    static int pacmanOpenDown[PACMAN_SIZE][PACMAN_SIZE];     // Pacman with open mouth facing down

    // Ghost movement paths and states
    vector<Position> ghostPath = { {13,11}, {13,12}, {13, 13}, {13, 14}, {13,15}, {13,16},
                                    {14,11}, {14,12}, {14, 13}, {14, 14}, {14,15}, {14,16},
                                    {15,11}, {15,12}, {15, 13}, {15, 14}, {15,15}, {15,16}
    };  // Predefined paths for ghosts to follow in the center of the maze

    int ghostEatAbleTime = 0;           // Timer for how long ghosts remain in edible state
    Ghost ghosts[5];                    // Array of ghost objects

    /**
     * @brief Handles user input events
     */
    void handleEvents();

    /**
     * @brief Initializes the game window and renderer
     * @param title Window title
     * @param xpos Initial x position of the window
     * @param ypos Initial y position of the window
     * @param width Width of the window
     * @param height Height of the window
     * @param fullscreen Whether to create window in fullscreen mode
     */
    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

    /**
     * @brief Cleans up resources before game shutdown
     */
    void clean();

    /**
     * @brief Renders the current game state to the screen
     */
    void render();

    /**
     * @brief Updates Pacman's position and state
     */
    void updatePacman();

    /**
     * @brief Main game loop function
     */
    void run();

    /**
     * @brief Checks if a move to the specified position is valid
     * @param row Row in the maze
     * @param col Column in the maze
     * @return true if the move is valid, false otherwise
     */
    static bool isValidMove(int row, int col);

    /**
     * @brief Checks if the game is over (Pacman caught by ghost)
     */
    void isGameover();

    /**
     * @brief Draws a circle with optional Pacman shape
     * @param renderer SDL renderer to draw on
     * @param centerX X-coordinate of the center
     * @param centerY Y-coordinate of the center
     * @param radius Radius of the circle
     * @param Packman Whether to draw as Pacman (with mouth) or full circle
     */
    void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius, bool Packman);

    /**
     * @brief Randomly shuffles the ghost paths
     *
     * Uses a random device to randomly reorganize the ghost path positions,
     * creating unpredictable ghost movement patterns.
     */
    void shuffleGhosts() {
        random_device rd;
        mt19937 g(rd());
        shuffle(ghostPath.begin(), ghostPath.end(), g);
    }

    // Getter methods

	static SDL_Window* getWindow() { return window; }
	static SDL_Renderer* getRenderer() { return renderer; }
	bool getRunning() { return isRunning; }
	static float getWidth() { return width; }
	static float getHeight() { return height; }


	static int getMazeRows() { return MAZE_ROWS; }
	static int getMazeCols() { return MAZE_COLS; }
    int getFrameCount() { return frameCount; }
    int getRandNum() { return randNum; }
    int getEventHandleRate() { return eventHandleRate; }
    int getPacmanUpdateRate() { return pacmanUpdateRate; }
    int getGhostUpdateRate() { return ghostUpdateRate; }
    int getScore() { return score; }
    GameState getcurrentState() { return currentState; };
    GameLevel getcurrentLevel() { return currentLevel; };

	int getPackmanSize() { return PACMAN_SIZE; }
	int getPackmanLocRow() { return packmanLocRow; }
	int getPackmanLocCol() { return packmanLocCol; }
	bool getPackmanOpen() { return pacmanOpen; }
	int getPackmanState() { return packmanState; }


	int getNumGhosts() { return numGhosts; }

	
    // Setter methods
	
    void setPackmanLocRow(int row) { packmanLocRow = row; }
	void setPackmanLocCol(int col) { packmanLocCol = col; }
	void setPacLoc(int row, int col) { packmanLocRow = row; packmanLocCol = col; }
    void setPackmanOpen(bool state) { pacmanOpen = state; }
    void setPackmanState(int state) { packmanState = state; }


    void setRunning(bool running) { isRunning = running; }
	void setRandNum(int num) { randNum = num; }
	void setFrameCount(int count) { frameCount = count; }
	void setWidth(int w) { width = w; }
	void setHeight(int h) { height = h; }
	void addScore(int s) { score += s; }
    void setcurrentState(GameState newState) { currentState = newState; };
    void setcurrentLevel(GameLevel newLevel) { currentLevel = newLevel; };


    void setGhostUpdateRate(int rate) { ghostUpdateRate = rate; }
	void setNumGhosts(int num) { numGhosts = num; }
};