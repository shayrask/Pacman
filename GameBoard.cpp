#include "GameBoard.h"
#include <iostream>
#include "Ghost.h"
#define M_PI 3.14159265358979323846

using namespace std;

SDL_Window* GameBoard::window;
SDL_Renderer* GameBoard::renderer;
float GameBoard::width;
float GameBoard::height;


GameBoard::GameBoard() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("PackMan Game", 868, 775, NULL);
	setWidth(868);
	setHeight(775);   
    renderer = SDL_CreateRenderer(window, NULL);
    setRunning(true);
    srand(time(0));
	setRandNum(rand() % 61);

    ghostY = new Ghost();
    ghostP = new Ghost();
    ghostR = new Ghost();
}

GameBoard::~GameBoard() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    delete ghostY;
    delete ghostP;
    delete ghostR;
}



void GameBoard::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            setRunning(false);
        }
        if (getcurrentState() == GameState::MAIN_MENU && event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            int x = event.button.x;
            int y = event.button.y; 

            if (x >= startButton.x && x <= (startButton.x + startButton.w) &&
                y >= startButton.y && y <= (startButton.y + startButton.h)) {
				setcurrentState(GameState::GAMEPLAY);   
				render();
            }
		}
        else if (getcurrentState() == GameState::GAMEPLAY && event.type == SDL_EVENT_KEY_DOWN) {
            switch (event.key.scancode) {
            case SDL_SCANCODE_UP:
				if (isValidMove(getPackmanLocRow() - 1, getPackmanLocCol()))
				{
					setPackmanLocRow(getPackmanLocRow() - 1);
					setPackmanState(3);
					maze[getPackmanLocRow()][getPackmanLocCol()] = 5;
				}
                break;
            case SDL_SCANCODE_DOWN:
				if (isValidMove(getPackmanLocRow() + 1, getPackmanLocCol()))
                {
                    setPackmanLocRow(getPackmanLocRow() + 1);
                    setPackmanState(4);
                    maze[getPackmanLocRow()][getPackmanLocCol()] = 5;
                }
                break;
            case SDL_SCANCODE_LEFT:
                if (getPackmanLocCol() == 0)
                {
                    setPackmanLocCol(getMazeCols() - 1);
                    setPackmanState(2);
                    maze[getPackmanLocRow()][getPackmanLocCol()] = 5;
                }
                else if (isValidMove(getPackmanLocRow(), getPackmanLocCol() - 1))
                {
                    setPackmanLocCol(getPackmanLocCol() - 1);
                    setPackmanState(2);
                    maze[getPackmanLocRow()][getPackmanLocCol()] = 5;
                }
                break;
            case SDL_SCANCODE_RIGHT:
                if (getPackmanLocCol() == getMazeCols() - 1) {
                    setPackmanLocCol(0);
                    setPackmanState(1);
                    maze[getPackmanLocRow()][getPackmanLocCol()] = 5;
                }
                else if (isValidMove(getPackmanLocRow(), getPackmanLocCol() + 1))
                {
                    setPackmanLocCol(getPackmanLocCol() + 1);
                    setPackmanState(1);
                    maze[getPackmanLocRow()][getPackmanLocCol()] = 5;
                }
                break;
            }
        }
    }
}

void GameBoard::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    switch (currentState) {
    case GameState::MAIN_MENU:
        renderMenu();
        break;
    case GameState::GAMEPLAY:
        renderGame();
        break;
    case GameState::GAMEOVER:
        renderGameOver();
        break;
    }

    SDL_RenderPresent(renderer);
}

void GameBoard::renderMenu() {
   SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  
   SDL_RenderClear(renderer);  

   SDL_Texture* buttonTexture = IMG_LoadTexture(renderer, "start-button.png");
   if (!buttonTexture) {
       printf("Error loading image\n");
   }

   startButton = { (getWidth() / 2) - 100, (getHeight() / 2) - 50, 200, 100};
   SDL_RenderTexture(renderer, buttonTexture, NULL, &startButton);

   SDL_RenderPresent(renderer);  
} 

void GameBoard::renderGameOver() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Texture* gameOverTexture = IMG_LoadTexture(renderer, "GameOver.png");
    if (!gameOverTexture) {
        printf("Error loading image\n");
    }

    gameOverButton = { (getWidth() / 2) - 150, (getHeight() / 2) - 100, 300, 200 };
    SDL_RenderTexture(renderer, gameOverTexture, NULL, &gameOverButton);

    SDL_RenderPresent(renderer);
}

