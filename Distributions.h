#pragma once

#include <random>
#include <vector>

namespace {
thread_local std::random_device rd;
thread_local std::mt19937 gen(rd());
thread_local std::uniform_real_distribution<> uniformDis(0, 1);
}

class Distribution {
 public:
  virtual double operator()() const = 0;
  std::vector<double> operator()(int n) const {
    std::vector<double> ret(n);
    for (int i = 0; i < n; ++i) {
      ret[i] = (*this)();
    }
    return ret;
  }
  virtual double variance() const = 0;
  virtual ~distribution() {};
};

class UniformDistribution : public Distribution {
 public:
  double operator()() const override { return uniformDis(gen); }
  static inline double rand() { return uniformDis(gen); }
  double variance() const override { return 1.0 / 12; }
}
