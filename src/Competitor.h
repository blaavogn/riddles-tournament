#pragma once
#include <string>

class Competitor {
public:
  float elo;
  std::string name;
  Competitor(std::string name);
};
