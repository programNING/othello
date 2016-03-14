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
    
    

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    // Board for player to keep track of
    Board b;

private:
	void initializeWeightTable();
	
	std::map<Move *, int> table;
	Side opp_side;
	Side pside;
	Move * minimax_best;
};

#endif
