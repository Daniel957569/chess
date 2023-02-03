#include "Board.hpp"
#include "Game.hpp"
#include "Object.hpp"
#include <SDL2/SDL_timer.h>
#include <cstdio>
#include <cstdlib>
#include <vector>

#define SIDE_EQUAL(num, op, side) (board[from op num]->side == side)

Object *textureBoard;

Board::Board() {}
Board::~Board() {}

std::vector<Move> allMoves;

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
  renderBlackSide(0);
  renderPawn(8, 8, 0, 100, Side::BLACK);

  renderBlank(16, 32, 0, 200);

  renderPawn(48, 8, 0, 600, Side::WHITE);
  renderWhiteSide(56);
  /* renderFEN("rnbqkbnr/4p3/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R"); */
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
                         Side Side) {
  return new Object(tex, x, y, pieceType, Side);
}

void Board::renderWhiteSide(int index) {
  board[index] = makePiece(wRook, 0, 700, Type::ROOK, Side::WHITE);
  board[index + 1] = makePiece(wNight, 100, 700, Type::KNIGHT, Side::WHITE);
  board[index + 2] = makePiece(wBishop, 200, 700, Type::BISHOP, Side::WHITE);
  board[index + 3] = makePiece(wQueen, 300, 700, Type::QUEEN, Side::WHITE);
  board[index + 4] = makePiece(wKing, 400, 700, Type::KING, Side::WHITE);
  board[index + 5] = makePiece(wBishop, 500, 700, Type::BISHOP, Side::WHITE);
  board[index + 6] = makePiece(wNight, 600, 700, Type::KNIGHT, Side::WHITE);
  board[index + 7] = makePiece(wRook, 700, 700, Type::ROOK, Side::WHITE);
}

void Board::renderBlackSide(int index) {
  board[index] = makePiece(bRook, 0, 0, Type::ROOK, Side::BLACK);
  board[index + 1] = makePiece(bNight, 100, 0, Type::KNIGHT, Side::BLACK);
  board[index + 2] = makePiece(bBishop, 200, 0, Type::BISHOP, Side::BLACK);
  board[index + 3] = makePiece(bQueen, 300, 0, Type::QUEEN, Side::BLACK);
  board[index + 4] = makePiece(bKing, 400, 0, Type::KING, Side::BLACK);
  board[index + 5] = makePiece(bBishop, 500, 0, Type::BISHOP, Side::BLACK);
  board[index + 6] = makePiece(bNight, 600, 0, Type::KNIGHT, Side::BLACK);
  board[index + 7] = makePiece(bRook, 700, 0, Type::ROOK, Side::BLACK);
}

void Board::renderPawn(int index, int amount, int x, int y, Side Side) {
  auto pawn = Side == Side::WHITE ? wPawn : bPawn;

  for (int i = index; i < index + amount; i++) {
    board[i] = makePiece(pawn, x, y, Type::PAWN, Side);
    x += 100;
  }
}

