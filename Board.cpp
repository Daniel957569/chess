#include "Board.hpp"
#include "Debug.hpp"
#include "Game.hpp"
#include "Object.hpp"
#include <SDL2/SDL_timer.h>
#include <cstdio>
#include <cstdlib>
#include <vector>

#define SIDE_EQUAL(num, op, side) (board[from op num]->side == side)
#define GET_X(pos) ((pos % 8) * 100)
#define GET_Y(pos) ((pos / 8) * 100)

Object *textureBoard;

std::vector<Move> allMoves;
int whiteKing;
int blackKing;

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
  renderBlackSide(0);
  renderPawn(8, 8, 0, 100, BLACK);

  renderBlank(16, 32, 0, 200);

  renderPawn(48, 8, 0, 600, WHITE);
  renderWhiteSide(56);

  /* renderFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"); */
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

  whiteKing = index + 4;
  board[index] = makePiece(wRook, 0, 700, ROOK, WHITE);
  board[index + 1] = makePiece(wNight, 100, 700, KNIGHT, WHITE);
  board[index + 2] = makePiece(wBishop, 200, 700, BISHOP, WHITE);
  board[index + 3] = makePiece(wQueen, 300, 700, QUEEN, WHITE);
  board[index + 4] = makePiece(wKing, 400, 700, KING, WHITE);
  board[index + 5] = makePiece(wBishop, 500, 700, BISHOP, WHITE);
  board[index + 6] = makePiece(wNight, 600, 700, KNIGHT, WHITE);
  board[index + 7] = makePiece(wRook, 700, 700, ROOK, WHITE);
}

void Board::renderBlackSide(int index) {
  blackKing = index + 4;
  board[index] = makePiece(bRook, 0, 0, ROOK, BLACK);
  board[index + 1] = makePiece(bNight, 100, 0, KNIGHT, BLACK);
  board[index + 2] = makePiece(bBishop, 200, 0, BISHOP, BLACK);
  board[index + 3] = makePiece(bQueen, 300, 0, QUEEN, BLACK);
  board[index + 4] = makePiece(bKing, 400, 0, KING, BLACK);
  board[index + 5] = makePiece(bBishop, 500, 0, BISHOP, BLACK);
  board[index + 6] = makePiece(bNight, 600, 0, KNIGHT, BLACK);
  board[index + 7] = makePiece(bRook, 700, 0, ROOK, BLACK);
}

void Board::renderPawn(int index, int amount, int x, int y, Side Side) {
  auto pawn = Side == WHITE ? wPawn : bPawn;

  for (int i = index; i < index + amount; i++) {
    board[i] = makePiece(pawn, x, y, PAWN, Side);
    x += 100;
  }
}

void Board::renderBlank(int index, int amount, int x, int y) {
  for (int i = index; i < index + amount; i++) {
    if (i % 8 == 0 && i != 0) {
      y += 100;
      x = 0;
    }
    board[i] = makePiece(NULL, x, y, NONE, BLANK);
    x += 100;
  }
}

