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

void get_all_possible_polyaminos(vector<bitset<M*N>> &polyaminos , bitset<M*N> polyamino){
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
void construct_polyaminos(vector<bitset<M*N>> &polyaminos , vector<bitset<M*N>> &user_polyaminos){
    
    for(int i = 0 ; i < user_polyaminos.size() ; i++)
        get_all_possible_polyaminos(polyaminos , user_polyaminos[i]);
}

// Before making any object of Game , make sure to invoke construct_polyaminos() once
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
        bool isTerminal(vector<bitset<M*N>> &polyaminos); //returns true for a terminal position(player 1 win / player 1 lose / draw)
        int evaluate(vector<bitset<M*N>> &polyaminos); // return +10 for player 1 win , -10 for loss/draw and 0 for intermediate
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

bool Game::isTerminal(vector<bitset<M*N>> &polyaminos){
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

int Game::evaluate(vector<bitset<M*N>> &polyaminos){
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
    construct_polyaminos();

    bitset<16> whites(string("0110001100000000"));
    bitset<16> blacks(string("1001000011000000"));

    Game* g = new Game(whites , blacks);

    cout<<g->evaluate()<<endl<<g->isTerminal()<<endl;


    return 0;
}*/

