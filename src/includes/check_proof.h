#ifndef CHECK_PROOF_H

#define CHECK_PROOF_H

namespace checkProof{

    struct Param_check{
        bool policy_applied;
        int depth_minimax;
        int policy_type;
        int board_type; // 0 for CharSS and 1 for Bitboard
    };

    int check_proof_main(const Param_check & parameters);

}

#endif