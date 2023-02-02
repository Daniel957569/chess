#include "Board.hpp"
#include "Game.hpp"
#include "Object.hpp"
#include <SDL2/SDL_timer.h>
#include <asm-generic/errno.h>
#include <cstdio>
#include <vector>

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

bool Board::makeMove(int from, int to, int x, int y, bool isWhiteTurn) {
  bool isLegal = false;
  auto fromSqaure = board[from];
  auto toSqaure = board[to];
  printf("%d %d\n", from, to);

  switch (fromSqaure->type) {
  case Type::PAWN:
    printf("pawn\n");
    if (fromSqaure->side == Side::WHITE) {
      isLegal = checkDownPawn(from, to, x, y);
    } else {
      isLegal = checkUpPawn(from, to, x, y);
    }
    break;
  case Type::KNIGHT:
    printf("night\n");
    if (fromSqaure->side == WHITE) {
      isLegal = checkNight(from, to, x, y, WHITE);
    } else {
      isLegal = checkNight(from, to, x, y, BLACK);
    }
    break;
  case Type::BISHOP:
    printf("bishop\n");
    if (fromSqaure->side == Side::WHITE) {
      isLegal = checkBishop(from, to, x, y, WHITE);
    } else {
      isLegal = checkBishop(from, to, x, y, BLACK);
    }
    break;
  case Type::ROOK:
    printf("rook\n");
    if (fromSqaure->side == Side::WHITE) {
      isLegal = checkRook(from, to, x, y, WHITE);
    } else {
      isLegal = checkRook(from, to, x, y, BLACK);
    }
    break;
  case Type::QUEEN:
    printf("queen\n");
    if (fromSqaure->side == Side::WHITE) {
      isLegal = checkQueen(from, to, x, y, WHITE);
    } else {
      isLegal = checkQueen(from, to, x, y, BLACK);
    }
    break;
  case Type::KING:
    printf("king\n");
    if (fromSqaure->side == Side::WHITE) {
      isLegal = checkKing(from, to, x, y, WHITE);
    } else {
      isLegal = checkKing(from, to, x, y, BLACK);
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

  if ((board[from - 8]->side == Side::BLANK && from - 8 == to) ||
      (board[from - 7]->side != Side::BLANK && from - 7 == to &&
       board[from - 7]->side != Side::WHITE) ||
      (board[from - 9]->side != Side::BLANK && from - 9 == to &&
       board[from - 9]->side != Side::WHITE) ||
      (doubleJump && board[from - 8]->side == Side::BLANK &&
       board[from - 16]->side == Side::BLANK && from - 16 == to)) {
    ok = true;
    swapSquares(from, to, x, y);
  }
  // en passant
  else if ((board[from - 1]->side != Side::BLANK && from - 9 == to &&
            board[from - 1]->side != Side::WHITE)) {
    swapEnPassant(from, to, x, y, from - 1);
    ok = true;
  } else if ((board[from + 1]->side != Side::BLANK && from - 7 == to &&
              board[from + 1]->side != Side::WHITE)) {
    swapEnPassant(from, to, x, y, from + 1);
    ok = true;
  }

  if (ok && to <= 7 && to >= 0) {
    makePromotion(from, to, Side::WHITE);
  }

  return ok;
}

bool Board::checkUpPawn(int from, int to, int x, int y) {
  bool ok = false;
  bool doubleJump = from >= 8 && from <= 15;

  if ((board[from + 8]->side == Side::BLANK && from + 8 == to) ||
      (board[from + 7]->side != Side::BLANK && from + 7 == to &&
       board[from + 7]->side != Side::BLACK) ||
      (board[from + 9]->side != Side::BLANK && from + 9 == to &&
       board[from + 9]->side != Side::BLACK) ||
      (doubleJump && board[from + 8]->side == Side::BLANK &&
       board[from + 16]->side == Side::BLANK && from + 16 == to)) {
    ok = true;
    swapSquares(from, to, x, y);
  }
  // en passant
  else if ((board[from + 1]->side != Side::BLANK && from + 9 == to &&
            board[from + 1]->side != Side::BLACK)) {
    swapEnPassant(from, to, x, y, from + 1);
    ok = true;
  } else if ((board[from - 1]->side != Side::BLANK && from + 7 == to &&
              board[from - 1]->side != Side::BLACK)) {
    swapEnPassant(from, to, x, y, from - 1);
    ok = true;
  }

  if (ok && to >= 55 && to <= 63) {
    makePromotion(from, to, Side::BLACK);
  }

  return ok;
}

bool Board::checkNight(int from, int to, int x, int y, Side side) {
  bool isLegal = false;
  int distance = (from % 8) - (to % 8);
  printf("%d\n", distance);

  std::vector<Move> moves;

  if (distance >= -3 && distance <= 3) {
    if (from - 15 == to && board[from - 15]->side != side) {
      pushMove(&moves, from, to, side);
    }

    if (from - 17 == to && board[from - 17]->side != side) {
      pushMove(&moves, from, to, side);
    }
    if (from - 6 == to && board[from - 6]->side != side) {
      pushMove(&moves, from, to, side);
    }
    if (from - 10 == to && board[from - 10]->side != side) {
      pushMove(&moves, from, to, side);
    }
    if (from + 6 == to && board[from + 6]->side != side) {
      pushMove(&moves, from, to, side);
    }
    if (from + 15 == to && board[from + 15]->side != side) {
      pushMove(&moves, from, to, side);
    }
    if (from + 17 == to && board[from + 17]->side != side) {
      pushMove(&moves, from, to, side);
    }
    if (from + 10 == to && board[from + 10]->side != side) {
      pushMove(&moves, from, to, side);
    }
  }

  copyVector(&moves);
  for (int i = 0; i < moves.size(); i++) {
    if (moves[i].to == to) {
      swapSquares(from, to, x, y);
      return true;
    }
  }
  return isLegal;
}

bool Board::checkBishop(int from, int to, int x, int y, Side side) {
  int edge = from % 8;

  std::vector<Move> moves;
  checkPossibleMoves(from, 9, edge, false, side, &moves);
  checkPossibleMoves(from, 7, edge, true, side, &moves);

  copyVector(&moves);
  for (int i = 0; i < moves.size(); i++) {
    if (moves[i].to == to) {
      swapSquares(from, to, x, y);
      return true;
    }
  }

  return false;
}

bool Board::checkRook(int from, int to, int x, int y, Side side) {
  bool isLegal = false;
  int edge = from % 8;

  std::vector<Move> moves;
  checkPossibleMoves(from, 1, edge, true, side, &moves);
  checkPossibleMoves(from, 8, edge, false, side, &moves);

  copyVector(&moves);
  for (int i = 0; i < moves.size(); i++) {
    if (moves[i].to == to) {
      swapSquares(from, to, x, y);
      return true;
    }
  }

  return false;
}

bool Board::checkQueen(int from, int to, int x, int y, Side side) {
  bool isLegal = false;
  int edge = from % 8;

  std::vector<Move> moves;
  checkPossibleMoves(from, 1, edge, true, side, &moves);
  checkPossibleMoves(from, 8, edge, false, side, &moves);
  checkPossibleMoves(from, 7, edge, true, side, &moves);
  checkPossibleMoves(from, 9, edge, false, side, &moves);

  copyVector(&moves);
  for (int i = 0; i < moves.size(); i++) {
    if (moves[i].to == to) {
      swapSquares(from, to, x, y);
      return true;
    }
  }

  return false;
}

bool Board::checkKing(int from, int to, int x, int y, Side side) {
  int edge = from % 8;
  auto oppSide = side == WHITE ? BLACK : WHITE;

  std::vector<Move> moves;
  checkPossibleMoves(from, 1, edge, true, side, &moves);

  // fix later mby

  if (from + 7 == to && board[from + 7]->side != side) {
    pushMove(&moves, from, to, side);
  }
  if (from + 8 == to && board[from + 8]->side != side) {
    pushMove(&moves, from, to, side);
  }
  if (from + 9 == to && board[from + 9]->side != side) {
    pushMove(&moves, from, to, side);
  }
  if (from - 7 == to && board[from - 7]->side != side) {
    pushMove(&moves, from, to, side);
  }
  if (from - 8 == to && board[from - 8]->side != side) {
    pushMove(&moves, from, to, side);
  }
  if (from - 9 == to && board[from - 9]->side != side) {
    pushMove(&moves, from, to, side);
  }

  copyVector(&moves);
  for (int i = 0; i < moves.size(); i++) {
    if (moves[i].to == to) {
      swapSquares(from, to, x, y);
      return true;
    }
  }

  return false;
}

std::vector<Move> *Board::checkPossibleMoves(int pos, int times, int edge,
                                             bool edgeSide, Side side,
                                             std::vector<Move> *moves) {
  // edgeSize == true is right side and false is left side mby fix later
  // kinda ugly
  int pos1 = pos;
  int pos2 = pos;
  auto oppSide = side == WHITE ? BLACK : WHITE;
  bool outOfBounds = edgeSide && edge > 7 || !edgeSide && edge < 0;

  for (;;) {
    if (pos1 + times > 63 || outOfBounds) {
      break;
    } else {
      edge++;
    }
    if (board[pos1 + times]->side == BLANK) {
      pos1 += times;
      Move move = {pos, pos1, board[pos]->type, false};
      moves->push_back(move);
      continue;
    }
    if (board[pos1 + times]->side == side) {
      break;
    }
    if (board[pos1 + times]->side == oppSide) {
      printf("lol: %d %d %d\n", pos1 + 14, pos1, pos);
      moves->push_back({pos, pos1 + times, board[pos]->type, true});
      break;
    }
  }

  for (;;) {
    if (pos2 - times < 0 || outOfBounds) {
      break;
    } else {
      edge--;
    }
    if (board[pos2 - times]->side == BLANK) {
      pos2 -= times;
      Move move = {pos, pos2 - times, board[pos1]->type, false};
      moves->push_back(move);
      continue;
    }
    if (board[pos2 - times]->side == side) {
      break;
    }
    if (board[pos2 - times]->side == oppSide) {
      moves->push_back({pos, pos2 - times, board[pos]->type, true});
      break;
    }
  }
  return moves;
}

void Board::copyVector(std::vector<Move> *moves) {
  auto len = allMoves.size();
  for (int i = 0; i < moves->size(); i++) {
    allMoves[len + i] = moves->at(i);
  }
}

void Board::swapSquares(int from, int to, int x, int y) {
  board[from]->changePosition(x, y);
  board[to]->objTexture = NULL;
  board[to]->type = Type::NONE;

  auto temp = board[from];
  board[from] = board[to];
  board[to] = temp;

  board[from]->side = BLANK;
}

void Board::swapEnPassant(int from, int to, int x, int y, int piecePos) {
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

void Board::pushMove(std::vector<Move> *moves, int from, int to, Side side) {
  auto oppSide = side == WHITE ? BLACK : WHITE;

  if (board[from + 7]->side == oppSide) {
    moves->push_back({from, to, board[from]->type, false});
  } else {
    moves->push_back({from, to, board[from]->type, true});
  }
}

std::vector<std::string> possibleMoves(std::vector<Move> moves, Type piece) {
  std::vector<std::string> convertedMoves;

  switch (piece) {
  case PAWN:
  case KNIGHT:
  case BISHOP:
  case ROOK:
  case QUEEN:
  case KING:
  default:
    // unreachable
    break;
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
