#include <random>

class KcRandom {
 public:
  KcRandom(): rd_(), gen_(rd_()), dis_(0, 1) {}
  double operator()() { return dis_(gen_); }
  std::random_device rd_;
  std::mt19937 gen_;
  std::uniform_real_distribution<> dis_;
};
