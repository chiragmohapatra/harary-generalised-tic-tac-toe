#include <bits/stdc++.h>
#include "generalised_tic_tac_toe.h" // can be changed for different games(board games)

using namespace std;
using namespace std::chrono;


#define inf 10000000
#define neginf -10000000

int ctr = 0;

struct Move{ // a move is charactersied by the row and the column
    int row , col;
};

// return maximum of a and b
/*int max(int a , int b){
    if(a >= b)
        return a;

    else
        return b;
}

int min(int a , int b){
    if(a <= b)
        return a;

    else
        return b;
}*/

// returns the value of the board after considering all possibilities
int minimax(Game* game, int depth, bool isMax , int alpha , int beta , int maxdepth){ 
    int score = game->evaluate(); 
    ctr++;
  
    // Maximizer has won 
    if (score == 10) 
        return (100*score-depth); // to make sure that the optimal move takes least possible steps to win
  
    // Minimizer has won
    if (score == -10) 
        return (100*score+depth); 
  
    // If there are no more moves and no winner then 
    // it is a tie 
    if (game->isMovesLeft()==false || depth >= maxdepth) 
        return 0; 
  
    // If this maximizer's move 
    if(isMax){ 
        int best = neginf;
  
        for (int i = 0; i < N; i++){ 
            for (int j = 0; j < N; j++){ 
                // Check if cell is empty 
                if (game->isValidMove(i,j)){ 
                    // Make the move 
                    game->make_move(true , i , j); 

                    int val = minimax(game, depth+1, !isMax , alpha , beta , maxdepth);
  
                    best = max(best, val);
                    alpha = max(alpha , best);

                    // Undo the move 
                    game->undo_move(i,j);

                    //alpha beta pruning
                    if(beta <= alpha) 
                        break;
  
                } 
            } 
        } 
        return best; 
    } 
  
    // If this minimizer's move 
    else{ 
        int best = inf;
  
        for (int i = 0; i < N; i++){ 
            for (int j = 0; j < N; j++){ 
                // Check if move is legal 
                if (game->isValidMove(i,j)){

                    // Make the move 
                    game->make_move(false , i , j); 
  
                    int val = minimax(game, depth+1, !isMax , alpha , beta , maxdepth);
  
                    best = min(best, val);
                    beta = min(beta , best); 

                    // Undo the move 
                    game->undo_move(i,j);

                    //alpha beta pruning
                    if(beta <= alpha) 
                        break;
                    
                } 
            } 
        } 
        return best; 
    } 
}

// returns the best move for the player
Move findBestMove(Game* game , int maxdepth){
    int bestVal = -1000; 
    Move bestMove; 
    bestMove.row = -1; 
    bestMove.col = -1; 
  
    /*Traverse all cells, evaluate minimax function for 
      all empty cells. And return the cell with optimal 
      value.*/ 
    for (int i = 0; i < N; i++) 
    { 
        for (int j = 0; j < N; j++) 
        { 
            // Check if cell is empty 
            if (game->isValidMove(i,j)) 
            { 
                // Make the move 
                game->make_move(true,i,j);
  
                // compute evaluation function for this 
                // move. 
                int moveVal = minimax(game, 0, false , neginf , inf , maxdepth); //iniialise alpha as -inf and beta as +inf
  
                // Undo the move 
                game->undo_move(i,j);
  
                // If the value of the current move is 
                // more than the best value, then update 
                // best 
                if (moveVal > bestVal) 
                { 
                    bestMove.row = i; 
                    bestMove.col = j; 
                    bestVal = moveVal; 
                } 
            } 
        } 
    } 
  
    return bestMove; 
} 

/*int main(){
    fstream newfile , outfile1 , outfile2 , outfile3;

    newfile.open("./src/input.txt",ios::in);
    outfile1.open("./output/output.txt",ios::out);
    outfile2.open("./output/times.txt",ios::out);
    outfile3.open("./output/nodes.txt",ios::out);

    if(newfile.is_open()){
        string temp;

        while(getline(newfile , temp)){
            if(temp.length() == N*N){
                char** board = make_board_from_file(temp);

                Game* game = new Game();
                game->set_board(board);
                
                auto start = high_resolution_clock::now();
                int moveVal = minimax(game , 0 , true , neginf , inf);
                auto stop = high_resolution_clock::now();

                auto duration = duration_cast<microseconds>(stop - start);

                outfile2<<duration.count()<<endl; 
                outfile3<<ctr<<endl;

                ctr = 0;

                if(moveVal > 0)
                    outfile1<<"Proved\n";
                else
                    outfile1<<"Disproved\n";

                for(int i = 0 ; i < N ; i++)
                    delete [] board[i];

                delete [] board;
                delete game;
            }
        }
    }


    newfile.close();
    outfile1.close();
    outfile2.close();
    outfile3.close();



    /*while(!isTerminal(board)){
        Move bestMove = findBestMove(board);

        board[bestMove.row][bestMove.col] = 'x';

        cout<<"\n";

        print_board(board);

        if(isTerminal(board)){
            cout<<"Computer wins\n";
            break;
        }

        cout<<"\nEnter human move\n";
        cin>>r>>c;

        board[r][c] = 'o';

        print_board(board);

        if(isTerminal(board)){
            cout<<"Human wins\n";
            break;
        }
    }*/
    

    /*cout<<"The optimal move is:\n";
    cout<<"Row: "<<bestMove.row<<endl;
    cout<<"Col: "<<bestMove.col<<endl;*/

    
/*
    return 0;
}*/

