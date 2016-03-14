#include "player.h"
#define START_ALPHA (-100)
#define START_BETA (100)

#define C_ADJ_WEIGHT  (-3)
#define CORNER_WEIGHT (4)
#define EDGE_WEIGHT (2)
#define C_DIAG_WEIGHT (-4)


/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = true;

    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
     
     // initialize a weight table
     
     initializeWeightTable();
     
     pside = side;
     opp_side = (side == BLACK) ? WHITE : BLACK;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}


void Player::initializeWeightTable() {
	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Move * m = new Move(i, j);
			 
			if (i == 0 || i == 7) {
				// next to corners but on edge
				if (j == 1 || j == 6) {
					table[m] = C_ADJ_WEIGHT;
				}
				// a corner
				else if (j == 0 || j == 7) {
					table[m] = CORNER_WEIGHT;
				}
				// an edge
				else {
					table[m] = EDGE_WEIGHT;
				}
			}
		
		// diagonal to corners
			else if (i == 1 || i == 6) {
				if (j == 6 || j == 1) {
					table[m] = C_DIAG_WEIGHT;
				}
			}
		
			else if (j == 0 || j == 7) {
				// next to corners but on edge
				if (i == 1 || i == 6) {
					table[m] = C_ADJ_WEIGHT;
				}
				// corner already accounted for
				// an edge
				else {
					table[m] = EDGE_WEIGHT;
				}
			}
			else {
				// do something with the rest of the squares
			} 
					 
		}
	}
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
			Board * copy = b.copy();
			copy->doMove(valid_moves[i], pside);
			int move_score = Heuristic(copy);
			if (move_score > best_score) {
				best_score = move_score;
				best_move = valid_moves[i];
			}
			delete copy;
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
		AB_Minimax(&b, 2, true, START_ALPHA, START_BETA);
		best_move = minimax_best;
		b.doMove(best_move, pside);
	}
	 
	return best_move;
}

int Player::AB_Minimax(Board * board, int depth, bool is_player, int alpha,
					int beta) {
	
	if (depth == 0) {
//		return board->count(pside) - board->count(opp_side);
		return Heuristic(board);
	}

	else {
		if (is_player) {
			std::vector<Move*> curr_moves = board->getMoves(pside);
			for (unsigned int i = 0; i < curr_moves.size(); i++) {
				// make a copy of the board
				Board * iboard = board->copy();
			
				// perform the possible move
				iboard->doMove(curr_moves[i], pside);
			
				// see what value it returns
				int curr_value = AB_Minimax(iboard, depth - 1, false,
								alpha, beta);
				alpha = std::max(alpha, curr_value);
				if (alpha == curr_value) {
					minimax_best = curr_moves[i];
				}
				
				delete iboard;
			}
			curr_moves.clear();
			return alpha;
			
		}
		else {
			std::vector<Move*> curr_moves = board->getMoves(opp_side);
			for (unsigned int i = 0; i < curr_moves.size(); i++) {
				// make a copy of the board
				Board * iboard = board->copy();
			
				// perform the possible move
				iboard->doMove(curr_moves[i], opp_side);
			
				// see what value it returns
				int curr_value = AB_Minimax(iboard, depth - 1, true, alpha,
										beta);
				beta = std::min(beta, curr_value);
				if (alpha > beta) {
					break;
				}
			}
			curr_moves.clear();
			return beta;
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
 
int Player::Heuristic(Board * board) {
	
	int player_disc = board->count(pside);
	int opp_disc = board->count(opp_side);
	
	double dpar = (double)(player_disc - opp_disc) / (double)(player_disc + opp_disc);
	int disc_parity = 10 * dpar;
	
	std::vector<Move*> moveslist = board->getMoves(pside);
	std::vector<Move*> opp_moves = board->getMoves(opp_side);
	
	double mpar = (double)moveslist.size() - (double)opp_moves.size();
	int move_parity = 10 * mpar;
	
	int heur_score = (move_parity + disc_parity);
	
	// clean up
	moveslist.clear();
	opp_moves.clear();
	
	// multiply if especially bad or good coordinate
	
	
	
	// free the copy board;
	return heur_score;
}
