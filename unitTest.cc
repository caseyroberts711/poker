// Copyright 2015- Casey Roberts. All rights reserved.
// @author Casey Roberts

#include <sstream>
#include <utility>
#include <vector>

#include "lib.h"
#include "Card.h"
#include "EquityCalculator.h"
#include "PokerConstants.h"
#include "Precalculation.h"

namespace {
const int kNumGamesToPlay = 1e6;

const double exactEps = 1e-9; // if exact precise answer
const double smallEps = 1e-4; // if precise answer
const double mediumEps = 1e-3; // if 1e6 simulations with many ties, or a strong favourite
const double largeEps = 2e-3; // if 1e6 simulations

std::unique_ptr<GameRules> texasRules(new TexasHoldEmRules());
std::unique_ptr<GameRules> omahaRules(new OmahaRules());
std::unique_ptr<GameRules> bargeRules(new BargeRules());
std::unique_ptr<GameRules> omahaBargeRules(new OmahaBargeRules());

bool isNear(double val, double tgt, double eps) {
  return 
     (val >= tgt - eps && val <= tgt + eps) ||
     (tgt != 0 && val / tgt >= 1 - eps && val / tgt <= 1 + eps);
}
}

std::vector<Card> handFromString(const std::string& input) {
  std::stringstream ss;
  ss << input;
  std::string s;
  std::vector<Card> hand;
  while (ss >> s) {
    hand.push_back(Card(s));
  }
  return hand;
}

struct TestCase {
 public:
  TestCase(const std::vector<std::pair<std::vector<Card>,
           double>>& hands,
           const GameRules& rules,
           const double eps,
           const bool solveExhaustively)
      : rules_(rules), eps_(eps), solveExhaustively_(solveExhaustively) {
    for (const auto pr : hands) {
      orderedHands_.push_back(pr.first);
      orderedWinProbabilities_.push_back(pr.second);
    }
  }

  const std::vector<std::vector<Card>>& hands() const { return orderedHands_; }
  const GameRules& rules() const { return rules_; }
  const std::vector<double>& winProbabilities() const { return orderedWinProbabilities_; }
  double eps() const { return eps_; }
  bool solveExhaustively() const { return solveExhaustively_; }
 private:
  std::vector<std::vector<Card>> orderedHands_;
  const GameRules& rules_;
  std::vector<double> orderedWinProbabilities_;
  const double eps_;
  const bool solveExhaustively_;
};

std::vector<TestCase> texasTests() {
  const GameRules& rules(*texasRules.get());
  return {
    TestCase({ {handFromString("AH AD"), 0.5000},
               {handFromString("AC AS"), 0.5000} },
             rules, mediumEps, false),
    TestCase({ {handFromString("AH AD"), 0.8126},
               {handFromString("KC KS"), 0.1874} },
             rules, largeEps, false),
    TestCase({ {handFromString("AH AD"), 0.8264},
               {handFromString("KH KD"), 0.1736} },
             rules, largeEps, false),
    TestCase({ {handFromString("KH KD"), 0.9492},
               {handFromString("KS 2H"), 0.0508} },
             rules, mediumEps, false),
    TestCase({ {handFromString("JH TC"), 0.5044},
               {handFromString("4D 4C"), 0.4956} },
             rules, largeEps, false),
    TestCase({ {handFromString("7D 6C"), 0.4054},
               {handFromString("4D 4C"), 0.1723},
               {handFromString("5D 5C"), 0.4228} },
             rules, largeEps, false),
    TestCase({ {handFromString("AC AD"), 0.9648},
               {handFromString("KC KD"), 0.0060},
               {handFromString("KS KH"), 0.0292} },
             rules, mediumEps, false)
  };
}

