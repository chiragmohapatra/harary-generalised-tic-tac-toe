#include <bits/stdc++.h>
#include "../include/generalised_tic_tac_toe.h"
#include "../include/minimax.h"

bool policy_applied = true;
int minimaxdepth = 5;

using namespace std;

/* given a text file containing the positions involved in proof line by line , check whether the proof is correct */

bool check_proof(){
    vector<string> nodes;

    fstream newf;
    newf.open("./output/proof.txt" , ios::in);
    string temp;

    while(getline(newf,temp)){
        nodes.push_back(temp);
    }

    queue<Game*> games;

    Game* g = new Game(make_board_from_file(nodes[0]));
    // assert the node is a MAX node
    games.push(g);

    while(!games.empty()){
        Game* g1 = games.front();
        games.pop();

        int score = g1->evaluate();

        if(score == -10){
            g1->print_board();
            return false;
        }

        else if(score == 10)
            continue;

        vector<string> temp = g1->generate_children();

        if(g1->isPlayer){
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
                if(policy_applied){
                    Move policy_move = findBestMove(g1 , minimaxdepth);
                    g1->make_move(true , policy_move.row , policy_move.col);
                    games.push(new Game(g1->board));
                    g1->undo_move(policy_move.row , policy_move.col);
                }

                else{
                    g1->print_board();
                    return false;
                }
            }

            else
                games.push(new Game(make_board_from_file(tp)));
        }

        else{
            for(int i = 0 ; i < temp.size() ; i++){
                games.push(new Game(make_board_from_file(temp[i])));
            }
        }
    }

    return true;
}

int main(){
    bool check = check_proof();

    if(check)
        cout<<"Correct proof\n";
    else
        cout<<"Wrong proof\n";

    return 0;
}
