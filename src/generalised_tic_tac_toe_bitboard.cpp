#include <bits/stdc++.h>
#include "generalised_tic_tac_toe_bitboard.h"

using namespace std;

// Assume that white represents player and black represents opponent

unsigned long long int hash_func(string str){
    unsigned long long int ctr = 0;

    for(int i = 0 ; i < M*N ; i++){
        if(str[i] == '1')
            ctr+=(pow(3,i));
        else if(str[i] == '2')
            ctr+=(2*pow(3,i));
    }

    return ctr;
}

bitset<M*(2*N+2)> initializeEvens(){
  bitset<M*(2*N+2)> target;
  for(int i = 0; i < M*(2*N+2); i++){
    if(i % 2 == 0){
      target.set(i);
    }
  }
  return target;
}

bitset<M*(2*N+2)> evens(initializeEvens());
bitset<M*(2*N+2)> odds(initializeEvens()<<1);

Bitboard::Bitboard(){
  legal_moves = M*N;
  is_player = true;
  hash_value = 0;
}

Bitboard::Bitboard(string str){
  legal_moves = 0;
  //bitset<M*(2*N+2)> board;
  int cntw = 0 , cntb = 0;
  for(int row = 0; row < M; row++){
    for(int col = 0 ; col < N ; col++){
      const int index = (2*N+2)*row + 2*col;
      char c = str.at(N*row+col);
      if(c == '1'){
        board.set(index);
        cntw++;
      }
      else if(c == '2'){
        board.set(index+1);
        cntb++;
      }
      else
        legal_moves++;
    }
  }
  is_player = cntw == cntb;
  hash_value = hash_func(str);
}

Bitboard::~Bitboard(){
}

Bitboard* Bitboard::clone() const {
  Bitboard* clone = new Bitboard();
  clone->board = board;
  clone->legal_moves = legal_moves;
  clone->hash_value = hash_value;
  clone->is_player = is_player;

  return clone;
}

int Bitboard::legalMoves(){
    return legal_moves;
}

unsigned long long int Bitboard::hashValue(){
    return hash_value;
}

//returns true if legal moves are left and false otherwise(completely filled board)
bool Bitboard::isMovesLeft(){
  return legal_moves != 0;
}

bool Bitboard::isTerminal(){
  bitset<M*(2*N+2)> t1;
  switch(polyamino_type){
  case 0:
    t1 = board & (board >> 2*N+4); // here&right+down
    if((t1 & (t1 >> 2)).any()) return true; // (.&rd)&(r&rdr)
    if((t1 & (t1 >> 2*N+2)).any()) return true; // (.&rd)&(d&rdd)
    t1 = board & (board << 2*N); // .&ru
    if((t1 & (t1 >> 2)).any()) return true; // (.&ru)&(r&rur)
    if((t1 & (t1 << 2*N+2)).any()) return true; // (.&ru)&(u&ruu)
    break;
  case 1:
    t1 = board & (board >> 2); // here&right
    if((t1 & (t1 >> 2*N+2)).any()) return true; // .&r&d&rd
    break;
  case 2:
    t1 = board & (board >> 2);
    if((t1 & (t1 >> 2)).any()) return true;
    t1 = board & (board >> 2*N+2);
    if((t1 & (t1 >> 2*N+2)).any()) return true;
    break;
  }
  return !isMovesLeft();
}

