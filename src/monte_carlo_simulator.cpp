#include <bits/stdc++.h>
#include <algorithm>
#include "generalised_tic_tac_toe.h"
#include "generalised_tic_tac_toe_bitboard.h"
#include "monte_carlo_simulator.h"

using namespace std;
using namespace std::chrono;

// plays n games randomly
void monte_carlo_simulator(const Game & game, int n){
    int ctr = 0 , r, c;

    while(ctr < n){
        Game* g = game.clone();
        //bool isPlayer = true;

        while(!g->isTerminal()){
            r = rand() % N;
            c = rand() % M; // AS: Careful, this needs to be M to properly handle non-square boards.

            while(!g->isValidMove(r,c)){
                r = rand() % N;
                c = rand() % M;
            }
            g->make_move(r , c);
            //isPlayer = !isPlayer;
        }
        ctr++;
    }
}

void fast_monte_carlo_simulator(const Game & game, int n){
  vector<int> moves;
  for(int i = 0; i < M*N; i++)
    if (game.isValidMove(i/M, i%M))
      moves.push_back(i);

  for(int ctr = 0; ctr < n; ctr++){
    Game* g = game.clone();
    int maxi = moves.size();
    bool isPlayer = true;

    while(!g->isTerminal()){
      const int index = rand() % maxi;
      const int move = moves[index];

      g->make_move(move/M, move%M);
      isPlayer = !isPlayer;
      maxi--;
      swap(moves[index], moves[maxi]);
    }

    //g->print_board();
    //cout<<endl<<endl;
  }
}

/*void monte_carlo_verifier(int n){
    int ctr = 0 , r, c;

    Bitboard game;
    CharSS game2;
    while(ctr < n){
        Game* g = game.clone();
        Game* g2 = game2.clone();
        bool isPlayer = true;

        while(!g->isTerminal()){
          if (g2->isTerminal()) {
            cout << "bb : " << g->print_as_string() << endl;
            cout << "ss : " << g2->print_as_string() << endl;
            assert(false);
          }

            r = rand() % N;
            c = rand() % N;

            while(!g->isValidMove(r,c)){
              assert(!g2->isValidMove(r,c));
                r = rand() % N;
                c = rand() % N;
            }
            assert(g2->isValidMove(r,c));

            g->make_move(r , c);
            g2->make_move(r , c);
            if (g->print_as_string() != g2->print_as_string()) {
              cout << "move : " << r << " " << c << endl;
              cout << "bb : " << g->print_as_string() << endl;
              cout << "ss : " << g2->print_as_string() << endl;
              assert(false);
            }
            isPlayer = !isPlayer;
        }
          assert(g2->isTerminal());

        ctr++;
    }
}*/

int monte_carlo_simulator_main(const Game & game){
    int n = 100000;

    auto start = high_resolution_clock::now();
    fast_monte_carlo_simulator(game, n);
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time taken by function: "
         << duration.count() << " seconds" << endl;

    return 0;
}
