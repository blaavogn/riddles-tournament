#include "Match.h"
using namespace std;


Match::Match(Tournament *tournament, string name1, string name2, int matchId, int timeBank, int timePerMove)
  : tournament(tournament), board(16,16), botHandle1(name1,matchId,1,timeBank), botHandle2(name2,matchId,2,timeBank),
    timeBank(timeBank), timePerMove(timePerMove) {
  InitBot(&botHandle1, 0);
  InitBot(&botHandle2, 1);
}

string Match::UpdateBot(BotHandle *botHandle, int round) {
  botHandle->Send("update game round " + to_string(round));
  botHandle->Send("update game field " + board.ToString());

  struct timeval tp;
  gettimeofday(&tp,NULL);
  long int before = tp.tv_sec * 1000 + tp.tv_usec / 1000;
  botHandle->Send("action move " + to_string(botHandle->TimeBank));
  string receive = botHandle->Receive();
  gettimeofday(&tp,NULL);
  long int after = tp.tv_sec * 1000 + tp.tv_usec / 1000;
  botHandle->TimeBank = botHandle->TimeBank - (after - before) + timePerMove;
  if (botHandle->TimeBank > timeBank)
    botHandle->TimeBank = timeBank;
  return receive;
}

GameState Match::PlayMatch(){
  for (int i = 0; true; i++) {
    string move1 = UpdateBot(&botHandle1, i);
    string move2 = UpdateBot(&botHandle2, i);

    tournament->PrintStats();

    int i_move1 = MoveStringToInt(move1);
    int i_move2 = MoveStringToInt(move2);
    move1.erase(std::remove(move1.begin(), move1.end(), '\n'), move1.end());
    move2.erase(std::remove(move2.begin(), move2.end(), '\n'), move2.end());

    printf("\033[34;1m%8.8s: %4.4s (%d)\033[0m        \033[31;1m%8.8s: %4.4s (%d)\033[0m\n",
            botHandle1.name.c_str(), move1.c_str(), botHandle1.TimeBank,
            botHandle2.name.c_str(), move2.c_str(), botHandle2.TimeBank);

    bool alive1 = true, alive2 = true;
    if (i_move1 == -1) {
      printf("Bot one made an illegal move %s\n", move1.c_str());
      alive1 = false;
    }
    if (i_move2 == -1) {
      printf("Bot two made an illegal move %s\n", move2.c_str());
      alive2 = false;
    }
    if (botHandle1.TimeBank <= 0) {
      printf("Bot one out of time");
      alive1 = false;
    }
    if (botHandle2.TimeBank <= 0) {
      printf("Bot two out of time");
      alive2 = false;
    }

    if (!alive1 && !alive2)
      return DRAW;
    else if (!alive1)
      return PL2_WON;
    else if (!alive2)
      return PL1_WON;

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
  botHandle->Send("settings timebank " + to_string(timeBank));
  botHandle->Send("settings time_per_move " + to_string(timePerMove));
  botHandle->Send("settings player_names player0,player1");
  botHandle->Send("settings your_bot player0");
  botHandle->Send("settings your_botid " + to_string(id));
  botHandle->Send("settings field_width 16");
  botHandle->Send("settings field_height 16");
}
