#ifndef board_game
#define board_game

#include "Board.hpp"
#include "Debug.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <type_traits>

class Game {
public:
  Game();
  ~Game();

  void handleEvents();
  void init(const char *name, int x, int y, int w, int h);
  void render();
  void update();
  void clean();
  bool running() { return isRunning; }
  bool dragging() { return isDragging; }
  bool whiteTurn() { return isWhiteTurn; }

  static SDL_Renderer *gRenderer;

private:
  bool isWhiteTurn = true;
  bool isRunning;
  bool isDragging = false;
  SDL_Window *gWindow;
};

#endif
