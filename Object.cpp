#include "Object.hpp"
#include "Game.hpp"
#include <cstdio>

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
  /* int pos = xpos / 100 + (ypos / 100) * 8; */
  /* printf("pos: %d x: %d y: %d \n", pos, xpos, ypos); */

  /* if (pos % 2 == 0) { */
  /*   SDL_SetRenderDrawColor(Game::gRenderer, 60, 60, 60, 60); */
  /*   SDL_RenderFillRect(Game::gRenderer, &dest); */
  /* } else { */
  /*   SDL_SetRenderDrawColor(Game::gRenderer, 160, 160, 160, 160); */
  /*   SDL_RenderFillRect(Game::gRenderer, &dest); */
  /* } */
  SDL_RenderCopy(Game::gRenderer, objTexture, NULL, &dest);
}

void Object::clean() { SDL_DestroyTexture(objTexture); }

void Object::changePosition(int x, int y) {
  xpos = x;
  ypos = y;
}
