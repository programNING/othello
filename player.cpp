#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
     
     pside = side;
     opp_side = (side == BLACK) ? WHITE : BLACK;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */ 
     
     if (opponentsMove != NULL) {
		 // update copy of board
		 b.doMove(opponentsMove, opp_side);
	 }
	
	std::vector<Move *> valid_moves;
	
	if (!b.hasMoves(pside)) {
		return NULL;
	}
	
	valid_moves = b.getMoves(pside);
	
	int best_score = -7000; // some nonsensical low number
							// fixed to be larger than 64 * 3
	Move * best_move; 
	
	if (!testingMinimax) {
		for (unsigned int i = 0; i < valid_moves.size(); i++) {
			if (Heuristic(valid_moves[i]) > best_score) {
				best_score = Heuristic(valid_moves[i]);
				best_move = valid_moves[i];
			}
		}
		
		// now to free valid moves
		for (unsigned int i = 0; i < valid_moves.size(); i++) {
			if (valid_moves[i]->x != best_move->x && valid_moves[i]->y
				!= best_move->y) {
				delete valid_moves[i];
			}
		}
		
		
		//update our personal board copy to reflect our chosen move		
		b.doMove(best_move, pside);
		 
	}
	
	else {
		Minimax(&b, 2, true);
		best_move = minimax_best;
		b.doMove(best_move, pside);
	}
	 
	return best_move;
}

int Player::Minimax(Board * board, int depth, bool is_player) {
	
	if (depth == 0) {
		return board->count(pside) - board->count(opp_side);
	}

	else {
		if (is_player) {
			int best_value = -100;
			std::vector<Move*> curr_moves = board->getMoves(pside);
			for (unsigned int i = 0; i < curr_moves.size(); i++) {
				// make a copy of the board
				Board * iboard = board->copy();
			
				// perform the possible move
				iboard->doMove(curr_moves[i], pside);
			
				// see what value it returns
				int curr_value = Minimax(iboard, depth - 1, false);
				best_value = std::max(best_value, curr_value);
				if (best_value == curr_value) {
					minimax_best = curr_moves[i];
				}
				
				delete iboard;
			}
			curr_moves.clear();
			return best_value;
			
		}
		else {
			int best_value = 100;
			std::vector<Move*> curr_moves = board->getMoves(opp_side);
			for (unsigned int i = 0; i < curr_moves.size(); i++) {
				// make a copy of the board
				Board * iboard = board->copy();
			
				// perform the possible move
				iboard->doMove(curr_moves[i], opp_side);
			
				// see what value it returns
				int curr_value = Minimax(iboard, depth - 1, true);
				best_value = std::min(best_value, curr_value);
			}
			curr_moves.clear();
			return best_value;
		}
	}
}


/**
 * 
 * Heuristics taken from cs.cornell.edu/~yuli/othello/othello.html.
 * 
 * **note: was suggested to make a table for weights of each grid
 * on othello board
 * 
 **/
 
int Player::Heuristic(Move * move) {
	Board * copy = b.copy();
	copy->doMove(move, pside);
	int disc_count = (copy->count(pside) - copy->count(opp_side));
	std::vector<Move*> moveslist = copy->getMoves(pside);
	int heur_score = (10 * moveslist.size()) + disc_count;
	
	// multiply if especially bad or good coordinate
	
	if (move->x == 0 || move->x == 7) {
		// next to corners but on edge
		if (move->y == 1 || move->y == 6) {
			heur_score = heur_score * -2;
		}
		// a corner
		else if (move->y == 0 || move->y == 7) {
			heur_score = heur_score * 100;
		}
		// an edge
		else {
			heur_score = heur_score * 2;
		}
	}
	
	// diagonal to corners
	if (move->x == 1 || move->x == 6) {
		if (move->y == 6 || move->y == 1) {
			heur_score = heur_score * -3;
		}
	}
	
	if (move->y == 0 || move->y == 7) {
		// next to corners but on edge
		if (move->x == 1 || move->x == 6) {
			heur_score = heur_score * -2;
		}
		// corner already accounted for
		// an edge
		else {
			heur_score = heur_score * 2;
		}
	}
	
	// free the copy board;
	delete copy;
	return heur_score;
}
