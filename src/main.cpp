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

  pnSearchUnified::pn_search_unified_main();
  pnSearchDAG::pn_search_DAG_main();
  //pn2_search_main();

  //check_proof_main();

  //polyamino_type = 0;

  CharSS game;
  monte_carlo_simulator_main(game);
  Bitboard game2;
  monte_carlo_simulator_main(game2);
}
