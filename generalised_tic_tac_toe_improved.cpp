#include <bits/stdc++.h>

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

class Game{
    public:
        bitset<(M+1)*(N+1)> expanded_whites; // the representation is rowwise with 1 if white is present and 0 if not
        bitset<(M+1)*(N+1)> expanded_blacks;
        int legal_moves;

        Game(){
            legal_moves = M*N;
        }

        Game(bitset<M*N> white_pos , bitset<M*N> black_pos){
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
};

//returns true if legal moves are left and false otherwise(completely filled board)
bool Game::isMovesLeft(){
    if(legal_moves == 0)
        return false;

    else
        return true;
}

bool Game::isTerminal(){
    if(evaluate() == 0)
        return false;

    else{
        return true;
    }
}

int Game::evaluate(){

    bitset<(M+1)*(N+1)> tempw1 = expanded_whites >> 1; // right
    bitset<(M+1)*(N+1)> tempw2 = tempw1 >> (N+1); // right and down
    bitset<(M+1)*(N+1)> tempw3 = tempw2 >> 1; // right , down , right

    bitset<(M+1)*(N+1)> tempw4 = expanded_whites >> (N+1); // down
    bitset<(M+1)*(N+1)> tempw5 = tempw4 >> 1; // down , right
    bitset<(M+1)*(N+1)> tempw6 = tempw5 >> (N+1);  // down , right , down

    bitset<(M+1)*(N+1)> tempw7 = expanded_whites >> 1; // right
    bitset<(M+1)*(N+1)> tempw8 = tempw7 << (N+1); // right and top
    bitset<(M+1)*(N+1)> tempw9 = tempw8 >> 1; // right , top , right

    bitset<(M+1)*(N+1)> tempw10 = expanded_whites << (N+1); // top
    bitset<(M+1)*(N+1)> tempw11= tempw10 >> 1; // top , right
    bitset<(M+1)*(N+1)> tempw12 = tempw11 << (N+1);  // down , left , down

    
    bitset<(M+1)*(N+1)> tempb1 = expanded_blacks >> 1;
    bitset<(M+1)*(N+1)> tempb2 = tempb1 >> (N+1);
    bitset<(M+1)*(N+1)> tempb3 = tempb2 >> 1;

    bitset<(M+1)*(N+1)> tempb4 = expanded_blacks >> (N+1);
    bitset<(M+1)*(N+1)> tempb5 = tempb4 >> 1;
    bitset<(M+1)*(N+1)> tempb6 = tempb5 >> (N+1);

    bitset<(M+1)*(N+1)> tempb7 = expanded_blacks >> 1; // left
    bitset<(M+1)*(N+1)> tempb8 = tempb7 << (N+1); // left and down
    bitset<(M+1)*(N+1)> tempb9 = tempb8 >> 1; // left , down , left

    bitset<(M+1)*(N+1)> tempb10 = expanded_blacks << (N+1); // down
    bitset<(M+1)*(N+1)> tempb11= tempb10 >> 1; // down , left
    bitset<(M+1)*(N+1)> tempb12 = tempb11 << (N+1);  // down , left , down

    bool cntw1 = (expanded_whites & tempw1 & tempw2 & tempw3).any();
    bool cntw2 = (expanded_whites & tempw4 & tempw5 & tempw6).any();
    bool cntw3 = (expanded_whites & tempw7 & tempw8 & tempw9).any();
    bool cntw4 = (expanded_whites & tempw10 & tempw11 & tempw12).any();

    bool cntb1 = (expanded_blacks & tempb1 & tempb2 & tempb3).any();
    bool cntb2 = (expanded_blacks & tempb4 & tempb5 & tempb6).any();
    bool cntb3 = (expanded_blacks & tempb7 & tempb8 & tempb9).any();
    bool cntb4 = (expanded_blacks & tempb10 & tempb11 & tempb12).any();

    if(cntw1  || cntw2  || cntw3|| cntw4)
        return 10;

    else if(cntb1  || cntb2 || cntb3 || cntb4)
        return -10;
    
    else if(!isMovesLeft())
        return -10;

    return 0;
}

// returns true if move made is valid
bool Game::isValidMove(int row, int col){
    if(row >= M || col >= N)
        return false;

    int index = (M+1)*(N+1) - ((N+1)*row + col) - 1;

    if(expanded_whites.test(index) || expanded_blacks.test(index))
        return false;

    else
        return true;
}

void Game::make_move(bool isPlayer , int row , int col){
    int index = (M+1)*(N+1) - ((N+1)*row + col) - 1;

    if(isPlayer)
        expanded_whites.set(index);
    else
        expanded_blacks.set(index);

    legal_moves--;
}

void Game::undo_move(int row , int col){
    int index = (M+1)*(N+1) - ((N+1)*row + col) - 1;

    expanded_whites.reset(index);
    expanded_blacks.reset(index);

    legal_moves++;
}

void Game::print_board(){

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
