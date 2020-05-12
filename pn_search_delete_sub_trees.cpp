#include <bits/stdc++.h>
#include "../include/generalised_tic_tac_toe.h"

using namespace std;
using namespace std::chrono;

#define inf -1

bool isMobile = true; // determines whether pn_search is mobile or not

/* Takes input of a game board and returns proved or disproved as applicable for the position assuming it is player's turn*/

int ctr_mobile = 0;
int ctr_immobile = 0;

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

    Game* game; // Todo just store moves instead of complte game in each node
    pn_node** children; // an array of pointers to the children
    bool type; // true for OR node and false for AND node 
    bool isInternal; // true for internal node and false otherwise
    int no_of_children;
    void generate_children(); // generates children of the node
    
    public:

        int proof_number , disproof_number;
        pn_node* parent;
        char value;
        
        pn_node(char** board_status){
            game = new Game(board_status);
            no_of_children = game->legal_moves;
            value = '2';
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
    for(int i = 0 ; i < no_of_children ; i++)
        delete children[i];

    delete [] children;
    isInternal = false;
    no_of_children = 0;
}

void pn_node::generate_children(){

    int k = 0;
    children = new pn_node*[no_of_children];
    
    for(int i = 0 ; i < M ; i++){
        for(int j = 0 ; j < N ; j++){
            if(game->isValidMove(i,j)){
                // make move
                game->make_move(type , i , j);

                pn_node* child = new pn_node(game->board);
                
                //undo the move
                game->undo_move(i,j);

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

    if(score == 10)
        value = '1';

    else if(score == -10)
        value = '0';

    else
        value = '2';
}

void pn_node::setProofandDisproofNumbers(){
    bool proof_inf = false , disproof_inf = false;
    if(isInternal){
        if(type){ // OR node
            proof_number = inf; disproof_number = 0;

            for(int i = 0 ; i < no_of_children ; i++){
                if(children[i]->disproof_number == inf)
                    disproof_inf = true;
                disproof_number+=(children[i]->disproof_number);
                proof_number = min(proof_number , children[i]->proof_number);
            }

            if(disproof_inf)
                disproof_number = inf;
        }

        else{ // AND node
            disproof_number = inf; proof_number = 0;

            for(int i = 0 ; i < no_of_children ; i++){
                if(children[i]->proof_number == inf)
                    proof_inf = true;
                proof_number+=(children[i]->proof_number);
                disproof_number = min(disproof_number , children[i]->disproof_number);
            }

            if(proof_inf)
                proof_number = inf;

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
                if(!isMobile){
                    proof_number = 1;
                    disproof_number = 1;
                }

                else{
                    if(type){
                        proof_number = 1;
                        disproof_number = game->legal_moves;
                    }

                    else{
                        proof_number = game->legal_moves;
                        disproof_number = 1;
                    }

                }
                
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

    return n;
}

void pn_node::ExpandNode(){
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
    while(n != root){
        int old_proof = n->proof_number;
        int old_disproof = n->disproof_number;
        n->setProofandDisproofNumbers();

        if(n->proof_number == old_proof && n->disproof_number == old_disproof)
            return n; // if unchanged , then return

        else if(n->proof_number == 0){
            n->delete_sub_tree();
            n->value = '1';
        }

        else if(n->disproof_number == 0){
            n->delete_sub_tree();
            n->value = '0';
        }

        n = n->parent;
    }

    root->setProofandDisproofNumbers();
    return root;
}

void pn_search(pn_node* root){
    root->evaluate_node();
    root->setProofandDisproofNumbers();
    pn_node* current = root;

    while(root->proof_number != 0 && root->disproof_number != 0){
        pn_node* most_proving = current->selectMostProvingNode();
        most_proving->ExpandNode();
        //most_proving->print_data();
        current = update_ancestors(most_proving , root);
        if(isMobile)
            ctr_mobile++;
        else
            ctr_immobile++;
        //cout<<"Current :";current->print_data();
        //cout<<root->proof_number<<" "<<root->disproof_number;
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
                pn_search(root_mobile);
                auto stop = high_resolution_clock::now();

                if(root_mobile->proof_number == 0)
                    outfile1<<"Proved\n";
                else
                    outfile1<<"Disproved\n";

                isMobile = false;

                pn_node* root_immobile = new pn_node(board);
                root_immobile->set_parent(NULL);

                auto start1 = high_resolution_clock::now();
                pn_search(root_immobile);
                auto stop1 = high_resolution_clock::now();

                auto duration = duration_cast<microseconds>(stop - start);
                auto duration1 = duration_cast<microseconds>(stop1 - start1);
                
                outfile2<<duration.count()<<"\t"<<duration1.count()<<endl; 
                outfile3<<ctr_mobile<<"\t"<<ctr_immobile<<endl;

                ctr_mobile = 0;
                ctr_immobile = 0;
                isMobile = true;

                for(int i = 0 ; i < M ; i++)
                    delete [] board[i];

                delete [] board;
                delete root_mobile;
                delete root_immobile;
            }
        }
    }


    newfile.close();
    outfile1.close();
    outfile2.close();
    outfile3.close();
    
    return 0;
}
