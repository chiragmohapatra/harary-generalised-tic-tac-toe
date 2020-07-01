#include <bits/stdc++.h>
#include "generalised_tic_tac_toe.h"
#include "generalised_tic_tac_toe_bitboard.h"
#include "minimax.h"
#include "pn2_search.h"

using namespace std;
using namespace std::chrono;

#define inf -1

namespace pn2Search
{

int ctr_nodes = 0;
int max_ctr = 0;

Param param;

/* Takes input of a game board and returns proved or disproved as applicable for the position assuming it is player's turn*/

// return preffered move for a position
Move policy(Game* game){
    if(param.policy_type == 1)
        return findBestMove(game , param.depth_minimax);

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
        pn_node** children; // an array of pointers to the children
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
            //isInternal = false;
        }

        ~pn_node(){
            ctr_nodes--;
            delete game;
            //delete children;
        }

        void delete_sub_tree();
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

void pn_node::delete_sub_tree(){
    // find better way
    if(value == '1' || value == '0')
        return;

    for(int i = 0 ; i < no_of_children ; i++)
        delete children[i];

    delete [] children;
    isInternal = false;
    //no_of_children = 0;
}

void pn_node::generate_children(){

    if(no_of_children == 0)
        return;

    int k = 0;
    children = new pn_node*[no_of_children];

    Move opt_move;

    if(param.minimal_policy)
        opt_move = policy(game);

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
                //pn_node* child = new pn_node(game->board);

                //undo the move
                //game->undo_move(i,j);

                child->set_parent(this);
                children[k++] = child;
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
    if(value == '1' || value == '0' || isInternal)
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

pn_node* update_ancestors(pn_node* n , pn_node* root){
    while(n != root){
        int old_proof = n->proof_number;
        int old_disproof = n->disproof_number;
        n->setProofandDisproofNumbers();

        if(n->proof_number == old_proof && n->disproof_number == old_disproof)
            return n; // if unchanged , then return

        if(param.delete_sub_trees){
            if(n->proof_number == 0){
                n->delete_sub_tree();
                /*n->value = '1';
                n->proof_number = 0;
                n->disproof_number = inf;*/
            }

            else if(n->disproof_number == 0){
                n->delete_sub_tree();
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
                    Move optimal_move = policy(n->game);
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
    }

    outfile4.close();
}

void store_disproof(pn_node* root){
    queue<pn_node*> q;

    set<string> node_set;

    node_set.insert(root->game->print_as_string());

    for(int i = 0 ; i < root->no_of_children ; i++){
        q.push(root->children[i]);
        node_set.insert(root->children[i]->game->print_as_string());
    }

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
                // just store the child with the smallest mpn
                if(!param.policy_applied){
                    for(int i = 0 ; i < n->no_of_children ; i++){
                        if(n->children[i]->proof_number == inf){
                            q.push(n->children[i]);
                            break;
                        }
                    }
                }

                else{
                    Move optimal_move = policy(n->game);
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
    }

    outfile4.close();
}

void delete_complete_tree(pn_node* root){
    if(root == NULL)
        return;

    for(int i = 0 ; i < root->no_of_children && root->isInternal ; i++)
        delete_complete_tree(root->children[i]);

    delete root;
}

void pn_search_second(pn_node* root , int itr){
    //root->evaluate_node();
    //root->setProofandDisproofNumbers();
    pn_node* current = root;
    int ctr = 0;

    //outfile4.open("./output/proof.txt",ios::out);

    while(root->proof_number != 0 && root->disproof_number != 0 && ctr < itr){
        pn_node* most_proving = current->selectMostProvingNode();
        //outfile4<<most_proving->game->print_as_string()<<endl;
        most_proving->ExpandNode();
        current = update_ancestors(most_proving , root);
        if(ctr % 1000 == 0){
            //cout<<root->proof_number<<" "<<root->disproof_number<<" "<<root->mpn<<" "<<root->dmpn<<endl;
            //current->print_data();
        }
        ctr++;
    }

    //store_proof(root);
}

void pn_search_first(pn_node* root){
    root->ExpandNode();

    int ctr = 0;

    while(root->proof_number != 0 && root->disproof_number != 0){
        pn_node* most_proving = root->selectMostProvingNode();
        //most_proving->print_data();
        ctr++;
        pn_search_second(most_proving , ctr);
        root->setProofandDisproofNumbers();
        //cout<<most_proving->proof_number<<" "<<most_proving->disproof_number<<endl;

        if(root->proof_number == 0)
            break;

        /*for(int i = 0 ; i < most_proving->no_of_children && most_proving != root ; i++){
            delete_complete_tree(most_proving->children[i]);
        }*/
        //cout<<most_proving->proof_number<<" "<<most_proving->disproof_number<<endl;

        //most_proving->isInternal = false;
        //most_proving->no_of_children = 0;
    }
}

int pn2_search_main(const Param & parameters){
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

                auto start = high_resolution_clock::now();
                pn_search_first(root_mobile);
                auto stop = high_resolution_clock::now();

                if(root_mobile->proof_number == 0){
                    outfile1<<"Proved\n";
                    //auto start = high_resolution_clock::now();
                    store_proof(root_mobile);
                    //auto stop = high_resolution_clock::now();
                }

                else{
                    outfile1<<"Disproved\n";
                    //auto start = high_resolution_clock::now();
                    store_disproof(root_mobile);
                    //auto stop = high_resolution_clock::now();
                }

                auto duration = duration_cast<milliseconds>(stop - start);
                outfile2<<duration.count()<<endl;

                outfile3<<max_ctr<<endl;

                ctr_nodes = 0;
                max_ctr = 0;

                for(int i = 0 ; i < root_mobile->no_of_children ; i++){
                    cout<<root_mobile->children[i]->proof_number<<" "<<root_mobile->children[i]->disproof_number<<endl;
                }

                cout<<root_mobile->proof_number<<"\t"<<root_mobile->disproof_number<<"\t"<<root_mobile->mpn<<endl;

//                delete root_mobile;
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
