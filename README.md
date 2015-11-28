The simplest way to get up and running is to compile and run the unitTest's, this will verify that
the calculator correctly assesses the odds for many hands.

Compile: clang++ -std=c++11 -DDEBUG -stdlib=libc++ -O3 unitTest.cc -o unitTest 2> /tmp/tmp
Run: ./unitTest

You could then start adding hands to the unit tests / interesting hands sections.
The unit test functions ending in "Tests" should contain hands verified using another source,
or which can be reasoned about such that we know they have the correct odds. Unit test functions
ending in "Hands" are just hands where the only way the odds have been calculated is using this
calculator and so the results are not verified.

For building HoldEm/Omaha unit tests, I recommend - http://www.propokertools.com/simulations

If you wish to use mercurial, resources include:
https://bitbucket.org/durin42/hg-git
http://hg-git.github.io/
