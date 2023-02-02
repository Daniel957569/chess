#pragma once
#include "Object.hpp"
#include <array>
#include <string>
#include <vector>

typedef struct {
  int from;
  int to;
  Type piece;
  bool isTaking;
} Move;

class Board {
public:
  Board();
  ~Board();

  void initBoard();
  void updateBoard();
  void RenderBoard();
  void cleanBoard();
  bool makeMove(int from, int to, int x, int y, bool isWhiteTurn);

  static int calcPosition(int x, int y);

  std::array<Object *, 63> board;
  std::vector<char> possiableMoves();

private:
  Object *makePiece(SDL_Texture *tex, int x, int y, Type pieceType,
                    Side isWhite);
  void renderWhiteSide(int index);
  void renderBlackSide(int index);
  void renderPawn(int index, int amount, int x, int y, Side isWhite);
  void renderBlank(int index, int amount, int x, int y);
  void renderFEN(std::string FEN);

  bool isLegal(Type piece, int position);
  bool checkDownPawn(int from, int to, int x, int y);
  bool checkUpPawn(int from, int to, int x, int y);
  bool checkNight(int from, int to, int x, int y, Side side);
  bool checkBishop(int from, int to, int x, int y, Side side);
  bool checkQueen(int from, int to, int x, int y, Side side);
  bool checkKing(int from, int to, int x, int y, Side side);
  bool checkRook(int from, int to, int x, int y, Side side);
  void markSquares();

  std::vector<Move> *checkPossibleMoves(int pos, int times, int edge,
                                        bool edgeSide, Side side,
                                        std::vector<Move> *moves);

  void swapSquares(int from, int to, int x, int y);
  void swapEnPassant(int from, int to, int x, int y, int piecePos);

  void makePromotion(int from, int to, bool isWhite);
  void addMove(int from, int to);

  std::vector<std::string> possibleMoves(std::vector<Move> *moves, Type piece);

  void pushMove(std::vector<Move> *moves, int from, int to, Side side);
  void copyVector(std::vector<Move> *moves);

  void printBoard();
  void printBoard2();

  SDL_Texture *cBoard;
  SDL_Texture *bRook;
  SDL_Texture *bNight;
  SDL_Texture *bBishop;
  SDL_Texture *bQueen;
  SDL_Texture *bKing;
  SDL_Texture *bPawn;
  SDL_Texture *wRook;
  SDL_Texture *wNight;
  SDL_Texture *wBishop;
  SDL_Texture *wQueen;
  SDL_Texture *wKing;
  SDL_Texture *wPawn;
};
