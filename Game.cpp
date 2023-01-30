#include "Game.hpp"
#include <cstdio>

Board *board;

SDL_Renderer *Game::gRenderer = nullptr;

Game::Game() {}

Game::~Game() {}

void Game::init(const char *name, int x, int y, int w, int h) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL Could not be created, SDL ERROR: %s\n", SDL_GetError());

  } else {
    gWindow = SDL_CreateWindow(name, x, y, w, h, SDL_WINDOW_SHOWN);

    if (gWindow == NULL) {
      printf("SDL Could not be created, SDL ERROR: %s\n", SDL_GetError());

    } else {
      gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

      if (gRenderer == NULL) {
        printf("SDL Could not be created, SDL ERROR: %s\n", SDL_GetError());

      } else {
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        isRunning = true;
      }
    }
  }
  board = new Board();
  board->initBoard();
}

void Game::handleEvents() {
  SDL_Event event;
  SDL_PollEvent(&event);
  switch (event.type) {
  case SDL_QUIT:
    isRunning = false;
    break;
  case SDL_MOUSEBUTTONDOWN: {
    isDragging = true;
    int idk = Board::calcPosition(event.motion.x, event.motion.y);
    printf("%d\n", idk);
    break;
  }
  default:
    break;
  }
}

void Game::update() { board->updateBoard(); }

void Game::render() {
  SDL_RenderClear(gRenderer);
  board->RenderBoard();
  SDL_RenderPresent(gRenderer);
}

void Game::clean() {
  SDL_DestroyWindow(gWindow);
  board->cleanBoard();
  SDL_DestroyRenderer(gRenderer);

  gWindow = NULL;
  gRenderer = NULL;

  SDL_Quit();
}
