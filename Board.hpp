#pragma once
#include "Object.hpp"
#include <vector>

class Board {
public:
  Board();
  ~Board();

  void initBoard();
  void updateBoard();
  void RenderBoard();
  void cleanBoard();

  static int calcPosition(int x, int y);

private:
  Object *makePiece(SDL_Texture *tex, int x, int y, Type pieceType,
                    bool isWhite);
  std::vector<Object *> board;
};
