#include <bits/stdc++.h>
#include "../include/generalised_tic_tac_toe_bitboard.h"

using namespace std;
using namespace std::chrono;

vector<bitset<M*N>> polyaminos;

// plays n games randomly
void monte_carlo_simulator(int n){
    int ctr = 0 , r, c;

    while(ctr < n){
        Game g;
        bool isPlayer = true;

        while(!g.isTerminal(polyaminos)){

            r = rand() % N;
            c = rand() % N;

            while(!g.isValidMove(r,c)){
                r = rand() % N;
                c = rand() % N;
            }

            g.make_move(isPlayer , r , c);
            isPlayer = !isPlayer;
        }

        /*g.print_board();
        cout<<"\n";*/

        ctr++;
    }
}

int main(){
    int n = 1000000;

    bitset<M*N> b1(string(bset1));
    bitset<M*N> b2(string(bset2));
    bitset<M*N> b3(string(bset3));
    bitset<M*N> b4(string(bset4));

    vector<bitset<M*N>> user_polyaminos({b1 , b2 , b3 , b4});

    construct_polyaminos(polyaminos , user_polyaminos);

    /*for(int i = 0 ; i < polyaminos.size() ; i++)
        cout<<polyaminos[i]<<endl;*/

    auto start = high_resolution_clock::now();
    monte_carlo_simulator(n);
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start); 
  
    cout << "Time taken by function: "
         << duration.count() << " seconds" << endl;

    /*bitset<16> whites(string("0110001100000000"));
    bitset<16> blacks(string("1001000011000000"));

    Game* g = new Game(whites , blacks);

    cout<<g->evaluate(polyaminos)<<endl<<g->isTerminal(polyaminos)<<endl;*/

    return 0;
}