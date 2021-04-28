#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "gamestate.hpp"
#include "game_logic.hpp"

#include <map>		// key-value container
#include <limits>	// INFINITY
#include <climits>	// INT_MIN, INT_MAX

// Max value an integer can have (n <= MAX_INT)
// const int MAX_INT = std::numeric_limits<int>::max();
// Infinite value for a float (f < INFINITY)
const float INFINITY = std::numeric_limits<float>::infinity();

// Max depth for ID-DL-Minimax
const int MAX_DEPTH = 1;

// Material values for each piece
const std::map<char,int> MATERIAL_VALS = 
{
	{'P', 1}, {'p', 1},
	{'B', 3}, {'b', 3},
	{'N', 3}, {'n', 3},
	{'R', 5}, {'r', 5},
	{'Q', 9}, {'q', 9},
	{'K', 0}, {'k', 0}
};


//////// Function Declarations ////////

// Iteratively search the game tree up to the max depth, attempting to
// find the best minimax move for the next player
std::string ID_DL_Minimax(const Gamestate& g);

// Recursively explore the game tree up to the given depth limit, and return the best move
std::string DL_Minimax_Choice(const Gamestate& g, const int depth_limit);

// Determine the max value of the given game state
float Max_Value(const Gamestate& g, const int depth);

// Determine the min value of the given game state
float Min_Value(const Gamestate& g, const int depth);

// Determine the "score" of the given terminal game state
// draw == 0
// black checkmated == INT_MAX
// white checkmated == INT_MIN
int Utility_Value(const Gamestate& g);

// Determine the "score" of the given game state based on material advantage only
int hValue_Material(const Gamestate& g);


//////// Function Implementations ////////

std::string ID_DL_Minimax(const Gamestate& g)
{
	std::string action = "";
	// Check each depth one at a time
	for (int i = 0; i <= MAX_DEPTH; i++)
	{
		// DL minimax with depth limit = i
		std::cout << "Depth: " << i << "\n";
		action = DL_Minimax_Choice(g, i);

		// Check time if that is a factor
	}

	return action;
}

std::string DL_Minimax_Choice(const Gamestate& g, const int depth_limit)
{
	// Find all valid moves for the next player in the current state
	std::vector<std::string> valid_moves = Generate_Player_Moves(g, g.next_turn);

	// Check every move to see if it's the best for the player
	int best_score = (g.next_turn == 'w' ? INT_MIN : INT_MAX);
	std::string best_move = "";
	Gamestate sim_state(g);

	std::vector<std::string> ties;
	for (int i = 0; i < valid_moves.size(); i++)
	{
		int new_score = 0;

		// Generate the result of the move
		sim_state = Simulate_Move(g, valid_moves[i]);

		// Check if that state has a better value than the previous best
		if (g.next_turn == 'w')		// White's turn
		{
			// If white finds a move with higher value than the last min
 			new_score = Min_Value(sim_state, depth_limit);
			if (new_score > best_score || Black_Checkmated(sim_state))		// Check if the next move can checkmate
 			{
 				// Update the best
 				best_score = new_score;
 				best_move = valid_moves[i];

 				// Clear the ties
 				ties.clear();
 			}
 			else if (new_score == best_score)
 			{
 				// Keep track of ties
 				ties.push_back(valid_moves[i]);
 			}
		}
		else	// Black's turn
		{
			// If black finds a move with lower value than the last max
			new_score = Max_Value(sim_state, depth_limit);
			if (new_score < best_score || White_Checkmated(sim_state))		// Check if the next move can checkmate
			{
				// Update the best
				best_score = new_score;
				best_move = valid_moves[i];

				// Clear the ties
				ties.clear();
			}
			else if (new_score == best_score)
			{
				// Keep track of ties
				ties.push_back(valid_moves[i]);
			}
		}
	}

	// If there is no good move (mate is being forced), just take the first move
	if (best_move == "")
	{
		best_move = valid_moves[0];

		std::cout << "\tMate forced, picked move " << valid_moves[0] << "\n";
	}
	// Else if there is a tie
	else if (ties.size() > 1)
	{
		std::cout << "\tNo best move out of ties: \n\t{ ";
		for (int i = 0; i < ties.size(); i++)
		{
			std::cout << ties[i] << (i < ties.size() - 1 ? ", " : " }\n");
		}

		// Get a random one
		best_move = Get_Random_Move(ties);
	}
	else
	{
		std::cout << "\tBest move for " << g.next_turn << " is " << best_move << "\n";
	}

	// The best move found by minimax up to the depth limit
	return best_move;
}

