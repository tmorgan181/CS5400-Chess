#ifndef GAME_LOGIC_HPP
#define GAME_LOGIC_HPP

#include "gamestate.hpp"

#include <vector>
#include <cstdlib> // isupper, islower, rand, time
#include <algorithm> // std::remove


// These values determine what index offset to add for each adjacent square
// ----------
// |+7|+8|+9|
// ----------
// |-1|+0|+1|
// ----------
// |-9|-8|-7|
// ----------
const int NW = 7;
const int N = 8;
const int NE = 9;
const int W = -1;
const int E = 1;
const int SW = -9;
const int S = -8;
const int SE = -7;


//////// Function Declarations ////////

// Convert the given square index (0-63) into the algebraic format (a1-h8)
std::string Convert_to_Algebraic(const int index);

// Convert the given algebraic square (a1-h8) into its equivalent index (0-63)
int Convert_to_Index(const std::string square);

// Given some game state, generate all valid moves for the current player color
std::vector<std::string> Generate_Player_Moves(const Gamestate& g, const char player_color);

// Generate all possible pawn moves from the current square. When "ignore_non_attacks" == true,
// straight-line and en passant pawn moves are not returned.
std::vector<std::string> Generate_Pawn_Moves(const Gamestate& g, const int index, const bool ignore_non_attacks);

// Generate all possible bishop moves from the current square
std::vector<std::string> Generate_Bishop_Moves(const Gamestate& g, const int index);

// Generate all possible rook moves from the current square
std::vector<std::string> Generate_Rook_Moves(const Gamestate& g, const int index);

// Generate all possible knight moves from the current square
std::vector<std::string> Generate_Knight_Moves(const Gamestate& g, const int index);

// Generate all possible queen moves from the current square
std::vector<std::string> Generate_Queen_Moves(const Gamestate& g, const int index);

// Generate all possible king moves from the current square. When "ignore_non_attacks" == true,
// the possible castling moves are not returned
std::vector<std::string> Generate_King_Moves(const Gamestate& g, const int index, const bool ignore_non_attacks);

// Given some gamestate, and a square index that contains a piece, determine every possible
// move that piece can make. When "ignore_non_attacks" == true, straight-line pawn moves, en passant,
// and castling moves are not returned
std::vector<std::string> Generate_Piece_Moves(const Gamestate& g, const int index, const bool ignore_non_attacks);

// Given some gamestate, some square index, and the current player's color, determine
// if that square is under attack by any opposing piece
bool Square_Under_Attack(const Gamestate& g, const int index, const char player_color);

// Given some gamestate and a move to make, update the gamestate accordingly
Gamestate Simulate_Move(const Gamestate& g, const std::string move);

// Given a list of possible moves, select one at random and return it
std::string Get_Random_Move(const std::vector<std::string> all_moves);


//////// Function Implementations ////////

std::string Convert_to_Algebraic(const int index)
{
	// Get the column as an ascii character value ('a' = 97)
	char file = index % 8 + 97;
	// Get the row as an integer, then convert to a string
	std::string rank = std::to_string(index / 8 + 1);

	return file + rank;
}

int Convert_to_Index(const std::string square)
{
	// First character is file, second is rank; both characters' ASCII values are used
	return ((square[1] - 49) * 8) + (square[0] - 97);
}

