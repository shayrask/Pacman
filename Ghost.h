#pragma once
#include "GameBoard.h"
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

class Ghost
{
private:
	SDL_Window* win = GameBoard::getWindow();
	SDL_Renderer* renderer = GameBoard::getRenderer();
	struct Position {
		int row;
		int col;
	};

	Position ghostPosition;
	int R, G, B;
	

public:
	Ghost();
	~Ghost();

	void init(int row, int col, int R, int G, int B);
	static const int GHOST_SIZE = 10;
	static int ghost[GHOST_SIZE][GHOST_SIZE];
	static vector<Position> destination;
	vector<Position> ghostCourse;
	void updateGhost();
	Position newDestination();
	void renderGhost(int x, int y);

	static int getGhostSize() { return GHOST_SIZE; }
    int getGhostLocRow() { return this->ghostPosition.row; }
	int getGhostLocCol() { return this->ghostPosition.col; }
	int getR() { return this->R; }
	int getG() { return this->G; }
	int getB() { return this->B; }
	
	void setLoc(int row, int col) { this->ghostPosition.row = row; this->ghostPosition.col = col; }
	void setGhostLocRow(int row) { this->ghostPosition.row = row; }
	void setGhostLocCol(int col) { this->ghostPosition.col = col; }
};


