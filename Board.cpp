#include "Board.hpp"
#include "Game.hpp"
#include "Object.hpp"
#include <SDL2/SDL_timer.h>
#include <cstdio>
#include <vector>

Object *textureBoard;

Board::Board() {}
Board::~Board() {}

void Board::initBoard() {

  cBoard = IMG_LoadTexture(Game::gRenderer, "../images/board.png");
  bRook = IMG_LoadTexture(Game::gRenderer, "../images/br.png");
  bNight = IMG_LoadTexture(Game::gRenderer, "../images/bn.png");
  bBishop = IMG_LoadTexture(Game::gRenderer, "../images/bb.png");
  bQueen = IMG_LoadTexture(Game::gRenderer, "../images/bq.png");
  bKing = IMG_LoadTexture(Game::gRenderer, "../images/bk.png");
  bPawn = IMG_LoadTexture(Game::gRenderer, "../images/bp.png");
  wRook = IMG_LoadTexture(Game::gRenderer, "../images/wr.png");
  wNight = IMG_LoadTexture(Game::gRenderer, "../images/wn.png");
  wBishop = IMG_LoadTexture(Game::gRenderer, "../images/wb.png");
  wQueen = IMG_LoadTexture(Game::gRenderer, "../images/wq.png");
  wKing = IMG_LoadTexture(Game::gRenderer, "../images/wk.png");
  wPawn = IMG_LoadTexture(Game::gRenderer, "../images/wp.png");

  textureBoard = new Object(cBoard, 0, 0, 800, 800);
  /* renderBlackSide(0); */
  /* renderPawn(8, 8, 0, 100, false); */

  /* renderBlank(16, 32, 0, 200); */

  /* renderPawn(48, 8, 0, 600, true); */
  /* renderWhiteSide(56); */
  renderFEN("rnbqkbnr/4p3/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R");
}

void Board::updateBoard() {
  textureBoard->update();
  for (int i = 0; i < 64; i++) {
    board[i]->update();
  }
}

void Board::RenderBoard() {
  textureBoard->render();

  for (int i = 0; i < 64; i++) {
    board[i]->render();
  }
}

