#pragma once

#include <algorithm>
#include <ncurses.h>
#include <string>
#include <sstream>
#include <sys/time.h>
#include "BotHandle.h"
#include "GameEnums.h"
#include "Tournament.h"
#include "VisualBoard.h"

class Tournament;

class Match {
private:
  Tournament *tournament;
  VisualBoard board;
  BotHandle botHandle1;
  BotHandle botHandle2;
  void InitBot(BotHandle *botHandle, int id);
  std::string UpdateBot(BotHandle *botHandle, int round);
  int MoveStringToInt(std::string);
  GameState EndMatch(GameState g);
  int timeBank;
  int timePerMove;
public:
  Match(Tournament *tournament, std::string bot1, std::string bot2, int matchId, int timeBank, int timePerMove);
  GameState PlayMatch();
};
