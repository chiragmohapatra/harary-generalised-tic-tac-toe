#include <bits/stdc++.h>
#include "pn_search_unified.h"
#include "pn2_search.h"
#include "pn_search_DAG.h"
#include "monte_carlo_simulator.h"
#include "generalised_tic_tac_toe.h"
#include "generalised_tic_tac_toe_bitboard.h"
#include "check_proof.h"
#include "minimax.h"

int polyamino_type = 1;

coeff_score coeff_tippy;  
coeff_score coeff_tic;
  

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

// policy type -1 refers to no policy
void verifyCorrectnessOfPolicies_DAG(pnSearchDAG::Param &parameters){
  for(int type = 0 ; type < 3 ; type+=2){
    polyamino_type = type;
    if(type == 0)
      parameters.coeff = coeff_tippy;
    else
      parameters.coeff = coeff_tic;
    std::cout << "Polyomino  " << polyamino_type << "." <<  std::endl;
    pnSearchDAG::verify_policies(parameters);
    cout<<endl<<endl;
  }
}

void verifyminimalpolicy_DAG(pnSearchDAG::Param &parameters){
  for(int type = 0 ; type < 3 ; type+=5){
    polyamino_type = type;
    if(type == 0)
      parameters.coeff = coeff_tippy;
    else
      parameters.coeff = coeff_tic;
    parameters.minimal_proof = true;
    parameters.minimal_policy = false;
    std::cout << "Polyomino  " << polyamino_type << "." <<  std::endl;
    pnSearchDAG::verify_minimal_policy(parameters);
    cout<<"\n\nMinimal policy\n\n";
    parameters.minimal_policy = true;
    pnSearchDAG::verify_minimal_policy(parameters);
    cout<<endl<<endl;
  }
}

void verify_transpositions(pnSearchUnified::Param &param_u , pnSearchDAG::Param &param_d){
    pnSearchUnified::generate_time_and_memory(param_u);
    cout<<endl<<endl<<"Transpositions\n";
    pnSearchDAG::generate_time_and_memory(param_d);
    cout<<endl<<endl;
  
}

int main(){
  const bool enableBenchmarkSpeed = true;
  const bool enableVerifyCorrectness = true;
  const bool generate_table5 = true;
  const bool generate_table7_DAG = true;
  const bool generate_table8_DAG = true;

  coeff_tippy.first = coeff_tippy.third = coeff_tippy.fifth = coeff_tippy.seventh = 2;
  coeff_tippy.second = coeff_tippy.fourth = coeff_tippy.sixth = coeff_tippy.eighth = 1;

  coeff_tic.first = coeff_tic.fourth = 2;
  coeff_tic.second = coeff_tic.third = coeff_tic.fifth = coeff_tic.sixth = 1;


  pnSearchUnified::Param pUnified;
  pnSearchDAG::Param pDAG;
  pn2Search::Param p2;
  checkProof::Param_check pCheck;

  pUnified.isMobile = pDAG.isMobile = p2.isMobile = false;
  pUnified.delete_sub_trees = p2.delete_sub_trees = false;
  pUnified.minimal_proof = pDAG.minimal_proof = p2.minimal_proof = false;
  pUnified.policy_applied = pDAG.policy_applied = p2.policy_applied = pCheck.policy_applied = true;
  pUnified.depth_minimax = pDAG.depth_minimax = p2.depth_minimax = pCheck.depth_minimax = 1;
  pUnified.minimal_policy = pDAG.minimal_policy = p2.minimal_policy = false;
  pUnified.policy_type = pDAG.policy_type = p2.policy_type = pCheck.policy_type = 1;
  pUnified.board_type = pDAG.board_type = p2.board_type = pCheck.board_type = 0; // 0 for CharSS and 1 for Bitboard

  if(polyamino_type == 0)
    pUnified.coeff = pDAG.coeff = pCheck.coeff = coeff_tippy;
  else if(polyamino_type == 2)
    pUnified.coeff = pDAG.coeff = pCheck.coeff = coeff_tic;

  //pnSearchUnified::pn_search_unified_main(pUnified);

  //pnSearchUnified::verify_policies(pUnified);
  
  //pnSearchDAG::pn_search_DAG_main(pDAG);
  //pn2_search_main(p2);

  //check_proof_main(pCheck);

  //pnSearchUnified::generate_time_and_memory(pUnified);
  //pnSearchDAG::generate_time_and_memory(pDAG);

  //if (enableBenchmarkSpeed)    benchmarkSpeed();
  //if (enableVerifyCorrectness) verifyCorrectness();
  //if(generate_table5)   verify_transpositions(pUnified , pDAG);
  //if (generate_table7_DAG)   verifyCorrectnessOfPolicies_DAG(pDAG);
  if(generate_table8_DAG)   verifyminimalpolicy_DAG(pDAG);

  return 0;
}