std::vector<std::string> Generate_Pawn_Moves(const Gamestate& g, const int index, const bool ignore_non_attacks)
{
	std::vector<std::string> pawn_moves;

	// Get the algebraic format of the square
	std::string curr_square = Convert_to_Algebraic(index);

	// If the pawn is white
	if (g.board[index] == 'P')
	{
		// If straight moves and en passant are not ignored
		if (!ignore_non_attacks)
		{
			// If rank < 7 and there is no piece N of the pawn
			if (index < 48 && g.board[index + N] == ' ')
			{
				// Add the N move
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + N));

				// If the pawn is on the 2nd rank, and there is no piece N * 2 of the pawn
				if (curr_square[1] == '2' && g.board[index + N*2] == ' ')
				{
					// Add the N*2 move
					pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + N*2));
				}
			}

			// If rank == 7 and there is no piece N of the pawn
			if (curr_square[1] == '7' && g.board[index + N] == ' ')
			{
				// Add all possible promotion moves
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + N) + 'q');
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + N) + 'n');
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + N) + 'r');
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + N) + 'b');
			}

			// If there is an en passant target NW of the pawn (and NW is in bounds)
			if (index % 8 != 0 && g.en_passant_target == Convert_to_Algebraic(index + NW))
			{
				// If the pawn started on rank 7
				if (curr_square[1] == '7')
				{
					// Add the NW capture plus any promotion
					pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + NW) + 'q');
					pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + NW) + 'n');
					pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + NW) + 'r');
					pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + NW) + 'b');
				}
				// Else just add the NW capture
				else
				{
					pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + NW));
				}
			}
			// Repeat for NE
			if (index % 8 != 7 && g.en_passant_target == Convert_to_Algebraic(index + NE))
			{
				// If the pawn started on rank 7
				if (curr_square[1] == '7')
				{
					// Add the NE capture plus any promotion
					pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + NE) + 'q');
					pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + NE) + 'n');
					pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + NE) + 'r');
					pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + NE) + 'b');
				}
				// Else just add the NE capture
				else
				{
					pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + NE));
				}
			}
		}

		// If there is an enemy piece NW of the pawn (and NW is in bounds)
		if (index % 8 != 0 && islower(g.board[index + NW]))
		{
			// If the pawn started on rank 7
			if (curr_square[1] == '7')
			{
				// Add the NW capture plus any promotion
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + NW) + 'q');
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + NW) + 'n');
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + NW) + 'r');
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + NW) + 'b');
			}
			// Else just add the NW capture
			else
			{
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + NW));
			}
		}
		// Repeat for NE
		if (index % 8 != 7 && islower(g.board[index + NE]))
		{
			// If the pawn started on rank 7
			if (curr_square[1] == '7')
			{
				// Add the NE capture plus any promotion
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + NE) + 'q');
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + NE) + 'n');
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + NE) + 'r');
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + NE) + 'b');
			}
			// Else just add the NE capture
			else
			{
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + NE));
			}
		}
	}

	// Else if the pawn is black
	else if (g.board[index] == 'p')
	{
		// If straight moves and en passant are not ignored
		if (!ignore_non_attacks)
		{
			// If rank > 2 and there is no piece S of the pawn
			if (index > 15 && g.board[index + S] == ' ')
			{
				// Add the S move
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + S));

				// If the pawn is on the 7th rank, and there is no piece S * 2 of the pawn
				if (curr_square[1] == '7' && g.board[index + S*2] == ' ')
				{
					// Add the S*2 move
					pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + S*2));
				}
			}

			// If rank == 2 and there is no piece S of the pawn
			if (curr_square[1] == '2' && g.board[index + S] == ' ')
			{
				// Add all possible promotion moves
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + S) + 'q');
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + S) + 'n');
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + S) + 'r');
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + S) + 'b');
			}
	
			// If there is an en passant target SW of the pawn and SW is in bounds
			if (index % 8 != 0 && g.en_passant_target == Convert_to_Algebraic(index + SW))
			{
				// If the pawn started on rank 2
				if (curr_square[1] == '2')
				{
					// Add the SW capture plus any promotion
					pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + SW) + 'q');
					pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + SW) + 'n');
					pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + SW) + 'r');
					pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + SW) + 'b');
				}
				// Else just add the SW capture
				else
				{
					pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + SW));
				}
			}
			// Repeat for SE
			if (index % 8 != 7 && g.en_passant_target == Convert_to_Algebraic(index + SE))
			{
				// If the pawn started on rank 2
				if (curr_square[1] == '2')
				{
					// Add the SE capture plus any promotion
					pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + SE) + 'q');
					pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + SE) + 'n');
					pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + SE) + 'r');
					pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + SE) + 'b');
				}
				// Else just add the SE capture
				else
				{
					pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + SE));
				}
			}
		}


		// If there is an enemy piece SW of the pawn (and SW is in bounds)
		if (index % 8 != 0 && isupper(g.board[index + SW]))
		{
			// If the pawn started on rank 2
			if (curr_square[1] == '2')
			{
				// Add the SW capture plus any promotion
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + SW) + 'q');
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + SW) + 'n');
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + SW) + 'r');
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + SW) + 'b');
			}
			// Else just add the SW capture
			else
			{
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + SW));
			}
		}
		// Repeat for SE
		if (index % 8 != 7 && isupper(g.board[index + SE]))
		{
			// If the pawn started on rank 2
			if (curr_square[1] == '2')
			{
				// Add the SE capture plus any promotion
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + SE) + 'q');
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + SE) + 'n');
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + SE) + 'r');
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + SE) + 'b');
			}
			// Else just add the SE capture
			else
			{
				pawn_moves.push_back(curr_square + Convert_to_Algebraic(index + SE));
			}
		}
	}

	return pawn_moves;
}

