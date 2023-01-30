#include "Board.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

  static SDL_Renderer *gRenderer;

private:
  bool isRunning;
  bool isDragging = false;
  SDL_Window *gWindow;
};
