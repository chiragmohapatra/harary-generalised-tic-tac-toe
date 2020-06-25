#include "pn_search_unified.h"
#include "pn2_search.h"
#include "pn_search_DAG.h"
#include "monte_carlo_simulator.h"
#include "generalised_tic_tac_toe.h"
#include "generalised_tic_tac_toe_bitboard.h"
#include "check_proof.h"

int polyamino_type = 0;

int main(){
  /*bitset<M*N> b1(string(bset1));
  bitset<M*N> b2(string(bset2));
  bitset<M*N> b3(string(bset3));
  bitset<M*N> b4(string(bset4));
  vector<bitset<M*N>> user_polyaminos({b1 , b2 , b3 , b4});
  construct_polyaminos(user_polyaminos);*/

  pnSearchUnified::Param pUnified;
  pUnified.isMobile = false;
  pUnified.delete_sub_trees = false;
  pUnified.minimal_proof = true;
  pUnified.policy_applied = false;
  pUnified.depth_minimax = 1;
  pUnified.minimal_policy = false;

  //pnSearchUnified::pn_search_unified_main(pUnified);
  //pnSearchDAG::pn_search_DAG_main();
  //pn2_search_main();

  //check_proof_main();

  //polyamino_type = 0;

  int n = 100000;
  CharSS game;
  Bitboard game2;
  benchmark_count(game, n);
  benchmark_count(game2, n);

  double seconds = 1;
  benchmark_time(game, seconds);
  benchmark_time(game2, seconds);
}
