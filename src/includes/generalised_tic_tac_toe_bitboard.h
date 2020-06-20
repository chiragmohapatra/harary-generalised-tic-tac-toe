#ifndef GENERALISED_TIC_TAC_TOE_BITBOARD_H

#define GENERALISED_TIC_TAC_TOE_BITBOARD_H

#include <bitset>

using namespace std;

#define M 4
#define N 4
#define player 'x'
#define opponent 'o'
#define bset1 "1100011000000000"
#define bset2 "0110110000000000"
#define bset3 "1000110001000000"
#define bset4 "0100110010000000"

class Game{
    public:
        bitset<M*N> whites; // the representation is rowwise with 1 if white is present and 0 if not
        bitset<M*N> blacks;
        int legal_moves;

        Game(){
            legal_moves = M*N;
        }

        Game(bitset<M*N> white_pos , bitset<M*N> black_pos){
            whites = white_pos;
            blacks = black_pos;

            legal_moves = M*N - whites.count() - blacks.count();
        }

        bool isMovesLeft(); //returns true if legal moves are left and false otherwise(completely filled board)
        bool isTerminal(vector<bitset<M*N>> &polyaminos); //returns true for a terminal position(player 1 win / player 1 lose / draw)
        int evaluate(vector<bitset<M*N>> &polyaminos); // return +10 for player 1 win , -10 for loss/draw and 0 for intermediate
        bool isValidMove(int,int); // returns true if move made is valid
        void make_move(bool,int,int);
        void undo_move(int,int); // undo the move at (int,int)
        void print_board();
};

void construct_polyaminos(vector<bitset<M*N>> &polyaminos , vector<bitset<M*N>> &user_polyaminos);
void get_all_possible_polyaminos(vector<bitset<M*N>> &polyaminos , bitset<M*N> polyamino);
pair<bitset<M*N>,bitset<M*N>>* make_board_from_file(string str);

#endif