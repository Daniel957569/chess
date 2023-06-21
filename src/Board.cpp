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

#define SIDES 1
#define UP_DOWN 8
#define DIAGNOL_LEFT 9
#define DIAGNOL_RIGHT 7

Object *textureBoard;

std::vector<Move> allMoves;
int whiteKing;
int blackKing;

bool blackKingMove = false;
bool whiteKingMove = false;

Board::Board() {}
Board::~Board() {}

void Board::initBoard() {
    isMated = false;
    blackInCheck = false;
    whiteInCheck = false;

    cBoard = IMG_LoadTexture(Game::gRenderer, "./images/board.png");
    bRook = IMG_LoadTexture(Game::gRenderer, "./images/br.png");
    bNight = IMG_LoadTexture(Game::gRenderer, "./images/bn.png");
    bBishop = IMG_LoadTexture(Game::gRenderer, "./images/bb.png");
    bQueen = IMG_LoadTexture(Game::gRenderer, "./images/bq.png");
    bKing = IMG_LoadTexture(Game::gRenderer, "./images/bk.png");
    bPawn = IMG_LoadTexture(Game::gRenderer, "./images/bp.png");
    wRook = IMG_LoadTexture(Game::gRenderer, "./images/wr.png");
    wNight = IMG_LoadTexture(Game::gRenderer, "./images/wn.png");
    wBishop = IMG_LoadTexture(Game::gRenderer, "./images/wb.png");
    wQueen = IMG_LoadTexture(Game::gRenderer, "./images/wq.png");
    wKing = IMG_LoadTexture(Game::gRenderer, "./images/wk.png");
    wPawn = IMG_LoadTexture(Game::gRenderer, "./images/wp.png");

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

void Board::RenderBoard(bool isDragging, int draggedPos) {
    int skip = isDragging ? draggedPos : -1;
    textureBoard->render();

    for (int i = 0; i < 64; i++) {
        if (i == skip) {
            continue;
        }
        board[i]->render();
    }
    if (isDragging) {
        board[skip]->render();
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

    if (to == from) {
        return false;
    }

    auto inCheck = isWhiteTurn ? &blackInCheck : &whiteInCheck;
    int king = isWhiteTurn ? whiteKing : blackKing;
    bool isLegal = false;
    auto side = isWhiteTurn ? WHITE : BLACK;
    auto fromSqaure = board[from];
    auto toSqaure = board[to];
    printf("from: %d to: %d\n", from, to);

    switch (fromSqaure->type) {
    case Type::PAWN:
        printf("pawn\n");
        isLegal = checkPawn(from, to, side);

        break;
    case Type::KNIGHT:
        printf("night\n");
        isLegal = checkNight(from, to, side);

        break;
    case Type::BISHOP:
        printf("bishop\n");
        isLegal = checkBishop(from, to, side);

        break;
    case Type::ROOK:
        printf("rook\n");
        isLegal = checkRook(from, to, side);

        break;
    case Type::QUEEN:
        printf("queen\n");
        isLegal = checkQueen(from, to, side);

        break;
    case Type::KING:
        printf("king\n");
        isLegal = checkKing(from, to, side);
        if (side == BLACK && !blackKingMove) {
            blackKingMove = isLegal;
        }
        if (side == WHITE && !whiteKingMove) {
            whiteKingMove = isLegal;
        }

        break;
    default:
        break;
    }

    checkChecking(side);
    if (*inCheck) {
        printf("incheck\n");
        auto oppSide = side == WHITE ? BLACK : WHITE;
        if (checkGameOver(oppSide)) {
            board[king]->isMarked = true;
            isMated = true;
        }
    }

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
        // enPassent == 1 means the piece is next + 1 and 2 is - 1 ok? ok. mby
        // make more clear later idk

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
            moves.clear();
            return true;
        }
    }

    moves.clear();
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
            moves.clear();
            return true;
        }
    }
    moves.clear();
    return isLegal;
}

bool Board::checkBishop(int from, int to, Side side) {
    int edge = from % 8;

    std::vector<Move> moves;
    bishopMoves(from, &moves, side, false);

    for (int i = 0; i < moves.size(); i++) {
        if (moves[i].to == to) {
            swapSquares(from, to, true);
            moves.clear();
            return true;
        }
    }

    moves.clear();
    return false;
}

bool Board::checkRook(int from, int to, Side side) {
    bool isLegal = false;
    int edge = from % 8;

    std::vector<Move> moves;
    rookMoves(from, &moves, side, false);

    for (int i = 0; i < moves.size(); i++) {
        if (moves[i].to == to) {
            swapSquares(from, to, true);

            moves.clear();
            return true;
        }
    }

    moves.clear();
    return false;
}

