#ifndef board_hpp
#define board_hpp

#include "Object.hpp"
#include <array>
#include <stdio.h>
#include <string>
#include <vector>

#define AS_SIDE(num) (chessBoard->at(num)->side)
#define AS_TYPE(num) (chessBoard->at(num)->side)
#define OUT_OF_BOARD(num)                                                      \
    (from % 8 == 0 && num % 8 == 7 || from % 8 == 7 && num % 8 == 0)

#define TO_COLUMN(move) ((char)move.to % 8 + 97)
#define TO_ROW(move) ((char)move.to / 8 + 49)
#define FROM_COLUMN(move) ((char)move.from % 8 + 97)
#define FROM_ROW(move) ((char)move.from / 8 + 49)

// 0  1  2  3  4  5  6  7
// 8  9  10 11 12 13 14 15
// 16 17 18 19 20 21 22 23
// 24 25 26 27 28 29 30 31
// 32 33 34 35 36 37 38 39
// 40 41 42 43 44 45 46 47
// 48 49 50 51 52 53 54 55
// 56 57 58 59 60 61 62 63

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

    std::array<Object *, 64> secondBoard;
    std::array<Object *, 64> board;
    std::vector<char> possiableMoves();

    bool blackInCheck;
    bool whiteInCheck;
    bool isPromotion;
    bool isMated;

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

    void knightMoves(int from, std::vector<Move> *moves, Side side,
                     bool isTesting);
    void queenMoves(int from, std::vector<Move> *moves, Side side,
                    bool isTesting);
    void rookMoves(int from, std::vector<Move> *moves, Side side,
                   bool isTesting);
    void bishopMoves(int from, std::vector<Move> *moves, Side side,
                     bool isTesting);
    void kingMoves(int from, std::vector<Move> *moves, Side side,
                   bool isTesting);
    void pawnMoves(int from, int to, std::vector<Move> *moves, Side side,
                   bool isTesting);
    void checkMove(std::vector<Move> *moves, int from, int amount, Type type,
                   Side side, bool isTesting);

    void getAllPossibleMoves(Side side, std::vector<Move> *moves);
    void getAllPieceMoves(int from, Side side, std::vector<Move> *moves,
                          bool isTesting);

    std::vector<Move> *checkPossibleMoves(int from, int times, Side side,
                                          std::vector<Move> *moves,
                                          bool isTesting);

    void swapSquares(int from, int to, bool firstBoard);
    void swapEnPassant(int from, int to, int piecePos, bool firstBoard);
    void makePromotion(int from, int to, bool isWhite, bool firstBoard);

    void addMove(int from, int to);

    bool hasMultiMovesForSquare(std::vector<Move> *moves, int to, Type piece);
    std::vector<std::string> generatePGN();

    void pushMove(std::vector<Move> *moves, int from, int to, Type piece,
                  Side side);

    void copyBoard();
    void copyVector(std::vector<Move> *moves);

    void checkChecking(Side side);
    bool willBeInCheck(int from, int to, bool isWhite);
    bool isInCheck(int to, std::vector<Move> *moves);

    bool checkGameOver(Side side);

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
