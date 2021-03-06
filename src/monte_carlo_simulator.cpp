#include <bits/stdc++.h>
#include <algorithm>
#include "generalised_tic_tac_toe.h"
#include "generalised_tic_tac_toe_bitboard.h"
#include "monte_carlo_simulator.h"

using namespace std;
using namespace std::chrono;

void monte_carlo_verifier(int n){
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

            r = rand() % M;
            c = rand() % N;

            while(!g->isValidMove(r,c)){
              assert(!g2->isValidMove(r,c));
                r = rand() % M;
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
        if (!g2->isTerminal()) {
          cout << "bb : " << g->print_as_string() << endl;
          cout << "ss : " << g2->print_as_string() << endl;
          assert(false);
        }

        ctr++;
    }
}

// plays n games randomly
void monte_carlo_simulator(const Game & game, int n){
    int ctr = 0 , r, c;

    while(ctr < n){
        Game* g = game.clone();
        //bool isPlayer = true;

        while(!g->isTerminal()){
            r = rand() % M;
            c = rand() % N; // AS: Careful, this needs to be M to properly handle non-square boards.

            while(!g->isValidMove(r,c)){
                r = rand() % M;
                c = rand() % N;
            }
            g->make_move(r , c);
            //isPlayer = !isPlayer;
        }
        ctr++;
        g->print_board();
        cout<<"\n\n";
    }
}

int fast_simulator_count(const Game & game, int n){
  auto start = high_resolution_clock::now();
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
      maxi--;
      swap(moves[index], moves[maxi]);
    }
  }
  auto duration = duration_cast<milliseconds>(high_resolution_clock::now() - start);
  int ms = duration.count();
  return ms;
}

int fast_simulator_time(const Game & game, int seconds){
  int target_ms = 1000*seconds;
  vector<int> moves;
  for(int i = 0; i < M*N; i++)
    if (game.isValidMove(i/M, i%M))
      moves.push_back(i);

  int ctr = 0;
  const int step = 10;
  auto start = high_resolution_clock::now();
  auto stop = high_resolution_clock::now();
  while(duration_cast<milliseconds>(stop - start).count() < target_ms){
    for(int i = 0; i < step; i++){
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
      ctr++;
    }
    stop = high_resolution_clock::now();
  }
  return ctr;
}

double benchmark_count(const Game & game, int simulations, int samples){
  double total_time = 0;

  for(int i = 0; i < samples; i++) {
    int ms = fast_simulator_count(game, simulations);
    cerr << ms << "ms ";
    total_time += ms;
  }
  cerr << endl;
  return total_time / samples;
}

double benchmark_time(const Game & game, double seconds, int samples){
  double total_simulations = 0;

  for(int i = 0; i < samples; i++) {
    int simus = fast_simulator_time(game, seconds);
    cerr << simus/1000. << "k ";
    total_simulations += simus;
  }
  cerr << endl;
  return total_simulations / samples / 1000;
}
