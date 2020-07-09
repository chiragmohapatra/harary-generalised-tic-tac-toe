#ifndef PN_SEARCH_DAG_H
#define PN_SEARCH_DAG_H

#include "game.hpp"

namespace pnSearchDAG {
  struct Param {
  bool isMobile; // determines whether pn_search is mobile or not
  bool minimal_proof; // determines whether to run minimal proof search
  bool policy_applied;
  int depth_minimax;
  bool minimal_policy; // determines whether to run minimal policy search or not
  int policy_type; // 0 for first move , 1 for minimax
  int board_type; // 0 for CharSS and 1 for Bitboard
  coeff_score coeff;
};

  int pn_search_DAG_main(const Param & parameters);
  void verify_policies(const Param & parameters);
  void verify_minimal_policy(const Param &parameters);
  void generate_time_and_memory(const Param &parameters);
}

#endif
