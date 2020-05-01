#include <bits/stdc++.h>

using namespace std;

#define M 4
#define N 4
#define player 'x'
#define opponent 'o'

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

bitset<(M+1)*(N+1)> expand_bitset_right_down(bitset<M*N> bset){
    int size = M*N;
    bitset<(M+1)*(N+1)> temp;

    for(int i = 0 ; i < size ; i++){
        temp[i + (N+2) + i/N] = bset[i];
    }

    return temp;
}

bitset<(M+1)*(N+1)> expand_bitset_left_down(bitset<M*N> bset){
    int size = M*N;
    bitset<(M+1)*(N+1)> temp;

    for(int i = 0 ; i < size ; i++){
        temp[i + (N+1) + i/N] = bset[i];
    }

    return temp;
}

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
        bool isTerminal(); //returns true for a terminal position(player 1 win / player 1 lose / draw)
        int evaluate(); // return +10 for player 1 win , -10 for loss/draw and 0 for intermediate
        bool isValidMove(int,int); // returns true if move made is valid
        void make_move(bool,int,int);
        void undo_move(int,int); // undo the move at (int,int)
        void print_board();
};

//returns true if legal moves are left and false otherwise(completely filled board)
bool Game::isMovesLeft(){
    if(legal_moves == 0)
        return false;

    else
        return true;
}

bool Game::isTerminal(){
    if(!isMovesLeft())
        return true;

    bitset<(M+1)*(N+1)> expanded_whites1 = expand_bitset_right_down(whites);
    bitset<(M+1)*(N+1)> expanded_whites2 = expand_bitset_left_down(whites);
    bitset<(M+1)*(N+1)> expanded_blacks1 = expand_bitset_right_down(blacks);
    bitset<(M+1)*(N+1)> expanded_blacks2 = expand_bitset_left_down(blacks);

    bitset<(M+1)*(N+1)> tempw1 = expanded_whites1 >> 1; // right
    bitset<(M+1)*(N+1)> tempw2 = tempw1 >> (N+1); // right and down
    bitset<(M+1)*(N+1)> tempw3 = tempw2 >> 1; // right , down , right

    bitset<(M+1)*(N+1)> tempw4 = expanded_whites1 >> (N+1); // down
    bitset<(M+1)*(N+1)> tempw5 = tempw4 >> 1; // down , right
    bitset<(M+1)*(N+1)> tempw6 = tempw5 >> (N+1);  // down , right , down

    bitset<(M+1)*(N+1)> tempw7 = expanded_whites2 << 1; // left
    bitset<(M+1)*(N+1)> tempw8 = tempw7 >> (N+1); // left and down
    bitset<(M+1)*(N+1)> tempw9 = tempw8 << 1; // left , down , left

    bitset<(M+1)*(N+1)> tempw10 = expanded_whites2 >> (N+1); // down
    bitset<(M+1)*(N+1)> tempw11= tempw10 << 1; // down , left
    bitset<(M+1)*(N+1)> tempw12 = tempw11 >> (N+1);  // down , left , down

    
    bitset<(M+1)*(N+1)> tempb1 = expanded_blacks1 >> 1;
    bitset<(M+1)*(N+1)> tempb2 = tempb1 >> (N+1);
    bitset<(M+1)*(N+1)> tempb3 = tempb2 >> 1;

    bitset<(M+1)*(N+1)> tempb4 = expanded_blacks1 >> (N+1);
    bitset<(M+1)*(N+1)> tempb5 = tempb4 >> 1;
    bitset<(M+1)*(N+1)> tempb6 = tempb5 >> (N+1);

    bitset<(M+1)*(N+1)> tempb7 = expanded_blacks2 << 1; // left
    bitset<(M+1)*(N+1)> tempb8 = tempb7 >> (N+1); // left and down
    bitset<(M+1)*(N+1)> tempb9 = tempb8 << 1; // left , down , left

    bitset<(M+1)*(N+1)> tempb10 = expanded_blacks2 >> (N+1); // down
    bitset<(M+1)*(N+1)> tempb11= tempb10 << 1; // down , left
    bitset<(M+1)*(N+1)> tempb12 = tempb11 >> (N+1);  // down , left , down



    int cntw1 = (expanded_whites1 & tempw1 & tempw2 & tempw3).count();
    int cntw2 = (expanded_whites1 & tempw4 & tempw5 & tempw6).count();
    int cntw3 = (expanded_whites2 & tempw7 & tempw8 & tempw9).count();
    int cntw4 = (expanded_whites2 & tempw10 & tempw11 & tempw12).count();

    int cntb1 = (expanded_blacks1 & tempb1 & tempb2 & tempb3).count();
    int cntb2 = (expanded_blacks1 & tempb4 & tempb5 & tempb6).count();
    int cntb3 = (expanded_blacks2 & tempb7 & tempb8 & tempb9).count();
    int cntb4 = (expanded_blacks2 & tempb10 & tempb11 & tempb12).count();

    if(cntw1 == 1 || cntw2 == 1 || cntw3 == 1 || cntw4 == 1 || cntb1 == 1 || cntb2 == 1 || cntb3 == 1 || cntb4 == 1)
        return true;

    else
        return false;
}

