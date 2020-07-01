#ifndef PN_SEARCH_UNIFIED_H

#define PN_SEARCH_UNIFIED_H

namespace pnSearchUnified {

struct Param {
  bool isMobile; // determines whether pn_search is mobile or not
  bool delete_sub_trees; // determines whether sub trees will be deleted or not
  bool minimal_proof; // determines whether to run minimal proof search
  bool policy_applied;
  int depth_minimax;
  bool minimal_policy; // determines whether to run minimal policy search or not
  int policy_type; // 0 for first move , 1 for minimax
  int board_type; // 0 for CharSS and 1 for Bitboard
};

int pn_search_unified_main(const Param & parameters);
void verify_policies(const Param & parameters);
}

#endif