std::vector<TestCase> omahaTests() {
  const GameRules& rules(*omahaRules.get());
  return {
    TestCase({ {handFromString("AH KD QC JS"), 0.5000},
               {handFromString("AD KC QS JH"), 0.5000} },
             rules, exactEps, false),
    TestCase({ {handFromString("AH KD QC JS"), 0.3333},
               {handFromString("AD KC QS JH"), 0.3333},
               {handFromString("AC KS QH JD"), 0.3333} },
             rules, smallEps, false),
    TestCase({ {handFromString("AH AD KC KS"), 0.5000},
               {handFromString("AC AS KH KD"), 0.5000} },
             rules, exactEps, false),
    TestCase({ {handFromString("AH AD KH KD"), 0.5000},
               {handFromString("AC AS KC KS"), 0.5000} },
             rules, mediumEps, false),
    TestCase({ {handFromString("AH AD KH KD"), 0.6338},
               {handFromString("QC QS JC JS"), 0.3662} },
             rules, mediumEps, false),
    TestCase({ {handFromString("AH AD KC KS"), 0.6943},
               {handFromString("QC QS JH JD"), 0.3057} },
             rules, largeEps, false),
    TestCase({ {handFromString("3H 3D 3C 3S"), 1.0000},
               {handFromString("2C 2S 2H 2D"), 0.0000} },
             rules, exactEps, false)
  };
}
std::vector<TestCase> bargeTests() {
  const GameRules& rules(*bargeRules.get());
  return {
  };
}
std::vector<TestCase> omahaBargeTests() {
  const GameRules& rules(*omahaBargeRules.get());
  return {
    TestCase({ {handFromString("3H 3D 3C 3S"), 1.0000},
               {handFromString("2C 2S 2H 2D"), 0.0000} },
             rules, exactEps, false),
    TestCase({ {handFromString("AH AD KH KD"), 0.5000},
               {handFromString("AC AS KC KS"), 0.5000} },
             rules, mediumEps, false),
    TestCase({ {handFromString("AH AD KC KS"), 0.5000},
               {handFromString("AC AS KH KD"), 0.5000} },
             rules, exactEps, false)
  };
}
std::vector<TestCase> omahaBargeHands() {
  const GameRules& rules(*omahaBargeRules.get());
  return {
    // 2 pair V 2 pair
    TestCase({ {handFromString("AH AD KH KD"), 0.6058},
               {handFromString("QC QS JC JS"), 0.3942} },
             rules, largeEps, false),
    TestCase({ {handFromString("AH AD KH KD"), 0.6561},
               {handFromString("QH QD JH JD"), 0.3439} },
             rules, largeEps, false),
    TestCase({ {handFromString("AH AD KH KD"), 0.6258},
               {handFromString("QH QC JH JC"), 0.3742} },
             rules, largeEps, false),
    TestCase({ {handFromString("AH AD KH KD"), 0.6207},
               {handFromString("7H 7C 6H 6C"), 0.3793} },
             rules, largeEps, false),

    // Big cards V little cards
    // That's right, 7654ds is a strong favourite against AKQJr, flush draws matter!
    TestCase({ {handFromString("7H 6H 5D 4D"), 0.5945},
               {handFromString("AH KD QC JS"), 0.4055} },
             rules, largeEps, false),
    // Big cards don't matter? Big cards v little cards, both rainbow
    TestCase({ {handFromString("AH KD QC JS"), 0.5560},
               {handFromString("7H 6C 5D 4S"), 0.4440} },
             rules, largeEps, false),
    // Big cards don't matter? Much better if both suited (even if opponent is too)! Different suits
    TestCase({ {handFromString("AH KH QD JD"), 0.5469},
               {handFromString("7C 6C 5S 4S"), 0.4531} },
             rules, largeEps, false),
    // Big cards don't matter? Much better if both suited! Dominating suits!
    TestCase({ {handFromString("AH KH QD JD"), 0.6232},
               {handFromString("7H 6H 5D 4D"), 0.3768} },
             rules, largeEps, false)
  };
}

int main() {
  std::vector<std::vector<TestCase>> testSuites {
    texasTests(),
    omahaTests(),
    bargeTests(),
    omahaBargeTests(),
    omahaBargeHands()
  };

  for (int suiteNum = 0; suiteNum < testSuites.size(); ++suiteNum) {
    auto& testCases = testSuites[suiteNum];
    for (int caseNum = 0; caseNum < testCases.size(); ++caseNum) {
      const auto& testCase = testCases[caseNum];
      std::vector<double> calculatedProbabilities = 
          EquityCalculator::calculate(testCase.rules(),
                                      testCase.hands(),
                                      kNumGamesToPlay,
                                      testCase.solveExhaustively());
      for (size_t playerNum = 0; playerNum < testCase.hands().size(); ++playerNum) {
        if (!isNear(calculatedProbabilities[playerNum],
                    testCase.winProbabilities()[playerNum],
                    testCase.eps())) {
          fprintf(
              stderr,
              "assert failed: suiteNum %d, caseNum %d, playerNum %d, "
              "received %lf, expected %lf, eps %lf\n",
              suiteNum,
              caseNum,
              playerNum,
              calculatedProbabilities[playerNum],
              testCase.winProbabilities()[playerNum],
              testCase.eps());
        }
      }
    }
  }
  return 0;
}
