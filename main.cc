// Copyright 2015- Casey Roberts. All rights reserved.
// @author Casey Roberts

#include <vector>

#include "lib.h"
#include "Card.h"
#include "EquityCalculator.h"
#include "PokerConstants.h"
#include "Precalculation.h"

namespace {
const int kNumGamesToPlay = 1e6;
const std::unique_ptr<GameRules> rulesByGameId[] = {
  std::unique_ptr<GameRules>(new TexasHoldEmRules()),
  std::unique_ptr<GameRules>(new OmahaRules()),
  std::unique_ptr<GameRules>(new BargeRules()),
  std::unique_ptr<GameRules>(new OmahaBargeRules())
};
}

// Main should be responsible for all IO
int main() {
  std::cout.precision(4);
  std::cout << std::fixed;

  // see rulesByGame in EquityCalculator.h for int -> game mapping
  int numPlayers, gameId;
  while (std::cin >> numPlayers && numPlayers != 0) {
    std::cin >> gameId;
    const GameRules& rules = *rulesByGameId[gameId];

    std::vector<std::vector<Card>> hands(numPlayers, std::vector<Card>(rules.cardsPerPlayer()));
    fprintf(stderr, "begin read hands\n");
    for(int i = 0; i < hands.size(); ++i)
      for (int j = 0; j < hands[i].size(); ++j) {
        std::string card;
        std::cin >> card;
        hands[i][j] = Card(card);
      }
    fprintf(stderr, "end read hands\n");

    const auto equity = EquityCalculator::calculate(rules, hands, kNumGamesToPlay, false);
    
    printf("== %d boards played ==\n", kNumGamesToPlay);
    for (size_t player = 0; player < hands.size(); ++player) {
      for (const auto& card : hands[player])
        printf("%s ", card.toStr().c_str());
      printf("==> ");
      printf("%.6lf\n", equity[player]);
    }
  }

  return 0;
}
