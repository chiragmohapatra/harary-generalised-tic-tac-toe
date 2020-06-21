#include <bits/stdc++.h>
#include "generalised_tic_tac_toe_bitboard.h"

using namespace std;

// Assume that white represents player and black represents opponent

/* polyamino is generalised such that the user has to enter the simplest form of the polyamino required in terms of a bitboard..start from the top left ideally
   Enter the basic polyamino in MxN string type
   example : 11000..(N times) 
             01100...
             00...
             .
             .
             (M times)
    is what I consider the simplest form of the polyamino Tippy |X||X| and similarly enter the reverse horizontal as well as the vertical forms
                                                                   |X||X|
    Do this for any polyamino that is required 
    The basic concept is to store all the possible winning opportunities initially as an array of bitvectors and then for our board , perform bitwise AND with each
    combination till the same combination is reached , that gives the winning condition .. This is done for both white and black */

vector<bitset<M*N>> polyaminos;

void get_all_possible_polyaminos(bitset<M*N> polyamino){
    int vertical_shifts_allowed , horizontal_shifts_allowed = N , size = M*N;

    for(int i = 0 ; i < size ; i++){
        if(polyamino.test(i)){
            vertical_shifts_allowed = i/N + 1;
            break;
        }
    }

    for(int i = 0 ; i < size ; i++){
        if(polyamino.test(i)){
            if(horizontal_shifts_allowed > i%N)
                horizontal_shifts_allowed = i%N;
        }
    }

    bitset<M*N> vertical_dummy = polyamino;

    for(int i = 0 ; i < vertical_shifts_allowed ; i++){
        polyaminos.push_back(vertical_dummy);
        bitset<M*N> horizontal_dummy = vertical_dummy;
        for(int j = 0 ; j < horizontal_shifts_allowed ; j++){
            polyaminos.push_back(horizontal_dummy >>= 1);
        }
        vertical_dummy >>= N;
    }
}

// store all possible in polyaminos
void construct_polyaminos(vector<bitset<M*N>> &user_polyaminos){
  polyaminos.clear();
  for(int i = 0 ; i < user_polyaminos.size() ; i++)
    get_all_possible_polyaminos(user_polyaminos[i]);
//  for(int i = 0; i < polyaminos.size(); i++)
//    cout << polyaminos[i].to_string()<< endl;
}

Bitboard::Bitboard(){
  legal_moves = M*N;
}

Bitboard::Bitboard(string str){
  legal_moves = 0;
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
//  legal_moves = M*N - whites.count() - blacks.count();
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
    if(!isMovesLeft())
        return true;

    for(int i = 0 ; i < polyaminos.size() ; i++){
        if((whites & polyaminos[i]) == polyaminos[i])
            return true;
        else if((blacks & polyaminos[i]) == polyaminos[i])
            return true;
    }

    return false;
}

int Bitboard::evaluate(){
    for(int i = 0 ; i < polyaminos.size() ; i++){
        if((whites & polyaminos[i]) == polyaminos[i])
            return 10;
        else if((blacks & polyaminos[i]) == polyaminos[i])
            return -10;
    }

    if(!isMovesLeft())
        return -10;

    return 0;
}

// returns true if move made is valid
bool Bitboard::isValidMove(int row, int col) const {
    if(row >= M || col >= N)
        return false;

//    int index = M*N - (M*row + col) - 1;
    int index = M*row + col;

    if(whites.test(index) || blacks.test(index))
        return false;

    else
        return true;
}

void Bitboard::make_move(bool isPlayer , int row , int col){
//    int index = M*N - (M*row + col) - 1;
    int index = M*row + col;

    if(isPlayer)
        whites.set(index);
    else
        blacks.set(index);

    legal_moves--;
}

void Bitboard::undo_move(int row , int col){
//    int index = M*N - (M*row + col) - 1;
    int index = M*row + col;

    whites.reset(index);
    blacks.reset(index);

    legal_moves++;
}

void Bitboard::print_board(){

    for(int i = (M*N - 1) ; i >= 0 ; i--){
        if(whites.test(i))
            cout<<"x ";
        else if(blacks.test(i))
            cout<<"o ";
        else
            cout<<"_ ";

        if(i % N == 0)
            cout<<endl;
    }
}

string Bitboard::print_as_string() const {
    string str;
    for(int row = 0 ; row < M ; row++){
        for(int col = 0 ; col < N ; col++){
//          int i = M*N - (M*row + col) - 1;
    int i = M*row + col;
//          int i = M*row + col;
            if(whites.test(i))
                str.push_back('1');
            else if(blacks.test(i))
                str.push_back('2');
            else
                str.push_back('0');
        }
    }

    return str;
}
/*void print_as_board(bitset<M*N> str){
    for(int i = (M*N - 1) ; i >= 0 ; i--){
        if(str.test(i))
            cout<<"x ";
        else
            cout<<"_ ";

        if(i % N == 0)
            cout<<endl;
    }
}

int main(){
    construct_polyaminos();

    bitset<16> whites(string("0110001100000000"));
    bitset<16> blacks(string("1001000011000000"));

    Game* g = new Game(whites , blacks);

    cout<<g->evaluate()<<endl<<g->isTerminal()<<endl;


    return 0;
}*/

