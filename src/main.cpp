#include <bits/stdc++.h>
#include "pn_search_unified.h"
#include "pn2_search.h"
#include "pn_search_DAG.h"
#include "monte_carlo_simulator.h"
#include "generalised_tic_tac_toe.h"
#include "generalised_tic_tac_toe_bitboard.h"
#include "check_proof.h"

int polyamino_type = 0;

void benchmarkSpeed(){
  CharSS game;
  Bitboard game2;

  double seconds = 1;
  int simulations = 100000;
  int samples = 10;

  for(int type = 0; type < 3; type++){
    polyamino_type = type;
    std::cout << "Polyomino  " << polyamino_type << "." << std::endl;
    double charSSSimus = benchmark_time(game, seconds, samples);
    double bitboardSimus = benchmark_time(game2, seconds, samples);
    std::cout << "Average simulations per " << seconds <<" seconds. CharSS: " << charSSSimus << "k Bitboard: " << bitboardSimus << "k." << std::endl;

    double charSSTime = benchmark_count(game, simulations, samples);
    double bitboardTime = benchmark_count(game2, simulations, samples);
    std::cout << "Average time per " << simulations <<" simulations. CharSS: " << charSSTime << "ms Bitboard: " << bitboardTime << "ms." << std::endl;
  }
}

void verifyCorrectness(){
  for(int type = 0; type < 3; type++){
    polyamino_type = type;
    std::cout << "Polyomino  " << polyamino_type << "." <<  std::endl;
    monte_carlo_verifier(1000);
    std::cout << "No difference detected between CharSS and Biboard." <<  std::endl;
  }
}

int main(){
  const bool enableBenchmarkSpeed = true;
  const bool enableVerifyCorrectness = true;

  pnSearchUnified::Param pUnified;
  pUnified.isMobile = false;
  pUnified.delete_sub_trees = false;
  pUnified.minimal_proof = true;
  pUnified.policy_applied = false;
  pUnified.depth_minimax = 1;
  pUnified.minimal_policy = false;

  //pnSearchUnified::pn_search_unified_main(pUnified);
  //pUnified.isMobile = true;
  //pnSearchUnified::pn_search_unified_main(pUnified);
  //pnSearchDAG::pn_search_DAG_main();
  //pn2_search_main();

  //check_proof_main();

  if (enableBenchmarkSpeed)    benchmarkSpeed();
  if (enableVerifyCorrectness) verifyCorrectness();
}
