#include <bits/stdc++.h>
#include "generalised_tic_tac_toe.h"
#include "generalised_tic_tac_toe_bitboard.h"
#include "minimax.h"
#include "pn_search_unified.h"
#include "check_proof.h"

using namespace std;
using namespace std::chrono;

#define inf -1

namespace pnSearchUnified
{

int ctr_nodes = 0;
int max_ctr = 0;
int size_of_proof = 0;

Param param;

/* Takes input of a game board and returns proved or disproved as applicable for the position assuming it is player's turn*/

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

int min(int a , int b){
    if(a == inf)
        return b;

    else if(b == inf)
        return a;

    else if(a <= b)
        return a;

    else
        return b;
}

class pn_node{

    // Todo replace board with a hash value and make a hash map

    public:
        Game* game; // Todo just store moves instead of complte game in each node
        int proof_number , disproof_number , mpn , dmpn;
        bool type; // true for OR node and false for AND node 
        pn_node* parent;
        char value;
        vector<pn_node*> children; // an array of pointers to the children
        bool isInternal; // true for internal node and false otherwise
        int no_of_children;
        void generate_children(); // generates children of the node
        bool reco_by_policy;

        pn_node(Game * board){
            game = board;
            const int legals = game->legalMoves();
            no_of_children = legals;
            isInternal = false;
            ctr_nodes++;
            if(ctr_nodes > max_ctr)
                max_ctr = ctr_nodes;
            evaluate_node();
            setProofandDisproofNumbers();
            if(game->isPlayer())
                type = true;
            else
                type = false;
            // initialise pn , dn , mpn
            // call evalute always here for each node
        }

        void set_parent(pn_node* par){
            parent = par;
            /*if(par == NULL)
                type = true;
            else
                type = !(par->type);*/
            isInternal = false;
        }

        ~pn_node(){
            ctr_nodes--;
            delete game;
        }

