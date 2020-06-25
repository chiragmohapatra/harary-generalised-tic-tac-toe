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

bitset<(M+1)*(N+1)> expand_bitset_right_down(bitset<M*N> bset){
    int size = M*N;
    bitset<(M+1)*(N+1)> temp;

    for(int i = 0 ; i < size ; i++){
        temp[i + (N+2) + i/N] = bset[i];
    }

    return temp;
}

Bitboard::Bitboard(){
  legal_moves = M*N;
  is_player = true;
  hash_value = 0;
}

Bitboard::Bitboard(string str){
  legal_moves = 0;
  bitset<M*N> whites,blacks;
  int cntw = 0 , cntb = 0;

    for(int i = 0 ; i < M*N ; i++){
        char c = str.at(i);
        if(c == '1'){
            whites.set(i);
            cntw++;
        }
        else if(c == '2'){
            blacks.set(i);
            cntb++;
        }
        else
          legal_moves++;
    }

    expanded_whites = expand_bitset_right_down(whites);
    expanded_blacks = expand_bitset_right_down(blacks);

    is_player = cntw == cntb;

    hash_value = hash_func(str);
}

Bitboard::~Bitboard(){
}

Bitboard* Bitboard::clone() const {
  Bitboard* clone = new Bitboard();
  clone->expanded_whites = expanded_whites;
  clone->expanded_blacks = expanded_blacks;
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
  return evaluate() != 0;
}

int Bitboard::evaluate(){
    bitset<(M+1)*(N+1)> t1;
    switch(polyamino_type){
        case 0:
            t1 = expanded_whites & (expanded_whites >> N+2); // here&right+down
            if((t1 & (t1 >> 1)).any()) return 10; // (.&rd)&(r&rdr)
            if((t1 & (t1 >> N+1)).any()) return 10; // (.&rd)&(d&rdd)
            t1 = expanded_whites & (expanded_whites << N); // .&ru
            if((t1 & (t1 >> 1)).any()) return 10; // (.&ru)&(r&rur)
            if((t1 & (t1 << N+1)).any()) return 10; // (.&ru)&(u&ruu)

            t1 = expanded_blacks & (expanded_blacks >> N+2);
            if((t1 & (t1 >> 1)).any()) return -10;
            if((t1 & (t1 >> N+1)).any()) return -10;
            t1 = expanded_blacks & (expanded_blacks << N);
            if((t1 & (t1 >> 1)).any()) return -10;
            if((t1 & (t1 << N+1)).any()) return -10;
            break;
        case 1:
            t1 = expanded_whites & (expanded_whites >> 1); // here&right
            if((t1 & (t1 >> N+1)).any()) return 10; // .&r&d&rd

            t1 = expanded_blacks & (expanded_blacks >> 1);
            if((t1 & (t1 >> N+1)).any()) return -10;
            break;
        case 2:
            t1 = expanded_whites & (expanded_whites >> 1);
            if((t1 & (t1 >> 1)).any()) return 10;
            t1 = expanded_whites & (expanded_whites >> N+1);
            if((t1 & (t1 >> N+1)).any()) return 10;

            t1 = expanded_blacks & (expanded_blacks >> 1);
            if((t1 & (t1 >> 1)).any()) return -10;
            t1 = expanded_blacks & (expanded_blacks >> N+1);
            if((t1 & (t1 >> N+1)).any()) return -10;
            break;
    }

    if(!isMovesLeft())
        return -10;

    return 0;
}

// returns true if move made is valid
bool Bitboard::isValidMove(int row, int col) const {
  assert(row < M && col < N);

  int index = (N+1)*row + col;

  return !expanded_whites.test(index) && !expanded_blacks.test(index);
}

void Bitboard::make_move(int row , int col){
  int index = (N+1)*row + col;

  if(is_player) expanded_whites.set(index);
  else          expanded_blacks.set(index);

  legal_moves--;
  if(is_player) hash_value +=   pow(3,N*row+col);
  else          hash_value += 2*pow(3,N*row+col);
  is_player = !is_player;
}

void Bitboard::undo_move(int row , int col){
  int index = (N+1)*row + col;

  expanded_whites.reset(index);
  expanded_blacks.reset(index);

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
    for(int i = 0 ; i < M*(N+1) ; i++){
        if(i % (N+1) == N)
            cout<<endl;
        else if(expanded_whites.test(i))
            cout<<"x ";
        else if(expanded_blacks.test(i))
            cout<<"o ";
        else
            cout<<"_ ";
    }
}

string Bitboard::print_as_string() const {
    string str;

    for(int i = 0 ; i < M*(N+1) ; i++){
//        if(i % (N+1) == N) str.push_back('\n');
        if(i % (N+1) != N) {
          if(expanded_whites.test(i))
            str.push_back('1');
          else if(expanded_blacks.test(i))
            str.push_back('2');
          else
            str.push_back('0');
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
