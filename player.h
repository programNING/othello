#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    
    Move *doMove(Move *opponentsMove, int msLeft);
    int AB_Minimax(Board * board, int depth, bool s, int alpha, int beta);
    int Heuristic(Board * b);
   	int getBoardWeight(Board * b);
    
    

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    // Board for player to keep track of
    Board b;

private:
	
	int table[64] = { 100, -30, 5, 5, 5, 5, -30, 100,
					 -30, -60, 0, 0, 0, 0, -60, -30,
					  5, 0, 3, 3, 3, 3, 0, 5,
					  5, 0, 3, 0, 0, 3, 0, 5, 
					  5, 0, 3, 0, 0, 3, 0, 5,
					  5, 0, 3, 3, 3, 3, 0, 5,
					 -30, -60, 0, 0, 0, 0, -60, -3,
					  100, -30, 5, 5, 5, 5, -3, 100};
	Side opp_side;
	Side pside;
	Move * minimax_best;
};

#endif