        //void delete_sub_tree();
        void evaluate_node(); // sets value to '0' if disproven , '1' if proven and '2' if unknown
        void setProofandDisproofNumbers(); // sets the proof and disproof numbers as per the algorithm
        pn_node* selectMostProvingNode(); // selects the most proving node as per the specs
        void ExpandNode(); // expand the leaf node , forming children and assigning their proof and disproof numbers
        void print_data(){
            cout<<"\n";
            game->print_board();
            cout<<"\n";
        }
};

void pn_node::generate_children(){

    if(no_of_children == 0)
        return;

    Move opt_move;

    if(param.minimal_policy)
        opt_move = policy(game,true);

    for(int i = 0 ; i < M ; i++){
        for(int j = 0 ; j < N ; j++){
            if(game->isValidMove(i,j)){
                // make move
                Game* copygame = game->clone();
                copygame->make_move(i , j);

                pn_node* child = new pn_node(copygame);

                if(param.minimal_policy){
                    if(opt_move.row == i && opt_move.col == j)
                        child->reco_by_policy = true;
                }

                child->set_parent(this);
                children.push_back(child);
            }
        }
    }
}

void pn_node::evaluate_node(){
    if(value == '1' || value == '0')
        return;

    int score = game->evaluate();

    if(score == 10){
        value = '1';
    }

    else if(score == -10)
        value = '0';

    else
        value = '2';
}

void pn_node::setProofandDisproofNumbers(){
    bool proof_inf = false , disproof_inf = false , proof_zero = false , disproof_zero = false;
    if(isInternal){
        if(type){ // OR node
            proof_number = inf; disproof_number = 0;
            mpn = inf; dmpn = 1;

            for(int i = 0 ; i < no_of_children ; i++){
                if(children[i]->disproof_number == inf)
                    disproof_inf = true;
                disproof_number+=(children[i]->disproof_number);
                dmpn+=(children[i]->dmpn);
                proof_number = min(proof_number , children[i]->proof_number);
                mpn = min(mpn , children[i]->mpn);
                if(children[i]->proof_number == 0)
                    proof_zero = true;
            }

            if(mpn != inf)
                mpn++;

            if(disproof_inf){
                disproof_number = inf;
                dmpn = inf;
            }

            if(proof_zero)
                proof_number = 0;
        }

        else{ // AND node
            disproof_number = inf; proof_number = 0;
            dmpn = inf; mpn = 1;

            for(int i = 0 ; i < no_of_children ; i++){
                if(children[i]->proof_number == inf)
                    proof_inf = true;
                proof_number+=(children[i]->proof_number);
                mpn+=(children[i]->mpn);
                disproof_number = min(disproof_number , children[i]->disproof_number);
                dmpn = min(dmpn , children[i]->dmpn);
                if(children[i]->disproof_number == 0)
                    disproof_zero = true;
            }

            if(dmpn != inf)
                dmpn++;


            if(proof_inf){
                proof_number = inf;
                mpn = inf;
            }

            if(disproof_zero)
                disproof_number = 0;

        }
    }

    // make separate function for this
    else{
        switch(value){
            case '0': // disproven
                proof_number = inf;
                mpn = inf;
                disproof_number = 0;
                dmpn = 1;
                break;

            case '1': //proven
                proof_number = 0;
                mpn = 1;
                disproof_number = inf;
                dmpn = inf;
                break;

            case '2': //unknown

                if(!param.isMobile){
                    proof_number = 1;
                    disproof_number = 1;
                    mpn = 1;
                    dmpn = 1;
                }

                else{
                    if(type){
                        proof_number = 1;
                        mpn = 1;
                        disproof_number = game->legalMoves();
                        dmpn = game->legalMoves();
                    }

                    else{
                        proof_number = game->legalMoves();
                        mpn = game->legalMoves();
                        disproof_number = 1;
                        dmpn = 1;
                    }

                }

                break;
        }
    }

    if(param.minimal_policy && reco_by_policy){
        if(mpn != inf)
            mpn--;

        if(dmpn != inf)
            dmpn--;
    }

}

pn_node* pn_node::selectMostProvingNode(){
    pn_node* n = this;

    if(!param.minimal_proof){
        while(n->isInternal){
            int value;
            pn_node* best;

            if(n->type){ // OR node
                value = n->children[0]->proof_number;
                best = n->children[0];
                for(int i = 1 ; i < n->no_of_children ; i++){
                    if((value > n->children[i]->proof_number || value == inf) && n->children[i]->proof_number != inf){ // select one with the minimum proof number
                        best = n->children[i];
                        value = n->children[i]->proof_number;
                    }
                }
            }

            else{ // AND node
                value = n->children[0]->disproof_number;
                best = n->children[0];
                for(int i = 1 ; i < n->no_of_children ; i++){
                    if((value > n->children[i]->disproof_number || value == inf) && n->children[i]->disproof_number != inf){ // select one with the minimum disproof number
                        best = n->children[i];
                        value = n->children[i]->disproof_number;
                    }

                }
            }

            n = best;
        }
    }

    else{
        while(n->isInternal){
            int value;
            pn_node* best;

            if(n->type){ // OR node
                value = n->children[0]->mpn;
                best = n->children[0];
                for(int i = 1 ; i < n->no_of_children ; i++){
                    if((value > n->children[i]->mpn || value == inf) && n->children[i]->mpn != inf){ // select one with the minimum proof number
                        best = n->children[i];
                        value = n->children[i]->mpn;
                    }
                }
            }

            else{ // AND node
                value = n->children[0]->dmpn;
                best = n->children[0];
                for(int i = 1 ; i < n->no_of_children ; i++){
                    if((value > n->children[i]->dmpn || value == inf) && n->children[i]->dmpn != inf){ // select one with the minimum disproof number
                        best = n->children[i];
                        value = n->children[i]->dmpn;
                    }

                }
            }

            n = best;
        }

    }

    return n;
}

void pn_node::ExpandNode(){
    if(value == '1' || value == '0')
        return;
    generate_children();

    /*for(int i = 0 ; i < no_of_children ; i++){
        //children[i]->evaluate_node();
        //children[i]->setProofandDisproofNumbers();

        if(type){ // OR node
            if(children[i]->proof_number == 0)
                break;
        }

        else{
            if(children[i]->disproof_number == 0)
                break;
        }
    }*/

    isInternal = true;
}

void form_stack(stack<pn_node*> &node_stack , pn_node* n){
    node_stack.push(n);

    if(n->children.size() == 0)
        return;

    else{
        for(int i = 0 ; i < n->children.size() ; i++){
            form_stack(node_stack , n->children[i]);
        }
    }
}

void delete_complete_subtree(pn_node* n){
    stack<pn_node*> node_stack;
    form_stack(node_stack , n);

    while(!node_stack.empty()){
        pn_node* m = node_stack.top();
        node_stack.pop();
        m->children.clear();
        if(m->no_of_children < n->no_of_children)
            delete m;
    }

    n->isInternal = false;
}

pn_node* update_ancestors(pn_node* n , pn_node* root){
    while(n != root){
        int old_proof = n->proof_number;
        int old_disproof = n->disproof_number;
        n->setProofandDisproofNumbers();

        if(n->proof_number == old_proof && n->disproof_number == old_disproof)
            return n; // if unchanged , then return

        if(param.delete_sub_trees){
            if(n->proof_number == 0){
                delete_complete_subtree(n);
                n->isInternal = false;
                n->value = '1';
                
                //n->delete_sub_tree();
                /*n->value = '1';
                n->proof_number = 0;
                n->disproof_number = inf;*/
            }

            else if(n->disproof_number == 0){
                delete_complete_subtree(n);
                n->isInternal = false;
                n->value = '0';
                //n->delete_sub_tree();
                /*n->value = '0';
                n->proof_number = inf;
                n->disproof_number = 0;*/
            }
        }

        n = n->parent;
    }

    root->setProofandDisproofNumbers();
    return root;
}

void store_proof(pn_node* root){
    queue<pn_node*> q;
    q.push(root);

    set<string> node_set;

    //outfile4<<root->game->print_as_string()<<endl;
    node_set.insert(root->game->print_as_string());

    while(!q.empty()){
        pn_node* n = q.front();
        q.pop();

        if(!n->type && !n->reco_by_policy){
            //outfile4<<n->game->print_as_string()<<endl;
            node_set.insert(n->game->print_as_string());
        }

        //node_set.insert(n->game->print_as_string());

        if(n->isInternal){
            if(n->type){
                // just store the child with the smallest mpn
                if(!param.policy_applied){
                    for(int i = 0 ; i < n->no_of_children ; i++){
                        if(n->children[i]->disproof_number == inf){
                            q.push(n->children[i]);
                            break;
                        }
                    }
                }

                else{
                    Move optimal_move = policy(n->game,true);
                    n->game->make_move(optimal_move.row , optimal_move.col);
                    string temp_board = n->game->print_as_string();
                    n->game->undo_move(optimal_move.row,optimal_move.col);

                    for(int i = 0 ; i < n->no_of_children ; i++){
                        if(n->children[i]->game->print_as_string() == temp_board){
                            if(n->children[i]->disproof_number != inf){
                                for(int j = 0 ; j < n->no_of_children ; j++){
                                    if(n->children[j]->disproof_number == inf){
                                        q.push(n->children[j]);
                                        break;
                                    }
                                }
                                break;
                            }

                            else{
                                n->children[i]->reco_by_policy = true;
                                q.push(n->children[i]);
                            }
                        }
                    }
                }
            }

            else{
                for(int i = 0 ; i < n->no_of_children ; i++){
                    q.push(n->children[i]);
                }

            }
        }
    }

    set<string>::iterator it;

    fstream outfile4;
    outfile4.open("./output/proof.txt",ios::out);

    outfile4<<"Proof\n";

    for(it = node_set.begin() ; it != node_set.end() ; it++){
        outfile4<<*it<<endl;
        size_of_proof++;
    }

    outfile4.close();
}

void store_disproof(pn_node* root){
    queue<pn_node*> q;
    q.push(root);

    set<string> node_set;

    //outfile4<<root->game->print_as_string()<<endl;
    node_set.insert(root->game->print_as_string());

    while(!q.empty()){
        pn_node* n = q.front();
        q.pop();

        if(n->type && !n->reco_by_policy){
            //outfile4<<n->game->print_as_string()<<endl;
            node_set.insert(n->game->print_as_string());
        }

        //node_set.insert(n->game->print_as_string());

        if(n->isInternal){
            if(!n->type){
                if(!param.policy_applied){
                    for(int i = 0 ; i < n->no_of_children ; i++){
                        if(n->children[i]->proof_number == inf){
                            q.push(n->children[i]);
                            break;
                        }
                    }
                }

                else{
                    Move optimal_move = policy(n->game,false);
                    n->game->make_move(optimal_move.row , optimal_move.col);
                    string temp_board = n->game->print_as_string();
                    n->game->undo_move(optimal_move.row,optimal_move.col);

                    for(int i = 0 ; i < n->no_of_children ; i++){
                        if(n->children[i]->game->print_as_string() == temp_board){
                            if(n->children[i]->proof_number != inf){
                                for(int j = 0 ; j < n->no_of_children ; j++){
                                    if(n->children[j]->proof_number == inf){
                                        q.push(n->children[j]);
                                        break;
                                    }
                                }
                                break;
                            }

                            else{
                                n->children[i]->reco_by_policy = true;
                                q.push(n->children[i]);
                            }
                        }
                    }
                }
            }

            else{
                for(int i = 0 ; i < n->no_of_children ; i++){
                    q.push(n->children[i]);
                }

            }
        }
    }

    set<string>::iterator it;

    fstream outfile4;
    outfile4.open("./output/proof.txt",ios::out);

    outfile4<<"Disproof\n";

    for(it = node_set.begin() ; it != node_set.end() ; it++){
        outfile4<<*it<<endl;
        size_of_proof++;
    }

    outfile4.close();
}

void pn_search(pn_node* root){
    //root->evaluate_node();
    //root->setProofandDisproofNumbers();
    pn_node* current = root;
    int ctr = 0;

    //outfile4.open("./output/proof.txt",ios::out);

    while(root->proof_number != 0 && root->disproof_number != 0){
        pn_node* most_proving = current->selectMostProvingNode();

        most_proving->ExpandNode();
        current = update_ancestors(most_proving , root);
        //if(ctr % 1000 == 0){
            //cout<<root->proof_number<<" "<<root->disproof_number<<" "<<root->mpn<<" "<<root->dmpn<<endl;
            //current->print_data();
        //}
        ctr++;
    }
    //store_proof(root);
}

/*void verify_policies(const Param &parameters){
    param = parameters;
    fstream newfile;

    checkProof::Param_check pCheck;
    pCheck.board_type = param.board_type;
    pCheck.depth_minimax = param.depth_minimax;

    newfile.open("input.txt",ios::in);

    if(newfile.is_open()){
        string temp;

        while(getline(newfile , temp)){
            if(temp.length() == M*N){
                cout<<"\nInput: "<<temp<<" ";
                for(int i = -1 ; i < 3 ; i++){
                    if(i == -1)
                        param.policy_applied = false;
                    else
                        param.policy_applied = true;
                    param.policy_type = pCheck.policy_type = i;

                    Game* board;
                    if(param.board_type == 1){
                        board = new Bitboard(temp);
                    }
                    else{
                        board = new CharSS(temp);
                    }

                    pn_node* root_mobile = new pn_node(board);
                    
                    root_mobile->set_parent(NULL);

                    pn_search(root_mobile);

                    if(root_mobile->proof_number == 0){
                        if(i == -1)
                            cout<<"Proof\n";
            
                        auto start = high_resolution_clock::now();
                        store_proof(root_mobile);
                        auto stop = high_resolution_clock::now();
                        auto duration = duration_cast<milliseconds>(stop - start);
                        //cout<<i<<" "<<duration.count()<<" "<<size_of_proof<<endl;
                        cout<<"Policy type: "<<i<<" , Duration for creating proof : "<<duration.count()<<" ms , Size of proof : "<<size_of_proof<<endl;
                        check_proof_main(pCheck);
                        size_of_proof = 0;
                    
                    }

                    else{
                        if(i == -1)
                            cout<<"Disproof\n";
                        auto start = high_resolution_clock::now();
                        store_disproof(root_mobile);
                        auto stop = high_resolution_clock::now();
                        auto duration = duration_cast<milliseconds>(stop - start);
                        //cout<<i<<" "<<duration.count()<<" "<<size_of_proof<<endl;
                        cout<<"Policy type: "<<i<<" , Duration for creating proof : "<<duration.count()<<" ms , Size of proof : "<<size_of_proof<<endl;
                        check_proof_main(pCheck);
                        size_of_proof = 0;

                    }

                    stack<pn_node*> node_stack;
                    form_stack(node_stack , root_mobile);
                    delete_complete_subtree(root_mobile);

                    delete root_mobile;
                }
            }
        }
    }

    newfile.close();
}*/

void generate_time_and_memory(const Param &parameters){
    param = parameters;

    fstream newfile;

    newfile.open("input.txt",ios::in);

    if(newfile.is_open()){
        string temp;

        while(getline(newfile , temp)){
            if(temp.length() == M*N){
                cout<<"Input : "<<temp<<" ";
                Game* board;
                if(param.board_type == 1){
                    board = new Bitboard(temp);
                }
                else{
                    board = new CharSS(temp);
                }

                pn_node* root_mobile = new pn_node(board);
                
                root_mobile->set_parent(NULL);

                auto start = high_resolution_clock::now();
                pn_search(root_mobile);
                auto stop = high_resolution_clock::now();

                if(root_mobile->proof_number == 0)
                    cout<<"Proof\n";
                else
                    cout<<"Disproof\n";

                auto duration = duration_cast<milliseconds>(stop - start);

                cout<<"Time taken : "<<duration.count()<<"ms\tNodes : "<<max_ctr<<endl;

                ctr_nodes = 0;
                max_ctr = 0;

                delete root_mobile;
            }
        }
    }

    newfile.close();

}


int pn_search_unified_main(const Param & parameters){
    param = parameters;

    fstream newfile , outfile1 , outfile2 , outfile3;

    newfile.open("input.txt",ios::in);
    outfile1.open("./output/output.txt",ios::out);
    outfile2.open("./output/times_pn_search(in microseconds).txt",ios::out);
    outfile3.open("./output/nodes_pn_search.txt",ios::out);

    if(newfile.is_open()){
        string temp;

        while(getline(newfile , temp)){
            if(temp.length() == M*N){
                Game* board;
                if(param.board_type == 1){
                    board = new Bitboard(temp);
                }
                else{
                    board = new CharSS(temp);
                }

                pn_node* root_mobile = new pn_node(board);
                
                root_mobile->set_parent(NULL);

                //cout<<root_mobile->game->print_as_string()<<"\t"<<temp<<"\t"<<board.print_as_string()<<endl;

                //auto start = high_resolution_clock::now();
                pn_search(root_mobile);
                //auto stop = high_resolution_clock::now();

                if(root_mobile->proof_number == 0){
                    outfile1<<"Proved\n";
                    auto start = high_resolution_clock::now();
                    store_proof(root_mobile);
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<milliseconds>(stop - start);
                    cout<<size_of_proof<<endl;
                    size_of_proof = 0;
                }

                else{
                    outfile1<<"Disproved\n";
                    auto start = high_resolution_clock::now();
                    store_disproof(root_mobile);
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<milliseconds>(stop - start);
                    cout<<size_of_proof<<endl;
                    size_of_proof = 0;
                }
                //auto stop = high_resolution_clock::now();

                //auto duration = duration_cast<milliseconds>(stop - start);
                //outfile2<<duration.count()<<endl;

                //outfile3<<max_ctr<<endl;

                //cout<<root_mobile->mpn<<"\t"<<root_mobile->dmpn<<endl;
                cout<<root_mobile->proof_number<<"\t"<<root_mobile->disproof_number<<"\t"<<size_of_proof<<"\t"<<"\t"<<max_ctr<<endl;

                ctr_nodes = 0;
                max_ctr = 0;

                //delete board;

                /*int score = minimax(&board , 0 , true , -10000000 , 10000000 , 100);

                cout<<score<<endl;*/

                delete root_mobile;
            }
        }
    }

    newfile.close();
    outfile1.close();
    outfile2.close();
    outfile3.close();

    return 0;
}
}
