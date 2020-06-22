#include <bits/stdc++.h>
#include "generalised_tic_tac_toe_improved.h"

using namespace std;

#define M 4
#define N 4
#define player 'x'
#define opponent 'o'

// Assume that white represents player and black represents opponent

bitset<(M+1)*(N+1)> expand_bitset_right_down(bitset<M*N> bset){
    int size = M*N;
    bitset<(M+1)*(N+1)> temp;

    for(int i = 0 ; i < size ; i++){
        temp[i + (N+2) + i/N] = bset[i];
    }

    return temp;
}

/*class Bitboard{
    public:
        bitset<(M+1)*(N+1)> expanded_whites; // the representation is rowwise with 1 if white is present and 0 if not
        bitset<(M+1)*(N+1)> expanded_blacks;
        int legal_moves;

        Bitboard(){
            legal_moves = M*N;
        }

        Bitboard(bitset<M*N> white_pos , bitset<M*N> black_pos){
            expanded_whites = expand_bitset_right_down(white_pos);
            expanded_blacks = expand_bitset_right_down(black_pos);

            legal_moves = M*N - white_pos.count() - black_pos.count();
        }

        bool isMovesLeft(); //returns true if legal moves are left and false otherwise(completely filled board)
        bool isTerminal(); //returns true for a terminal position(player 1 win / player 1 lose / draw)
        int evaluate(); // return +10 for player 1 win , -10 for loss/draw and 0 for intermediate
        bool isValidMove(int,int); // returns true if move made is valid
        void make_move(bool,int,int);
        void undo_move(int,int); // undo the move at (int,int)
        void print_board();
};*/

Bitboard::Bitboard(){
  legal_moves = M*N;
}

Bitboard::Bitboard(string str){
  legal_moves = 0;
  bitset<M*N> whites,blacks;
    for(int i = 0 ; i < M*N ; i++){
        char c = str.at(i);
        if(c == '1'){
            whites.set(i);
        }
        else if(c == '2')
            blacks.set(i);
        else
          legal_moves++;
    }

    expanded_whites = expand_bitset_right_down(whites);
    expanded_blacks = expand_bitset_right_down(blacks);
}

Bitboard::~Bitboard(){
}


Bitboard* Bitboard::clone() const {
  Bitboard* clone = new Bitboard(print_as_string());
  return clone; // AS: can be made faster by not going via strings
}

int Bitboard::legalMoves(){
    return legal_moves;
}

unsigned long long int Bitboard::hashValue(){
    assert(false);
//    return hash_value;
}


//returns true if legal moves are left and false otherwise(completely filled board)
bool Bitboard::isMovesLeft(){
    if(legal_moves == 0)
        return false;

    else
        return true;
}

bool Bitboard::isTerminal(){
    if(evaluate() == 0)
        return false;

    else{
        return true;
    }
}

int Bitboard::evaluate(){

    bitset<(M+1)*(N+1)> tempw1 = expanded_whites >> 1; // right
    bitset<(M+1)*(N+1)> tempw2 = tempw1 >> (N+1); // right and down
    bitset<(M+1)*(N+1)> tempw3 = tempw2 >> 1; // right , down , right

    if((expanded_whites & tempw1 & tempw2 & tempw3).any())
        return 10;

    tempw1 = expanded_whites >> (N+1); // down
    tempw2 = tempw1 >> 1; // down , right
    tempw3 = tempw2 >> (N+1);  // down , right , down

    if((expanded_whites & tempw1 & tempw2 & tempw3).any())
        return 10;

    tempw1 = expanded_whites >> 1; // right
    tempw2 = tempw1 << (N+1); // right and top
    tempw3 = tempw2 >> 1; // right , top , right

    if((expanded_whites & tempw1 & tempw2 & tempw3).any())
        return 10;

    tempw1 = expanded_whites << (N+1); // top
    tempw2= tempw1 >> 1; // top , right
    tempw3 = tempw2 << (N+1);  // down , left , down

    if((expanded_whites & tempw1 & tempw2 & tempw3).any())
        return 10;

    bitset<(M+1)*(N+1)> tempb1 = expanded_blacks >> 1;
    bitset<(M+1)*(N+1)> tempb2 = tempb1 >> (N+1);
    bitset<(M+1)*(N+1)> tempb3 = tempb2 >> 1;

    if((expanded_blacks & tempb1 & tempb2 & tempb3).any())
        return -10;

    tempb1 = expanded_blacks >> (N+1);
    tempb2 = tempb1 >> 1;
    tempb3 = tempb2 >> (N+1);

    if((expanded_blacks & tempb1 & tempb2 & tempb3).any())
        return -10;

    tempb1 = expanded_blacks >> 1; // left
    tempb2 = tempb1 << (N+1); // left and down
    tempb3 = tempb2 >> 1; // left , down , left

    if((expanded_blacks & tempb1 & tempb2 & tempb3).any())
        return -10;

    tempb1 = expanded_blacks << (N+1); // down
    tempb2 = tempb1 >> 1; // down , left
    tempb3 = tempb2 << (N+1);  // down , left , down

    if((expanded_blacks & tempb1 & tempb2 & tempb3).any())
        return -10;
    
    else if(!isMovesLeft())
        return -10;

    return 0;
}

// returns true if move made is valid
bool Bitboard::isValidMove(int row, int col) const{
    if(row >= M || col >= N)
        return false;

    int index = (M+1)*(N+1) - ((N+1)*row + col) - 1;

    if(expanded_whites.test(index) || expanded_blacks.test(index))
        return false;

    else
        return true;
}

void Bitboard::make_move(bool isPlayer , int row , int col){
    int index = (M+1)*(N+1) - ((N+1)*row + col) - 1;

    if(isPlayer)
        expanded_whites.set(index);
    else
        expanded_blacks.set(index);

    legal_moves--;
}

void Bitboard::undo_move(int row , int col){
    int index = (M+1)*(N+1) - ((N+1)*row + col) - 1;

    expanded_whites.reset(index);
    expanded_blacks.reset(index);

    legal_moves++;
}

void Bitboard::print_board(){

    for(int i = ((M+1)*(N+1) - 1) ; i > N ; i--){
        if(i % (N+1) == 0)
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

    for(int i = ((M+1)*(N+1) - 1) ; i > N ; i--){
        if(expanded_whites.test(i))
            str.push_back('1');
        else if(expanded_blacks.test(i))
            str.push_back('2');
        else
            str.push_back('0');
    }

    return str;
    
}

pair<bitset<M*N>,bitset<M*N>>* make_board_from_file(string str){
    bitset<M*N> whites , blacks;
    char c;

    for(int i = 0 ; i < M*N ; i++){
        c = str.at(i);

        if(c == '1'){
            whites.set(i);
        }

        else if(c == '2')
            blacks.set(i);
    }

    return new pair<bitset<M*N>,bitset<M*N>>(whites,blacks);
}