std::vector<std::string> Generate_Bishop_Moves(const Gamestate& g, const int index)
{
	std::vector<std::string> bishop_moves;

	// Get the algebraic format of the square
	std::string curr_square = Convert_to_Algebraic(index);

	// Check each diagonal direction one at a time
	std::vector<int> diagonals = {NW, NE, SE, SW};
	for (int i = 0; i < 4; i++)
	{
		int dir = diagonals[i];

		// Keep checking that direction until a piece or the edge of the board is encountered
		int next_index = index;
		while (true)
		{
			// Check if the next index will be out bounds
			// Check top rank and leftmost file
			if (dir == NW && (next_index % 8 == 0 || next_index >= 56))
			{
				break;
			}
			// Check top rank and rightmost file
			else if (dir == NE && (next_index % 8 == 7 || next_index >= 56))
			{
				break;
			}
			// Check bottom rank and rightmost file
			else if (dir == SE && (next_index % 8 == 7 || next_index <= 7))
			{
				break;
			}
			// Check bottom rank and leftmost file
			else if (dir == SW && (next_index % 8 == 0 || next_index <= 7))
			{
				break;
			}

			// Update the next index
			next_index += dir;

			// Check if there's a piece
			if (g.board[next_index] != ' ')
			{
				// If the piece is the same color as the bishop
				if (isupper(g.board[index]) && isupper(g.board[next_index]) || islower(g.board[index]) && islower(g.board[next_index]))
				{
					// Then the bishop can't move any farther in this direction
					break;
				}
				// Else the pieces are opposite colors
				else
				{
					// Add the capturing move, then the bishop cannot move any farther
					bishop_moves.push_back(curr_square + Convert_to_Algebraic(next_index));
					break;
				}
			}
			// Else the square is open
			else
			{
				// Add the move
				bishop_moves.push_back(curr_square + Convert_to_Algebraic(next_index));
			}
		}
	}

	return bishop_moves;
}

std::vector<std::string> Generate_Rook_Moves(const Gamestate& g, const int index)
{
	std::vector<std::string> rook_moves;

	// Get the algebraic format of the square
	std::string curr_square = Convert_to_Algebraic(index);

	// Check each straight direction one at a time
	std::vector<int> straights = {N, E, S, W};
	for (int i = 0; i < 4; i++)
	{
		int dir = straights[i];

		// Keep checking that direction until a piece or the edge of the board is encountered
		int next_index = index;
		while (true)
		{
			// Check if this loop will be out bounds
			// Check top rank
			if (dir == N && (next_index >= 56))
			{
				break;
			}
			// Check rightmost file
			else if (dir == E && (next_index % 8 == 7))
			{
				break;
			}
			// Check bottom rank
			else if (dir == S && (next_index <= 7))
			{
				break;
			}
			// Check leftmost file
			else if (dir == W && (next_index % 8 == 0))
			{
				break;
			}

			// Update the next index
			next_index += dir;

			// Check if there's a piece
			if (g.board[next_index] != ' ')
			{
				// If the piece is the same color as the rook
				if (isupper(g.board[index]) && isupper(g.board[next_index]) || islower(g.board[index]) && islower(g.board[next_index]))
				{
					// Then the rook can't move any farther in this direction
					break;
				}
				// Else the pieces are opposite colors
				else
				{
					// Add the capturing move, then the rook cannot move any farther
					rook_moves.push_back(curr_square + Convert_to_Algebraic(next_index));
					break;
				}
			}
			// Else the square is open
			else
			{
				// Add the move
				rook_moves.push_back(curr_square + Convert_to_Algebraic(next_index));
			}
		}
	}

	return rook_moves;
}

