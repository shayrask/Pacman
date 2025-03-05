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

class GameBoard;

class Ghost
{
private:
	static SDL_Window* win;
	static SDL_Renderer* renderer;
	struct Position {
		int row;
		int col;
	};

	struct Node {
		int row, col;
		vector<Position> path;
	};

	Position ghostPosition;
	int R, G, B;
	

public:
	enum class GhostState {
		ALIVE,
		DEAD,
		EATABLE
	};

	Ghost();
	~Ghost();

	void init(int row, int col, int R, int G, int B);
	static const int GHOST_SIZE = 10;
	static int ghost[GHOST_SIZE][GHOST_SIZE];
	static vector<Position> Possibledestinations;
	Position target;
	Position deadPosition;
	vector<Position> ghostCourse;
	GhostState currentState;


	void updateGhost();
	Position newDestination();
	void renderGhost(int x, int y);
	vector<Position> findPath(Position start, Position end);

	static int getGhostSize() { return GHOST_SIZE; }
    int getGhostLocRow() { return this->ghostPosition.row; }
	int getGhostLocCol() { return this->ghostPosition.col; }
	int getR() { return this->R; }
	int getG() { return this->G; }
	int getB() { return this->B; }
	GhostState getcurrentState() { return currentState; }
	
	void setLoc(int row, int col) { this->ghostPosition.row = row; this->ghostPosition.col = col; }
	void setGhostLocRow(int row) { this->ghostPosition.row = row; }
	void setGhostLocCol(int col) { this->ghostPosition.col = col; }
	void setR(int R) { this->R = R; }
	void setG(int G) { this->G = G; }
	void setB(int B) { this->B = B; }
	void setRGB(int R, int G, int B) { this->R = R; this->G = G; this->B = B; }
	void setcurrentState(GhostState newState) { currentState = newState; }
};


