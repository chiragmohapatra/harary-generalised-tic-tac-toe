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

int polyamino_type = 0; // 0 for Tippy , 1 for Box/Fatty and 2 for Tic

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
            isPlayer = true;
        }

        Game(char** board_status); // set board to board_status
        //Game(char** board_status , char*** ZobristTable); // for transposition table based searches

        ~Game(){
            for(int i = 0 ; i < M ; i++)
                delete [] board[i];

            delete [] board;
        }

        bool isPlayer;
        bool isMovesLeft(); //returns true if legal moves are left and false otherwise(completely filled board)
        bool isTerminal(); //returns true for a terminal position(player 1 win / player 1 lose / draw)
        int evaluate(); // return +10 for player 1 win , -10 for loss/draw and 0 for intermediate
        bool isValidMove(int,int); // returns true if move made is valid
        void make_move(bool,int,int);
        void undo_move(int,int); // undo the move at (int,int)
        void print_board();
        string print_as_string();
        vector<string> generate_children();
};

Game::Game(char** board_status){
    int ctr = 0 , ctrp = 0 , ctro = 0;
    board = new char*[M];
    for(int i = 0 ; i < M ; i++)
        board[i] = new char[N];

    for(int i = 0 ; i < M ; i++){
        for(int j = 0 ; j < N ; j++){
            board[i][j] = board_status[i][j];
            if(board_status[i][j] == '_')
                ctr++;
            else if(board_status[i][j] == player)
                ctrp++;
            else
                ctro++;
        }
    }

    legal_moves = ctr;
    if(ctrp == ctro)
        isPlayer = true;
    else
        isPlayer = false;
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

    switch(polyamino_type){

        case 0:
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
            break;

        case 1:
            for(i = 0 ; i < (M-1) ; i++){
                for(j = 0 ; j < (N-1) ; j++){
                    if(board[i][j] != '_' && board[i][j] == board[i][j+1] && board[i][j] == board[i+1][j] && board[i][j] == board[i+1][j+1])
                        return true;
                }
            }

            return false;
            break;

        case 2:
            for(i = 0 ; i < M ; i++){
                for(j = 0 ; j < N ; j++){
                    if(j < (N-2) && board[i][j] != '_' && board[i][j] == board[i][j+1] && board[i][j] == board[i][j+2])
                        return true;

                    if(i < (M-2) &&board[i][j] != '_' && board[i][j] == board[i+1][j] && board[i][j] == board[i+2][j])
                        return true; 
                }
            }

            return false;
            break;

        default:
            break;
    }
}


// return +10 for player 1 win , -10 for loss/draw and 0 for intermediate
int Game::evaluate(){
    int i , j;

    switch(polyamino_type){

        case 0:
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

            break;

        case 1:
            for(i = 0 ; i < (M-1) ; i++){
                for(j = 0 ; j < (N-1) ; j++){
                    if(board[i][j] != '_' && board[i][j] == board[i][j+1] && board[i][j] == board[i+1][j] && board[i][j] == board[i+1][j+1]){
                       if(board[i][j] == player)
                        return 10;
                       else
                        return -10;
                    }
                }
            }

            break;

        case 2:
            for(i = 0 ; i < M ; i++){
                for(j = 0 ; j < N ; j++){
                    if(j < (N-2) && board[i][j] != '_' && board[i][j] == board[i][j+1] && board[i][j] == board[i][j+2]){
                        if(board[i][j] == player)
                                return 10;
                            else
                                return -10;
                    }

                    if(i < (M-2) &&board[i][j] != '_' && board[i][j] == board[i+1][j] && board[i][j] == board[i+2][j]){
                        if(board[i][j] == player)
                                return 10;
                            else
                                return -10; 
                    }
                }
            }

            break;

        default:
            break;
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

    isPlayer = !isPlayer;
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

string Game::print_as_string(){
    string str;
    for(int i = 0 ; i < M ; i++){
        for(int j = 0 ; j < N ; j++){
            if(board[i][j] == '_')
                str.push_back('0');
            else if(board[i][j] == player)
                str.push_back('1');
            else
                str.push_back('2');
        }
    }

    return str;
}

vector<string> Game::generate_children(){
    vector<string> children;
    for(int i = 0 ; i < M ; i++){
        for(int j = 0 ; j < N ; j++){
            if(board[i][j] == '_'){
                if(isPlayer)
                    board[i][j] = player;
                else
                    board[i][j] = opponent;
                
                children.push_back(print_as_string());
                board[i][j] = '_';
            }
        }
    }

    return children;
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

/*unsigned long long int hash_func(char** board , char*** ZobristTable){
    unsigned long long int h = 0;

    for(int i = 0 ; i < M ; i++){
        for(int j = 0 ; j < N ; j++){
            if(board[i][j] == 'x')
                h ^= ZobristTable[i][j][0];
            else if(board[i][j] == 'o')
                h ^= ZobristTable[i][j][1];
        }
    }

    return h;
}*/
