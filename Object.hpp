#pragma once
#include <SDL2/SDL.h>

typedef enum {
  PAWN,
  NIGHT,
  BISHOP,
  ROOK,
  QUEEN,
  KING,
  NONE,
  BOARD,
} Type;

typedef enum {
  WHITE,
  BLACK,
  BLANK,
} Side;

class Object {
public:
  Object(SDL_Texture *tex, int x, int y, Type type, Side isWhite);
  Object(SDL_Texture *tex, int x, int y, int w, int h);
  ~Object();

  void render();
  void update();
  void clean();
  void changePosition(int x, int y);
  Type type;
  Side side;
  SDL_Texture *objTexture;

private:
  int xpos, ypos, width, height;
  SDL_Rect src, dest;
};