void GameBoard::renderGame() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int cellSizeX = width / getMazeCols();
    int cellSizeY = height / getMazeRows();

    for (int row = 0; row < getMazeRows(); row++) {
        for (int col = 0; col < getMazeCols(); col++) {
            SDL_FRect cellRect = { col * cellSizeX, row * cellSizeY, cellSizeX, cellSizeY };

            if (maze[row][col] == 1) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderFillRect(renderer, &cellRect);
            }
            else if (maze[row][col] == 0) {
				Ghost::Possibledestinations.push_back({ row, col });
                if (getRandNum() == 0)
                {
                    renderPacman(row, col, getPackmanOpen(), getPackmanState());
					setPacLoc(row, col);
					maze[row][col] = 5;
                }
                else if (row == getPackmanLocRow() && col == getPackmanLocCol()) {
					renderPacman(row, col, getPackmanOpen(), getPackmanState());
                }
                else
                {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                    int centerX = cellRect.x + cellRect.w / 2;
                    int centerY = cellRect.y + cellRect.h / 2;
                    int dotRadius = cellRect.w / 8;

                    drawCircle(renderer, centerX, centerY, dotRadius, false);
                }
                setRandNum(getRandNum() - 1);
            }
            else if (maze[row][col] == 3)
            {
                Ghost::Possibledestinations.push_back({ row, col });
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                int centerX = cellRect.x + cellRect.w / 2;
                int centerY = cellRect.y + cellRect.h / 2;
                int dotRadius = cellRect.w / 4;

                drawCircle(renderer, centerX, centerY, dotRadius, false);
            }
            else if (maze[row][col] == 5 && row == getPackmanLocRow() && col == getPackmanLocCol())
            {
                renderPacman(row, col, getPackmanOpen(), getPackmanState());
                Ghost::Possibledestinations.push_back({ row, col });
            }
			if (row == ghostY->getGhostLocRow() && col == ghostY->getGhostLocCol())
			{
				ghostY->renderGhost(row, col);
                Ghost::Possibledestinations.push_back({ row, col });
			}
			else if (row == ghostR->getGhostLocRow() && col == ghostR->getGhostLocCol())
			{
				ghostR->renderGhost(row, col);
                Ghost::Possibledestinations.push_back({ row, col });
			}
			else if (row == ghostP->getGhostLocRow() && col == ghostP->getGhostLocCol())
			{
				ghostP->renderGhost(row, col);
                Ghost::Possibledestinations.push_back({ row, col });
			}
        }
    }
    SDL_RenderPresent(renderer);
}

void GameBoard::renderPacman(int row, int col, bool mouthOpen, int pacState) {
    int cellSizeX = width / getMazeCols();
    int cellSizeY = height / getMazeRows();

    int pixelSizeX = cellSizeX / PACMAN_SIZE;
    int pixelSizeY = cellSizeY / PACMAN_SIZE;

    int (*pacmanShape)[PACMAN_SIZE];

    if (mouthOpen) {
        switch (pacState) {
        case 1:  
            pacmanShape = pacmanOpenRight;
            break;
        case 2:  
            pacmanShape = pacmanOpenLeft;
            break;
        case 3:  
            pacmanShape = pacmanOpenUp;
            break;
        case 4:  
            pacmanShape = pacmanOpenDown;
            break;
        default:
            pacmanShape = pacmanClosed; 
        }
    }
    else {
        pacmanShape = pacmanClosed; 
    }


    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); 

    
    int startX = col * cellSizeX + (cellSizeX - PACMAN_SIZE * pixelSizeX) / 2;
    int startY = row * cellSizeY + (cellSizeY - PACMAN_SIZE * pixelSizeY) / 2;

    for (int r = 0; r < PACMAN_SIZE; r++) {
        for (int c = 0; c < PACMAN_SIZE; c++) {
            if (pacmanShape[r][c] == 1) {
                SDL_FRect pixel = { startX + c * pixelSizeX, startY + r * pixelSizeY, pixelSizeX, pixelSizeY };
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }
}

void GameBoard::updatePacman() {
    Uint32 currentTime = SDL_GetTicks(); 
    if (currentTime - lastToggleTime >= TOGGLE_INTERVAL) {
        setPackmanOpen((getPackmanOpen() == true) ? false : true);
        lastToggleTime = currentTime; 
    }
}

bool GameBoard::isValidMove(int newX, int newY) {
	return maze[newX][newY] != 1;
}

void GameBoard::run() {
    renderMenu();
	shuffleGhosts();
	ghostR->init(ghostPath[0].row, ghostPath[0].col, 255, 0, 0);
	ghostP->init(ghostPath[1].row, ghostPath[1].col, 255, 192, 203);
	ghostY->init(ghostPath[2].row, ghostPath[2].col, 0, 255, 255);

    while (getRunning()) {
        handleEvents();
        updatePacman();

        if (getFrameCount() % getGhostUpdateRate() == 0 && getcurrentState() == GameState::GAMEPLAY)
        {
			ghostP->updateGhost();
			ghostR->updateGhost();
			ghostY->updateGhost();
			setFrameCount(0);
        }
		setFrameCount(getFrameCount() + 1); 
        render();
        SDL_Delay(16); 
    }
    setcurrentState(GameState::GAMEOVER);
	render();
}

void GameBoard::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    
}

void GameBoard::clean() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
    cout << "Game over !" << endl;
}

void GameBoard::drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius, bool Packman) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                if (Packman)
                {
                    float angle = atan2(dy, dx) * 180 / M_PI;
                    if (angle < -30 || angle > 30) { 
                        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                        SDL_RenderPoint(renderer, centerX + dx, centerY + dy);
                    }
                }
                else
                {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderPoint(renderer, centerX + dx, centerY + dy);
                }
            }
        }
    }
}