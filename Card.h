// Copyright 2015- Casey Roberts. All rights reserved.
// @author Casey Roberts

#pragma once

#include "PokerConstants.h"

struct Card {
  Card() : index_(63) {} // never call this, for default array use only
  Card(size_t index) : index_(index) {
    assert(index_ >= 0 && index_ < NUM_CARDS_IN_DECK,
           "Constructing card from bad index");
  }
  Card(const std::string& s)
      : index_(4 * denoms_.find_first_of(s[0]) + suits_.find_first_of(s[1])) {
    assert(index_ >= 0 && index_ < NUM_CARDS_IN_DECK, "misconstructed index");
    assert(s[0] == denoms_[denom()], "read bad denominator");
    assert(s[1] == suits_[suit()], "read bad suit");
  }
  Card(const Card& other) : index_(other.index_) {}
  std::string toStr() const { return denoms_.substr(denom(), 1) + suits_.substr(suit(), 1); }
  inline size_t denom() const { return index_ >> 2; }
  inline size_t suit() const { return index_ & 3; }
  inline size_t index() const { return index_; }
 private:
  static const std::string denoms_;
  static const std::string suits_;
  size_t index_;
};
const std::string Card::denoms_ = "23456789TJQKA";
const std::string Card::suits_ = "DCHS";
