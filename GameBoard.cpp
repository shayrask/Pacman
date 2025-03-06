#include "GameBoard.h"
#include <iostream>
#include <string.h>
#include <sstream>
#include "Ghost.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <fstream>
#define M_PI 3.14159265358979323846

using namespace std;

// Static members initialization
SDL_Window* GameBoard::window;
SDL_Renderer* GameBoard::renderer;
float GameBoard::width;
float GameBoard::height;


/**
 * @brief Constructor initializes the game board, SDL, and game elements.
 */
GameBoard::GameBoard() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("PackMan Game", 868, 775, NULL);
    setWidth(868);
    setHeight(775);
    renderer = SDL_CreateRenderer(window, NULL);
    setRunning(true);
    srand(time(0));
    setRandNum(rand() % 61);

    // Initialize ghosts with default positions
    for (int i = 0; i < 5; i++) {
        ghosts[i] = Ghost();
        ghosts[i].deadPosition = { 14, 11 + i };
    }
}

/**
 * @brief Destructor cleans up SDL resources.
 */
GameBoard::~GameBoard() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void GameBoard::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {

}

/**
 * @brief Handles user input events and updates the game state accordingly
 *
 * This function processes SDL events such as mouse clicks (in the main menu) and
 * key presses (in gameplay mode) to allow the user to select difficulty levels and
 * control the Pacman character. The function also handles window closing events
 * and updates the game state.
 */
