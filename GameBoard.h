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

class Ghost;

class GameBoard
{
private:
	static SDL_Window* window;
	static SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_FRect startButton;
	SDL_FRect gameOverButton;
	Uint32 lastToggleTime = 0;
	const Uint32 TOGGLE_INTERVAL = 250;
	bool isRunning;
	bool pacmanOpen = true;
	int x, y;
	static float width, height;
	int randNum;
	int packmanLocRow, packmanLocCol;
	int packmanState = 1;               // 1 = Right, 2 = left , 3 = up, 4 = down
	int pacmanUpdateRate = 4; 
	int eventHandleRate = 4;
	int ghostUpdateRate = 16;
	int frameCount = 1;

	enum class GameState {
		MAIN_MENU,
		GAMEPLAY,
		GAMEOVER
	};

	enum Direction {
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	struct Position {
		int row;
		int col;
	};

	GameState currentState;

	void renderMenu();
	void renderGame();
	void renderGameOver();
	void renderPacman(int x, int y, bool mouthOpen, int pacState);

	void setcurrentState(GameState newState) { currentState = newState; };
	GameState getcurrentState() { return currentState; };



public:
	GameBoard();
	~GameBoard();

	static const int MAZE_ROWS = 31;
	static const int MAZE_COLS = 28;
	static int maze[MAZE_ROWS][MAZE_COLS];
	static const int PACMAN_SIZE = 10;
	static int pacmanClosed[PACMAN_SIZE][PACMAN_SIZE];
	static int pacmanOpenRight[PACMAN_SIZE][PACMAN_SIZE];
	static int pacmanOpenLeft[PACMAN_SIZE][PACMAN_SIZE];
	static int pacmanOpenUp[PACMAN_SIZE][PACMAN_SIZE];
	static int pacmanOpenDown[PACMAN_SIZE][PACMAN_SIZE];
	vector<Position> ghostPath = { {13,11}, {13,12}, {13, 13}, {13, 14}, {13,15}, {13,16},
										{14,11}, {14,12}, {14, 13}, {14, 14}, {14,15}, {14,16},
										{15,11}, {15,12}, {15, 13}, {15, 14}, {15,15}, {15,16}
	};
	Ghost* ghostY;
	Ghost* ghostP;
	Ghost* ghostR;



	void handleEvents();
	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void clean();
	void render();
	void updatePacman();
	void run();
	static bool isValidMove(int row, int col);

	static SDL_Window* getWindow() { return window; }
	static SDL_Renderer* getRenderer() { return renderer; }
	bool getRunning() { return isRunning; }
	static float getWidth() { return width; }
	static float getHeight() { return height; }
	static int getMazeRows() { return MAZE_ROWS; }
	static int getMazeCols() { return MAZE_COLS; }
	int getPackmanSize() { return PACMAN_SIZE; }
	int getPackmanLocRow() { return packmanLocRow; }
	int getPackmanLocCol() { return packmanLocCol; }
	int getRandNum() { return randNum; }
	bool getPackmanOpen() { return pacmanOpen; }
	int getPackmanState() { return packmanState; }
	int getFrameCount() { return frameCount; }
	int getEventHandleRate() { return eventHandleRate; }
	int getPacmanUpdateRate() { return pacmanUpdateRate; }
	int getGhostUpdateRate() { return ghostUpdateRate; }
	
	
    void setPackmanLocRow(int row) { packmanLocRow = row; }
	void setPackmanLocCol(int col) { packmanLocCol = col; }
	void setPacLoc(int row, int col) { packmanLocRow = row; packmanLocCol = col; }
    void setRunning(bool running) { isRunning = running; }
	void setRandNum(int num) { randNum = num; }
	void setPackmanOpen(bool state) { pacmanOpen = state; }
	void setPackmanState(int state) { packmanState = state; }
	void setFrameCount(int count) { frameCount = count; }
	void setWidth(int w) { width = w; }
	void setHeight(int h) { height = h; }
	

    void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius, bool Packman);

	void shuffleGhosts() {
		random_device rd;
		mt19937 g(rd());
		shuffle(ghostPath.begin(), ghostPath.end(), g);
	}
};