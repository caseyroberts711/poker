// Copyright 2015- Casey Roberts. All rights reserved.
// @author Casey Roberts

#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include "GameRules.h"
#include "PokerLib.h"
#include "PokerMath.h"
#include "Precalculation.h"
#include "Random.h"


// it is important to make a copy of used
std::vector<Card> buildBoard(std::vector<bool> used, size_t numCardsOnBoard, KcRandom& kcRand) {
  std::vector<Card> board;
  while (board.size() < numCardsOnBoard) {
    int c = 52 * kcRand();
    if (!used[c]) {
      board.push_back(c);
      used[c] = true;
    }
  }
  return board;
}

std::vector<bool> getUsedCards(const std::vector<std::vector<Card>>& hands) {
  std::vector<bool> used(52, false);
  for (const auto& hand : hands) {
    for (const auto& card : hand) {
      assert(!used[card.index()], "card used twice");
      used[card.index()] = true;
    }
  }
  return used;
}

// TODO - multithread the construction, then copy to thread local copies somehow...
// TODO - this is accessed from other files, Singleton maybe? Gotta avoid SIOF
Precalculation precalc;

class EquityCalculator {
 public:
  static std::vector<double> calculate(
      const GameRules& rules,
      const std::vector<std::vector<Card>> hands,
      const size_t numGamesToPlay,
      const bool solveExhaustively) {

    assert(!solveExhaustively || rules.canSolveExhaustively(),
           "trying to exhaustively solve a game for which it cannot be done");

    // declare data
    std::vector<bool> initialUsed = getUsedCards(hands);
    std::vector<double> equities(hands.size());
    KcRandom kcRand;

    for (size_t gameNum = 0; gameNum < numGamesToPlay; ++gameNum) {

      std::vector<Card> boardCards = buildBoard(initialUsed, rules.numCardsOnBoard(), kcRand);

      // evaluate board
      std::vector<size_t> ranks;

      for (const auto& hand : hands) {
        if (!solveExhaustively) {
          if (!rules.mustUseTwoFromHand()) {
            ranks.push_back(precalc.rank(makeSevenHand(hand, boardCards)));
          } else {
            size_t rank = 0;
            for (const auto& board : rules.boards()) {
              // assumes there is two in the hand, five on the board
              for (int h1 = 0; h1 < hand.size(); ++h1)
                for (int h2 = 0; h2 < h1; ++h2)
                  for (int b1 = 0; b1 < board.size(); ++b1)
                    for (int b2 = 0; b2 < b1; ++b2)
                      for (int b3 = 0; b3 < b2; ++b3)
                        rank = std::max(rank, precalc.rank(makeFiveHand(
                          boardCards[board[b1]].index(),
                          boardCards[board[b2]].index(),
                          boardCards[board[b3]].index(),
                          hand[h1].index(),
                          hand[h2].index()
                        )));
            }
            ranks.push_back(rank);
          }
        } else {
          // TODO
          exit(1);
        }
      }

      giveEquityFromRanks(ranks, &equities);
    }

    for (auto& equity : equities)
      equity /= numGamesToPlay;
    return equities;
  }
};
