#include <bits/stdc++.h>
#include "../include/generalised_tic_tac_toe.h"

using namespace std;
using namespace std::chrono;

#define inf -1

/* Takes input of a game board and returns proved or disproved as applicable for the position assuming it is player's turn*/

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

int max(int a , int b){
    if(a == inf || b == inf)
        return inf;

    else if(a >= b)
        return a;

    else
        return b;
}

class pn_node{

    // Todo replace board with a hash value and make a hash map
    
    public:
        Game* game; // Todo just store moves instead of complte game in each node
        vector<pn_node*> children; // an array of pointers to the children
        bool type; // true for OR node and false for AND node
        char value; 
        bool isInternal; // true for internal node and false otherwise
        void generate_children(); // generates children of the node

        int proof_number , disproof_number;
        pn_node* parent;
        
        pn_node(char** board_status){
            game = new Game(board_status);
        }

        void set_parent(pn_node* par){
            parent = par;
            if(par == NULL)
                type = true;
            else 
                type = !(par->type);
            isInternal = false;
        }

        ~pn_node(){
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

class transposition_table{
    pn_node** TT;


    public:
        transposition_table(){
            unsigned long long int table_size = pow(3,M*N);
            TT = new pn_node*[table_size];
        }

        bool lookUpTT(pn_node* n , int &proof , int &disproof){
            if(TT[n->game->hash_value] == NULL)
                return false;

            else{
                proof = TT[n->game->hash_value]->proof_number;
                disproof = TT[n->game->hash_value]->disproof_number;
                return true;
            }
        }

        void putInTT(pn_node* n){
            TT[n->game->hash_value] = n;
        }

        void findChildrenInTT(pn_node* n){
            for(int i = 0 ; i < n->children.size() ; i++){
                if(TT[n->children[i]->game->hash_value] != NULL){
                    n->children[i]->proof_number = TT[n->children[i]->game->hash_value]->proof_number;
                    n->children[i]->disproof_number = TT[n->children[i]->game->hash_value]->disproof_number;
                }
            }
        }

};

void pn_node::generate_children(){
    
    for(int i = 0 ; i < M ; i++){
        for(int j = 0 ; j < N ; j++){
            if(game->isValidMove(i,j)){
                // make move
                game->make_move(type , i , j);

                pn_node* child = new pn_node(game->board);
                
                //undo the move
                game->undo_move(i,j);

                child->set_parent(this);
                children.push_back(child);
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

            for(int i = 0 ; i < children.size() ; i++){
                if(children[i]->disproof_number == inf)
                    disproof_inf = true;
                disproof_number+=(children[i]->disproof_number);
                proof_number = min(proof_number , children[i]->proof_number);
                if(children[i]->proof_number == 0)
                    proof_zero = true;
            }

            if(disproof_inf)
                disproof_number = inf;

            if(proof_zero)
                proof_number = 0;
        }

        else{ // AND node
            disproof_number = inf; proof_number = 0;

            for(int i = 0 ; i < children.size() ; i++){
                if(children[i]->proof_number == inf)
                    proof_inf = true;
                proof_number+=(children[i]->proof_number);
                disproof_number = min(disproof_number , children[i]->disproof_number);
                if(children[i]->disproof_number == 0)
                    disproof_zero = true;
            }

            if(proof_inf)
                proof_number = inf;

            if(disproof_zero)
                disproof_number = 0;

        }
    }

    else{
        switch(value){
            case '0': // disproven
                proof_number = inf;
                disproof_number = 0;
                break;

            case '1': //proven
                proof_number = 0;
                disproof_number = inf;
                break;

            case '2': //unknown
                proof_number = 1;
                disproof_number = 1;

                /*if(type){
                    proof_number = 1;
                    disproof_number = game->legal_moves;
                }

                else{
                    proof_number = game->legal_moves;
                    disproof_number = 1;
                }*/
                
                break;
        }
    }

}

pn_node* pn_node::selectMostProvingNode(){
    pn_node* n = this;

    while(n->isInternal){
        int value;
        pn_node* best;

        if(n->type){ // OR node
            value = n->children[0]->proof_number;
            best = n->children[0];
            for(int i = 1 ; i < n->children.size() ; i++){
                if((value > n->children[i]->proof_number || value == inf) && n->children[i]->proof_number != inf){ // select one with the minimum proof number
                    best = n->children[i];
                    value = n->children[i]->proof_number;
                }
            }
        }

        else{ // AND node
            value = n->children[0]->disproof_number;
            best = n->children[0];
            for(int i = 1 ; i < n->children.size() ; i++){
                if((value > n->children[i]->disproof_number || value == inf) && n->children[i]->disproof_number != inf){ // select one with the minimum disproof number
                    best = n->children[i];
                    value = n->children[i]->disproof_number;
                }
            
            }
        }

        n = best;
    }

    return n;
}

void pn_node::ExpandNode(){
    generate_children();

    for(int i = 0 ; i < children.size() ; i++){
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
    while(n != root){
        int old_proof = n->proof_number;
        int old_disproof = n->disproof_number;
        n->setProofandDisproofNumbers();

        if(n->proof_number == old_proof && n->disproof_number == old_disproof)
            return n; // if unchanged , then return

        n = n->parent;
    }

    root->setProofandDisproofNumbers();
    return root;
}

void pn_search(pn_node* root){
    root->evaluate_node();
    root->setProofandDisproofNumbers();
    pn_node* current = root;
    int ctr = 0;

    while(root->proof_number != 0 && root->disproof_number != 0 && ctr < 100000){
        pn_node* most_proving = current->selectMostProvingNode();
        most_proving->ExpandNode();
        current = update_ancestors(most_proving , root);
        ctr++;
        //cout<<root->proof_number<<" "<<root->disproof_number<<endl;
    }
}

// calculate sum of proof numbers of children
int proof_sum(pn_node* n){
    int ctr = 0;

    for(int i = 0 ; i < n->children.size() ; i++){
        ctr+=(n->children[i]->proof_number);
        if(n->children[i]->proof_number == inf)
            return inf;
    }

    return ctr;
}

int disproof_min(pn_node* n){
    if(n->children.empty())
        return -1;

    int temp = n->children[0]->disproof_number;

    for(int i = 0 ; i < n->children.size()  ; i++){
        temp = min(temp , n->children[i]->disproof_number);

        if(temp == 0)
            return 0;
    }

    return temp;
}

pn_node* selectChild(pn_node* n , int proof){
    int min_proof = inf;
    int min_disproof = inf;
    pn_node* best;

    for(int i = 0 ; i < n->children.size() ; i++){
        int disproof_child = n->children[i]->disproof_number;
        if(disproof_child != 0)
            disproof_child = max(disproof_child , proof);

        if((disproof_child < min_disproof || min_disproof == inf) || (disproof_child == min_disproof && (n->children[i]->proof_number < min_proof || min_proof == inf))){
            best = n->children[i];
            min_proof = n->children[i]->proof_number;
            min_disproof = n->children[i]->disproof_number;
        }
    }

    return best;
}

void explore_node(pn_node* n , transposition_table* table){
    int proof , disproof;
    if(table->lookUpTT(n,proof,disproof)){
        if(proof == 0 || disproof == 0 || (proof >= n->proof_number && disproof >= n->disproof_number)){
            n->proof_number = n->disproof_number = 0;
            return;
        }
    }

    else{
        proof = 0;
    }

    if(n->game->isTerminal()){
        n->setProofandDisproofNumbers();
        table->putInTT(n);
        return;
    }

    n->ExpandNode();
    n->setProofandDisproofNumbers();
    table->putInTT(n);

    //multiple iterative deepening
    while(true){
        table->findChildrenInTT(n);
        int proof_s = proof_sum(n) , disproof_m = disproof_min(n);
        //cout<<proof_s<<" "<<disproof_m<<" "<<n->proof_number<<" "<<n->disproof_number<<endl;

        if(proof_s == 0 || disproof_m == 0 || (n->proof_number < disproof_m && n->disproof_number < proof_s)){
            n->proof_number = disproof_m;
            n->disproof_number = proof_s;
            table->putInTT(n);
            return;
        }

        proof = max(proof , disproof_m);
        pn_node* n_child = selectChild(n , proof);

        if(n->disproof_number > proof_s && (n_child->proof_number <= n_child->disproof_number || n->proof_number <= disproof_m))
            n_child->proof_number++;
        else
            n_child->disproof_number++;

        if(!table->lookUpTT(n_child,proof,disproof)){
            pn_search(n_child);
            table->putInTT(n_child);
        }

        else{
            explore_node(n_child,table);
        }
    }

    
}

void pds_pn(pn_node* root){
    root->proof_number = 1;
    root->disproof_number = 1;
    transposition_table*  table = new transposition_table();

    while(true){
        explore_node(root , table);

        if(root->proof_number == 0 || root->disproof_number == 0)
            break;

        if(root->proof_number <= root->disproof_number)
            root->proof_number++;

        else
            root->disproof_number++;

    }
}

int main(){

    fstream newfile , outfile1 , outfile2 , outfile3;

    newfile.open("./src/input.txt",ios::in);
    outfile1.open("./output/output.txt",ios::out);
    outfile2.open("./output/times_pn_search(in microseconds).txt",ios::out);
    outfile3.open("./output/nodes_pn_search.txt",ios::out);

    outfile2<<"The left column represents mobility enabled and right is mobility disabled\n";
    outfile3<<"The left column represents mobility enabled and right is mobility disabled\n";

    if(newfile.is_open()){
        string temp;

        while(getline(newfile , temp)){
            if(temp.length() == M*N){
                char** board = make_board_from_file(temp);
                pn_node* root_mobile = new pn_node(board);
                root_mobile->set_parent(NULL);
                
                auto start = high_resolution_clock::now();
                pds_pn(root_mobile);
                auto stop = high_resolution_clock::now();

                if(root_mobile->proof_number == 0)
                    outfile1<<"Proved\n";
                else
                    outfile1<<"Disproved\n";

                auto duration = duration_cast<microseconds>(stop - start);
                
                outfile2<<duration.count()<<endl; 

                for(int i = 0 ; i < M ; i++)
                    delete [] board[i];

                delete [] board;
                delete root_mobile;
                //delete root_immobile;
            }
        }
    }


    newfile.close();
    outfile1.close();
    outfile2.close();
    outfile3.close();
    
    return 0;
}

