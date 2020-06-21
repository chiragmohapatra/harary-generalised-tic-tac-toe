#ifndef GENERALISED_TIC_TAC_TOE_BITBOARD_H
#define GENERALISED_TIC_TAC_TOE_BITBOARD_H

#include <bitset>
#include "game.hpp"

#define bset1 "1100011000000000"
#define bset2 "0110110000000000"
#define bset3 "1000110001000000"
#define bset4 "0100110010000000"

// Before making any object of Game , make sure to invoke construct_polyaminos() once
class Bitboard : public Game {
public:
  std::bitset<M*N> whites; // the representation is rowwise with 1 if white is present and 0 if not
  std::bitset<M*N> blacks;
  int legal_moves;

  Bitboard();
  Bitboard(std::string str);
  ~Bitboard();
  Bitboard* clone() const;
  int legalMoves();
  unsigned long long int hashValue();

  bool isMovesLeft(); //returns true if legal moves are left and false otherwise(completely filled board)
  bool isTerminal(); //returns true for a terminal position(player 1 win / player 1 lose / draw)
  int evaluate(); // return +10 for player 1 win , -10 for loss/draw and 0 for intermediate
  bool isValidMove(int, int) const; // returns true if move made is valid
  void make_move(bool,int,int);
  void undo_move(int,int); // undo the move at (int,int)
  std::string print_as_string() const;
  void print_board();
};

void construct_polyaminos(std::vector<std::bitset<M*N>> &user_polyaminos);
//void get_all_possible_polyaminos(vector<bitset<M*N>> &polyaminos , bitset<M*N> polyamino);

#endif
