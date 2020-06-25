#include <bits/stdc++.h>
#include "../includes/generalised_tic_tac_toe.h"
#include "../includes/generalised_tic_tac_toe_bitboard.h"
#include "../includes/minimax.h"
#include "../includes/check_proof.h"

bool policy_app = false;
int minimaxdepth = 1;

using namespace std;
using namespace std::chrono;

/* given a text file containing the positions involved in proof line by line , check whether the proof is correct */

// return preffered move for a position
Move policyMove(Game* game){
    return findBestMove(game , minimaxdepth);

    Move opt;
    for(int i = 0; i < M; i++){
        for(int j = 0 ; j < N ; j++){
            if(game->isValidMove(i,j)){
                opt.row = i;
                opt.col = j;
                break;
            }
        }
    }  
    
    return opt;
}

bool check_proof(){
    vector<string> nodes;

    fstream newf;
    newf.open("./output/proof.txt" , ios::in);
    string temp;

    while(getline(newf,temp)){
        nodes.push_back(temp);
    }

    newf.close();

    queue<Game*> games;

    //Bitboard board(nodes[0]);
    CharSS board(nodes[0]);

    Game* g = board.clone();
    // assert the node is a MAX node
    games.push(g);

    while(!games.empty()){
        Game* g1 = games.front();
        games.pop();

        int score = g1->evaluate();

        if(score == -10){
            g1->print_board();
            //cout<<g1->print_as_string()<<endl;
            return false;
        }

        else if(score == 10)
            continue;

        vector<string> temp = g1->generateChildren();

        if(g1->isPlayer()){
            bool found = false;
            vector<string>::iterator it;
            string tp;

            for(int i = 0 ; i < temp.size() ; i++){
                it = find(nodes.begin() , nodes.end() , temp[i]);

                if(it != nodes.end()){
                    found = true;
                    tp = temp[i];
                    break;
                }
            }

            if(!found){
                //g1->print_board();
                if(policy_app){
                    Move policy_move = policyMove(g1);
                    Game* copygame = g1->clone();
                    copygame->make_move(policy_move.row , policy_move.col);
                    games.push(copygame);
                }

                else{
                    g1->print_board();
                    return false;
                }
            }

            else{
                //Bitboard board(tp);
                CharSS board(tp);

                Game* g = board.clone();
                games.push(g);
            }
        }

        else{
            for(int i = 0 ; i < temp.size() ; i++){
                //Bitboard board(temp[i]);
                CharSS board(temp[i]);

                Game* g = board.clone();
                games.push(g);
            }
        }
    }

    return true;
}

/*bool check_proof(){
    vector<string> nodes;

    fstream outfile2;
    outfile2.open("./output/proof.txt",ios::in);

    string temp;

    while(getline(outfile2,temp)){
        nodes.push_back(temp);
    }

    outfile2.close();

    queue<Game*> test;

    CharSS board(nodes[0]);


}*/

int check_proof_main(){
    fstream outfile1;
    outfile1.open("./output/output.txt",ios::out);

    auto start = high_resolution_clock::now();
    bool check = check_proof();
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);
    outfile1<<duration.count()<<endl;

    if(check)
        cout<<"Correct proof\n";
    else
        cout<<"Wrong proof\n";

    outfile1.close();

    return 0;
}