std::vector<std::string> Generate_Knight_Moves(const Gamestate& g, const int index)
{
	std::vector<std::string> knight_moves;

	// Get the algebraic format of the square
	std::string curr_square = Convert_to_Algebraic(index);

	// Possible knight moves and their relative indices:
	// ---------------------
	// |   |+15|   |+17|   |
	// ---------------------
	// |+6 |   |   |   |+10|
	// ---------------------
	// |   |   |+0 |   |   |
	// ---------------------
	// |-10|   |   |   |-6 |
	// ---------------------
	// |   |-17|   |-15|   |
	// ---------------------

	// Determine which moves are in bounds
	std::vector<int> possible_moves;

	// Check +6
	if (index % 8 > 1 && index < 56)
	{
		possible_moves.push_back(6);
	}
	// Check +15
	if (index % 8 > 0 && index < 48)
	{
		possible_moves.push_back(15);
	}
	// Check +17
	if (index % 8 < 7 && index < 48)
	{
		possible_moves.push_back(17);
	}
	// Check +10
	if (index % 8 < 6 && index < 56)
	{
		possible_moves.push_back(10);
	}
	// Check -6
	if (index % 8 < 6 && index >= 8)
	{
		possible_moves.push_back(-6);
	}
	// Check -15
	if (index % 8 < 7 && index >= 16)
	{
		possible_moves.push_back(-15);
	}
	// Check -17
	if (index % 8 > 0 && index >= 16)
	{
		possible_moves.push_back(-17);
	}
	// Check -10
	if (index % 8 > 1 && index >= 8)
	{
		possible_moves.push_back(-10);
	}

	// For every in-bounds move, determine if it is a valid move
	for (int i = 0; i < possible_moves.size(); i++)
	{
		int next_index = index + possible_moves[i];

		// Check if there's a piece at the new square
		if (g.board[next_index] != ' ')
		{
			// If the piece is the opposite color as the knight
			if (isupper(g.board[index]) && islower(g.board[next_index]) || islower(g.board[index]) && isupper(g.board[next_index]))
			{
				// Add the capturing move
				knight_moves.push_back(curr_square + Convert_to_Algebraic(next_index));
			}
		}
		// Else the square is open
		else
		{
			// Add the move
			knight_moves.push_back(curr_square + Convert_to_Algebraic(next_index));
		}
	}

	return knight_moves;
}

