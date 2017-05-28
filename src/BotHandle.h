#pragma once

#include <string>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <signal.h>

class BotHandle {
private:
  int pid;
  int reader;
  int writer;
public:
  std::string name;
  BotHandle(std::string name, int matchId, int playerId);
  void Send(std::string msg);
  std::string Receive();
  void Kill();
};
