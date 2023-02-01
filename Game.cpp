#include "Game.hpp"
#include <cstdio>
#include <vector>

Board *chessboard;

Object *squareDragged;
int fromPos, toPos;

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
  chessboard = new Board();
  chessboard->initBoard();
}

void Game::handleEvents() {

  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      isRunning = false;
      return;
    case SDL_MOUSEBUTTONDOWN: {
      isDragging = true;
      fromPos = chessboard->calcPosition(event.motion.x, event.motion.y);
      squareDragged = chessboard->board[fromPos];
      if ((isWhiteTurn && squareDragged->side != WHITE) ||
          (!isWhiteTurn) && squareDragged->side != BLACK) {
        squareDragged = NULL;
      }

      return;
    }
    case SDL_MOUSEBUTTONUP: {
      isDragging = false;
      if (squareDragged == NULL) {
        break;
      }
      int x = (event.motion.x / 100) * 100;
      int y = (event.motion.y / 100) * 100;

      toPos = chessboard->calcPosition(x, y);

      if (chessboard->makeMove(fromPos, toPos, x, y, isWhiteTurn)) {
        isWhiteTurn = !isWhiteTurn;
      } else {
        chessboard->board[fromPos]->changePosition((fromPos % 8) * 100,
                                                   (fromPos / 8) * 100);
      }

      squareDragged = NULL;

      return;
    }
    case SDL_MOUSEMOTION:
      if (isDragging && squareDragged != NULL) {

        squareDragged->changePosition(event.motion.x - 50, event.motion.y - 50);
      }
      return;
    default:
      return;
    }
  }
}

void Game::update() { chessboard->updateBoard(); }

void Game::render() {
  SDL_RenderClear(gRenderer);
  chessboard->RenderBoard();
  SDL_RenderPresent(gRenderer);
}

void Game::clean() {
  SDL_DestroyWindow(gWindow);
  chessboard->cleanBoard();
  SDL_DestroyRenderer(gRenderer);

  gWindow = NULL;
  gRenderer = NULL;

  SDL_Quit();
}
