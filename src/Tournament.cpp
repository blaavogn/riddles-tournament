#include "Tournament.h"
using namespace std;

Tournament::Tournament() {
  DIR *dirp = opendir("bots");
  dirent *dp;
  while ((dp = readdir(dirp)) != NULL)
    if (dp->d_type == DT_REG)
      competitors.push_back(string(dp->d_name));
}

void Tournament::NeverEndingTournament() {
  int matchCounter = 0;
  while (matchCounter < 1) {
    for (uint i = 0; i < competitors.size(); i++) {
      uint op;
      for(op = i; op == i; op = rand() % competitors.size()){}
      Competitor *comp1 = &competitors[i];
      Competitor *comp2 = &competitors[op];
      Match match(this, comp1->name, comp2->name, matchCounter++);

      GameState g = match.PlayMatch();

      // Compute ELO don't know about source
      // https://metinmediamath.wordpress.com/2013/11/27/how-to-calculate-the-elo-rating-including-example/
      float R1 = pow(10.0f, comp1->elo / 400.0f);
      float R2 = pow(10.0f, comp2->elo / 400.0f);
      float E1 = R1 / (R1 + R2);
      float E2 = R2 / (R1 + R2);
      float S1, S2;
      switch (g) {
        case PL1_WON: S1 = 1.0f; S2 = 0.0f; break;
        case PL2_WON: S1 = 0.0f; S2 = 1.0f; break;
        default: S1 = 0.5f; S2 = 0.5f; break;
      }
      comp1->elo += 32 * (S1 - E1);
      comp2->elo += 32 * (S2 - E2);

      int incIndex, decIndex;
      if ((S1 - E1) > 0) {
        incIndex = i;
        decIndex = op;
      } else {
        incIndex = op;
        decIndex = i;
      }
      if (incIndex != -1) {
        while (incIndex-1 >= 0 && competitors[incIndex].elo > competitors[incIndex-1].elo){
          swap(competitors[incIndex], competitors[incIndex-1]);
          if (incIndex-1 == decIndex)
            decIndex++;
          incIndex--;
        }
        while (decIndex+1 < competitors.size() && competitors[decIndex].elo < competitors[decIndex+1].elo){
          swap(competitors[decIndex], competitors[decIndex+1]);
          decIndex++;
        }
      }
    }
  }
}

void Tournament::PrintStats() {
  printf("\nStanding: \n");
  for (uint j = 0; j < competitors.size(); j++)
    printf("  %10.10s: %.0f\n", competitors[j].name.c_str(), competitors[j].elo);
}