std::vector<std::string> Generate_Queen_Moves(const Gamestate& g, const int index)
{
	std::vector<std::string> queen_moves;

	// Get the algebraic format of the square
	std::string curr_square = Convert_to_Algebraic(index);

	// Check each direction one at a time
	std::vector<int> directions = {NW, N, NE, E, SE, S, SW, W};
	for (int i = 0; i < 8; i++)
	{
		int dir = directions[i];

		// Keep checking that direction until a piece or the edge of the board is encountered
		int next_index = index;
		while (true)
		{
			// Check if the next index will be out bounds
			// Check top rank and leftmost file
			if (dir == NW && (next_index % 8 == 0 || next_index >= 56))
			{
				break;
			}
			// Check top rank
			if (dir == N && next_index >= 56)
			{
				break;
			}
			// Check top rank and rightmost file
			else if (dir == NE && (next_index % 8 == 7 || next_index >= 56))
			{
				break;
			}
			// Check rightmost file
			else if (dir == E && next_index % 8 == 7)
			{
				break;
			}
			// Check bottom rank and rightmost file
			else if (dir == SE && (next_index % 8 == 7 || next_index <= 7))
			{
				break;
			}
			// Check bottom rank
			else if (dir == S && next_index <= 7)
			{
				break;
			}
			// Check leftmost file
			else if (dir == W && next_index % 8 == 0)
			{
				break;
			}
			// Check bottom rank and leftmost file
			else if (dir == SW && (next_index % 8 == 0 || next_index <= 7))
			{
				break;
			}

			// Update the next index
			next_index += dir;

			// Check if there's a piece
			if (g.board[next_index] != ' ')
			{
				// If the piece is the same color as the queen
				if (isupper(g.board[index]) && isupper(g.board[next_index]) || islower(g.board[index]) && islower(g.board[next_index]))
				{
					// Then the queen can't move any farther in this direction
					break;
				}
				// Else the pieces are opposite colors
				else
				{
					// Add the capturing move, then the queen cannot move any farther
					queen_moves.push_back(curr_square + Convert_to_Algebraic(next_index));
					break;
				}
			}
			// Else the square is open
			else
			{
				// Add the move
				queen_moves.push_back(curr_square + Convert_to_Algebraic(next_index));
			}
		}
	}

	return queen_moves;
}

std::vector<std::string> Generate_King_Moves(const Gamestate& g, const int index, const bool ignore_non_attacks)
{
	std::vector<std::string> king_moves;

	// Get the algebraic format of the square
	std::string curr_square = Convert_to_Algebraic(index);

	// Determine which moves are in bounds
	std::vector<int> possible_moves;

	// Check NW
	if (index % 8 != 0 && index < 56)
	{
		possible_moves.push_back(NW);
	}
	// Check N
	if (index < 56)
	{
		possible_moves.push_back(N);
	}
	// Check NE
	if (index % 8 != 7 && index < 56)
	{
		possible_moves.push_back(NE);
	}
	// Check E
	if (index % 8 != 7)
	{
		possible_moves.push_back(E);
	}
	// Check SE
	if (index % 8 != 7 && index >= 8)
	{
		possible_moves.push_back(SE);
	}
	// Check S
	if (index >= 8)
	{
		possible_moves.push_back(S);
	}
	// Check SW
	if (index % 8 != 0 && index >= 8)
	{
		possible_moves.push_back(SW);
	}
	// Check W
	if (index % 8 != 0)
	{
		possible_moves.push_back(W);
	}

	// For every in-bounds move, determine if it is a valid move
	for (int i = 0; i < possible_moves.size(); i++)
	{
		int next_index = index + possible_moves[i];

		// Check if there's a piece at the new square
		if (g.board[next_index] != ' ')
		{
			// If the piece is the opposite color as the king
			if (isupper(g.board[index]) && islower(g.board[next_index]) || islower(g.board[index]) && isupper(g.board[next_index]))
			{
				// Add the capturing move
				king_moves.push_back(curr_square + Convert_to_Algebraic(next_index));
			}
		}
		// Else the square is open
		else
		{
			// Add the move
			king_moves.push_back(curr_square + Convert_to_Algebraic(next_index));
		}
	}

	// If castling moves are not ignored
	if (!ignore_non_attacks)
	{
		// Check the castles string
		for (int i = 0; i < g.castles.length(); i++)
		{
			// If the king is white
			if (isupper(g.board[index]))
			{
				// If 'K' is still part of the castles string and the kingside is clear
				// (indexes 5, 6 = f1, g1)
				if (g.castles[i] == 'K' && g.board[5] == ' ' && g.board[6] == ' ')
				{
					// If neither of those squares nor the king are under attack
					if (!Square_Under_Attack(g, i, 'w') && !Square_Under_Attack(g, 5, 'w') && !Square_Under_Attack(g, 6, 'w'))
					{
						// Then kingside castling (e1g1) is valid
						king_moves.push_back(curr_square + "g1");
						std::cout << "added k-side white" << std::endl;
					}
				}

				// If 'Q' is still part of the castles string and the queenside is clear
				// (indexes 1, 2, 3 = b1, c1, d1)
				if (g.castles[i] == 'Q' && g.board[1] == ' ' && g.board[2] == ' ' && g.board[3] == ' ')
				{
					// If none of those squares nor the king are under attack
					if (!Square_Under_Attack(g, i, 'w') && !Square_Under_Attack(g, 1, 'w') && !Square_Under_Attack(g, 2, 'w') && !Square_Under_Attack(g, 3, 'w'))
					{
						// Then queenside castling (e1b1) is valid
						king_moves.push_back(curr_square + "b1");
						std::cout << "added q-side white" << std::endl;
					}
				}
			}

			// If the king is black
			if (islower(g.board[index]))
			{
				// If 'k' is still part of the castles string and the kingside is clear
				// (indexes 61, 62 = f8, g8)
				if (g.castles[i] == 'k' && g.board[61] == ' ' && g.board[62] == ' ')
				{
					// If neither of those squares nor the king are under attack
					if (!Square_Under_Attack(g, i, 'b') && !Square_Under_Attack(g, 61, 'b') && !Square_Under_Attack(g, 62, 'b'))
					{
						// Then kingside castling (e8g8) is valid
						king_moves.push_back(curr_square + "g8");
						std::cout << "added k-side black" << std::endl;
					}
				}

				// If 'q' is still part of the castles string and the queenside is clear
				// (indexes 57, 58, 59 = b8, c8, d8)
				if (g.castles[i] == 'q' && g.board[57] == ' ' && g.board[58] == ' ' && g.board[59] == ' ')
				{
					// If none of those squares nor the king are under attack
					if (!Square_Under_Attack(g, i, 'b') && !Square_Under_Attack(g, 57, 'b') && !Square_Under_Attack(g, 58, 'b') && !Square_Under_Attack(g, 59, 'b'))
					{
						// Then queenside castling (e8b8) is valid
						king_moves.push_back(curr_square + "b8");
						std::cout << "added q-side black" << std::endl;
					}
				}
			}
		}
	}

	return king_moves;
}

