// Copyright 2015- Casey Roberts. All rights reserved.
// @author Casey Roberts

#pragma once

class Precalculation {
 public:
  Precalculation() {
    // prep ranks for hands
    int upto = 0;
    for (int i = 0; i < NUM_CARDS_IN_DECK; ++i)
    for (int j = 0; j < i; ++j)
    for (int k = 0; k < j; ++k)
    for (int x = 0; x < k; ++x)
    for (int y = 0; y < x; ++y) {
      hands_[upto++] = PokerHandSlow(
          (1LL<<i) | (1LL<<j) | (1LL<<k) | (1LL<<x) | (1LL<<y));
      assert(hands_[upto - 1].index() == upto - 1,
             "Bad index calculated for a hand");
    }
    assert(upto == NUM_5_HANDS,
           "Prep constructed a bad number of hands");
    std::sort(hands_.begin(), hands_.end());
    indexFiveToRank_[hands_[0].index()] = 0;
    for (int i = 1; i < NUM_5_HANDS; ++i) {
      indexFiveToRank_[hands_[i].index()] = 
          indexFiveToRank_[hands_[i - 1].index()] + (hands_[i - 1] < hands_[i] ? 1 : 0);
    }

    // calculate number of hands with each rank
    for(int i = 0; i < NUM_RANKS; ++i)
      numHandsWithRank_[i] = 0;
    for(int i = 0; i < NUM_5_HANDS; ++i)
      ++numHandsWithRank_[indexFiveToRank_[i]];
    cumulativeNumHandsWithRank_[0] = numHandsWithRank_[0];
    for (int i = 1; i < NUM_RANKS; ++i)
      cumulativeNumHandsWithRank_[i] =
          cumulativeNumHandsWithRank_[i - 1] + numHandsWithRank_[i];
    percentileOfRank_[0] = numHandsWithRank_[0] / 2.0 / NUM_5_HANDS;
    for (int i = 1; i < NUM_RANKS; ++i)
      percentileOfRank_[i] =
          (cumulativeNumHandsWithRank_[i - 1] + numHandsWithRank_[i] / 2.0)
          / NUM_5_HANDS;

    // TODO - this approach would be cleaner, check the speed impact
    // prep ranks for seven hands - at this point, we have populated the data
    // that constructing a 5 card hand, it can figure out it's own rank
    // TODO - make this a dp, solve for 6 first, then do 7 off of that
    //   O( 52C6 * 6C1 + 52C7 * 7C1 ) = 18s < O( 52C7 * 7C2 ) = 105s
    for(int i = 0; i < NUM_CARDS_IN_DECK; ++i) {
      if (i > 44) std::cerr << "i = " << i << std::endl;
    for (int j = 0; j < i; ++j)
    for (int k = 0; k < j; ++k)
    for (int x = 0; x < k; ++x)
    for (int y = 0; y < x; ++y)
    for (int z = 0; z < y; ++z) {
      size_t bestRank = 0;
      std::array<int, 6> cards{i, j, k, x, y, z};
      ll sixHand = makeSixHand(i, j, k, x, y, z);
      for (int drop = 0; drop < 6; ++drop) {
        ll fiveHand = sixHand - (1LL << cards[drop]);
        int fiveIndex = getIndex(fiveHand);
        bestRank = std::max(bestRank, indexFiveToRank_[fiveIndex]);
      }
      indexSixToRank_[getIndex(sixHand)] = bestRank;
    }
    }

    for(int i = 0; i < NUM_CARDS_IN_DECK; ++i) {
      if (i > 44) std::cerr << "i = " << i << std::endl;
    for (int j = 0; j < i; ++j)
    for (int k = 0; k < j; ++k)
    for (int w = 0; w < k; ++w)
    for (int x = 0; x < w; ++x)
    for (int y = 0; y < x; ++y)
    for (int z = 0; z < y; ++z) {
      size_t bestRank = 0;
      std::array<int, 7> cards {i, j, k, w, x, y, z};
      ll sevenHand = makeSevenHand(i, j, k, w, x, y, z);
      // TODO - actually, I only need to try removing 6 of these I think
      // end result has 2/7 removed, so I can not try one of them removed
      // and all cases will still be covered :)
      for(int drop = 0; drop < 6; ++drop) { // hacky hacky, see not just above
        ll sixHand = sevenHand - (1LL << cards[drop]);
        int sixIndex = getIndex(sixHand);
        bestRank = std::max(bestRank, indexSixToRank_[sixIndex]);
      }
      indexSevenToRank_[getIndex(sevenHand)] = bestRank;
    }
    }
  }
  static inline size_t rank(const ll hand) {
    if (__builtin_popcountll(hand) == 7) {
      return indexSevenToRank_[getIndex(hand)];
    }
    if (__builtin_popcountll(hand) == 6) {
      return indexSixToRank_[getIndex(hand)];
    }
    if (__builtin_popcountll(hand) == 5) {
      return indexFiveToRank_[getIndex(hand)];
    }
    assert(false, "bad argument to precalculation::rank");
  }
 private:
  class PokerHandSlow {
   public:
    PokerHandSlow() {} // Never use this, only exists so I can create an array of these things
    PokerHandSlow(ll bitset) : index_(0) {
      assert(__builtin_popcountll(bitset) == 5,
             "PokerHandSlow must be constructed from 5 cards");
      for (int i = 0; i < 5; ++i) {
        int topBit = 63 - __builtin_clzll(bitset);
        index_ += PokerMath::nck(topBit, 5 - i);
        bitset = bitset - (1LL << topBit);
        cards_[i] = Card(topBit);
      }
      std::sort(cards_.begin(), cards_.end(), [](const Card& a, const Card& b) -> bool {
        return a.denom() < b.denom() ||
               (a.denom() == b.denom() && a.suit() < b.suit());
      });
      for (int i = 0; i < 4; ++i)
        if (cards_[i].denom() == cards_[i + 1].denom()) {
          pairDenom_ = cards_[i].denom();
          ++numPairs_;
        }
      for (int i = 0; i < 3; ++i)
        if (cards_[i].denom() == cards_[i + 2].denom()) ++numTrips_;
      for (int i = 1; i < 5; ++i)
        if (cards_[i].suit() != cards_[0].suit()) isFlush_ = false;
      for (int i = 1; i < 5; ++i) {
        // A-5 straight is a special case
        if (i == 4 && isStraight_ && cards_[3].denom() == 3 && cards_[4].denom() == 12) {
          Card tmp = cards_[4];
          for (int j = 1; j < 5; ++j)
            cards_[j] = cards_[j - 1];
          cards_[0] = tmp;
          break;
        }
        if (cards_[i].denom() != cards_[i - 1].denom() + 1) isStraight_ = false;
      }
      rank_ = HI_CARD;
      if (numPairs_ == 1) rank_ = PAIR;
      if (numPairs_ == 2) rank_ = TWO_PAIR;
      if (numTrips_ == 1) rank_ = TRIPS;
      if (isStraight_) rank_ = STRAIGHT;
      if (isFlush_) rank_ = FLUSH;
      if (numTrips_ == 1 && numPairs_ == 3) rank_ = BOAT;
      if (numTrips_ == 2) rank_ = QUADS;
      if (isStraight_ && isFlush_) rank_ = SFLUSH;
    }
    // less than = worse than operator
    bool operator<(const PokerHandSlow& other) const {
      assert(index() < NUM_5_HANDS,
             "PokerHandSlow with bad index");
      if (rank_ != other.rank_)
        return rank_ < other.rank_;
      if ((rank_ == QUADS || rank_ == BOAT || rank_ == TRIPS) && cards_[2].denom() != other.cards_[2].denom())
        return cards_[2].denom() < other.cards_[2].denom();
      if (rank_ == TWO_PAIR) {
        if (cards_[3].denom() != other.cards_[3].denom())
          return cards_[3].denom() < other.cards_[3].denom();
        if (cards_[1].denom() != other.cards_[1].denom())
          return cards_[1].denom() < other.cards_[1].denom();
      }
      if (rank_ == PAIR && pairDenom_ != other.pairDenom_)
        return pairDenom_ < other.pairDenom_;
      for (int i = 4; i >= 0; --i)
        if (cards_[i].denom() != other.cards_[i].denom())
          return cards_[i].denom() < other.cards_[i].denom();
      return false;
    }
    size_t index() const { return index_; }
   private:
    enum PokerHandRank {
      HI_CARD = 0, PAIR = 1,     TWO_PAIR = 2,
      TRIPS = 3,   STRAIGHT = 4, FLUSH = 5,
      BOAT = 6,    QUADS = 7,    SFLUSH = 8
    };
    bool isStraight_ = true, isFlush_ = true;
    size_t numPairs_ = 0, numTrips_ = 0, pairDenom_ = -1;
    size_t index_; // index amongst 52C5 hands
    size_t rank_;
    std::array<Card, 5> cards_;
  };
  static std::array<PokerHandSlow, NUM_5_HANDS> hands_;
  static std::array<size_t, NUM_7_HANDS> indexSevenToRank_;
  static std::array<size_t, NUM_6_HANDS> indexSixToRank_;
  static std::array<size_t, NUM_5_HANDS> indexFiveToRank_;
  static std::array<size_t, NUM_RANKS> numHandsWithRank_;
  static std::array<size_t, NUM_RANKS> cumulativeNumHandsWithRank_;
  static std::array<double, NUM_RANKS> percentileOfRank_;
};
std::array<Precalculation::PokerHandSlow, NUM_5_HANDS> Precalculation::hands_;
std::array<size_t, NUM_5_HANDS> Precalculation::indexFiveToRank_;
std::array<size_t, NUM_7_HANDS> Precalculation::indexSevenToRank_;
std::array<size_t, NUM_6_HANDS> Precalculation::indexSixToRank_;
std::array<size_t, NUM_RANKS> Precalculation::numHandsWithRank_;
std::array<size_t, NUM_RANKS> Precalculation::cumulativeNumHandsWithRank_;
std::array<double, NUM_RANKS> Precalculation::percentileOfRank_;
