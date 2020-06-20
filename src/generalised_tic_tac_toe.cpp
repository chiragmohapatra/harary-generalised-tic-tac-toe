#include <bits/stdc++.h>
#include "generalised_tic_tac_toe.h"

using namespace std;

/* The generalised tic tac toe is implemented on a 4x4 2-d character array with X,O or _ . X is the first player . 
   The desired polyamino is Tippy:
            |X|X|
              |X|X| 
    Sizes and polyaminos can be updated , just update the evaluate function . 
    Currently , evaluate function : gives +10 if player 1 wins and -10 in case of draw or player 1 lose . 
    The board is of size MxN i.e M rows and N columns*/

int polyamino_type = 0; // 0 for Tippy , 1 for Box/Fatty and 2 for Tic

unsigned long long int hash_func(char**);

CharSS::CharSS(){
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



// input a string of form 000011201.. till NxN where 0 represents _ , 1 respresents player and 2 represents opponent and it is row-wise
CharSS::CharSS(string str){
    int ctr = 0 , ctrp = 0 , ctro = 0;
    board = new char*[M];
    for(int i = 0 ; i < M ; i++)
        board[i] = new char[N];

    for(int i = 0 ; i < M ; i++){
        for(int j = 0 ; j < N ; j++){
            char c = str.at(M*i + j);
            if(c == '0') {
                board[i][j] = '_';
                ctr++;
            } else if(c == '1') {
                board[i][j] = player;
                ctrp++;
            } else {
                board[i][j] = opponent;
                ctro++;
            }
        }
    }

    legal_moves = ctr;
    if(ctrp == ctro)
        isPlayer = true;
    else
        isPlayer = false;
    hash_value = hash_func(board);
}

CharSS::~CharSS(){
  for(int i = 0 ; i < M ; i++)
    delete [] board[i];
  delete [] board;
}

CharSS* CharSS::clone() const {
  return new CharSS(print_as_string());
}


//returns true if legal moves are left and false otherwise(completely filled board)
bool CharSS::isMovesLeft(){
    if(legal_moves == 0)
        return false;
    else
        return true;
}

int CharSS::legalMoves(){
    return legal_moves;
}

unsigned long long int CharSS::hashValue(){
    return hash_value;
}

//returns true for a terminal position(player 1 win / player 1 lose / draw)
bool CharSS::isTerminal(){

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
int CharSS::evaluate(){
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

bool CharSS::isValidMove(int row , int col){
    if(row >= M || col >= N)
        return false;

    if(board[row][col] != '_')
        return false;

    else
        return true;
}

void CharSS::make_move(bool isPlayer , int row , int col){
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

void CharSS::undo_move(int row , int col){
    if(board[row][col] == player)
        hash_value-=(pow(3,N*row + col));
    else
        hash_value-=(2*(pow(3,N*row + col)));

    board[row][col] = '_';
    legal_moves++;
}

void CharSS::print_board(){
    for(int i = 0 ; i < M ; i++){
        for(int j = 0 ; j < N ; j++)
            cout<<board[i][j]<<" ";

        cout<<endl;
    }
}

string CharSS::print_as_string() const {
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

vector<string> CharSS::generate_children(){
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
