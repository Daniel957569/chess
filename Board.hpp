#ifndef board_hpp
#define board_hpp

#include "Object.hpp"
#include <array>
#include <string>
#include <vector>

#define AS_SIDE(num) (board[num]->side)

#define TO_COLUMN(move) ((char)move.to % 8 + 97)
#define TO_ROW(move) ((char)move.to / 8 + 49)
#define FROM_COLUMN(move) ((char)move.from % 8 + 97)
#define FROM_ROW(move) ((char)move.from / 8 + 49)

typedef struct {
  int from;
  int to;
  Type piece;
  bool isTaking;
  int enPassent;
} Move;

class Board {
public:
  Board();
  ~Board();

  void initBoard();
  void updateBoard();
  void RenderBoard();
  void cleanBoard();
  bool makeMove(int from, int to, bool isWhiteTurn);

  static int calcPosition(int x, int y);

  std::array<Object *, 63> board;
  std::array<Object *, 63> secondBoard;
  std::vector<char> possiableMoves();

private:
  Object *makePiece(SDL_Texture *tex, int x, int y, Type pieceType,
                    Side isWhite);
  void renderWhiteSide(int index);
  void renderBlackSide(int index);
  void renderPawn(int index, int amount, int x, int y, Side isWhite);
  void renderBlank(int index, int amount, int x, int y);
  void renderFEN(std::string FEN);

  bool checkDownPawn(int from, int to);
  bool checkPawn(int from, int to, Side side);
  bool checkNight(int from, int to, Side side);
  bool checkBishop(int from, int to, Side side);
  bool checkQueen(int from, int to, Side side);
  bool checkKing(int from, int to, Side side);
  bool checkRook(int from, int to, Side side);
  void markSquares();

  void knightMoves(int from, int to, std::vector<Move> *moves, Side side);
  void kingMoves(int from, int to, std::vector<Move> *moves, Side side);
  void pawnMoves(int from, int to, std::vector<Move> *moves, Side side,
                 bool isUp);
  void checkMove(std::vector<Move> *moves, int from, int to, int amount,
                 Type type, Side side);

  std::vector<Move> *checkPossibleMoves(int pos, int times, Side side,
                                        std::vector<Move> *moves);
  std::vector<Move> *getAllPossibleMoves();
  std::vector<Move> *getAllPieceMoves(Type type, Side side);

  void swapSquares(int from, int to);
  void swapEnPassant(int from, int to, int piecePos);

  void makePromotion(int from, int to, bool isWhite);
  void addMove(int from, int to);

  bool hasMultiMovesForSquare(std::vector<Move> *moves, int to, Type piece);
  std::vector<std::string> possibleMoves();

  void pushMove(std::vector<Move> *moves, int from, int to, Type piece,
                Side side);
  void copyVector(std::vector<Move> *moves);
  bool willBeInCheck(Move *move);

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

#endif
