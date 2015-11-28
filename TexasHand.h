// Copyright 2015- Casey Roberts. All rights reserved.
// @author Casey Roberts

#pragma once

#include "PokerConstants.h"

class TexasHand {
 public:
  TexasHand(ll bitset) : bitset_(bitset) {
    assert(__builtin_popcountll(bitset_) == 2,
           "Texas hand must contain two cards");
    assert(bitset <= (1LL << NUM_CARDS_IN_DECK),
           "Cards must be from within the deck");
  }
  TexasHand(Card c1, Card c2) : bitset_((1LL<<c1.index()) | (1LL<<c2.index())) {
    assert(c1.index() != c2.index(),
           "Texas hand must contain two cards");
    assert(c1.index() < NUM_CARDS_IN_DECK && c2.index() < NUM_CARDS_IN_DECK,
           "Cards must be from within the deck");
  }
  ll bitset() const { return bitset_; }
  Card card1() const { return Card(__builtin_ctzll(bitset_)); }
  Card card2() const { return Card(63 - __builtin_clzll(bitset_)); }
 private:
  ll bitset_;
};
