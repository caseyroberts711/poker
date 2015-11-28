// Copyright 2015- Casey Roberts. All rights reserved.
// @author Casey Roberts

#pragma once

#include "PokerConstants.h"
#include "lib.h"

class PokerMath {
 public:
  PokerMath() {
    // Only construct this once, in serial!
    // prep nck
    memset(nck_, 0, sizeof(nck_));
    for (size_t n = 0; n <= N_; ++n)
      nck_[n][0] = 1;
    for (size_t k = 0; k <= N_; ++k)
      nck_[k][k] = 1;
    for (size_t n = 2; n <= N_; ++n)
      for (size_t k = 1; k <= n; ++k)
        nck_[n][k] = nck_[n-1][k] + nck_[n-1][k-1];

    assert(nck_[7][0] == 1, "");
    assert(nck_[9][1] == 9, "");
    assert(nck_[50][2] == 50 * 49 / 2, "");
    assert(nck_[47][46] == 47, "");
    assert(nck_[43][41] == 43 * 42 / 2, "");
    assert(nck_[13][14] == 0, "");
    assert(nck_[17][44] == 0, "");
  }
  static inline size_t nck(int n, int k) {
    assert(n >= 0 && k >= 0 && n <= N_ && k <= N_,
           "nck only built for ways to choose cards from a deck");
    return nck_[n][k];
  }
 private:
  static const size_t N_ = NUM_CARDS_IN_DECK;
  static size_t nck_[N_ + 1][N_ + 1];
};
const size_t PokerMath::N_;
size_t PokerMath::nck_[PokerMath::N_ + 1][PokerMath::N_ + 1];
PokerMath pokerMathInitializer;