std::vector<std::string> Generate_Player_Moves(const Gamestate& g, const char player_color)
{
	// Store all valid moves in this vector
	std::vector<std::string> valid_moves;

	// Temp vector to store valid moves for each piece
	std::vector<std::string> new_moves;

	// Iterate over every square on the board
	for (int i = 0; i < g.board.size(); i++)
	{
		// If the piece and player are the same color
		if ((isupper(g.board[i]) && player_color == 'w') || (islower(g.board[i]) && player_color == 'b'))
		{
			// Generate all the valid moves that piece can make
			// We want to include non-attacking moves, so the last parameter must be false
			new_moves = Generate_Piece_Moves(g, i, false);

			// Append the new moves to the total list
			valid_moves.insert(valid_moves.end(), new_moves.begin(), new_moves.end());
		}
	}

	// Now that we have all the available valid moves, we need to check if any of these moves
	// will allow our king to be under attack

	// Simulate every move we have generated thus far to see if any of them will put the king in danger
	Gamestate sim_state(g);
	int king_index;

	// Make a copy of the valid moves so we can iterate and delete simultaneously
	std::vector<std::string> iter_moves = valid_moves;

	for (int i = 0; i < iter_moves.size(); i++)
	{
		// Simulate the move from the current state
		sim_state = Simulate_Move(g, iter_moves[i]);

		// Find this player's king's square index
		for (int j = 0; j < sim_state.board.size(); j++)
		{
			if ((sim_state.board[j] == 'K' && player_color == 'w') || (sim_state.board[j] == 'k' && player_color == 'b'))
			{
				king_index = j;
			}
		}

		// Check if the king's square is under attack in the new state
		if (Square_Under_Attack(sim_state, king_index, player_color))
		{
			// The king is under attack, so this move is not valid - remove it
			valid_moves.erase(std::remove(valid_moves.begin(), valid_moves.end(), iter_moves[i]), valid_moves.end());
		}
	}

	// Return the complete list
	return valid_moves;
}

