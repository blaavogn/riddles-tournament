#pragma once

#include <string>
#include <sstream>
#include "BotHandle.h"
#include "VisualBoard.h"
#include "GameEnums.h"

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
public:
  Match(Tournament *tournament, std::string bot1, std::string bot2, int matchId);
  GameState PlayMatch();
};
