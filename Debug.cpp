#include "Debug.hpp"
#include "Board.hpp"
#include <stdio.h>

void Debug::printBoardPieces(std::array<Object *, 63> *board) {
  printf("|");
  for (int i = 0; i < 64; i++) {
    if (i % 8 == 0 && i != 0) {
      printf("\n---------------------------------\n");
      printf("|");
    }
    switch (board->at(i)->type) {
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

void Debug::printBoardSides(std::array<Object *, 63> *board) {
  printf("|");
  for (int i = 0; i < 64; i++) {
    if (i % 8 == 0 && i != 0) {
      printf("\n---------------------------------\n");
      printf("|");
    }

    switch (board->at(i)->side) {
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