bool Board::checkQueen(int from, int to, Side side) {
    auto oppKing = side == WHITE ? blackKing : whiteKing;
    bool isWhite = side == WHITE ? true : false;

    std::vector<Move> moves;

    queenMoves(from, &moves, side, false);
    printf("-----------\n");
    for (int i = 0; i < moves.size(); i++) {
        printf("from: %d to: %d\n", moves[i].from, moves[i].to);
    }
    printf("-----------\n");
    printf("%ld\n", moves.size());

    for (int i = 0; i < moves.size(); i++) {
        if (moves[i].to == to) {
            swapSquares(from, to, true);
            moves.clear();

            return true;
        }
    }

    moves.clear();
    return false;
}

bool Board::checkKing(int from, int to, Side side) {
    auto oppSide = side == WHITE ? BLACK : WHITE;
    auto isWhite = side == WHITE ? true : false;

    std::vector<Move> moves;

    kingMoves(from, &moves, side, false);

    for (int i = 0; i < moves.size(); i++) {
        if (moves[i].to == to) {

            if (from + 2 == to) {
                makeSmallCastle(from, to, isWhite, true);
            } else if (from - 2 == to) {
                makeLongCastle(from, to, isWhite, true);
            } else {
                swapSquares(from, to, true);
            }

            if (side == WHITE) {
                whiteKing = to;
            } else {
                blackKing = to;
            }

            moves.clear();
            return true;
        }
    }

    moves.clear();
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
    int currSqr = pos1 % 8;

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
            Move move = {pos, pos1, board[pos]->type, false};
            moves->push_back(move);
            pos1 += times;
            prevSqr = currSqr;
            currSqr = pos1 % 8;
        }
    }

    prevSqr = pos % 8;
    currSqr = pos2 % 8;

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
            Move move = {pos, pos2, board[pos]->type, false};
            moves->push_back(move);
            pos2 -= times;
            prevSqr = currSqr;
            currSqr = pos2 % 8;
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

void Board::bishopMoves(int from, std::vector<Move> *moves, Side side,
                        bool isTesting) {
    checkPossibleMoves(from, DIAGNOL_LEFT, side, moves, isTesting);
    checkPossibleMoves(from, DIAGNOL_RIGHT, side, moves, isTesting);
}

void Board::rookMoves(int from, std::vector<Move> *moves, Side side,
                      bool isTesting) {
    checkPossibleMoves(from, SIDES, side, moves, isTesting);
    checkPossibleMoves(from, UP_DOWN, side, moves, isTesting);
}

void Board::queenMoves(int from, std::vector<Move> *moves, Side side,
                       bool isTesting) {
    checkPossibleMoves(from, SIDES, side, moves, isTesting);
    checkPossibleMoves(from, UP_DOWN, side, moves, isTesting);
    checkPossibleMoves(from, DIAGNOL_RIGHT, side, moves, isTesting);
    checkPossibleMoves(from, DIAGNOL_LEFT, side, moves, isTesting);
}

void Board::kingMoves(int from, std::vector<Move> *moves, Side side,
                      bool isTesting) {
    bool kingMoved = side == WHITE ? whiteKingMove : blackKingMove;

    checkCastle(moves, from, kingMoved, side, isTesting);
    checkMove(moves, from, DIAGNOL_RIGHT, KING, side, isTesting);
    checkMove(moves, from, SIDES, KING, side, isTesting);
    checkMove(moves, from, UP_DOWN, KING, side, isTesting);
    checkMove(moves, from, DIAGNOL_LEFT, KING, side, isTesting);
}

