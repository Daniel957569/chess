#include "Object.hpp"
#include "Game.hpp"

Object::Object(SDL_Texture *tex, int x, int y, Type pieceType, Side isItWhite) {
  objTexture = tex;

  height = 100;
  width = 100;
  xpos = x;
  ypos = y;
  type = pieceType;
  side = isItWhite;
}

Object::Object(SDL_Texture *tex, int x, int y, int w, int h) {
  objTexture = tex;

  height = h;
  width = w;
  xpos = x;
  ypos = y;
}

void Object::update() {
  src.h = 0;
  src.w = 0;
  src.x = src.y = 0;

  dest.x = xpos;
  dest.y = ypos;
  dest.h = height;
  dest.w = width;
}

void Object::render() {
  SDL_RenderCopy(Game::gRenderer, objTexture, NULL, &dest);
}

void Object::clean() { SDL_DestroyTexture(objTexture); }

void Object::changePosition(int x, int y) {
  xpos = x;
  ypos = y;
}
