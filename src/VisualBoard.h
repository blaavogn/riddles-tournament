#pragma once

#include <cstdint>
#include <stdio.h>
#include <string>
#include <sstream>
#include "GameEnums.h"

class VisualBoard {
private:
  char *board;
  int width, height;
  int pl1Pos, pl2Pos;
public:
  void Print();
  VisualBoard(int width, int height);
  GameState MakeMove(int pl, int move);
  std::string ToString();
};