void GameBoard::handleEvents() {
    SDL_Event event;
    // Loop to handle all pending events
    while (SDL_PollEvent(&event)) {
        // Check if user requested to close the window
        if (event.type == SDL_EVENT_QUIT) {
            setRunning(false);
        }

        // Handle mouse events in the main menu
        if (getcurrentState() == GameState::MAIN_MENU && event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            int x = event.button.x;
            int y = event.button.y;

            // Check if user clicked on the EASY difficulty button (left part of button)
            if (x >= startButton.x && x <= (startButton.x + startButton.w / 3) &&
                y >= startButton.y && y <= (startButton.y + startButton.h)) {
                setcurrentState(GameState::GAMEPLAY);  // Switch to gameplay state
                setcurrentLevel(GameLevel::EASY);      // Set easy difficulty level
                setNumGhosts(3);                       // Set 3 ghosts for the game
                render();                              // Render the game
            }
            // Check if user clicked on the MEDIUM difficulty button (middle part of button)
            else if (x >= (startButton.x + startButton.w / 3) && x <= (startButton.x + (startButton.w / 3 * 2)) &&
                y >= startButton.y && y <= (startButton.y + startButton.h)) {
                setcurrentState(GameState::GAMEPLAY);  // Switch to gameplay state
                setcurrentLevel(GameLevel::MEDIUM);    // Set medium difficulty level
                setNumGhosts(4);                       // Set 4 ghosts for the game
                render();                              // Render the game
            }
            // Check if user clicked on the HARD difficulty button (right part of button)
            else if (x >= (startButton.x + (startButton.w / 3 * 2)) && x <= (startButton.x + startButton.w) &&
                y >= startButton.y && y <= (startButton.y + startButton.h)) {
                setcurrentState(GameState::GAMEPLAY);  // Switch to gameplay state
                setcurrentLevel(GameLevel::HARD);      // Set hard difficulty level
                setNumGhosts(5);                       // Set 5 ghosts for the game
                render();                              // Render the game
            }
        }
        // Handle keyboard events during gameplay
        else if (getcurrentState() == GameState::GAMEPLAY && event.type == SDL_EVENT_KEY_DOWN) {
            switch (event.key.scancode) {
            case SDL_SCANCODE_UP:
                // Check if the move is valid before updating Pacman position
                if (isValidMove(getPackmanLocRow() - 1, getPackmanLocCol())) {
                    setPackmanLocRow(getPackmanLocRow() - 1);  // Move Pacman up
                    setPackmanState(3);                        // Set Pacman direction to up

                    // Check for dot collection and update score
                    if (maze[getPackmanLocRow()][getPackmanLocCol()] == 0) {
                        maze[getPackmanLocRow()][getPackmanLocCol()] = 5;  // Mark cell as visited
                        addScore(10);                                       // Add points for dot
                    }
                    // Check for power pill collection
                    else if (maze[getPackmanLocRow()][getPackmanLocCol()] == 3) {
                        maze[getPackmanLocRow()][getPackmanLocCol()] = 5;  // Mark cell as visited
                        addScore(50);                                       // Add points for power pill

                        // Activate ghost vulnerable mode for 5 seconds
                        GameBoard::ghostEatAbleTime = SDL_GetTicks() + 5000;

                        // Make all non-dead ghosts eatable and set their colors
                        for (int i = 0; i < getNumGhosts(); i++) {
                            if (ghosts[i].getcurrentState() != Ghost::GhostState::DEAD) {
                                ghosts[i].currentState = Ghost::GhostState::EATABLE;
                                ghosts[i].setRGB(100, 0, 255);  // Dark blue with purple (indigo)                               
                            }
                        }

                        // Adjust ghost speed based on current difficulty level
                        switch (getcurrentLevel()) {
                        case GameLevel::EASY:
                            setGhostUpdateRate(12);  // Slowest ghost speed for easy level
                            break;
                        case GameLevel::MEDIUM:
                            setGhostUpdateRate(9);   // Medium ghost speed
                            break;
                        case GameLevel::HARD:
                            setGhostUpdateRate(7);   // Fastest ghost speed for hard level
                            break;
                        default:
                            break;
                        }
                    }
                }
                break;

            case SDL_SCANCODE_DOWN:
                // Handle Pacman movement downward
                // Similar logic as UP direction with Pacman state = 4 for down direction
                if (isValidMove(getPackmanLocRow() + 1, getPackmanLocCol())) {
                    setPackmanLocRow(getPackmanLocRow() + 1);  // Move Pacman down
                    setPackmanState(4);                        // Set Pacman direction to down

                    // Same dot collection, power pill, and ghost logic as above
                    if (maze[getPackmanLocRow()][getPackmanLocCol()] == 0) {
                        maze[getPackmanLocRow()][getPackmanLocCol()] = 5;
                        addScore(10);
                    }
                    else if (maze[getPackmanLocRow()][getPackmanLocCol()] == 3) {
                        maze[getPackmanLocRow()][getPackmanLocCol()] = 5;
                        addScore(50);
                        GameBoard::ghostEatAbleTime = SDL_GetTicks() + 5000;
                        for (int i = 0; i < getNumGhosts(); i++) {
                            if (ghosts[i].getcurrentState() != Ghost::GhostState::DEAD) {
                                ghosts[i].currentState = Ghost::GhostState::EATABLE;
                                ghosts[i].setRGB(100, 0, 255);  // Dark blue with purple (indigo)
                            }
                        }
                        switch (getcurrentLevel()) {
                        case GameLevel::EASY:
                            setGhostUpdateRate(12);
                            break;
                        case GameLevel::MEDIUM:
                            setGhostUpdateRate(9);
                            break;
                        case GameLevel::HARD:
                            setGhostUpdateRate(7);
                            break;
                        default:
                            break;
                        }
                    }
                }
                break;

            case SDL_SCANCODE_LEFT:
                // Handle Pacman movement to the left, with special portal logic at maze edge
                if (getPackmanLocCol() == 0) {
                    // Portal logic: When Pacman reaches the left edge, wrap to the right edge
                    setPackmanLocCol(getMazeCols() - 1);      // Wrap to right side of maze
                    setPackmanState(2);                       // Set Pacman direction to left

                    // Same dot collection, power pill, and ghost logic as above
                    if (maze[getPackmanLocRow()][getPackmanLocCol()] == 0) {
                        maze[getPackmanLocRow()][getPackmanLocCol()] = 5;
                        addScore(10);
                    }
                    else if (maze[getPackmanLocRow()][getPackmanLocCol()] == 3) {
                        maze[getPackmanLocRow()][getPackmanLocCol()] = 5;
                        addScore(50);
                        GameBoard::ghostEatAbleTime = SDL_GetTicks() + 5000;
                        for (int i = 0; i < getNumGhosts(); i++) {
                            if (ghosts[i].getcurrentState() != Ghost::GhostState::DEAD) {
                                ghosts[i].currentState = Ghost::GhostState::EATABLE;
                                ghosts[i].setRGB(100, 0, 255);  // Dark blue with purple (indigo)
                            }
                        }
                        switch (getcurrentLevel()) {
                        case GameLevel::EASY:
                            setGhostUpdateRate(12);
                            break;
                        case GameLevel::MEDIUM:
                            setGhostUpdateRate(9);
                            break;
                        case GameLevel::HARD:
                            setGhostUpdateRate(7);
                            break;
                        default:
                            break;
                        }
                    }
                }
                // Regular movement to the left when not at maze edge
                else if (isValidMove(getPackmanLocRow(), getPackmanLocCol() - 1)) {
                    setPackmanLocCol(getPackmanLocCol() - 1);  // Move Pacman left
                    setPackmanState(2);                        // Set Pacman direction to left

                    // Same dot collection and power pill logic as above
                    if (maze[getPackmanLocRow()][getPackmanLocCol()] == 0) {
                        maze[getPackmanLocRow()][getPackmanLocCol()] = 5;
                        addScore(10);
                    }
                    else if (maze[getPackmanLocRow()][getPackmanLocCol()] == 3) {
                        maze[getPackmanLocRow()][getPackmanLocCol()] = 5;
                        addScore(50);
                        GameBoard::ghostEatAbleTime = SDL_GetTicks() + 5000;
                        for (int i = 0; i < getNumGhosts(); i++) {
                            if (ghosts[i].getcurrentState() != Ghost::GhostState::DEAD) {
                                ghosts[i].currentState = Ghost::GhostState::EATABLE;
                                ghosts[i].setRGB(100, 0, 255);  // Dark blue with purple (indigo)
                            }
                        }
                        switch (getcurrentLevel()) {
                        case GameLevel::EASY:
                            setGhostUpdateRate(12);
                            break;
                        case GameLevel::MEDIUM:
                            setGhostUpdateRate(9);
                            break;
                        case GameLevel::HARD:
                            setGhostUpdateRate(7);
                            break;
                        default:
                            break;
                        }
                    }
                }
                break;

            case SDL_SCANCODE_RIGHT:
                // Handle Pacman movement to the right, with special portal logic at maze edge
                if (getPackmanLocCol() == getMazeCols() - 1) {
                    // Portal logic: When Pacman reaches the right edge, wrap to the left edge
                    setPackmanLocCol(0);                       // Wrap to left side of maze
                    setPackmanState(1);                        // Set Pacman direction to right

                    // Same dot collection, power pill, and ghost logic as above
                    if (maze[getPackmanLocRow()][getPackmanLocCol()] == 0) {
                        maze[getPackmanLocRow()][getPackmanLocCol()] = 5;
                        addScore(10);
                    }
                    else if (maze[getPackmanLocRow()][getPackmanLocCol()] == 3) {
                        maze[getPackmanLocRow()][getPackmanLocCol()] = 5;
                        addScore(50);
                        GameBoard::ghostEatAbleTime = SDL_GetTicks() + 5000;
                        for (int i = 0; i < getNumGhosts(); i++) {
                            if (ghosts[i].getcurrentState() != Ghost::GhostState::DEAD) {
                                ghosts[i].currentState = Ghost::GhostState::EATABLE;
                                ghosts[i].setRGB(100, 0, 255);  // Dark blue with purple (indigo)
                            }
                        }
                        switch (getcurrentLevel()) {
                        case GameLevel::EASY:
                            setGhostUpdateRate(12);
                            break;
                        case GameLevel::MEDIUM:
                            setGhostUpdateRate(9);
                            break;
                        case GameLevel::HARD:
                            setGhostUpdateRate(7);
                            break;
                        default:
                            break;
                        }
                    }
                }
                // Regular movement to the right when not at maze edge
                else if (isValidMove(getPackmanLocRow(), getPackmanLocCol() + 1)) {
                    setPackmanLocCol(getPackmanLocCol() + 1);  // Move Pacman right
                    setPackmanState(1);                        // Set Pacman direction to right

                    // Same dot collection and power pill logic as above
                    if (maze[getPackmanLocRow()][getPackmanLocCol()] == 0) {
                        maze[getPackmanLocRow()][getPackmanLocCol()] = 5;
                        addScore(10);
                    }
                    else if (maze[getPackmanLocRow()][getPackmanLocCol()] == 3) {
                        maze[getPackmanLocRow()][getPackmanLocCol()] = 5;
                        addScore(50);
                        GameBoard::ghostEatAbleTime = SDL_GetTicks() + 5000;
                        for (int i = 0; i < getNumGhosts(); i++) {
                            if (ghosts[i].getcurrentState() != Ghost::GhostState::DEAD) {
                                ghosts[i].currentState = Ghost::GhostState::EATABLE;
                                ghosts[i].setRGB(100, 0, 255);  // Dark blue with purple (indigo)
                            }
                        }
                        switch (getcurrentLevel()) {
                        case GameLevel::EASY:
                            setGhostUpdateRate(12);
                            break;
                        case GameLevel::MEDIUM:
                            setGhostUpdateRate(9);
                            break;
                        case GameLevel::HARD:
                            setGhostUpdateRate(7);
                            break;
                        default:
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
}

/**
 * @brief Renders the appropriate game screen based on the current game state
 *
 * This function handles the rendering of different game screens (menu, gameplay, or game over)
 * depending on the current state of the game. It first clears the screen, then calls the
 * appropriate rendering function, and finally presents the rendered content to the display.
 */
void GameBoard::render() {
    // Set background color to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Clear the renderer with the background color
    SDL_RenderClear(renderer);

    // Render different screens based on game state
    switch (currentState) {
    case GameState::MAIN_MENU:
        renderMenu();     // Render the main menu screen
        break;
    case GameState::GAMEPLAY:
        renderGame();     // Render the actual gameplay screen
        break;
    case GameState::GAMEOVER:
        renderGameOver(); // Render the game over screen
        break;
    }

    // Present the rendered content to the display
    SDL_RenderPresent(renderer);
}

/**
 * @brief Renders the game menu screen.
 *
 * This function clears the screen, sets a blue background,
 * and displays a start button at the center of the screen.
 */
void GameBoard::renderMenu() {
    // Set background color to blue
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);  // Clear the screen with the blue color

    // Load the button texture from an image file
    SDL_Texture* buttonTexture = IMG_LoadTexture(renderer, "EMHbutton.png");
    if (!buttonTexture) {
        cerr << "Failed to load button texture: " << SDL_GetError() << endl;
        return;
    }

    // Define the position and size of the start button (centered)
    startButton = { (getWidth() / 2) - 200, (getHeight() / 2) - 100, 400, 200 };

    // Render the button texture onto the screen
    SDL_RenderTexture(renderer, buttonTexture, NULL, &startButton);

    // Present the updated menu screen
    SDL_RenderPresent(renderer);

    // Free the texture after rendering to avoid memory leaks
    SDL_DestroyTexture(buttonTexture);
}

/**
 * @brief Renders the game over screen.
 *
 * This function clears the screen, sets a black background,
 * and displays a game over message with the final score.
 */
void GameBoard::renderGameOver() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Texture* gameOverTexture = IMG_LoadTexture(renderer, "GameOver.png");

    gameOverButton = { (getWidth() / 2) - 150, (getHeight() / 2) - 100, 300, 200 };
    SDL_RenderTexture(renderer, gameOverTexture, NULL, &gameOverButton);

    //TTF_Font* font = TTF_OpenFont("C:\\Users\\shayr\\gisha.ttf", 36); 

    //SDL_Color textColor = { 255, 255, 255, 255 }; 
    //string scoreText = "Score: " + to_string(getScore());

    //SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), scoreText.size(), textColor);
    //SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

    //int textWidth = scoreSurface->w;
    //int textHeight = scoreSurface->h;
    //SDL_FRect scoreRect = {
    //    (getWidth() / 2) - (textWidth / 2),  
    //    (getHeight() / 2) + 120,             
    //    textWidth,
    //    textHeight
    //};

    //SDL_RenderTexture(renderer, scoreTexture, NULL, &scoreRect);

    //SDL_DestroySurface(scoreSurface);
    //SDL_DestroyTexture(scoreTexture);
    //TTF_CloseFont(font);

    SDL_RenderPresent(renderer);
}

/**
 * @brief Renders the game board, including walls, dots, Pac-Man, and ghosts.
 *
 * This function iterates through the maze array and renders each element accordingly.
 * It draws walls, places Pac-Man at a random valid position, and ensures ghosts
 * are displayed correctly.
 */
void GameBoard::renderGame() {
    // Set background color to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);  // Clear the screen with the black color

    // Calculate the size of each maze cell based on the window dimensions
    int cellSizeX = width / getMazeCols();
    int cellSizeY = height / getMazeRows();

    // Iterate through the maze to render each cell
    for (int row = 0; row < getMazeRows(); row++) {
        for (int col = 0; col < getMazeCols(); col++) {
            SDL_FRect cellRect = { col * cellSizeX, row * cellSizeY, cellSizeX, cellSizeY };

            // Render walls (value 1 in the maze)
            if (maze[row][col] == 1) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Blue color for walls
                SDL_RenderFillRect(renderer, &cellRect);
            }
            // Render paths, Pac-Man, and dots (value 0 in the maze)
            else if (maze[row][col] == 0) {
                Ghost::Possibledestinations.push_back({ row, col });

                // Place Pac-Man at a random valid location
                if (getRandNum() == 0) {
                    renderPacman(row, col, getPackmanOpen(), getPackmanState());
                    setPacLoc(row, col);
                    maze[row][col] = 5; // Mark the cell as occupied by Pac-Man
                }
                // Render Pac-Man at its current location
                else if (row == getPackmanLocRow() && col == getPackmanLocCol()) {
                    renderPacman(row, col, getPackmanOpen(), getPackmanState());
                }
                // Render small dots (food for Pac-Man)
                else {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color for dots

                    int centerX = cellRect.x + cellRect.w / 2;
                    int centerY = cellRect.y + cellRect.h / 2;
                    int dotRadius = cellRect.w / 8;

                    drawCircle(renderer, centerX, centerY, dotRadius, false);
                }
                // Decrease random number counter for Pac-Man placement
                setRandNum(getRandNum() - 1);
            }
            // Render power pellets (value 3 in the maze)
            else if (maze[row][col] == 3) {
                Ghost::Possibledestinations.push_back({ row, col });
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color for power pellets

                int centerX = cellRect.x + cellRect.w / 2;
                int centerY = cellRect.y + cellRect.h / 2;
                int dotRadius = cellRect.w / 4;

                drawCircle(renderer, centerX, centerY, dotRadius, false);
            }
            // Render Pac-Man when the cell is marked as occupied (value 5 in the maze)
            else if (maze[row][col] == 5 && row == getPackmanLocRow() && col == getPackmanLocCol()) {
                renderPacman(row, col, getPackmanOpen(), getPackmanState());
                Ghost::Possibledestinations.push_back({ row, col });
            }

            // Render ghosts at their respective positions
            for (int i = 0; i < getNumGhosts(); i++) {
                if (row == ghosts[i].getGhostLocRow() && col == ghosts[i].getGhostLocCol()) {
                    ghosts[i].renderGhost(row, col);
                    Ghost::Possibledestinations.push_back({ row, col });
                }
            }
        }
    }

    // Present the updated game board
    SDL_RenderPresent(renderer);
}

/**
 * @brief Renders Pac-Man on the screen at a specified position.
 *
 * This function determines the correct Pac-Man sprite (open or closed mouth)
 * based on the movement direction and draws it on the screen using the SDL renderer.
 *
 * @param row The row index in the maze where Pac-Man is located.
 * @param col The column index in the maze where Pac-Man is located.
 * @param mouthOpen Whether Pac-Man's mouth is open (true) or closed (false).
 * @param pacState The direction Pac-Man is facing (1 = right, 2 = left, 3 = up, 4 = down).
 */
void GameBoard::renderPacman(int row, int col, bool mouthOpen, int pacState) {
    // Determine the size of a single cell in the grid
    int cellSizeX = width / getMazeCols();
    int cellSizeY = height / getMazeRows();

    // Determine the size of each pixel in Pac-Man's sprite
    int pixelSizeX = cellSizeX / PACMAN_SIZE;
    int pixelSizeY = cellSizeY / PACMAN_SIZE;

    // Pointer to the appropriate Pac-Man shape array
    int (*pacmanShape)[PACMAN_SIZE];

    // Select the appropriate sprite based on Pac-Man's mouth state and direction
    if (mouthOpen) {
        switch (pacState) {
        case 1: pacmanShape = pacmanOpenRight; break;  // Facing right
        case 2: pacmanShape = pacmanOpenLeft; break;   // Facing left
        case 3: pacmanShape = pacmanOpenUp; break;     // Facing up
        case 4: pacmanShape = pacmanOpenDown; break;   // Facing down
        default: pacmanShape = pacmanClosed;           // Default to closed mouth
        }
    }
    else {
        pacmanShape = pacmanClosed; // Closed mouth state
    }

    // Set Pac-Man's color (yellow)
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    // Calculate the top-left pixel position for rendering Pac-Man in the cell
    int startX = col * cellSizeX + (cellSizeX - PACMAN_SIZE * pixelSizeX) / 2;
    int startY = row * cellSizeY + (cellSizeY - PACMAN_SIZE * pixelSizeY) / 2;

    // Render Pac-Man pixel by pixel based on the selected shape
    for (int r = 0; r < PACMAN_SIZE; r++) {
        for (int c = 0; c < PACMAN_SIZE; c++) {
            if (pacmanShape[r][c] == 1) { // Draw only active pixels
                SDL_FRect pixel = { startX + c * pixelSizeX, startY + r * pixelSizeY, pixelSizeX, pixelSizeY };
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }
}

/**
 * @brief Updates Pac-Man's mouth state (open/closed).
 *
 * This function toggles Pac-Man's mouth state at a fixed interval to create
 * the effect of an opening and closing mouth.
 */
void GameBoard::updatePacman() {
    Uint32 currentTime = SDL_GetTicks(); // Get the current time

    // Toggle Pac-Man's mouth state at defined intervals
    if (currentTime - lastToggleTime >= TOGGLE_INTERVAL) {
        setPackmanOpen(!getPackmanOpen()); // Toggle the state
        lastToggleTime = currentTime; // Update the last toggle time
    }
}

/**
 * @brief Checks if a given move is valid.
 *
 * This function verifies whether Pac-Man or a ghost can move to the specified coordinates.
 * A move is considered valid if the target position in the maze is not a wall (represented by 1).
 *
 * @param newX The target X-coordinate in the maze.
 * @param newY The target Y-coordinate in the maze.
 * @return true if the move is valid, false otherwise.
 */
bool GameBoard::isValidMove(int newX, int newY) {
    return maze[newX][newY] != 1;
}

/**
 * @brief Runs the main game loop.
 *
 * This function initializes the game, updates the game state, and renders the screen continuously
 * until the game ends. It handles Pac-Man's movement, ghost behavior, collision detection,
 * and gameplay timing.
 */
void GameBoard::run() {
    renderMenu();  // Display the main menu
    shuffleGhosts(); // Randomize ghost starting positions

    // Initialize ghosts with their respective colors and positions
    for (int i = 0; i < getNumGhosts(); i++) {
        switch (i) {
        case 0: ghosts[i].init(ghostPath[i].row, ghostPath[i].col, 255, 0, 0); break;       // Red ghost
        case 1: ghosts[i].init(ghostPath[i].row, ghostPath[i].col, 255, 192, 203); break;   // Pink ghost
        case 2: ghosts[i].init(ghostPath[i].row, ghostPath[i].col, 0, 255, 255); break;     // Cyan ghost
        case 3: ghosts[i].init(ghostPath[i].row, ghostPath[i].col, 255, 165, 0); break;     // Orange ghost
        case 4: ghosts[i].init(ghostPath[i].row, ghostPath[i].col, 50, 205, 50); break;     // Green ghost
        default: break;
        }
    }

    // Main game loop
    while (getRunning()) {
        handleEvents(); // Handle user input (keyboard, etc.)
        updatePacman(); // Update Pac-Man's movement

        // Update ghosts at a specific rate during gameplay
        if (getFrameCount() % getGhostUpdateRate() == 0 && getcurrentState() == GameState::GAMEPLAY) {
            for (int i = 0; i < getNumGhosts(); i++) {
                ghosts[i].updateGhost();
            }
            setFrameCount(0);
        }

        setFrameCount(getFrameCount() + 1);  // Increment frame counter

        render(); // Render the game objects
        isGameover(); // Check if the game has ended

        // Check if the "EATABLE" state for ghosts has expired
        if (SDL_GetTicks() > GameBoard::ghostEatAbleTime) {
            // Restore original ghost colors
            for (int i = 0; i < getNumGhosts(); i++) {
                switch (i) {
                case 0: ghosts[i].setRGB(255, 0, 0); break;       // Red
                case 1: ghosts[i].setRGB(255, 192, 203); break;   // Pink
                case 2: ghosts[i].setRGB(0, 255, 255); break;     // Cyan
                case 3: ghosts[i].setRGB(255, 165, 0); break;     // Orange
                case 4: ghosts[i].setRGB(50, 205, 50); break;     // Green
                default: break;
                }
            }

            // Adjust ghost speed based on difficulty level
            if (getcurrentLevel() == GameLevel::EASY) {
                setGhostUpdateRate(16);
            }
            else if (getcurrentLevel() == GameLevel::MEDIUM) {
                setGhostUpdateRate(13);
            }
            else if (getcurrentLevel() == GameLevel::HARD) {
                setGhostUpdateRate(10);
            }

            ghostEatAbleTime = 0; // Reset eatable state timer
        }

        SDL_Delay(16); // Maintain a steady frame rate (approx. 60 FPS)
    }

    // Game over state
    setcurrentState(GameState::GAMEOVER);
    render(); // Render final game state
    SDL_Delay(3000); // Pause before exiting
}

/**
 * @brief Checks if the game is over based on the state and position of the ghosts.
 *
 * This function determines if the game should end based on different conditions:
 * 1. If a ghost is in the "EATABLE" state and Pac-Man collides with it, the ghost becomes "DEAD",
 *    moves to a respawn location, and the player earns points.
 * 2. If a ghost is in the "DEAD" state, it is counted towards the total number of defeated ghosts.
 * 3. If a ghost is in any other state and collides with Pac-Man, the game ends.
 * 4. If all ghosts are "DEAD", the game ends.
 */
void GameBoard::isGameover() {
    int NumOfDeadGhosts = 0;

    // Iterate through all ghosts
    for (int i = 0; i < getNumGhosts(); i++) {
        // Check if the ghost is in an eatable state
        if (ghosts[i].getcurrentState() == Ghost::GhostState::EATABLE) {
            // Check if Pac-Man collides with any ghost
            for (int i = 0; i < getNumGhosts(); i++) {
                if (ghosts[i].getGhostLocRow() == getPackmanLocRow() &&
                    ghosts[i].getGhostLocCol() == getPackmanLocCol()) {

                    // Change ghost state to DEAD and move it to a respawn position
                    ghosts[i].setcurrentState(Ghost::GhostState::DEAD);

                    // Set respawn position based on ghost index
                    switch (i) {
                    case 0: ghosts[i].setGhostLocRow(14); ghosts[i].setGhostLocCol(11); addScore(200); break;
                    case 1: ghosts[i].setGhostLocRow(14); ghosts[i].setGhostLocCol(12); addScore(200); break;
                    case 2: ghosts[i].setGhostLocRow(14); ghosts[i].setGhostLocCol(13); addScore(200); break;
                    case 3: ghosts[i].setGhostLocRow(14); ghosts[i].setGhostLocCol(14); addScore(200); break;
                    case 4: ghosts[i].setGhostLocRow(14); ghosts[i].setGhostLocCol(15); addScore(200); break;
                    default: break;
                    }
                }
            }
        }
        // Count ghosts that are in the DEAD state
        else if (ghosts[i].getcurrentState() == Ghost::GhostState::DEAD) {
            NumOfDeadGhosts++;
        }
        // Check if Pac-Man collides with an active (non-eatable) ghost
        else {
            for (int i = 0; i < getNumGhosts(); i++) {
                if (ghosts[i].getGhostLocRow() == getPackmanLocRow() &&
                    ghosts[i].getGhostLocCol() == getPackmanLocCol()) {

                    setRunning(false); // End the game
                    return;
                }
            }
        }
    }

    // If all ghosts are dead, end the game
    if (NumOfDeadGhosts == getNumGhosts()) {
        setRunning(false);
    }
}

/**
 * @brief Cleans up game resources and shuts down SDL.
 *
 * This function destroys the SDL renderer and window,
 * deallocates resources, and quits SDL before displaying a "Game over" message.
 */
void GameBoard::clean() {
    // Destroy the SDL renderer if it exists
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    // Destroy the SDL window if it exists
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    // Quit SDL
    SDL_Quit();

    // Display game over message
    cout << "Game over !" << endl;
}

/**
 * @brief Draws a filled circle on the SDL renderer.
 *
 * This function renders a filled circle at a given position with the specified radius.
 * If the `Packman` flag is true, it renders a Pac-Man shape with an open mouth by omitting
 * pixels in a specific angular range.
 *
 * @param renderer Pointer to the SDL renderer used for drawing.
 * @param centerX The X-coordinate of the circle's center.
 * @param centerY The Y-coordinate of the circle's center.
 * @param radius The radius of the circle.
 * @param Packman If true, draws a Pac-Man shape with an open mouth; otherwise, draws a full circle.
 */
void GameBoard::drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius, bool Packman) {
    // Loop through the bounding box of the circle
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            // Calculate the relative position of the pixel to the circle's center
            int dx = radius - w;
            int dy = radius - h;

            // Check if the pixel falls within the circle's boundary using the equation (x^2 + y^2 <= r^2)
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                if (Packman) {
                    // Calculate the angle of the current pixel relative to the circle's center
                    float angle = atan2(dy, dx) * 180 / M_PI;

                    // Remove pixels in the mouth area (around 60° opening)
                    if (angle < -30 || angle > 30) {
                        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow for Pac-Man
                        SDL_RenderPoint(renderer, centerX + dx, centerY + dy);
                    }
                }
                else {
                    // Draw a full white circle
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderPoint(renderer, centerX + dx, centerY + dy);
                }
            }
        }
    }
}
