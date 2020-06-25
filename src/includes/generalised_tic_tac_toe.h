#ifndef GENERALISED_TIC_TAC_TOE_H
#define GENERALISED_TIC_TAC_TOE_H

#include <string>
#include <vector>
#include "game.hpp"

using namespace std;

class CharSS : public Game {
public:
  CharSS();
  CharSS(std::string board); // input a string of form 000011201.. till NxN where 0 represents _ , 1 respresents player and 2 represents opponent and it is row-wise
  ~CharSS();
  CharSS* clone() const;
  int legalMoves();
  unsigned long long int hashValue();
  string print_as_string() const;
private:
  char** board;
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
  vector<string> generateChildren();
  bool isPlayer();
};

#endif
