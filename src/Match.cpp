#include <algorithm>
#include <string>
#include "Match.h"
#include "Tournament.h"
using namespace std;


Match::Match(Tournament *tournament, string name1, string name2, int matchId) :
  tournament(tournament), board(16,16), botHandle1(name1,matchId,1), botHandle2(name2,matchId,2) {
  InitBot(&botHandle1, 0);
  InitBot(&botHandle2, 1);
}

string Match::UpdateBot(BotHandle *botHandle, int round) {
  stringstream ss;
  ss << "update game round " << round;
  botHandle->Send(ss.str());
  stringstream sss;
  sss << "update game field " << board.ToString();
  botHandle->Send(sss.str());
  botHandle->Send("action move 10000");
  return botHandle->Receive();
}

GameState Match::PlayMatch(){
  for (int i = 0; true; i++) {
    string move1 = UpdateBot(&botHandle1, i);
    string move2 = UpdateBot(&botHandle2, i);
    int i_move1 = MoveStringToInt(move1);
    int i_move2 = MoveStringToInt(move2);

    tournament->PrintStats();
    move1.erase(std::remove(move1.begin(), move1.end(), '\n'), move1.end());
    move2.erase(std::remove(move2.begin(), move2.end(), '\n'), move2.end());

    printf(" \033[34;1m%8.8s: %4.4s\033[0m                \033[31;1m%8.8s: %4.4s\033[0m\n",
            botHandle1.name.c_str(), move1.c_str(), botHandle2.name.c_str(),  move2.c_str());
    if (i_move1 == -1 && i_move2 == -1) {
      printf("Both bots made an illegal move (%s,%s)\n",move1.c_str(),move2.c_str());
      return EndMatch(DRAW);
    } else if (i_move1 == -1) {
      printf("Bot one made an illegal move %s\n", move1.c_str());
      return EndMatch(PL2_WON);
    } else if (i_move2 == -1) {
      printf("Bot two made an illegal move %s\n", move2.c_str());
      return EndMatch(PL1_WON);
    }

    GameState g1 = board.MakeMove(0, i_move1);
    GameState g2 = board.MakeMove(1, i_move2);

    if (g1 != NONE && g2 != NONE)
      return EndMatch(DRAW);
    else if (g1 != NONE)
      return EndMatch(g1);
    else if (g2 != NONE)
      return EndMatch(g2);

    board.Print();
  }
  return EndMatch(DRAW);
}

GameState Match::EndMatch(GameState g){
  switch (g){
    case PL1_WON:
      printf("\033[34;1mPlayer 1 won\033[0m\n");
      break;
    case PL2_WON:
      printf("\033[31;1mPlayer 2 won\033[0m\n");
      break;
    case NONE:
    case DRAW:
      printf("\033[32;1mDRAW\033[0m\n");
      break;
  }
  botHandle1.Kill();
  botHandle2.Kill();
  return g;
}

int Match::MoveStringToInt(string s){
  if (s == "up\n") return 0;
  if (s == "down\n") return 1;
  if (s == "left\n") return 2;
  if (s == "right\n") return 3;
  return -1;
}

void Match::InitBot(BotHandle *botHandle, int id) {
  botHandle->Send("settings timebank 10000");
  botHandle->Send("settings time_per_move 500");
  botHandle->Send("settings player_names player0,player1");
  botHandle->Send("settings your_bot player0");
  botHandle->Send("settings your_botid " + to_string(id));
  botHandle->Send("settings field_width 16");
  botHandle->Send("settings field_height 16");
}
