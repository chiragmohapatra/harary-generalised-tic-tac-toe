#include <bits/stdc++.h>

using namespace std;

/* The generalised tic tac toe is implemented on a 4x4 2-d character array with X,O or _ . X is the first player . 
   The desired polyamino is Tippy:
            |X|X|
              |X|X| 
    Sizes and polaminos can be updated , just update the evaluate function . 
    Currently , evaluate function : gives +10 if player 1 wins and -10 in case of draw or player 1 lose . */

#define N 4
#define player 'x'
#define opponent 'o'

class Game{
    public:
        char** board;
        int legal_moves;
        Game(){
            board = new char*[N];
            for(int i = 0 ; i < N ; i++)
                board[i] = new char[N];

            for(int i = 0 ; i < N ; i++){
                for(int j = 0 ; j < N ; j++)
                    board[i][j] = '_';
            }

            legal_moves = N*N;
        }

        ~Game(){
            for(int i = 0 ; i < N ; i++)
                delete [] board[i];

            delete [] board;
        }

        void set_board(char** board_status); // set board to board_status
        bool isMovesLeft(); //returns true if legal moves are left and false otherwise(completely filled board)
        bool isTerminal(); //returns true for a terminal position(player 1 win / player 1 lose / draw)
        int evaluate(); // return +10 for player 1 win , -10 for loss/draw and 0 for intermediate
        bool isValidMove(int,int); // returns true if move made is valid
        void make_move(bool,int,int);
        void undo_move(int,int); // undo the move at (int,int)
        void print_board();
};

void Game::set_board(char** board_status){
    int ctr = 0;
    for(int i = 0 ; i < N ; i++){
        for(int j = 0 ; j < N ; j++){
            board[i][j] = board_status[i][j];
            if(board_status[i][j] == '_')
                ctr++;
        }
    }

    legal_moves = ctr;
}

//returns true if legal moves are left and false otherwise(completely filled board)
bool Game::isMovesLeft(){
    if(legal_moves == 0)
        return false;

    else
        return true;
}

//returns true for a terminal position(player 1 win / player 1 lose / draw)
bool Game::isTerminal(){

    int i , j;

    for(i = 0 ; i < N ; i++){
        for(j = 0 ; j < (N-1) ; j++){
            if(board[i][j] == board[i][j+1] && board[i][j] != '_'){ // two in a row position like |X||X|
                if(j < (N-2) && i < (N-1) && board[i+1][j+1] == board[i+1][j+2] && board[i+1][j+1] == board[i][j]){ /* Represents a position like
                                                                                                                        |X||X|
                                                                                                                           |X||X| */
                    return true;

                }

                else if(j > 0 && i < (N-1) && board[i+1][j-1] == board[i+1][j] && board[i+1][j] == board[i][j]){ /* Represents a position like
                                                                                                                        |X||X|
                                                                                                                     |X||X| */
                    return true;

                }

                else if(i > 0 && i < (N-1) && board[i-1][j] == board[i][j] && board[i+1][j+1] == board[i][j]){ /* Represents a position like
                                                                                                                    |X|
                                                                                                                    |X||X|
                                                                                                                       |X| */
                    return true;

                }

                else if(i > 0 && i < (N-1) && board[i-1][j+1] == board[i][j] && board[i+1][j] == board[i][j]){  /* Represents a position like
                                                                                                                        |X|
                                                                                                                     |X||X|
                                                                                                                     |X| */
                    return true;

                }

            } 
        }
    }

    if(!isMovesLeft())
        return true;

    return false;
}


// return +10 for player 1 win , -10 for loss/draw and 0 for intermediate
int Game::evaluate(){
    int i , j;

    for(i = 0 ; i < N ; i++){
        for(j = 0 ; j < (N-1) ; j++){
            if(board[i][j] == board[i][j+1] && board[i][j] != '_'){ // two in a row position like |X||X|
                if(j < (N-2) && i < (N-1) && board[i+1][j+1] == board[i+1][j+2] && board[i+1][j+1] == board[i][j]){ /* Represents a position like
                                                                                                                        |X||X|
                                                                                                                           |X||X| */
                    if(board[i][j] == player)
                        return 10;
                    else
                        return -10;

                }

                else if(j > 0 && i < (N-1) && board[i+1][j-1] == board[i+1][j] && board[i+1][j] == board[i][j]){ /* Represents a position like
                                                                                                                        |X||X|
                                                                                                                     |X||X| */
                    if(board[i][j] == player)
                        return 10;
                    else
                        return -10;

                }

                else if(i > 0 && i < (N-1) && board[i-1][j] == board[i][j] && board[i+1][j+1] == board[i][j]){ /* Represents a position like
                                                                                                                    |X|
                                                                                                                    |X||X|
                                                                                                                       |X| */
                    if(board[i][j] == player)
                        return 10;
                    else
                        return -10;

                }

                else if(i > 0 && i < (N-1) && board[i-1][j+1] == board[i][j] && board[i+1][j] == board[i][j]){  /* Represents a position like
                                                                                                                        |X|
                                                                                                                     |X||X|
                                                                                                                     |X| */
                    if(board[i][j] == player)
                        return 10;
                    else
                        return -10;

                }

            } 
        }
    }

    if(!isMovesLeft())
        return -10;

    return 0;
}

bool Game::isValidMove(int row , int col){
    if(row >= N || col >= N)
        return false;

    if(board[row][col] != '_')
        return false;

    else
        return true;
}

void Game::make_move(bool isPlayer , int row , int col){
    if(board[row][col] != '_')
        return;

    else{
        if(isPlayer)
            board[row][col] = player;
        else
            board[row][col] = opponent;
        legal_moves--;
    }
}

void Game::undo_move(int row , int col){
    board[row][col] = '_';
    legal_moves++;
}

void Game::print_board(){
    for(int i = 0 ; i < N ; i++){
        for(int j = 0 ; j < N ; j++)
            cout<<board[i][j]<<" ";

        cout<<endl;
    }
}

// input a string of form 000011201.. till NxN where 0 represents _ , 1 respresents player and 2 represents opponent and it is row-wise
char** make_board_from_file(string str){
    char** board = new char*[N];
    char c;

    for(int i = 0 ; i < N ; i++)
        board[i] = new char[N];

    for(int i = 0 ; i < N ; i++){
        for(int j = 0 ; j < N ; j++){
            c = str.at(N*i + j);

            if(c == '0')
                board[i][j] = '_';

            else if(c == '1')
                board[i][j] = player;

            else
                board[i][j] = opponent;
            
        }
            
    }

    return board;
}
