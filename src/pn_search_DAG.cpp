#include <bits/stdc++.h>
#include "generalised_tic_tac_toe.h"
#include "generalised_tic_tac_toe_bitboard.h"
#include "minimax.h"
#include "pn_search_DAG.h"

using namespace std;
using namespace std::chrono;

#define inf -1

bool isMobile = false;
bool minimal_proof = false;
bool policy_applied = false;
bool minimal_policy_search = false;
int depth_minimax = 2;

/* Takes input of a game board and returns proved or disproved as applicable for the position assuming it is player's turn*/

Move policy(Game* game){
    return findBestMove(game , depth_minimax);

    /*Move opt;
    for(int i = 0; i < M; i++){
        for(int j = 0 ; j < N ; j++)
        if(game->isValidMove(i,j)){
            opt.row = i;
            opt.col = j;
            break;
        }
    }  
    
    return opt;*/
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

int ctr_nodes = 0;

class pn_node{

    // Todo replace board with a hash value and make a hash map
    
    public:

        Game* game; // Todo just store moves instead of complte game in each node
        pn_node** children; // an array of pointers to the children
        bool type; // true for OR node and false for AND node
        char value; 
        bool isInternal; // true for internal node and false otherwise
        void generate_children(); // generates children of the node
        int no_of_children;
        bool reco_by_policy;

        int proof_number , disproof_number , mpn , dmpn;
        vector<pn_node*> parents;
        
        pn_node(Game* board);

        void set_parent(pn_node* par){
            
            if(par == NULL)
                type = true;
            else{
                type = !(par->type);
                parents.push_back(par);
            }
            isInternal = false;
        }

        ~pn_node(){
            ctr_nodes--;
            delete game;
        }

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

pn_node** transposition_table;

pn_node::pn_node(Game* board){
    game = board;
    const int legals = game->legalMoves();
    no_of_children = legals;
    value = '2';
    ctr_nodes++;
    // initialise pn , dn , mpn
    // call evalute always here for each node
    if(!isMobile){
            proof_number = 1;
            disproof_number = 1;
            mpn = 1;
            dmpn = 1;
        }

    else{
            if(type){
                proof_number = 1;
                mpn = 1;
                disproof_number = legals;
                dmpn = legals;
            }

            else{
                proof_number = legals;
                mpn = legals;
                disproof_number = 1;
                dmpn = 1;
            }

        }
    transposition_table[game->hashValue()] = this;
    reco_by_policy = false;
}

void pn_node::generate_children(){

    if(no_of_children == 0)
        return;

    int k = 0;
    children = new pn_node*[no_of_children];

    Move opt_move;

    if(minimal_policy_search){
        opt_move = policy(game);
    }

    for(int i = 0 ; i < M ; i++){
        for(int j = 0 ; j < N ; j++){
            if(game->isValidMove(i,j)){
                // make move
                Game* copygame = game->clone();
                copygame->make_move(i , j);

                if(transposition_table[copygame->hashValue()] != NULL){
                    children[k++] = transposition_table[copygame->hashValue()];
                    transposition_table[copygame->hashValue()]->set_parent(this);
                }

                else{
                    pn_node* child = new pn_node(copygame);
                    if(minimal_policy_search){
                        if(i == opt_move.row && j == opt_move.col)
                            child->reco_by_policy = true;
                    }
                    transposition_table[game->hashValue()] = child;
                    //pn_node* child = new pn_node(game->board);

                    //undo the move
                    //game->undo_move(i,j);

                    child->set_parent(this);
                    children[k++] = child;

                }

                
            }
        }
    }
}

void pn_node::evaluate_node(){
    int score = game->evaluate();

    if(score == 10)
        value = '1';

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

                if(!isMobile){
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

    if(minimal_policy_search && reco_by_policy){
        if(mpn != inf)
            mpn--;

        if(dmpn != inf)
            dmpn--;
    }

}

pn_node* pn_node::selectMostProvingNode(){
    pn_node* n = this;

    if(!minimal_proof){
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
    if(game->legalMoves() == 0)
        return;
    generate_children();

    for(int i = 0 ; i < no_of_children ; i++){
        children[i]->evaluate_node();
        children[i]->setProofandDisproofNumbers();

        if(type){ // OR node
            if(children[i]->proof_number == 0)
                break;
        }

        else{
            if(children[i]->disproof_number == 0)
                break;
        }

    }

    isInternal = true;
}

pn_node* update_ancestors(pn_node* n , pn_node* root){

    queue <pn_node*> nodes;
    nodes.push(n);

    pn_node* dummy;
    queue <pn_node*> base_ancestors;

    while(!nodes.empty()){
        dummy = nodes.front();
        nodes.pop();

        int old_proof = dummy->proof_number;
        int old_disproof = dummy->disproof_number;
        dummy->setProofandDisproofNumbers();

        if(old_proof == dummy->proof_number && old_disproof == dummy->disproof_number)
            base_ancestors.push(dummy);

        else{
            for(int i = 0 ; i < dummy->parents.size() ; i++)
                nodes.push(dummy->parents[i]);
        }
    }

    root->setProofandDisproofNumbers();

    if(base_ancestors.size() == 1 && !base_ancestors.front()->isInternal)
        return base_ancestors.front();

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

        if(!n->type){
            //outfile4<<n->game->print_as_string()<<endl;
            node_set.insert(n->game->print_as_string());
        }

        //node_set.insert(n->game->print_as_string());

        if(n->isInternal){
            if(n->type){
                // just store the child with the smallest mpn
                if(!policy_applied){
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
                    for(int i = 0 ; i < n->no_of_children ; i++){
                        if(n->children[i]->game->print_as_string() == temp_board){
                            if(n->children[i]->disproof_number != inf){
                                for(int j = 0 ; j < n->no_of_children ; j++){
                                    if(n->children[j]->disproof_number == inf){
                                        q.push(n->children[j]);
                                        break;
                                    }
                                }
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

    for(it = node_set.begin() ; it != node_set.end() ; it++){
        outfile4<<*it<<endl;
    } 

    outfile4.close();
}

void pn_search(pn_node* root){

    root->evaluate_node();
    root->setProofandDisproofNumbers();
    pn_node* current = root;
    int ctr = 0;

    while(root->proof_number != 0 && root->disproof_number != 0){
        pn_node* most_proving = current->selectMostProvingNode();
        most_proving->ExpandNode();
        current = update_ancestors(most_proving , root);
            
        /*if(ctr % 1000 == 0){
            cout<<root->proof_number<<"\t"<<root->disproof_number<<"\t"<<current->parents.size()<<endl;
            //current->print_data();
        }*/

        ctr++;
    }
}

int pn_search_DAG_main(){

    fstream newfile , outfile1 , outfile2 , outfile3;

    newfile.open("input.txt",ios::in);
    outfile1.open("./output/output.txt",ios::out);
    outfile2.open("./output/times_pn_search(in microseconds).txt",ios::out);
    outfile3.open("./output/nodes_pn_search.txt",ios::out);

    unsigned long long int table_size = pow(3,M*N);

    if(newfile.is_open()){
        string temp;

        while(getline(newfile , temp)){
            if(temp.length() == M*N){
                transposition_table = new pn_node*[table_size];
                //Bitboard board(temp);
                CharSS board(temp);
                pn_node* root_mobile = new pn_node(&board);
                root_mobile->set_parent(NULL);
                
                //auto start = high_resolution_clock::now();
                pn_search(root_mobile);
                //auto stop = high_resolution_clock::now();

                cout<<root_mobile->mpn<<" "<<root_mobile->dmpn<<endl;

                for(int i = 0 ; i < root_mobile->no_of_children ; i++){
                    cout<<root_mobile->children[i]->proof_number<<" "<<root_mobile->children[i]->disproof_number<<" "<<root_mobile->children[i]->mpn<<" "<<root_mobile->children[i]->dmpn<<endl;
                }

                auto start = high_resolution_clock::now();
                //store_proof(root_mobile);
                auto stop = high_resolution_clock::now();

                if(root_mobile->proof_number == 0)
                    outfile1<<"Proved\n";
                else
                    outfile1<<"Disproved\n";

                auto duration = duration_cast<microseconds>(stop - start);
                
                outfile2<<duration.count()<<endl; 
                outfile3<<ctr_nodes<<endl;

                
                //delete root_mobile;
                delete [] transposition_table;
            }
        }
    }


    newfile.close();
    outfile1.close();
    outfile2.close();
    outfile3.close();
    
    return 0;
}

