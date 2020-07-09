#ifndef MONTE_CARLO_SIMULATOR_H
#define MONTE_CARLO_SIMULATOR_H

#include "game.hpp"

void monte_carlo_verifier(int simulations);
double benchmark_count(const Game & start, int simulations, int samples);
double benchmark_time(const Game & start, double seconds, int samples);
void monte_carlo_simulator(const Game & game, int n ,  coeff_score);

#endif
