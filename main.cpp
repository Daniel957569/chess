#include "Game.hpp"
#include <SDL2/SDL_video.h>
#include <cstdio>

Game *game = NULL;

int main(int argc, char *argv[]) {
  const int FPS = 60;
  const int frameDelay = 1000 / FPS;

  Uint32 frameStart;
  int frameTime;

  game = new Game();
  game->init("chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800);

  while (game->running()) {
    frameStart = SDL_GetTicks();

    game->handleEvents();
    game->update();
    game->render();

    frameTime = SDL_GetTicks() - frameStart;
    if (frameDelay > frameTime) {
      if (game->dragging()) {
        SDL_Delay(1);
      } else {
        SDL_Delay(frameDelay - frameTime);
      }
    }
  }

  game->clean();

  return 0;
}
