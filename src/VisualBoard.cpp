#include "VisualBoard.h"
using namespace std;

VisualBoard::VisualBoard(int width, int height) : width(width), height(height) {
  board = new char[width * height];
  for(int i = 0; i < width * height; i++)
    board[i] = '.';
  pl1Pos = 7 * 16 + 3;
  pl2Pos = 8 * 16 - 4;
  board[pl1Pos] = '0';
  board[pl2Pos] = '1';
}

GameState VisualBoard::MakeMove(int pl, int move) {
  int pos = (pl == 0) ? pl1Pos : pl2Pos;
  int newIndex = 0;
  switch(move) {
    case 0:
      if (pos - width < 0){
        return (pl == 0) ? PL2_WON : PL1_WON;
      }
      newIndex = pos-width;
      break;
    case 1:
      if (pos + width > width * height){
        return (pl == 0) ? PL2_WON : PL1_WON;
      }
      newIndex = pos+width;
      break;
    case 2:
      if (pos % width == 0){
        return (pl == 0) ? PL2_WON : PL1_WON;
      }
      newIndex = pos-1;
      break;
    case 3:
      if (pos % width == width - 1){
        return (pl == 0) ? PL2_WON : PL1_WON;
      }
      newIndex = pos+1;
      break;
  }
  if (pl == 1 && newIndex == pl1Pos)
    return DRAW;
  if (board[newIndex] != '.')
    return (pl == 0) ? PL2_WON : PL1_WON;
  board[newIndex] = '0' + pl;
  if (pl == 0)
    pl1Pos = newIndex;
  else
    pl2Pos = newIndex;
  return NONE;
}

void VisualBoard::Print() {
  printf("%s", " ┌");
  for (int i = 0; i < width; i++)
    printf("─%d─", i%10);
  printf("%s", "┐\n");
  for (int y = 0; y < height; y++){
    printf("%d│", y%10);
    for (int x = 0; x < width; x++){
      char c = board[y * width + x];
      switch (c) {
        case '.': printf(" \033[30;1m•\033[0m "); break;
        case '0': if (pl1Pos == width * y + x)
                    printf(" \033[34;1mO\033[0m ");
                  else
                    printf(" \033[34;1m■\033[0m ");
                  break;
        case '1': if (pl2Pos == width * y + x)
                    printf(" \033[31;1mO\033[0m ");
                  else
                    printf(" \033[31;1m■\033[0m ");
                  break;
      }
    }
    printf("│%d\n", y%10);
  }
  printf("%s", " └");
  for (int i = 0; i < width; i++)
    printf("%s", "───");
  printf("%s\n", "┘");
}

string VisualBoard::ToString() {
  stringstream ss;
  for (int i = 0; i < width*height; i++){
    if (board[i] == '.')
      ss << board[i];
    else if (board[i] == '0')
      ss << ((pl1Pos == i) ? '0' : 'x');
    else if (board[i] == '1')
      ss << ((pl2Pos == i) ? '1' : 'x');

    if (i < width*height-1)
      ss << ',';
  }
  return ss.str();
}
