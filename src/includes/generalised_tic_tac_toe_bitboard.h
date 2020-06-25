#ifndef GENERALISED_TIC_TAC_TOE_IMPROVED_H

#define GENERALISED_TIC_TAC_TOE_IMPROVED_H

#include <bitset>
#include "game.hpp"

using namespace std;

class Bitboard: public Game{
    public:
        Bitboard();
        Bitboard(std::string str);
        ~Bitboard();
        Bitboard* clone() const;
        int legalMoves();
        unsigned long long int hashValue();
        std::string print_as_string() const;

    private:

        bitset<(M+1)*(N+1)> expanded_whites; // the representation is rowwise with 1 if white is present and 0 if not
        bitset<(M+1)*(N+1)> expanded_blacks;
        int legal_moves;
        unsigned long long int hash_value;
        bool is_player;

        bool isMovesLeft(); //returns true if legal moves are left and false otherwise(completely filled board)
        bool isTerminal(); //returns true for a terminal position(player 1 win / player 1 lose / draw)
        int evaluate(); // return +10 for player 1 win , -10 for loss/draw and 0 for intermediate
        bool isValidMove(int,int) const; // returns true if move made is valid
        void make_move(int,int);
        void undo_move(int,int); // undo the move at (int,int)
        void print_board();
        bool isPlayer();
        vector<string> generateChildren();
};

pair<bitset<M*N>,bitset<M*N>>* make_board_from_file(string str);

#endif
