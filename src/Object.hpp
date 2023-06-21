#pragma once
#include <SDL2/SDL.h>

typedef enum {
  PAWN,
  KNIGHT,
  BISHOP,
  ROOK,
  QUEEN,
  KING,
  NONE,
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
  void markSquare();
  void changePosition(int x, int y);
  Type type;
  Side side;
  SDL_Texture *objTexture;
  bool isMarked = false;

private:
  int xpos, ypos, width, height;
  SDL_Rect dest;
};
