#pragma once
#include <SDL2/SDL.h>

typedef enum {
  PAWN,
  NIGHT,
  BISHOP,
  ROOK,
  QUEEN,
  KING,
  BLANK,
  BOARD,
} Type;

class Object {
public:
  Object(SDL_Texture *tex, int x, int y, Type type, bool isWhite);
  Object(SDL_Texture *tex, int x, int y, int w, int h);
  ~Object();

  void render();
  void update();
  void clean();

private:
  int xpos, ypos, width, height;
  SDL_Texture *objTexture;
  SDL_Rect src, dest;
  Type type;
  bool isWhite;
};
