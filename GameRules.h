// Copyright 2015- Casey Roberts. All rights reserved.
// @author Casey Roberts

#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

namespace {
const std::vector<std::vector<size_t>> kNormalBoard = {
  { 0, 1, 2, 3, 4 }
};

const std::vector<std::vector<size_t>> kBargeBoards = {
  { 0, 1, 2, 9, 11 },
  { 3, 4, 5, 9, 11 },
  { 3, 4, 5, 10, 11 },
  { 6, 7, 8, 10, 11 }
};

const size_t kTexasCardsPerPlayer = 2;
const size_t kOmahaCardsPerPlayer = 4;
}

class GameRules {
 public:
  GameRules() {}
  void initialize() {
    for (const auto& board : boards())
      for (const auto index : board)
        numCardsOnBoard_ = std::max(numCardsOnBoard_, index + 1);
  }
  virtual const std::vector<std::vector<size_t>> boards() const = 0;
  virtual const size_t cardsPerPlayer() const = 0;
  virtual const bool mustUseTwoFromHand() const = 0;
  virtual const bool canSolveExhaustively() const = 0;
  const size_t numCardsOnBoard() const { return numCardsOnBoard_; }
 private:
  size_t numCardsOnBoard_ = 0;
};

class TexasHoldEmRules : public GameRules {
 public:
  TexasHoldEmRules() : GameRules() {
    initialize();
  }
  virtual const std::vector<std::vector<size_t>> boards() const override { return kNormalBoard; }
  virtual const size_t cardsPerPlayer() const override { return kTexasCardsPerPlayer; }
  virtual const bool mustUseTwoFromHand() const override { return false; };
  virtual const bool canSolveExhaustively() const override { return true; };
};
class OmahaRules : public GameRules {
 public:
  OmahaRules() : GameRules() {
    initialize();
  }
  virtual const std::vector<std::vector<size_t>> boards() const override { return kNormalBoard; }
  virtual const size_t cardsPerPlayer() const override { return kOmahaCardsPerPlayer; }
  virtual const bool mustUseTwoFromHand() const override { return true; };
  virtual const bool canSolveExhaustively() const override { return true; };
};
class BargeRules : public GameRules {
 public:
  BargeRules() : GameRules() {
    initialize();
  }
  virtual const std::vector<std::vector<size_t>> boards() const override { return kBargeBoards; }
  virtual const size_t cardsPerPlayer() const override { return kTexasCardsPerPlayer; }
  virtual const bool mustUseTwoFromHand() const override { return false; };
  virtual const bool canSolveExhaustively() const override { return false; };
};
class OmahaBargeRules : public GameRules {
 public:
  OmahaBargeRules() : GameRules() {
    initialize();
  }
  virtual const std::vector<std::vector<size_t>> boards() const override { return kBargeBoards; }
  virtual const size_t cardsPerPlayer() const override { return kOmahaCardsPerPlayer; }
  virtual const bool mustUseTwoFromHand() const override { return true; };
  virtual const bool canSolveExhaustively() const override { return false; };
};
