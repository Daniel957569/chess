#ifndef board_Debug
#define board_Debug

#include "Board.hpp"

class Debug {
public:
  Debug();
  ~Debug();

  static void printBoardPieces(std::array<Object *, 63> *board);
  static void printBoardSides(std::array<Object *, 63> *board);

private:
  /* data */
};

#endif