void Board::renderFEN(std::string FEN) {
  bool isOk = true;
  int i = 0;
  int count = 0;
  while (isOk) {
    int y = GET_Y(count);
    int x = GET_X(count);
    switch (FEN[i]) {
    case 'r':
      board[count] = makePiece(bRook, x, y, ROOK, BLACK);
      break;
    case 'n':
      board[count] = makePiece(bNight, x, y, KNIGHT, BLACK);
      break;
    case 'b':
      board[count] = makePiece(bBishop, x, y, BISHOP, BLACK);
      break;
    case 'q':
      board[count] = makePiece(bQueen, x, y, QUEEN, BLACK);
      break;
    case 'k':
      board[count] = makePiece(bKing, x, y, KING, BLACK);
      break;
    case 'p':
      board[count] = makePiece(bPawn, x, y, PAWN, BLACK);
      break;
    case 'R':
      board[count] = makePiece(wRook, x, y, ROOK, WHITE);
      break;
    case 'N':
      board[count] = makePiece(wNight, x, y, KNIGHT, WHITE);
      break;
    case 'B':
      board[count] = makePiece(wBishop, x, y, BISHOP, WHITE);
      break;
    case 'Q':
      board[count] = makePiece(wQueen, x, y, QUEEN, WHITE);
      break;
    case 'K':
      board[count] = makePiece(wKing, x, y, KING, WHITE);
      break;
    case 'P':
      board[count] = makePiece(wPawn, x, y, PAWN, WHITE);
      break;
    case ' ':
      // for now fix later
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
  printf("from: %d to: %d\n", from, to);
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
  printf("blackKing: %d whiteKing: %d\n", blackKing, whiteKing);
  /* auto arrStr = possibleMoves(); */
  /* for (int i = 0; i < arrStr.size(); i++) { */
  /*   printf("%s\n", arrStr[i].c_str()); */
  /* } */
  /* printBoard2(); */
  /* printBoard(); */

  allMoves.clear();
  return isLegal;
}

bool Board::checkPawn(int from, int to, Side side) {
  int ok = 0;
  bool doubleJump = from >= 48 && from <= 55;
  bool isWhite = side == WHITE ? true : false;

  std::vector<Move> moves;

  pawnMoves(from, to, &moves, side, false);
  for (int i = 0; i < moves.size(); i++) {
    printf("%d %d %ld\n", moves.at(i).from, moves.at(i).to, moves.size());
  }

  for (int i = 0; i < moves.size(); i++) {
    // enPassent == 1 means the piece is next + 1 and 2 is - 1 ok? ok. mby make
    // more clear later idk

    if (moves[i].to == to) {

      if (moves[i].enPassent == 1) {
        swapEnPassant(from, to, from + 1, true);
        return true;
      } else if (moves[i].enPassent == 2) {
        swapEnPassant(from, to, from - 1, true);
        return true;
      } else {
        swapSquares(from, to, true);
      }
      if (ok && to <= 7 && to >= 0) {
        makePromotion(from, to, WHITE, true);
      }
      return true;
    }
  }

  return ok;
}

bool Board::checkNight(int from, int to, Side side) {
  bool isLegal = false;
  int distance = (from % 8) - (to % 8);
  auto oppSide = side == WHITE ? BLACK : WHITE;

  std::vector<Move> moves;
  if (!(distance >= -3 && distance <= 3)) {
    return false;
  }

  knightMoves(from, &moves, side, false);
  for (int i = 0; i < moves.size(); i++) {
    if (moves[i].to == to) {
      swapSquares(from, to, true);
      return true;
    }
  }
  return isLegal;
}

bool Board::checkBishop(int from, int to, Side side) {
  int edge = from % 8;

  std::vector<Move> moves;
  checkPossibleMoves(from, 9, side, &moves, false);
  checkPossibleMoves(from, 7, side, &moves, false);

  for (int i = 0; i < moves.size(); i++) {
    if (moves[i].to == to) {
      swapSquares(from, to, true);
      return true;
    }
  }

  return false;
}

bool Board::checkRook(int from, int to, Side side) {
  bool isLegal = false;
  int edge = from % 8;

  std::vector<Move> moves;
  checkPossibleMoves(from, 1, side, &moves, false);
  checkPossibleMoves(from, 8, side, &moves, false);

  for (int i = 0; i < moves.size(); i++) {
    if (moves[i].to == to) {
      swapSquares(from, to, true);
      return true;
    }
  }

  return false;
}

bool Board::checkQueen(int from, int to, Side side) {
  auto oppKing = side == WHITE ? blackKing : whiteKing;
  bool isLegal = false;
  int edge = from % 8;

  std::vector<Move> moves;
  checkPossibleMoves(from, 1, side, &moves, false);
  checkPossibleMoves(from, 8, side, &moves, false);
  checkPossibleMoves(from, 7, side, &moves, false);
  checkPossibleMoves(from, 9, side, &moves, false);

  copyVector(&moves);
  for (int i = 0; i < moves.size(); i++) {
    printf("%d %d \n", moves[i].from, moves[i].to);
  }

  for (int i = 0; i < moves.size(); i++) {
    if (moves[i].to == to) {
      swapSquares(from, to, true);
      return true;
    }
  }

  return false;
}

bool Board::checkKing(int from, int to, Side side) {
  int edge = from % 8;
  auto oppSide = side == WHITE ? BLACK : WHITE;

  std::vector<Move> moves;
  checkPossibleMoves(from, 1, side, &moves, false);

  kingMoves(from, &moves, side, false);

  for (int i = 0; i < moves.size(); i++) {
    if (moves[i].to == to) {
      swapSquares(from, to, true);

      if (side == WHITE) {
        whiteKing = to;
      } else {
        blackKing = to;
      }
      return true;
    }
  }

  return false;
}

std::vector<Move> *Board::checkPossibleMoves(int pos, int times, Side side,
                                             std::vector<Move> *moves,
                                             bool isTesting) {
  auto oppSide = side == WHITE ? BLACK : WHITE;
  auto chessBoard = isTesting ? &secondBoard : &board;
  bool isWhite = side == WHITE ? true : false;

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
  if (!isTesting) {
    std::vector<Move> tempMoves;
    for (int i = 0; i < moves->size(); i++) {
      if (!willBeInCheck(moves->at(i).from, moves->at(i).to, isWhite))
        tempMoves.push_back(moves->at(i));
    }

    moves->swap(tempMoves);
  }
  return moves;
}

void Board::knightMoves(int from, std::vector<Move> *moves, Side side,
                        bool isTesting) {
  checkMove(moves, from, 6, KNIGHT, side, isTesting);
  checkMove(moves, from, 10, KNIGHT, side, isTesting);
  checkMove(moves, from, 15, KNIGHT, side, isTesting);
  checkMove(moves, from, 17, KNIGHT, side, isTesting);
}

void Board::kingMoves(int from, std::vector<Move> *moves, Side side,
                      bool isTesting) {
  checkMove(moves, from, 7, KING, side, isTesting);
  checkMove(moves, from, 8, KING, side, isTesting);
  checkMove(moves, from, 9, KING, side, isTesting);
}

void Board::pawnMoves(int from, int to, std::vector<Move> *moves, Side side,
                      bool isTesting) {
  bool isWhite = side == WHITE ? true : false;
  bool doubleJump =
      isWhite ? from >= 48 && from <= 55 : from >= 8 && from <= 15;
  auto oppSide = side == WHITE ? BLACK : WHITE;
  auto chessBoard = isTesting ? &secondBoard : &board;

  int toSquare8 = isWhite ? from - 8 : from + 8;
  int toSquare7 = isWhite ? from - 7 : from + 7;
  int toSquare9 = isWhite ? from - 9 : from + 9;
  int toSquare16 = isWhite ? from - 16 : from + 16;

  if (toSquare8 >= 0 && toSquare8 <= 63)
    if (AS_SIDE(toSquare8) == BLANK) {
      moves->push_back({from, toSquare8, PAWN, false, 0});
    }

  if (toSquare7 >= 0 && toSquare7 <= 63)
    if (AS_SIDE(toSquare7) == oppSide) {
      moves->push_back({from, toSquare7, PAWN, true, 0});
    }

  if (toSquare9 >= 0 && toSquare9 <= 63)
    if (AS_SIDE(toSquare9) == oppSide) {
      moves->push_back({from, toSquare9, PAWN, true, 0});
    }

  if (toSquare16 >= 0 && toSquare16 <= 63)
    if (doubleJump && AS_SIDE(toSquare8) == BLANK &&
        AS_SIDE(toSquare16) == BLANK) {
      moves->push_back({from, toSquare16, PAWN, false, 0});
    }

  // en passant
  if (AS_SIDE(from + 1) == oppSide) {
    moves->push_back({from, toSquare7, PAWN, true, 1});
  }
  if (AS_SIDE(from - 1) == oppSide) {
    moves->push_back({from, toSquare9, PAWN, true, 2});
  }

  if (!isTesting) {
    std::vector<Move> tempMoves;
    for (int i = 0; i < moves->size(); i++) {
      if (!willBeInCheck(moves->at(i).from, moves->at(i).to, isWhite))
        tempMoves.push_back(moves->at(i));
    }

    moves->swap(tempMoves);
  }
}

void Board::checkMove(std::vector<Move> *moves, int from, int amount, Type type,
                      Side side, bool isTesting) {
  auto chessBoard = isTesting ? &secondBoard : &board;
  bool isWhite = side == WHITE ? true : false;

  if (!(from - amount < 0))
    if (AS_SIDE(from - amount) != side) {
      pushMove(moves, from, from - amount, type, side);
    }
  if (!(from + amount > 63))
    if (AS_SIDE(from + amount) != side) {
      pushMove(moves, from, from + amount, type, side);
    }

  if (!isTesting) {
    std::vector<Move> tempMoves;
    for (int i = 0; i < moves->size(); i++) {
      if (!willBeInCheck(moves->at(i).from, moves->at(i).to, isWhite))
        tempMoves.push_back(moves->at(i));
    }

    moves->swap(tempMoves);
  }
}

bool testwhiteCheck = false;
bool testblackCheck = false;

void Board::getAllPossibleMoves(int from, Side side, std::vector<Move> *moves) {
  bool isWhite = side == BLACK ? false : true;

  switch (secondBoard[from]->type) {
  case PAWN:
    pawnMoves(from, 1, moves, side, true);
    break;
  case KNIGHT:
    knightMoves(from, moves, side, true);
    break;
  case BISHOP:
    checkPossibleMoves(from, 7, side, moves, true);
    checkPossibleMoves(from, 9, side, moves, true);
    break;
  case ROOK:
    checkPossibleMoves(from, 1, side, moves, true);
    checkPossibleMoves(from, 8, side, moves, true);
    break;
  case QUEEN:
    checkPossibleMoves(from, 1, side, moves, true);
    checkPossibleMoves(from, 8, side, moves, true);
    checkPossibleMoves(from, 7, side, moves, true);
    checkPossibleMoves(from, 9, side, moves, true);
    for (int i = 0; i < moves->size(); i++) {
      printf("piece: %d from: %d to: %d\n", moves->at(i).piece,
             moves->at(i).from, moves->at(i).to);
    }
    break;
  case KING:
    kingMoves(from, moves, side, true);
    break;
  default:
    // Unreachable
    break;
  }
}

void Board::pushMove(std::vector<Move> *moves, int from, int to, Type piece,
                     Side oppSide) {
  if (board[to]->side == oppSide) {
    moves->push_back({from, to, piece, true});
  } else {
    moves->push_back({from, to, piece, false});
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

bool Board::willBeInCheck(int from, int to, bool isWhite) {
  auto oppSide = isWhite ? BLACK : WHITE;
  auto side = isWhite ? WHITE : BLACK;
  std::vector<Move> moves;
  copyBoard();

  swapSquares(from, to, false);
  Debug::printBoardPieces(&secondBoard);

  for (int i = 0; i < 64; i++) {
    if (secondBoard[i]->side == oppSide) {
      getAllPossibleMoves(i, oppSide, &moves);
      if (isInCheck(&moves, isWhite))
        return true;
    }
  }

  moves.clear();

  return false;
}

bool Board::isInCheck(std::vector<Move> *moves, bool isWhite) {
  int king = isWhite ? whiteKing : blackKing;
  auto side = isWhite ? WHITE : BLACK;
  for (int i = 0; i < moves->size(); i++) {
    printf("%d %d\n", moves->at(i).to, king);
    if (moves->at(i).to == king) {
      break;
    }
  }

  for (int i = 0; i < moves->size(); i++) {
    if (moves->at(i).to == king)
      return true;
  }
  return false;
}

void Board::swapSquares(int from, int to, bool firstBoard) {
  int x = GET_X(to);
  int y = GET_Y(to);
  auto chessBoard = firstBoard ? board : secondBoard;

  chessBoard[to]->objTexture = chessBoard[from]->objTexture;
  chessBoard[to]->side = chessBoard[from]->side;
  chessBoard[to]->type = chessBoard[from]->type;
  chessBoard[to]->changePosition(x, y);
  chessBoard[from]->objTexture = NULL;
  chessBoard[from]->side = BLANK;
  chessBoard[from]->type = NONE;
}

void Board::swapEnPassant(int from, int to, int piecePos, bool firstBoard) {
  int x = GET_X(to);
  int y = GET_Y(to);
  auto chessBoard = firstBoard ? board : secondBoard;

  chessBoard[from]->changePosition(x, y);
  chessBoard[to]->objTexture = NULL;
  chessBoard[to]->type = Type::NONE;
  chessBoard[piecePos]->objTexture = NULL;
  chessBoard[piecePos]->side = Side::BLANK;
  chessBoard[piecePos]->type = Type::NONE;

  auto temp = chessBoard[from];
  chessBoard[from] = chessBoard[to];
  chessBoard[to] = temp;
}

void Board::makePromotion(int from, int to, bool Side, bool firstBoard) {
  auto chessBoard = firstBoard ? board : secondBoard;

  auto temp = chessBoard[from];
  chessBoard[from] = chessBoard[to];
  chessBoard[to] = temp;

  chessBoard[to]->objTexture = Side == Side::WHITE ? wQueen : bQueen;
  chessBoard[from]->objTexture = NULL;

  chessBoard[from]->type = Type::NONE;
  chessBoard[to]->type = Type::QUEEN;
  chessBoard[to]->side = Side::WHITE;
  chessBoard[from]->side = Side::BLANK;
}

void Board::copyBoard() {
  for (int i = 0; i < 64; i++) {
    Side side = board[i]->side;
    Type type = board[i]->type;

    if (type == ROOK) {
      printf("ddd: %d \n", i);
    }
    secondBoard[i] = new Object(NULL, GET_X(i), GET_Y(i), type, side);
  }
}

void Board::copyVector(std::vector<Move> *moves) {
  auto len = allMoves.size();

  for (int i = 0; i < moves->size(); i++) {
    allMoves.push_back(moves->at(i));
  }
}

std::vector<std::string> Board::generatePGN() {
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
