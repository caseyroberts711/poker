// Copyright 2015- Casey Roberts. All rights reserved.
// @author Casey Roberts

#pragma once

#include <vector>

#include "PokerMath.h"

ll makeFiveHand(size_t a, size_t b, size_t c, size_t d, size_t e) {
  return (1LL << a) | (1LL << b) | (1LL << c) |
         (1LL << d) | (1LL << e);
}

ll makeSixHand(size_t a, size_t b, size_t c, size_t d, size_t e, size_t f) {
  return (1LL << a) | (1LL << b) | (1LL << c) |
         (1LL << d) | (1LL << e) | (1LL << f);
}

ll makeSevenHand(size_t a, size_t b, size_t c, size_t d, size_t e, size_t f, size_t g) {
  return (1LL << a) | (1LL << b) | (1LL << c) |
         (1LL << d) | (1LL << e) | (1LL << f) |
         (1LL << g);
}

ll makeSevenHand(const std::vector<Card>& twoHand, const std::vector<Card>& fiveBoard) {
  assert(twoHand.size() == 2, "makeSevenHand given wrong size hand");
  assert(fiveBoard.size() == 5, "makeSevenHand given wrong size board");
  return makeSevenHand(
      twoHand[0].index(),
      twoHand[1].index(),
      fiveBoard[0].index(),
      fiveBoard[1].index(),
      fiveBoard[2].index(),
      fiveBoard[3].index(),
      fiveBoard[4].index());
}

size_t getIndex(ll x) {
  int togo = __builtin_popcountll(x);
  size_t ret = 0;
  while (x) {
    int topBit = 63 - __builtin_clzll(x);
    ret += PokerMath::nck(topBit, togo--);
    x -= 1LL << topBit;
  }
  return ret;
}

void giveEquityFromRanks(const std::vector<size_t>& ranks,
                         std::vector<double>* equity) {
  assert(ranks.size() == equity->size(),
         "giveEquityFromRanks received different sized vectors");
  auto bestRank = ranks[0];
  for (auto z : ranks)
    bestRank = std::max(bestRank, z);
  int numWinners = 0;
  for (auto z : ranks)
    if (z == bestRank)
      ++numWinners;
  for(int z = 0; z < ranks.size(); ++z)
    if (ranks[z] == bestRank)
      (*equity)[z] += 1.0 / numWinners;
}