void Board::cleanBoard() {
  textureBoard->clean();

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

void Board::renderWhiteSide(int index) {
  board[index] = makePiece(wRook, 0, 700, Type::ROOK, true);
  board[index + 1] = makePiece(wNight, 100, 700, Type::NIGHT, true);
  board[index + 2] = makePiece(wBishop, 200, 700, Type::BISHOP, true);
  board[index + 3] = makePiece(wQueen, 300, 700, Type::QUEEN, true);
  board[index + 4] = makePiece(wKing, 400, 700, Type::KING, true);
  board[index + 5] = makePiece(wBishop, 500, 700, Type::BISHOP, true);
  board[index + 6] = makePiece(wNight, 600, 700, Type::NIGHT, true);
  board[index + 7] = makePiece(wRook, 700, 700, Type::ROOK, true);
}

void Board::renderBlackSide(int index) {
  board[index] = makePiece(bRook, 0, 0, Type::ROOK, false);
  board[index + 1] = makePiece(bNight, 100, 0, Type::NIGHT, false);
  board[index + 2] = makePiece(bBishop, 200, 0, Type::BISHOP, false);
  board[index + 3] = makePiece(bQueen, 300, 0, Type::QUEEN, false);
  board[index + 4] = makePiece(bKing, 400, 0, Type::KING, false);
  board[index + 5] = makePiece(bBishop, 500, 0, Type::BISHOP, false);
  board[index + 6] = makePiece(bNight, 600, 0, Type::NIGHT, false);
  board[index + 7] = makePiece(bRook, 700, 0, Type::ROOK, false);
}

void Board::renderPawn(int index, int amount, int x, int y, bool isWhite) {
  auto pawn = isWhite ? wPawn : bPawn;

  for (int i = index; i < index + amount; i++) {
    board[i] = makePiece(pawn, x, y, Type::PAWN, isWhite);
    x += 100;
  }
}

void Board::renderBlank(int index, int amount, int x, int y) {
  for (int i = index; i < index + amount; i++) {
    if (i % 8 == 0 && i != 0) {
      y += 100;
      x = 0;
    }
    board[i] = makePiece(NULL, x, y, Type::BLANK, true);
    x += 100;
  }
}

void Board::renderFEN(std::string FEN) {
  bool isOk = true;
  int i = 0;
  int count = 0;
  while (isOk) {
    int y = (count / 8) * 100;
    int x = (count % 8) * 100;
    switch (FEN[i]) {
    case 'r':
      board[count] = makePiece(bRook, x, y, Type::ROOK, false);
      break;
    case 'n':
      board[count] = makePiece(bNight, x, y, Type::NIGHT, false);
      break;
    case 'b':
      board[count] = makePiece(bBishop, x, y, Type::BISHOP, false);
      break;
    case 'q':
      board[count] = makePiece(bQueen, x, y, Type::QUEEN, false);
      break;
    case 'k':
      board[count] = makePiece(bKing, x, y, Type::KING, false);
      break;
    case 'p':
      board[count] = makePiece(bPawn, x, y, Type::PAWN, false);
      break;
    case 'R':
      board[count] = makePiece(wRook, x, y, Type::ROOK, true);
      break;
    case 'N':
      board[count] = makePiece(wNight, x, y, Type::NIGHT, true);
      break;
    case 'B':
      board[count] = makePiece(wBishop, x, y, Type::BISHOP, true);
      break;
    case 'Q':
      board[count] = makePiece(wQueen, x, y, Type::QUEEN, true);
      break;
    case 'K':
      board[count] = makePiece(wKing, x, y, Type::KING, true);
      break;
    case 'P':
      board[count] = makePiece(wPawn, x, y, Type::PAWN, true);
      break;
    case ' ':
      // for now fix later please
      isOk = false;
    default:
      if (FEN[i] >= '0' && FEN[i] <= '9') {
        int times = FEN[i] - 48;
        for (int i = 0; i < times; i++) {
          renderBlank(count, 1, x, y);
          x += 100;
          count++;
        }
      }
      i++;
      continue;
    }
    count++;
    i++;
  }
}

bool Board::makeMove(int from, int to, int x, int y) {
  bool isLegal = false;
  auto fromSqaure = board[from];
  auto toSqaure = board[to];

  switch (fromSqaure->type) {
  case Type::PAWN:
    if (fromSqaure->isWhite) {
      isLegal = checkDownPawn(from, to, x, y);
    } else {
      isLegal = checkUpPawn(from, to, x, y);
    }
    break;
  case Type::NIGHT:
    if (fromSqaure->isWhite) {
      isLegal = checkDownNight(from, to, x, y);
    } else {
      isLegal = checkUpNight(from, to, x, y);
    }
    break;
  case Type::BISHOP:
    if (fromSqaure->isWhite) {
      isLegal = checkDownBishop(from, to, x, y);
    } else {
      isLegal = checkUpBishop(from, to, x, y);
    }
    break;
  case Type::ROOK:
    if (fromSqaure->isWhite) {
      isLegal = checkDownRook(from, to, x, y);
    } else {
      isLegal = checkUpRook(from, to, x, y);
    }
    break;
  case Type::QUEEN:
    if (fromSqaure->isWhite) {
      isLegal = checkDownQueen(from, to, x, y);
    } else {
      isLegal = checkUpQueen(from, to, x, y);
    }
    break;
  case Type::KING:
    if (fromSqaure->isWhite) {
      isLegal = checkDownKing(from, to, x, y);
    } else {
      isLegal = checkUpKing(from, to, x, y);
    }
    break;
  default:
    break;
  }
  return isLegal;
}

bool Board::checkDownPawn(int from, int to, int x, int y) {
  bool ok = false;
  bool doubleJump = from >= 48 && from <= 55;

  if ((board[from - 8]->type == Type::BLANK && from - 8 == to) ||
      (board[from - 7]->type != Type::BLANK && from - 7 == to &&
       !board[from - 9]->isWhite) ||
      (board[from - 9]->type != Type::BLANK && from - 9 == to &&
       !board[from - 9]->isWhite) ||
      (doubleJump && board[from - 8]->type == Type::BLANK &&
       board[from - 16]->type == Type::BLANK && from - 16 == to)) {
    ok = true;
    swapSquares(from, to, x, y);
  }
  // en passant
  else if ((board[from - 1]->type != Type::BLANK && from - 9 == to)) {
    swapEnPassant(from, to, x, y, from - 1);
    ok = true;
  } else if ((board[from + 1]->type != Type::BLANK && from - 7 == to)) {
    swapEnPassant(from, to, x, y, from + 1);
    ok = true;
  }

  if (ok && to >= 0 && to <= 7) {
    isPromotion = 1;
    printf("Dsdsd\n");
  }

  return ok;
}

bool Board::checkUpPawn(int from, int to, int x, int y) { return false; }

bool Board::checkDownNight(int from, int to, int x, int y) { return false; }

bool Board::checkUpNight(int from, int to, int x, int y) { return false; }

bool Board::checkDownBishop(int from, int to, int x, int y) { return false; }

bool Board::checkUpBishop(int from, int to, int x, int y) { return false; }

bool Board::checkDownRook(int from, int to, int x, int y) { return false; }

bool Board::checkUpRook(int from, int to, int x, int y) { return false; }

bool Board::checkDownQueen(int from, int to, int x, int y) { return false; }

bool Board::checkUpQueen(int from, int to, int x, int y) { return false; }

bool Board::checkDownKing(int from, int to, int x, int y) { return false; }

bool Board::checkUpKing(int from, int to, int x, int y) { return false; }

std::vector<char> Board::possiableMoves() {}

void Board::swapSquares(int from, int to, int x, int y) {
  board[from]->changePosition(x, y);
  board[to]->objTexture = NULL;
  board[to]->type = Type::BLANK;

  auto temp = board[from];
  board[from] = board[to];
  board[to] = temp;
}

void Board::swapEnPassant(int from, int to, int x, int y, int piecePos) {
  board[from]->changePosition(x, y);
  board[to]->objTexture = NULL;
  board[to]->type = Type::BLANK;
  board[piecePos]->objTexture = NULL;
  board[piecePos]->type = Type::BLANK;

  auto temp = board[from];
  board[from] = board[to];
  board[to] = temp;
}
