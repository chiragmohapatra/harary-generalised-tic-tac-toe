#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <vector>

#define M 4
#define N 4
#define player 'x'
#define opponent 'o'

class Game{
public:
  virtual ~Game() {};
  virtual Game* clone() const = 0;

  virtual int legalMoves() = 0;
  virtual unsigned long long int hashValue() = 0;

  virtual bool isMovesLeft() = 0; //returns true if legal moves are left and false otherwise(completely filled board)
  virtual bool isTerminal() = 0; //returns true for a terminal position(player 1 win / player 1 lose / draw)
  virtual int evaluate() = 0; // return +10 for player 1 win , -10 for loss/draw and 0 for intermediate
  virtual bool isValidMove(int,int) = 0; // returns true if move made is valid
  virtual void make_move(bool,int,int) = 0;
  virtual void undo_move(int,int) = 0; // undo the move at (int,int)
  virtual void print_board() = 0;
  virtual std::string print_as_string() const = 0;
};

#endif
