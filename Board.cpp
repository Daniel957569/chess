#include "Board.hpp"
#include "Game.hpp"
#include "Object.hpp"
#include <cstdio>

Object *chessBoard;

Board::Board() {}
Board::~Board() {}

void Board::initBoard() {
  printf("initBefore\n");

  SDL_Texture *cBoard = IMG_LoadTexture(Game::gRenderer, "../images/board.png");
  SDL_Texture *bRook = IMG_LoadTexture(Game::gRenderer, "../images/br.png");
  SDL_Texture *bNight = IMG_LoadTexture(Game::gRenderer, "../images/bn.png");
  SDL_Texture *bBishop = IMG_LoadTexture(Game::gRenderer, "../images/bb.png");
  SDL_Texture *bQueen = IMG_LoadTexture(Game::gRenderer, "../images/bq.png");
  SDL_Texture *bKing = IMG_LoadTexture(Game::gRenderer, "../images/bk.png");
  SDL_Texture *bPawn = IMG_LoadTexture(Game::gRenderer, "../images/bp.png");
  SDL_Texture *wRook = IMG_LoadTexture(Game::gRenderer, "../images/wr.png");
  SDL_Texture *wNight = IMG_LoadTexture(Game::gRenderer, "../images/wn.png");
  SDL_Texture *wBishop = IMG_LoadTexture(Game::gRenderer, "../images/wb.png");
  SDL_Texture *wQueen = IMG_LoadTexture(Game::gRenderer, "../images/wq.png");
  SDL_Texture *wKing = IMG_LoadTexture(Game::gRenderer, "../images/wk.png");
  SDL_Texture *wPawn = IMG_LoadTexture(Game::gRenderer, "../images/wp.png");

  board.push_back(makePiece(bRook, 0, 0, Type::ROOK, false));
  board.push_back(makePiece(bNight, 100, 0, Type::NIGHT, false));
  board.push_back(makePiece(bBishop, 200, 0, Type::BISHOP, false));
  board.push_back(makePiece(bQueen, 300, 0, Type::QUEEN, false));
  board.push_back(makePiece(bKing, 400, 0, Type::KING, false));
  board.push_back(makePiece(bBishop, 500, 0, Type::BISHOP, false));
  board.push_back(makePiece(bNight, 600, 0, Type::NIGHT, false));
  board.push_back(makePiece(bBishop, 700, 0, Type::ROOK, false));

  int x = 0;
  for (int i = 0; i < 8; i++) {
    board.push_back(makePiece(bPawn, x, 100, Type::PAWN, false));
    x += 100;
  }

  x = 0;
  int y = 200;
  for (int i = 0; i < 32; i++) {
    if (i % 8 == 0 && i != 0) {
      y += 100;
      x = 0;
    }
    board.push_back(makePiece(NULL, x, y, Type::BLANK, true));
    x += 100;
  }

  x = 0;
  for (int i = 0; i < 8; i++) {
    board.push_back(makePiece(wPawn, x, 600, Type::PAWN, true));
    x += 100;
  }

  board.push_back(makePiece(wRook, 0, 700, Type::ROOK, true));
  board.push_back(makePiece(wNight, 100, 700, Type::NIGHT, true));
  board.push_back(makePiece(wBishop, 200, 700, Type::BISHOP, true));
  board.push_back(makePiece(wQueen, 300, 700, Type::QUEEN, true));
  board.push_back(makePiece(wKing, 400, 700, Type::KING, true));
  board.push_back(makePiece(wBishop, 500, 700, Type::BISHOP, true));
  board.push_back(makePiece(wNight, 600, 700, Type::NIGHT, true));
  board.push_back(makePiece(wRook, 700, 700, Type::ROOK, true));

  chessBoard = new Object(cBoard, 0, 0, 800, 800);
}

void Board::updateBoard() {
  chessBoard->update();

  for (int i = 0; i < 64; i++) {
    board[i]->update();
  }
}

void Board::RenderBoard() {
  chessBoard->render();

  for (int i = 0; i < 64; i++) {
    board[i]->render();
  }
}

void Board::cleanBoard() {
  chessBoard->clean();

  for (int i = 0; i < 64; i++) {
    board[i]->clean();
  }
}

int Board::calcPosition(int x, int y) {
  x = x / 100;
  y = y / 100;
  return y * 8 + x;
}

Object *Board::makePiece(SDL_Texture *tex, int x, int y, Type pieceType,
                         bool isWhite) {
  return new Object(tex, x, y, pieceType, isWhite);
}
