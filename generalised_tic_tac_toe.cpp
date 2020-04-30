#include <bits/stdc++.h>

using namespace std;

/* The generalised tic tac toe is implemented on a 4x4 2-d character array with X,O or _ . X is the first player . 
   The desired polyamino is Tippy:
            |X|X|
              |X|X| 
    Sizes and polyaminos can be updated , just update the evaluate function . 
    Currently , evaluate function : gives +10 if player 1 wins and -10 in case of draw or player 1 lose . 
    The board is of size MxN i.e M rows and N columns*/

#define M 4
#define N 4
#define player 'x'
#define opponent 'o'

unsigned long long int hash_func(char**);

class Game{
    public:
        char** board;
        int legal_moves;
        unsigned long long int hash_value;
        Game(){
            board = new char*[M];
            for(int i = 0 ; i < M ; i++)
                board[i] = new char[N];

            for(int i = 0 ; i < M ; i++){
                for(int j = 0 ; j < N ; j++)
                    board[i][j] = '_';
            }

            legal_moves = M*N;
            hash_value = 0;
        }

        Game(char** board_status); // set board to board_status

        ~Game(){
            for(int i = 0 ; i < M ; i++)
                delete [] board[i];

            delete [] board;
        }

        bool isMovesLeft(); //returns true if legal moves are left and false otherwise(completely filled board)
        bool isTerminal(); //returns true for a terminal position(player 1 win / player 1 lose / draw)
        int evaluate(); // return +10 for player 1 win , -10 for loss/draw and 0 for intermediate
        bool isValidMove(int,int); // returns true if move made is valid
        void make_move(bool,int,int);
        void undo_move(int,int); // undo the move at (int,int)
        void print_board();
};

Game::Game(char** board_status){
    int ctr = 0;
    board = new char*[M];
    for(int i = 0 ; i < M ; i++)
        board[i] = new char[N];

    for(int i = 0 ; i < M ; i++){
        for(int j = 0 ; j < N ; j++){
            board[i][j] = board_status[i][j];
            if(board_status[i][j] == '_')
                ctr++;
        }
    }

    legal_moves = ctr;
    hash_value = hash_func(board_status);
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

    if(!isMovesLeft())
        return true;

    for(i = 0 ; i < M ; i++){
        for(j = 0 ; j < (N-1) ; j++){
            if(board[i][j] == board[i][j+1] && board[i][j] != '_'){ // two in a row position like |X||X|
                if(j < (N-2) && i < (M-1) && board[i+1][j+1] == board[i+1][j+2] && board[i+1][j+1] == board[i][j]){ /* Represents a position like
                                                                                                                        |X||X|
                                                                                                                           |X||X| */
                    return true;

                }

                else if(j > 0 && i < (M-1) && board[i+1][j-1] == board[i+1][j] && board[i+1][j] == board[i][j]){ /* Represents a position like
                                                                                                                        |X||X|
                                                                                                                     |X||X| */
                    return true;

                }

                else if(i > 0 && i < (M-1) && board[i-1][j] == board[i][j] && board[i+1][j+1] == board[i][j]){ /* Represents a position like
                                                                                                                    |X|
                                                                                                                    |X||X|
                                                                                                                       |X| */
                    return true;

                }

                else if(i > 0 && i < (M-1) && board[i-1][j+1] == board[i][j] && board[i+1][j] == board[i][j]){  /* Represents a position like
                                                                                                                        |X|
                                                                                                                     |X||X|
                                                                                                                     |X| */
                    return true;

                }

            } 
        }
    }

    return false;
}


// return +10 for player 1 win , -10 for loss/draw and 0 for intermediate
int Game::evaluate(){
    int i , j;

    for(i = 0 ; i < M ; i++){
        for(j = 0 ; j < (N-1) ; j++){
            if(board[i][j] == board[i][j+1] && board[i][j] != '_'){ // two in a row position like |X||X|
                if(j < (N-2) && i < (M-1) && board[i+1][j+1] == board[i+1][j+2] && board[i+1][j+1] == board[i][j]){ /* Represents a position like
                                                                                                                        |X||X|
                                                                                                                           |X||X| */
                    if(board[i][j] == player)
                        return 10;
                    else
                        return -10;

                }

                else if(j > 0 && i < (M-1) && board[i+1][j-1] == board[i+1][j] && board[i+1][j] == board[i][j]){ /* Represents a position like
                                                                                                                        |X||X|
                                                                                                                     |X||X| */
                    if(board[i][j] == player)
                        return 10;
                    else
                        return -10;

                }

                else if(i > 0 && i < (M-1) && board[i-1][j] == board[i][j] && board[i+1][j+1] == board[i][j]){ /* Represents a position like
                                                                                                                    |X|
                                                                                                                    |X||X|
                                                                                                                       |X| */
                    if(board[i][j] == player)
                        return 10;
                    else
                        return -10;

                }

                else if(i > 0 && i < (M-1) && board[i-1][j+1] == board[i][j] && board[i+1][j] == board[i][j]){  /* Represents a position like
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
    if(row >= M || col >= N)
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
        if(isPlayer){
            board[row][col] = player;
            hash_value+=(pow(3,N*row + col));
        }
        else{
            board[row][col] = opponent;
            hash_value+=(2*(pow(3,N*row + col)));
        }

        legal_moves--;
    }
}

void Game::undo_move(int row , int col){
    if(board[row][col] == player)
        hash_value-=(pow(3,N*row + col));
    else
        hash_value-=(2*(pow(3,N*row + col)));

    board[row][col] = '_';
    legal_moves++;
}

void Game::print_board(){
    for(int i = 0 ; i < M ; i++){
        for(int j = 0 ; j < N ; j++)
            cout<<board[i][j]<<" ";

        cout<<endl;
    }
}

// input a string of form 000011201.. till NxN where 0 represents _ , 1 respresents player and 2 represents opponent and it is row-wise
char** make_board_from_file(string str){
    char** board = new char*[M];
    char c;

    for(int i = 0 ; i < M ; i++)
        board[i] = new char[N];

    for(int i = 0 ; i < M ; i++){
        for(int j = 0 ; j < N ; j++){
            c = str.at(M*i + j);

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

unsigned long long int hash_func(char** board){
    unsigned long long int ctr = 0;

    for(int i = 0 ; i < M ; i++){
        for(int j = 0 ; j < N ; j++){
            if(board[i][j] == player)
                ctr+=(pow(3,N*i + j));
            else if(board[i][j] == opponent)
                ctr+=(2*(pow(3,N*i + j)));
        }
    }

    return ctr;
}