void Board::checkCastle(std::vector<Move> *moves, int from, bool kingMoved,
                        Side side, bool isTesting) {
    if (kingMoved) {
        return;
    }

    auto chessBoard = isTesting ? &secondBoard : &board;
    if (AS_SIDE(from + 1) == BLANK && AS_SIDE(from + 2) == BLANK &&
        AS_TYPE(from + 3) == ROOK) {
        moves->push_back({from, from + 2, KING, false, 0});
    }

    if (AS_SIDE(from - 1) == BLANK && AS_SIDE(from - 2) == BLANK &&
        AS_SIDE(from - 3) == BLANK && AS_TYPE(from - 4) == ROOK) {
        moves->push_back({from, from - 2, KING, false, 0});
    }

    bool isWhite = side == WHITE ? true : false;
    if (!isTesting) {
        std::vector<Move> tempMoves;
        for (int i = 0; i < moves->size(); i++) {
            if (!willBeInCheck(moves->at(i).from, moves->at(i).to, isWhite))
                tempMoves.push_back(moves->at(i));
        }

        moves->swap(tempMoves);
    }
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

    if (toSquare7 >= 0 && toSquare7 <= 63 && !OUT_OF_BOARD(toSquare7))
        if (AS_SIDE(toSquare7) == oppSide) {
            moves->push_back({from, toSquare7, PAWN, true, 0});
        }

    if (toSquare9 >= 0 && toSquare9 <= 63 && !OUT_OF_BOARD(toSquare9))
        if (AS_SIDE(toSquare9) == oppSide) {
            moves->push_back({from, toSquare9, PAWN, true, 0});
        }

    if (toSquare16 >= 0 && toSquare16 <= 63)
        if (doubleJump && AS_SIDE(toSquare8) == BLANK &&
            AS_SIDE(toSquare16) == BLANK) {
            moves->push_back({from, toSquare16, PAWN, false, 0});
        }

    // en passant
    if (AS_SIDE(from + 1) == oppSide && !OUT_OF_BOARD(from + 1)) {
        moves->push_back({from, toSquare7, PAWN, true, 1});
    }
    if (AS_SIDE(from - 1) == oppSide && !OUT_OF_BOARD(from - 1)) {
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

void Board::getAllPieceMoves(int from, Side side, std::vector<Move> *moves,
                             bool isTesting) {
    bool isWhite = side == BLACK ? false : true;
    auto chessBoard = isTesting ? &secondBoard : &board;

    switch (chessBoard->at(from)->type) {
    case PAWN:
        pawnMoves(from, 1, moves, side, isTesting);
        break;
    case KNIGHT:
        knightMoves(from, moves, side, isTesting);
        break;
    case BISHOP:
        bishopMoves(from, moves, side, isTesting);
        break;
    case ROOK:
        rookMoves(from, moves, side, isTesting);
        break;
    case QUEEN:
        queenMoves(from, moves, side, isTesting);
        break;
    case KING:
        kingMoves(from, moves, side, isTesting);
        break;
    default:
        // Unreachable
        break;
    }
}

void Board::getAllPossibleMoves(Side side, std::vector<Move> *moves) {
    for (int i = 0; i < 64; i++) {
        if (board[i]->side == side)
            getAllPieceMoves(i, side, moves, true);
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
    int tempKing = isWhite ? whiteKing : blackKing;
    int originalKingPos = isWhite ? whiteKing : blackKing;

    if (board[from]->type == KING) {
        tempKing = to;
    }
    std::vector<Move> moves;
    copyBoard();

    swapSquares(from, to, false);

    for (int i = 0; i < 64; i++) {
        if (secondBoard[i]->side == oppSide) {
            getAllPieceMoves(i, oppSide, &moves, true);
        }
    }
    if (isInCheck(tempKing, &moves)) {
        return true;
    }

    moves.clear();

    return false;
}

bool Board::isInCheck(int king, std::vector<Move> *moves) {
    for (int i = 0; i < moves->size(); i++) {
        if (moves->at(i).to == king)
            return true;
    }
    return false;
}

void Board::checkChecking(Side side) {
    auto kingPos = side == WHITE ? blackKing : whiteKing;
    auto inCheck = side == WHITE ? &blackInCheck : &whiteInCheck;

    std::vector<Move> moves;

    for (int i = 0; i < 64; i++) {
        if (board[i]->side == side)
            getAllPieceMoves(i, side, &moves, false);
    }

    if (isInCheck(kingPos, &moves))
        *inCheck = true;
}

bool Board::checkGameOver(Side side) {
    std::vector<Move> moves;
    if (side == WHITE) {
        printf("white\n");
    } else {
        printf("black\n");
    }

    for (int i = 0; i < 64; i++) {
        if (board[i]->side == side)
            getAllPieceMoves(i, side, &moves, false);
    }
    printf("-------- moves size: %ld\n", moves.size());

    if (moves.size() == 1) {
        printf("%d %d %d\n", moves[0].from, moves[0].to, moves[0].piece);
    }
    if (moves.size() == 0) {
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
    auto chessBoard = firstBoard ? board : secondBoard;

    swapSquares(from, to, firstBoard);

    chessBoard[piecePos]->objTexture = NULL;
    chessBoard[piecePos]->side = BLANK;
    chessBoard[piecePos]->type = NONE;

}

void Board::makePromotion(int from, int to, bool isWhite, bool firstBoard) {
    auto chessBoard = firstBoard ? board : secondBoard;

    swapSquares(from, to, firstBoard);

    chessBoard[from]->type = Type::NONE;
    chessBoard[to]->type = Type::QUEEN;
    chessBoard[to]->side = isWhite ? WHITE : BLACK;
    chessBoard[from]->side = Side::BLANK;
}

void Board::makeSmallCastle(int from, int to, bool isWhite, bool firstBoard) {
    swapSquares(from, to, firstBoard);
    swapSquares(from + 3, from + 1, firstBoard);
}

void Board::makeLongCastle(int from, int to, bool isWhite, bool firstBoard) {
    swapSquares(from, to, firstBoard);
    swapSquares(from - 4, from - 1, firstBoard);
}

void Board::copyBoard() {
    for (int i = 0; i < 64; i++) {
        Side side = board[i]->side;
        Type type = board[i]->type;

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