std::vector<std::string> Generate_Piece_Moves(const Gamestate& g, const int index, const bool ignore_non_attacks)
{
	// Store the valid moves
	std::vector<std::string> moves;

	// Determine what kind of piece it is, and generate its moves
	// If piece is a pawn
	if (g.board[index] == 'P' || g.board[index] == 'p')
	{
		// Get the new moves, then copy them to the existing list
		moves = Generate_Pawn_Moves(g, index, ignore_non_attacks);
	}

	// If piece is a bishop
	else if (g.board[index] == 'B' || g.board[index] == 'b')
	{
		moves = Generate_Bishop_Moves(g, index);
	}

	// If piece is a rook
	else if (g.board[index] == 'R' || g.board[index] == 'r')
	{
		moves = Generate_Rook_Moves(g, index);
	}

	// If piece is a knight
	else if (g.board[index] == 'N' || g.board[index] == 'n')
	{
		moves = Generate_Knight_Moves(g, index);
	}

	// If piece is a queen
	else if (g.board[index] == 'Q' || g.board[index] == 'q')
	{
		moves = Generate_Queen_Moves(g, index);
	}

	// If piece is a king
	else if (g.board[index] == 'K' || g.board[index] == 'k')
	{
		moves = Generate_King_Moves(g, index, ignore_non_attacks);
	}
	else
	{
		std::cout << "There is no piece on square " << Convert_to_Algebraic(index) << ".\n";
	}

	return moves;
}

bool Square_Under_Attack(const Gamestate& g, const int index, const char player_color)
{
	// Iterate over every square on the board
	for (int i = 0 ; i < g.board.size(); i++)
	{
		// If there is a piece
		if (g.board[i] != ' ')
		{
			// If the piece is an enemy
			if ((player_color == 'w' && islower(g.board[i])) || (player_color == 'b' && isupper(g.board[i])))
			{
				// Generate every valid move for that piece, ignoring straight pawn moves, en passant moves, and castling
				std::vector<std::string> moves = Generate_Piece_Moves(g, i, true);

				// std::cout << "All attacking moves for " << g.board[i] << " on " << Convert_to_Algebraic(i) << ":" << std::endl;
				// for (int n = 0; n < moves.size(); n++)
				// {
				// 	std::cout << moves[n] << std::endl;
				// }

				// Check if the ending square of any of those moves is our index square
				for (int j = 0; j < moves.size(); j++)
				{
					// Get the target square from the move (characters 3 & 4)
					std::string target = moves[j].substr(2,2);
					// std::cout << "target: " << target << std::endl;

					if (target == Convert_to_Algebraic(index))
					{
						// The index square is under attack
						return true;
					}
				}
			}
		}
	}

	// If we get here, then the square is not under attack
	return false;
}

Gamestate Simulate_Move(const Gamestate& g, const std::string move)
{
	// For now i just want to update the board. other updates will come later // todo
	// also will deal with promotions later??

	// Create a copy of the gamestate
	Gamestate new_state(g);

	// Start square is first 2 chars, target square is next 2 chars
	// Convert the squares to board indicies to be used later
	int src_sq = Convert_to_Index(move.substr(0,2));
	int dest_sq = Convert_to_Index(move.substr(2,2));

	// Copy the contents of the src_sq to the dest_sq
	new_state.board[dest_sq] = new_state.board[src_sq];

	// The src_sq then becomes empty
	new_state.board[src_sq] = ' ';

	return new_state;
}

std::string Get_Random_Move(const std::vector<std::string> all_moves)
{
	// Seed random
	srand(time(NULL));

	// Get a random number between 0 and (length - 1)
	int i = rand() % all_moves.size();

	// Return the move at random index i
	return all_moves[i];


}


#endif