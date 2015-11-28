// Copyright 2015- Casey Roberts. All rights reserved.
// @author Casey Roberts

#pragma once


#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/time.h>


// count trailing zeros
#define lgLowestBit(x) __builtin_ctz(x)
// count leading zeros
// #define lgHighestBit(x) (63 - __builtin_clz(x))


// count number of 1s
#define bitCount(x) __builtin_popcount(x)


#ifdef DEBUG
#define assert(cond,msg) \
  if(!(cond)){ \
    fprintf(stderr, "assert failed at line %d: %s\n", __LINE__, msg); \
    exit(1); \
  }
#else
#define assert(cond,msg)
#endif


using ll = long long;


const double EPS = 1e-9;


struct timeval startTime, finishTime;
double timeElapsed(){
  gettimeofday(&finishTime, NULL);
  int top = finishTime.tv_sec-startTime.tv_sec-(startTime.tv_usec > finishTime.tv_usec);
  int bot = finishTime.tv_usec-startTime.tv_usec;
  if(bot < 0)
    bot += 1e6;
  return top+bot/1e6;
}
void beginBenchmark() {
  gettimeofday(&startTime, NULL);
  srand(startTime.tv_usec); // side effect :p
}
void benchmark(const std::string& s) {
  std::cerr << s << ": " << timeElapsed();
}


ll nextComb(ll x){ // x = xxx0 1111 0000
  assert(x, "nextComb(0) called!");
  ll smallest = x&-x;        // 0000 0001 0000
  ll ripple = x+smallest;    // xxx1 0000 0000
  ll ones = x^ripple;        // 0001 1111 0000
  ones = (ones>>2)/smallest;  // 0000 0000 0111
  return ripple|ones;         // xxx1 0000 0111
}
