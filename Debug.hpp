#ifndef board_Debug
#define board_Debug

#include "Board.hpp"

class Debug {
public:
  Debug();
  ~Debug();

  static void printBoardPieces(std::array<Object *, 64> *board);
  static void printBoardSides(std::array<Object *, 64> *board);

private:
  /* data */
};

#endif
