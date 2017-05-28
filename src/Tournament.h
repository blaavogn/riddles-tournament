#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <sys/types.h>
#include <dirent.h>
#include "Competitor.h"
#include "Match.h"

class Tournament {
private:
  std::vector<Competitor> competitors;
public:
  Tournament();
  void NeverEndingTournament();
  void PrintStats();
};