float Max_Value(const Gamestate& g, const int depth)
{
	// If the gamestate is terminal (end of game)
	if (Game_Draw(g) || Game_Checkmate(g))
	{
		// Return the state's utility value
		return Utility_Value(g);
	}

	// If we hit the depth limit
	if (depth == 0)
	{
		// Return the material score of this state
		return hValue_Material(g);
	}

	// Keep searching for the best move
	// Find all valid moves for white in the current state
	std::vector<std::string> valid_moves = Generate_Player_Moves(g, 'w');

	// Check every move to see if it's the best for the max
	int best_score = INT_MIN;
	std::string best_move = "";
	Gamestate sim_state(g);

	for (int i = 0; i < valid_moves.size(); i++)
	{
		// std::cout << "Considering move: " << valid_moves[i] << " at depth " << depth << "\n";
		int new_score = 0;

		// Generate the result of the move
		sim_state = Simulate_Move(g, valid_moves[i]);

		// If max finds a move with higher value than the last max
		new_score = Min_Value(sim_state, depth - 1);

		// std::cout << "Move " << valid_moves[i] << " has a min score of " << new_score << "\n";

		if (new_score > best_score)
		{
			// Update the best
			best_score = new_score;
			best_move = valid_moves[i];
		}
	}

	// std::cout << "The maximum min score is " << best_score << "\n";

	// The best score for max at this depth
	return best_score;
}

float Min_Value(const Gamestate& g, const int depth)
{
	// If the gamestate is terminal (end of game)
	if (Game_Draw(g) || Game_Checkmate(g))
	{
		// Return the state's utility value
		return Utility_Value(g);
	}

	// If we hit the depth limit
	if (depth == 0)
	{
		// Return the material score of this state
		return hValue_Material(g);
	}

	// Keep searching for the best move
	// Find all valid moves for white in the current state
	std::vector<std::string> valid_moves = Generate_Player_Moves(g, 'b');

	// Check every move to see if it's the best for the min
	int best_score = INT_MAX;
	std::string best_move = "";
	Gamestate sim_state(g);

	for (int i = 0; i < valid_moves.size(); i++)
	{

		// std::cout << "Considering move: " << valid_moves[i] << " at depth " << depth << "\n";
		int new_score = 0;

		// Generate the result of the move
		sim_state = Simulate_Move(g, valid_moves[i]);

		// If min finds a move with lower value than the last min
		new_score = Max_Value(sim_state, depth - 1);

		// std::cout << "Move " << valid_moves[i] << " has a max score of " << new_score << "\n";



		if (new_score < best_score)
		{
			// Update the best
			best_score = new_score;
			best_move = valid_moves[i];
		}
	}

	// std::cout << "The minimum max score is " << best_score << "\n";

	// The best score for min at this depth
	return best_score;
}

int Utility_Value(const Gamestate& g)
{
	// draw == 0
	// black checkmated == INT_MAX
	// white checkmated == INT_MIN

	if (Game_Draw(g))
	{
		return 0;
	}
	else if (White_Checkmated(g))
	{
		return INT_MIN;
	}
	else if (Black_Checkmated(g))
	{
		return INT_MAX;
	}
	else
	{
		// non-terminal state
		return 1;
	}
}

int hValue_Material(const Gamestate& g)
{
	int white_mat = 0;
	int black_mat = 0;

	// Iterate over every square
	for (int i = 0; i < g.board.size(); i++)
	{
		// If it's a white piece
		if (isupper(g.board[i]))
		{
			// Increment the white material according to the table
			white_mat += MATERIAL_VALS.at(g.board[i]);
		}
		// If it's a black piece
		else if (islower(g.board[i]))
		{
			// Increment black material
			black_mat += MATERIAL_VALS.at(g.board[i]);
		}
	}

	return white_mat - black_mat;
}

#endif