int Game::evaluate(){

    bitset<(M+1)*(N+1)> expanded_whites1 = expand_bitset_right_down(whites);
    bitset<(M+1)*(N+1)> expanded_whites2 = expand_bitset_left_down(whites);
    bitset<(M+1)*(N+1)> expanded_blacks1 = expand_bitset_right_down(blacks);
    bitset<(M+1)*(N+1)> expanded_blacks2 = expand_bitset_left_down(blacks);

    bitset<(M+1)*(N+1)> tempw1 = expanded_whites1 >> 1; // right
    bitset<(M+1)*(N+1)> tempw2 = tempw1 >> (N+1); // right and down
    bitset<(M+1)*(N+1)> tempw3 = tempw2 >> 1; // right , down , right

    bitset<(M+1)*(N+1)> tempw4 = expanded_whites1 >> (N+1); // down
    bitset<(M+1)*(N+1)> tempw5 = tempw4 >> 1; // down , right
    bitset<(M+1)*(N+1)> tempw6 = tempw5 >> (N+1);  // down , right , down

    bitset<(M+1)*(N+1)> tempw7 = expanded_whites2 << 1; // left
    bitset<(M+1)*(N+1)> tempw8 = tempw7 >> (N+1); // left and down
    bitset<(M+1)*(N+1)> tempw9 = tempw8 << 1; // left , down , left

    bitset<(M+1)*(N+1)> tempw10 = expanded_whites2 >> (N+1); // down
    bitset<(M+1)*(N+1)> tempw11= tempw10 << 1; // down , left
    bitset<(M+1)*(N+1)> tempw12 = tempw11 >> (N+1);  // down , left , down

    
    bitset<(M+1)*(N+1)> tempb1 = expanded_blacks1 >> 1;
    bitset<(M+1)*(N+1)> tempb2 = tempb1 >> (N+1);
    bitset<(M+1)*(N+1)> tempb3 = tempb2 >> 1;

    bitset<(M+1)*(N+1)> tempb4 = expanded_blacks1 >> (N+1);
    bitset<(M+1)*(N+1)> tempb5 = tempb4 >> 1;
    bitset<(M+1)*(N+1)> tempb6 = tempb5 >> (N+1);

    bitset<(M+1)*(N+1)> tempb7 = expanded_blacks2 << 1; // left
    bitset<(M+1)*(N+1)> tempb8 = tempb7 >> (N+1); // left and down
    bitset<(M+1)*(N+1)> tempb9 = tempb8 << 1; // left , down , left

    bitset<(M+1)*(N+1)> tempb10 = expanded_blacks2 >> (N+1); // down
    bitset<(M+1)*(N+1)> tempb11= tempb10 << 1; // down , left
    bitset<(M+1)*(N+1)> tempb12 = tempb11 >> (N+1);  // down , left , down



    int cntw1 = (expanded_whites1 & tempw1 & tempw2 & tempw3).count();
    int cntw2 = (expanded_whites1 & tempw4 & tempw5 & tempw6).count();
    int cntw3 = (expanded_whites2 & tempw7 & tempw8 & tempw9).count();
    int cntw4 = (expanded_whites2 & tempw10 & tempw11 & tempw12).count();

    int cntb1 = (expanded_blacks1 & tempb1 & tempb2 & tempb3).count();
    int cntb2 = (expanded_blacks1 & tempb4 & tempb5 & tempb6).count();
    int cntb3 = (expanded_blacks2 & tempb7 & tempb8 & tempb9).count();
    int cntb4 = (expanded_blacks2 & tempb10 & tempb11 & tempb12).count();

    if(cntw1 == 1 || cntw2 == 1 || cntw3 == 1 || cntw4 == 1)
        return 10;

    else if(cntb1 == 1 || cntb2 == 1 || cntb3 == 1 || cntb4 == 1)
        return -10;
    
    else if(!isMovesLeft())
        return -10;

    return 0;
}

// returns true if move made is valid
bool Game::isValidMove(int row, int col){
    if(row >= M || col >= N)
        return false;

    int index = M*N - (M*row + col) - 1;

    if(whites.test(index) || blacks.test(index))
        return false;

    else
        return true;
}

void Game::make_move(bool isPlayer , int row , int col){
    int index = M*N - (M*row + col) - 1;

    if(isPlayer)
        whites.set(index);
    else
        blacks.set(index);

    legal_moves--;
}

void Game::undo_move(int row , int col){
    int index = M*N - (M*row + col) - 1;

    whites.reset(index);
    blacks.reset(index);

    legal_moves++;
}

void Game::print_board(){

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

    bitset<16> whites(string("0110001100000000"));
    bitset<16> blacks(string("1001000011000000"));

    cout<<expand_bitset(whites)<<endl<<expand_bitset(blacks)<<endl;


    return 0;
}*/

