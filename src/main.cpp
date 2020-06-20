#include "pn_search_unified.h"
#include "monte_carlo_simulator.h"
#include "generalised_tic_tac_toe.h"
#include "generalised_tic_tac_toe_bitboard.h"

int main(){
  bitset<M*N> b1(string(bset1));
  bitset<M*N> b2(string(bset2));
  bitset<M*N> b3(string(bset3));
  bitset<M*N> b4(string(bset4));
  vector<bitset<M*N>> user_polyaminos({b1 , b2 , b3 , b4});
  construct_polyaminos(user_polyaminos);

  pn_search_unified_main();

  CharSS game;
  monte_carlo_simulator_main(game);
  Bitboard game2;
  monte_carlo_simulator_main(game2);
}
