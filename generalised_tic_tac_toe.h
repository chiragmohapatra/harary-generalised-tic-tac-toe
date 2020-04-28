#ifndef GENERALISED_TIC_TAC_TOE_H

#define GENERALISED_TIC_TAC_TOE_H

#define N 4
#define player 'x'
#define opponent 'o'

using namespace std;

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

char** make_board_from_file(string str);// input a string of form 000011201.. till NxN where 0 represents _ , 1 respresents player and 2 represents opponent and it is row-wise

#endif