int Bitboard::evaluate(){
    bitset<M*(2*N+2)> t1, t2;
    switch(polyamino_type){
        case 0:
            t1 = board & (board >> 2*N+4); // here&right+down
            t2 = t1 & (t1 >> 2); // (.&rd)&(r&rdr)
            if((t2 & evens).any()) return 10;
            if((t2 & odds).any()) return -10;
            t2 = t1 & (t1 >> 2*N+2); // (.&rd)&(d&rdd)
            if((t2 & evens).any()) return 10;
            if((t2 & odds).any()) return -10;
            t1 = board & (board << 2*N); // .&ru
            t2 = t1 & (t1 >> 2); // (.&ru)&(r&rur)
            if((t2 & evens).any()) return 10;
            if((t2 & odds).any()) return -10;
            t2 = t1 & (t1 << 2*N+2); // (.&ru)&(u&ruu)
            if((t2 & evens).any()) return 10;
            if((t2 & odds).any()) return -10;
            break;
        case 1:
            t1 = board & (board >> 2); // here&right
            t2 = t1 & (t1 >> 2*N+2); // .&r&d&rd
            if((t2 & evens).any()) return 10;
            if((t2 & odds).any()) return 10;
            break;
        case 2:
            t1 = board & (board >> 2);
            t2 = t1 & (t1 >> 2);
            if((t2 & evens).any()) return 10;
            if((t2 & odds).any()) return -10;
            t1 = board & (board >> 2*N+2);
            t2 = t1 & (t1 >> 2*N+2);
            if((t2 & evens).any()) return 10;
            if((t2 & odds).any()) return -10;
            break;
    }

    if(!isMovesLeft())
        return -10;

    return 0;
}

int Bitboard::score(){
  int score = 0;
  bitset<M*(2*N+2)> t1, t2;

  switch(polyamino_type){
      case 2:
          t1 = board & (board >> 1);
          t2 = t1 & (t1 >> 1);
          score = score + (t2 & evens).count() - (t2 & odds).count();
          t1 = board & (board >> 2*N);
          t2 = t1 & (t1 >> 2*N);
          score = score + (t2 & evens).count() - (t2 & odds).count();
          break;        
  }

  return score;
}

// returns true if move made is valid
bool Bitboard::isValidMove(int row, int col) const {
  assert(row < M && col < N);

  int index = (2*N+2)*row + 2*col;

  return !board.test(index) && !board.test(index+1);
}

void Bitboard::make_move(int row , int col){
  int index = (2*N+2)*row + 2*col;

  if(is_player) board.set(index);
  else          board.set(index+1);

  legal_moves--;
  if(is_player) hash_value +=   pow(3,N*row+col);
  else          hash_value += 2*pow(3,N*row+col);
  is_player = !is_player;
}

void Bitboard::undo_move(int row , int col){
  int index = (2*N+2)*row + 2*col;

  board.reset(index);
  board.reset(index+1);

  legal_moves++;
  if(is_player) hash_value -=   pow(3,N*row+col);
  else          hash_value -= 2*pow(3,N*row+col);
  is_player = !is_player;
}

bool Bitboard::isPlayer(){
  return is_player;
}

vector<string> Bitboard::generateChildren(){
    string str = print_as_string();
    vector<string> children;

    for(int i = 0 ; i < M*N ; i++){
        if(str[i] == '0'){
            if(is_player){
                str[i] = '1';
                children.push_back(str);
                str[i] = '0';
            }
            else{
                str[i] = '2';
                children.push_back(str);
                str[i] = '0';
            }
        }
    }

    return children;
}

void Bitboard::print_board(){
  for(int row = 0; row < M; row++){
    for(int col = 0 ; col < N ; col++){
      const int index = (2*N+2)*row + 2*col;
      if(board.test(index))         cout << "x ";
      else if(board.test(index+1))  cout << "o ";
      else                          cout << "_ ";
    }
    cout<<endl;
  }
}

string Bitboard::print_as_string() const {
  string str;
  for(int row = 0; row < M; row++){
    //str.push_back('\n');
    for(int col = 0 ; col < N ; col++){
      const int index = (2*N+2)*row + 2*col;
      if(board.test(index))        str.push_back('1');
      else if(board.test(index+1)) str.push_back('2');
      else                         str.push_back('0');
    }
  }

  return str;
}

pair<bitset<M*N>,bitset<M*N>>* make_board_from_file(string str){
    bitset<M*N> whites , blacks;
    for(int i = 0 ; i < M*N ; i++){
        char c = str.at(i);

        if(c == '1')
            whites.set(i);
        else if(c == '2')
            blacks.set(i);
    }

    return new pair<bitset<M*N>,bitset<M*N>>(whites,blacks);
}
