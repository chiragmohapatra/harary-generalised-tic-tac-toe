#include <bits/stdc++.h>
#include "generalised_tic_tac_toe.h"
#include "monte_carlo_simulator.h"

using namespace std;
using namespace std::chrono;

// plays n games randomly
void monte_carlo_simulator(int n){
    int ctr = 0 , r, c;

    while(ctr < n){
        Game g;
        bool isPlayer = true;

        while(!g.isTerminal()){

            r = rand() % N;
            c = rand() % N;

            while(!g.isValidMove(r,c)){
                r = rand() % N;
                c = rand() % N;
            }

            g.make_move(isPlayer , r , c);
            isPlayer = !isPlayer;
        }

        ctr++;
    }
}

int monte_carlo_simulator_main(){
    int n = 1000000;

    auto start = high_resolution_clock::now();
    monte_carlo_simulator(n);
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start); 
  
    cout << "Time taken by function: "
         << duration.count() << " seconds" << endl; 

    return 0;
}
