#include <bits/stdc++.h>
#include "../includes/generalised_tic_tac_toe.h"
#include "../includes/generalised_tic_tac_toe_bitboard.h"
#include "../includes/minimax.h"
#include "../includes/check_proof.h"

using namespace std;
using namespace std::chrono;

/* given a text file containing the positions involved in proof line by line , check whether the proof is correct */

namespace checkProof{

    Param_check param;

Move score_max_move(Game* game , bool is_proof){
    Move opt ; int score_max;
    if(is_proof)    score_max = -100;
    else        score_max = 100;
    
    for(int i = 0; i < M; i++){
        for(int j = 0 ; j < N ; j++){
            if(game->isValidMove(i,j)){
                game->make_move(i,j);
                int temp_score = game->score();
                game->undo_move(i,j);
                if(is_proof){
                    if(temp_score > score_max){
                        opt.row = i;
                        opt.col = j;
                        score_max = temp_score;
                    }
                }

                else{
                    if(temp_score < score_max){
                        opt.row = i;
                        opt.col = j;
                        score_max = temp_score;
                    }
                }
            }
        }
    }
    return opt;
}

Move score_max_move(Game* game , coeff_score coeff , bool is_proof){
    Move opt ; int score_max;
    if(is_proof)    score_max = -100;
    else        score_max = 100;
    for(int i = 0; i < M; i++){
        for(int j = 0 ; j < N ; j++){
            if(game->isValidMove(i,j)){
                game->make_move(i,j);
                int temp_score = game->score(coeff);
                game->undo_move(i,j);
                if(is_proof){
                    if(temp_score > score_max){
                        opt.row = i;
                        opt.col = j;
                        score_max = temp_score;
                    }
                }

                else{
                    if(temp_score < score_max){
                        opt.row = i;
                        opt.col = j;
                        score_max = temp_score;
                    }
                }
            }
        }
    }
    return opt;
}

Move minimax_move(Game* game , bool is_proof){
    Move opt ; int score_max;
    if(is_proof)    score_max = -100;
    else        score_max = 100;
    
    for(int i = 0; i < M; i++){
        for(int j = 0 ; j < N ; j++){
            if(game->isValidMove(i,j)){
                game->make_move(i,j);
                int temp_score = game->evaluate();
                game->undo_move(i,j);
                if(is_proof){
                    if(temp_score > score_max){
                        opt.row = i;
                        opt.col = j;
                        score_max = temp_score;
                    }
                    if(score_max == 10)
                        break;
                }

                else{
                    if(temp_score < score_max){
                        opt.row = i;
                        opt.col = j;
                        score_max = temp_score;
                    }
                    if(score_max == -10)
                        break;
                }
            }
        }
    }
    return opt;

}

// return preffered move for a position
Move policy(Game* game , bool is_proof){

    if(param.policy_type == 0){
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

    else if(param.policy_type == 1)
        return minimax_move(game , is_proof);

    else if(param.policy_type == 2){
        return score_max_move(game , is_proof);
    }

    else if(param.policy_type == 3){
        return score_max_move(game , param.coeff , is_proof);
    }
}

bool check_proof(vector<string> &nodes){
    
    queue<Game*> games;

    //Bitboard board(nodes[1]);
    Game* g;
    if(param.board_type == 1){
        g = new Bitboard(nodes[1]);
    }
    else{
        g = new CharSS(nodes[1]);
    }

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
                if(param.policy_applied){
                    Move policy_move = policy(g1,true);
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
                Game* g;
                if(param.board_type == 1){
                    g = new Bitboard(tp);
                }
                else{
                    g = new CharSS(tp);
                }
                games.push(g);
            }
        }

        else{
            for(int i = 0 ; i < temp.size() ; i++){
                Game* g;
                if(param.board_type == 1){
                    g = new Bitboard(temp[i]);
                }
                else{
                    g = new CharSS(temp[i]);
                }
                games.push(g);
            }
        }

        delete g1;
    }

    return true;
}

bool check_disproof(vector<string> &nodes){
    
    queue<Game*> games;

    Game* g;
    if(param.board_type == 1){
        g = new Bitboard(nodes[1]);
    }
    else{
        g = new CharSS(nodes[1]);
    }
    
    games.push(g);

    while(!games.empty()){
        Game* g1 = games.front();
        games.pop();

        int score = g1->evaluate();

        if(score == 10){
            g1->print_board();
            //cout<<g1->print_as_string()<<endl;
            return false;
        }

        else if(score == -10)
            continue;

        vector<string> temp = g1->generateChildren();

        if(!g1->isPlayer()){
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
                if(param.policy_applied){
                    Move policy_move = policy(g1,false);
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
                Game* g;
                if(param.board_type == 1){
                    g = new Bitboard(tp);
                }
                else{
                    g = new CharSS(tp);
                }
                games.push(g);
            }
        }

        else{
            for(int i = 0 ; i < temp.size() ; i++){
                Game* g;
                if(param.board_type == 1){
                    g = new Bitboard(temp[i]);
                }
                else{
                    g = new CharSS(temp[i]);
                }
                games.push(g);
            }
        }

        delete g1;
    }

    return true;
}



int check_proof_main(const Param_check & parameters){

    param = parameters;

    vector<string> nodes;

    fstream newf;
    newf.open("./output/proof.txt" , ios::in);
    string temp;

    while(getline(newf,temp)){
        nodes.push_back(temp);
    }

    newf.close();

    bool check;
    
    auto start = high_resolution_clock::now();
    if(nodes[0] == "Proof")
        check = check_proof(nodes);
    else
        check = check_disproof(nodes);
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start);
    fstream outfile1;
    outfile1.open("./output/output.txt",ios::out);
    outfile1<<duration.count()<<endl;

    if(check)
        cout<<"Correct proof\n";
    else
        cout<<"Wrong proof\n";

    cout<<"Checking time :"<<duration.count()<<" ms"<<endl<<endl;

    


    outfile1.close();

    return 0;
}

}