void Board::renderBlank(int index, int amount, int x, int y) {
  for (int i = index; i < index + amount; i++) {
    if (i % 8 == 0 && i != 0) {
      y += 100;
      x = 0;
    }
    board[i] = makePiece(NULL, x, y, Type::NONE, Side::BLANK);
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
      board[count] = makePiece(bRook, x, y, Type::ROOK, Side::BLACK);
      break;
    case 'n':
      board[count] = makePiece(bNight, x, y, Type::KNIGHT, Side::BLACK);
      break;
    case 'b':
      board[count] = makePiece(bBishop, x, y, Type::BISHOP, Side::BLACK);
      break;
    case 'q':
      board[count] = makePiece(bQueen, x, y, Type::QUEEN, Side::BLACK);
      break;
    case 'k':
      board[count] = makePiece(bKing, x, y, Type::KING, Side::BLACK);
      break;
    case 'p':
      board[count] = makePiece(bPawn, x, y, Type::PAWN, Side::BLACK);
      break;
    case 'R':
      board[count] = makePiece(wRook, x, y, Type::ROOK, Side::WHITE);
      break;
    case 'N':
      board[count] = makePiece(wNight, x, y, Type::KNIGHT, Side::WHITE);
      break;
    case 'B':
      board[count] = makePiece(wBishop, x, y, Type::BISHOP, Side::WHITE);
      break;
    case 'Q':
      board[count] = makePiece(wQueen, x, y, Type::QUEEN, Side::WHITE);
      break;
    case 'K':
      board[count] = makePiece(wKing, x, y, Type::KING, Side::WHITE);
      break;
    case 'P':
      board[count] = makePiece(wPawn, x, y, Type::PAWN, Side::WHITE);
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

bool Board::makeMove(int from, int to, bool isWhiteTurn) {
  bool isLegal = false;
  auto fromSqaure = board[from];
  auto toSqaure = board[to];
  printf("from: %d to: %d x: %d y: %d\n", from, to);
  std::vector<int> idk;

  switch (fromSqaure->type) {
  case Type::PAWN:
    printf("pawn\n");
    if (fromSqaure->side == WHITE) {
      isLegal = checkPawn(from, to, WHITE);
    } else {
      isLegal = checkPawn(from, to, BLACK);
    }
    break;
  case Type::KNIGHT:
    printf("night\n");
    if (fromSqaure->side == WHITE) {
      isLegal = checkNight(from, to, WHITE);
    } else {
      isLegal = checkNight(from, to, BLACK);
    }
    break;
  case Type::BISHOP:
    printf("bishop\n");
    if (fromSqaure->side == WHITE) {
      isLegal = checkBishop(from, to, WHITE);
    } else {
      isLegal = checkBishop(from, to, BLACK);
    }
    break;
  case Type::ROOK:
    printf("rook\n");
    if (fromSqaure->side == WHITE) {
      isLegal = checkRook(from, to, WHITE);
    } else {
      isLegal = checkRook(from, to, BLACK);
    }
    break;
  case Type::QUEEN:
    printf("queen\n");
    if (fromSqaure->side == WHITE) {
      isLegal = checkQueen(from, to, WHITE);
    } else {
      isLegal = checkQueen(from, to, BLACK);
    }
    break;
  case Type::KING:
    printf("king\n");
    if (fromSqaure->side == WHITE) {
      isLegal = checkKing(from, to, WHITE);
    } else {
      isLegal = checkKing(from, to, BLACK);
    }
    break;
  default:
    break;
  }
  /* auto arrStr = possibleMoves(); */
  /* for (int i = 0; i < arrStr.size(); i++) { */
  /*   printf("%s\n", arrStr[i].c_str()); */
  /* } */
  /* printBoard2(); */
  printBoard();

  allMoves.clear();
  return isLegal;
}

bool Board::checkPawn(int from, int to, Side side) {
  int ok = 0;
  bool doubleJump = from >= 48 && from <= 55;

  std::vector<Move> moves;

  pawnMoves(from, to, &moves, side, side == WHITE ? true : false);

  for (int i = 0; i < moves.size(); i++) {
    // enPassent == 1 means the piece is next + 1 and 2 is - 1 ok? ok. mby make
    // more clear later idk
    if (moves[i].enPassent == 1) {
      swapEnPassant(from, to, from + 1);
    } else if (moves[i].enPassent == 2) {
      swapEnPassant(from, to, from - 1);
    }
    if (moves[i].to == to) {
      swapSquares(from, to);
      if (ok && to <= 7 && to >= 0) {
        makePromotion(from, to, Side::WHITE);
      }
      return true;
    }
  }

  return ok;
}

/* bool Board::checkUpPawn(int from, int to) { */
/*   int ok = 0; */
/*   bool doubleJump = from >= 8 && from <= 15; */
/*   std::vector<Move> moves; */

/*   if (AS_SIDE(from + 8) == BLANK && from + 8 == to) { */
/*     moves.push_back({from, to, PAWN, false}); */
/*   } */
/*   if (AS_SIDE(from + 7) == WHITE && from + 7 == to) { */
/*     moves.push_back({from, to, PAWN, true}); */
/*   } */
/*   if (AS_SIDE(from + 9) == WHITE && from + 9 == to) { */
/*     moves.push_back({from, to, PAWN, true}); */
/*   } */
/*   if (doubleJump && AS_SIDE(from + 8) == BLANK && AS_SIDE(from + 16) == BLANK
 * && */
/*       from + 16 == to) { */
/*     moves.push_back({from, to, PAWN, false}); */
/*   } */

/*   // en passant */
/*   if ((AS_SIDE(from + 1) != BLANK && from + 9 == to && */
/*        AS_SIDE(from + 1) != BLACK)) { */
/*     swapEnPassant(from, to, from + 1); */
/*     ok = 1; */
/*   } */
/*   if ((AS_SIDE(from - 1) != BLANK && from + 7 == to && */
/*        AS_SIDE(from - 1) != BLACK)) { */
/*     swapEnPassant(from, to, from - 1); */
/*     ok = 2; */
/*   } */

/*   if (ok == 1) { */
/*     swapEnPassant(from, to, from + 1); */
/*     return true; */
/*   } else if (ok == 2) { */
/*     swapEnPassant(from, to, from - 1); */
/*     return true; */
/*   } */

/*   for (int i = 0; i < moves.size(); i++) { */
/*     if (moves[i].to == to) { */
/*       swapSquares(from, to); */
/*       if (ok && to <= 7 && to >= 0) { */
/*         makePromotion(from, to, Side::BLACK); */
/*       } */
/*       return true; */
/*     } */
/*   } */

/*   return ok; */
/* } */

bool Board::checkNight(int from, int to, Side side) {
  bool isLegal = false;
  int distance = (from % 8) - (to % 8);
  auto oppSide = side == WHITE ? BLACK : WHITE;

  std::vector<Move> moves;
  if (!(distance >= -3 && distance <= 3)) {
    return false;
  }
  for (int i = 0; i < moves.size(); i++) {
    if (moves[i].to == to) {
      swapSquares(from, to);
      return true;
    }
  }
  return isLegal;
}

bool Board::checkBishop(int from, int to, Side side) {
  int edge = from % 8;

  std::vector<Move> moves;
  checkPossibleMoves(from, 9, side, &moves);
  checkPossibleMoves(from, 7, side, &moves);

  for (int i = 0; i < moves.size(); i++) {
    if (moves[i].to == to) {
      swapSquares(from, to);
      return true;
    }
  }

  return false;
}

bool Board::checkRook(int from, int to, Side side) {
  bool isLegal = false;
  int edge = from % 8;

  std::vector<Move> moves;
  checkPossibleMoves(from, 1, side, &moves);
  checkPossibleMoves(from, 8, side, &moves);

  for (int i = 0; i < moves.size(); i++) {
    if (moves[i].to == to) {
      swapSquares(from, to);
      return true;
    }
  }

  return false;
}

bool Board::checkQueen(int from, int to, Side side) {
  bool isLegal = false;
  int edge = from % 8;

  std::vector<Move> moves;
  checkPossibleMoves(from, 1, side, &moves);
  checkPossibleMoves(from, 8, side, &moves);
  checkPossibleMoves(from, 7, side, &moves);
  checkPossibleMoves(from, 9, side, &moves);

  copyVector(&moves);
  for (int i = 0; i < moves.size(); i++) {
    if (moves[i].to == to) {
      swapSquares(from, to);
      return true;
    }
  }

  return false;
}

bool Board::checkKing(int from, int to, Side side) {
  int edge = from % 8;
  auto oppSide = side == WHITE ? BLACK : WHITE;

  std::vector<Move> moves;
  checkPossibleMoves(from, 1, side, &moves);

  kingMoves(from, to, &moves, side);

  for (int i = 0; i < moves.size(); i++) {
    if (moves[i].to == to) {
      swapSquares(from, to);
      return true;
    }
  }

  return false;
}

std::vector<Move> *Board::checkPossibleMoves(int pos, int times, Side side,
                                             std::vector<Move> *moves) {
  auto oppSide = side == WHITE ? BLACK : WHITE;
  int pos1 = pos + times;
  int pos2 = pos - times;
  int edge = pos % 8;
  int prevSqr = pos % 8;
  int currSqr = prevSqr;

  for (;;) {
    if (pos1 > 63 || prevSqr == 7 && currSqr == 0 ||
        prevSqr == 0 && currSqr == 7) {
      break;
    }
    if (AS_SIDE(pos1) == side) {
      break;
    }
    if (AS_SIDE(pos1) == oppSide) {
      moves->push_back({pos, pos1, board[pos]->type, true});
      break;
    }
    if (AS_SIDE(pos1) == BLANK) {
      prevSqr = currSqr;
      currSqr++;
      Move move = {pos, pos1, board[pos]->type, false};
      moves->push_back(move);
      pos1 += times;
    }
  }

  prevSqr = pos % 8;
  currSqr = prevSqr;

  for (;;) {
    if (pos2 < 0 || prevSqr == 7 && currSqr == 0 ||
        prevSqr == 0 && currSqr == 7) {
      break;
    }
    if (AS_SIDE(pos2) == side) {
      break;
    }
    if (AS_SIDE(pos2) == oppSide) {
      moves->push_back({pos, pos2, board[pos]->type, true});
      break;
    }
    if (AS_SIDE(pos2) == BLANK) {
      prevSqr = currSqr;
      currSqr--;
      Move move = {pos, pos2, board[pos]->type, false};
      moves->push_back(move);
      pos2 -= times;
    }
  }
  return moves;
}

void Board::knightMoves(int from, int to, std::vector<Move> *moves, Side side) {
  checkMove(moves, from, to, 6, KNIGHT, side);
  checkMove(moves, from, to, 10, KNIGHT, side);
  checkMove(moves, from, to, 15, KNIGHT, side);
  checkMove(moves, from, to, 17, KNIGHT, side);
}

void Board::kingMoves(int from, int to, std::vector<Move> *moves, Side side) {
  checkMove(moves, from, to, 7, KING, side);
  checkMove(moves, from, to, 8, KING, side);
  checkMove(moves, from, to, 9, KING, side);
}

void Board::pawnMoves(int from, int to, std::vector<Move> *moves, Side side,
                      bool isUp) {
  bool doubleJump = isUp ? from >= 48 && from <= 55 : from >= 8 && from <= 15;
  auto oppSide = side == WHITE ? BLACK : WHITE;
  int toSquare8 = isUp ? from - 8 : from + 8;
  int toSquare7 = isUp ? from - 7 : from + 7;
  int toSquare9 = isUp ? from - 9 : from + 9;
  int toSquare16 = isUp ? from - 16 : from + 16;

  if (AS_SIDE(toSquare8) == BLANK && toSquare8 == to) {
    moves->push_back({from, to, PAWN, false});
  }
  if (AS_SIDE(toSquare7) == oppSide && toSquare7 == to) {
    moves->push_back({from, to, PAWN, true});
  }
  if (AS_SIDE(toSquare9) == oppSide && toSquare9 == to) {
    moves->push_back({from, to, PAWN, true});
  }

  if (doubleJump && AS_SIDE(toSquare8) == BLANK &&
      AS_SIDE(toSquare16) == BLANK && toSquare16 == to) {
    moves->push_back({from, to, PAWN, false});
  }

  // en passant
  if ((AS_SIDE(from + 1) == WHITE && from + 9 == to)) {
    moves->push_back({from, to, PAWN, true, 1});
  }
  if ((AS_SIDE(from - 1) != WHITE && from + 7 == to)) {
    moves->push_back({from, to, PAWN, true, 2});
  }
}

void Board::checkMove(std::vector<Move> *moves, int from, int to, int amount,
                      Type type, Side side) {
  if (from - amount == to && AS_SIDE(from - amount) != side) {
    pushMove(moves, from, to, KING, side);
  }
  if (from + amount == to && AS_SIDE(from + amount) != side) {
    pushMove(moves, from, to, KING, side);
  }
}

void Board::copyVector(std::vector<Move> *moves) {
  auto len = allMoves.size();

  for (int i = 0; i < moves->size(); i++) {
    allMoves.push_back(moves->at(i));
  }
}

std::vector<Move> *Board::getAllPieceMoves(Type type, Side side) {
  bool isWhite = side == WHITE;
}

std::vector<Move> *Board::getAllPossibleMoves() {}

void Board::swapSquares(int from, int to) {
  int x = (to % 8) * 100;
  int y = (to / 8) * 100;
  printf("side: %d, sid2: %d\n", board[from]->side, board[to]->side);

  board[to]->objTexture = board[from]->objTexture;
  board[to]->side = board[from]->side;
  board[to]->type = board[from]->type;
  board[to]->changePosition(x, y);
  board[from]->objTexture = NULL;
  board[from]->side = BLANK;
  board[from]->type = NONE;
}

void Board::swapEnPassant(int from, int to, int piecePos) {
  int x = (to % 8) * 100;
  int y = (to / 8) * 100;

  board[from]->changePosition(x, y);
  board[to]->objTexture = NULL;
  board[to]->type = Type::NONE;
  board[piecePos]->objTexture = NULL;
  board[piecePos]->side = Side::BLANK;
  board[piecePos]->type = Type::NONE;

  auto temp = board[from];
  board[from] = board[to];
  board[to] = temp;
}

void Board::makePromotion(int from, int to, bool Side) {
  auto temp = board[from];
  board[from] = board[to];
  board[to] = temp;

  board[to]->objTexture = Side == Side::WHITE ? wQueen : bQueen;
  board[from]->objTexture = NULL;

  board[from]->type = Type::NONE;
  board[to]->type = Type::QUEEN;
  board[to]->side = Side::WHITE;
  board[from]->side = Side::BLANK;
}

void Board::pushMove(std::vector<Move> *moves, int from, int to, Type piece,
                     Side oppSide) {
  if (board[to]->side == oppSide) {
    moves->push_back({from, to, PAWN, true});
  } else {
    moves->push_back({from, to, PAWN, false});
  }
}

bool Board::hasMultiMovesForSquare(std::vector<Move> *moves, int to,
                                   Type piece) {
  int count = 0;
  for (int i = 0; i < moves->size(); i++) {
    if (count == 2) {
      return true;
    }
    if (moves->at(i).to == to && piece == moves->at(i).piece) {
      count++;
    }
  }
  return false;
}

bool Board::willBeInCheck(Move *move) { return true; }

std::vector<std::string> Board::possibleMoves() {
  std::vector<std::string> convertedMoves;

  for (int i = 0; i < allMoves.size(); i++) {
    auto move = allMoves[i];
    std::string str;
    int count = 0;

    switch (move.piece) {
    case PAWN:
      if (move.isTaking) {
        str.push_back(FROM_COLUMN(move));
        str.push_back('x');
      }
      str.push_back(TO_COLUMN(move));
      str.push_back(TO_ROW(move));
      break;
    case KNIGHT:
      str.push_back('N');
      if (hasMultiMovesForSquare(&allMoves, move.to, KNIGHT)) {
        str.push_back(FROM_COLUMN(move));
      }
      if (move.isTaking) {
        str.push_back('x');
      }

      // turn it into a COLUMn
      str.push_back(TO_COLUMN(move));
      str.push_back(TO_ROW(move));
      break;

    case BISHOP:
      if (hasMultiMovesForSquare(&allMoves, move.to, BISHOP)) {
        str.push_back('B');
        if (move.isTaking) {
          str.push_back('x');
        }
      }

      str.push_back(TO_COLUMN(move));
      str.push_back(TO_ROW(move));
      break;

    case ROOK:
      if (hasMultiMovesForSquare(&allMoves, move.to, ROOK)) {
        str.push_back('R');
        if (move.isTaking) {
          str.push_back('x');
        }
      }
      str.push_back(TO_COLUMN(move));
      str.push_back(TO_ROW(move));
      break;

    case QUEEN:
      if (hasMultiMovesForSquare(&allMoves, move.to, QUEEN)) {
        str.push_back('Q');
        if (move.isTaking) {
          str.push_back('x');
        }
      }
      str.push_back(TO_COLUMN(move));
      str.push_back(TO_ROW(move));
      break;

    case KING:
      if (hasMultiMovesForSquare(&allMoves, move.to, KING)) {
        str.push_back('K');
        if (move.isTaking) {
          str.push_back('x');
        }
      }
      str.push_back(TO_COLUMN(move));
      str.push_back(TO_ROW(move));
      break;

    default:
      // unreachable
      break;
    }
    convertedMoves.push_back(str);
  }
  return convertedMoves;
}

void Board::printBoard() {
  printf("|");
  for (int i = 0; i < 64; i++) {
    if (i % 8 == 0 && i != 0) {
      printf("\n---------------------------------\n");
      printf("|");
    }
    switch (board[i]->type) {
    case Type::PAWN:
      printf(" P |");
      break;
    case Type::KNIGHT:
      printf(" N |");
      break;
    case Type::BISHOP:
      printf(" B |");
      break;
    case Type::QUEEN:
      printf(" Q |");
      break;
    case Type::KING:
      printf(" K |");
      break;
    case Type::ROOK:
      printf(" R |");
      break;
    case Type::NONE:
      printf("   |");
      break;
    default:
      // unreachable
      break;
    }
  }
  printf("\n----------------------------------\n");
}

void Board::printBoard2() {
  printf("|");
  for (int i = 0; i < 64; i++) {
    if (i % 8 == 0 && i != 0) {
      printf("\n---------------------------------\n");
      printf("|");
    }
    switch (board[i]->side) {
    case WHITE:
      printf(" W |");
      break;
    case BLACK:
      printf(" B |");
      break;
    case BLANK:
      printf("   |");
      break;
    default:
      // unreachable
      break;
    }
  }
  printf("\n----------------------------------\n");
}
