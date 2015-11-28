// Copyright 2015- Casey Roberts. All rights reserved.
// @author Casey Roberts

#pragma once

#include "Card.h"
#include "PokerConstants.h"
#include "TexasHand.h"

// suited at the top, aces at [0][0]
class TexasRange {
 public:
  TexasRange(const array<array<bool, 13>, 13>& matrixRange)
      : matrixRange_(matrixRange) {
    fo(i, NUM_CARDS_IN_DECK)
      fo(j, i) {
        pair<int, int> indices = getMatrixIndices(TexasHand(Card(i), Card(j)));
        if (matrixRange_[indices.first][indices.second])
          hands_.emplace_back(Card(i), Card(j));
      }
  }
  const vector<TexasHand>& hands() {
    return hands_;
  }
  static pair<int, int> getMatrixIndices(const TexasHand& hand) {
    int d1 = hand.card1().denom(), d2 = hand.card2().denom();
    int s1 = hand.card1().suit(), s2 = hand.card2().suit();
    if (d1 == d2)
      return make_pair(12 - d1, 12 - d2);
    if (s1 == s2)
      return make_pair(min(12 - d1, 12 - d2), max(12 - d1, 12 - d2));
    else
      return make_pair(max(12 - d1, 12 - d2), min(12 - d1, 12 - d2));
  }
 private:
  array<array<bool, 13>, 13> matrixRange_;
  vector<TexasHand> hands_;
};
