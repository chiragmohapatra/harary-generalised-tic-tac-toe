#ifndef MONTE_CARLO_SIMULATOR_H
#define MONTE_CARLO_SIMULATOR_H

#include "game.hpp"

void benchmark_count(const Game & start, int simulations);
void benchmark_time(const Game & start, double seconds);

#endif
