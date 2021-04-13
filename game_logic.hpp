#ifndef GAME_LOGIC_HPP
#define GAME_LOGIC_HPP

#include "gamestate.hpp"

#include <vector>
#include <cstdlib> // isupper, islower


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

// Generate all possible valid pawn moves from the current square
std::vector<std::string> Generate_Pawn_Moves(const Gamestate& g, const int index);

// Generate all possible valid bishop moves from the current square
std::vector<std::string> Generate_Bishop_Moves(const Gamestate& g, const int index);

// Generate all possible valid rook moves from the current square
std::vector<std::string> Generate_Rook_Moves(const Gamestate& g, const int index);

// Generate all possible valid knight moves from the current square
std::vector<std::string> Generate_Knight_Moves(const Gamestate& g, const int index);

// Generate all possible valid queen moves from the current square
std::vector<std::string> Generate_Queen_Moves(const Gamestate& g, const int index);

// Generate all possible valid king moves from the current square
std::vector<std::string> Generate_King_Moves(const Gamestate& g, const int index);


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

std::vector<std::string> Generate_Pawn_Moves(const Gamestate& g, const int index)
{
	std::vector<std::string> pawn_moves;

	// Get the algebraic format of the square
	std::string curr_square = Convert_to_Algebraic(index);

	// If the pawn is white
	if (g.board[index] == 'P')
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

		// If there is an enemy piece or en passant target NW of the pawn (and NW is in bounds)
		if (index % 8 != 0 && (islower(g.board[index + NW]) || g.en_passant_target == Convert_to_Algebraic(index + NW)))
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
		if (index % 8 != 7 && (islower(g.board[index + NE]) || g.en_passant_target == Convert_to_Algebraic(index + NE)))
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

		// If there is an enemy piece or en passant target SW of the pawn (and SW is in bounds)
		if (index % 8 != 0 && (isupper(g.board[index + SW]) || g.en_passant_target == Convert_to_Algebraic(index + SW)))
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
		if (index % 8 != 7 && (isupper(g.board[index + SE]) || g.en_passant_target == Convert_to_Algebraic(index + SE)))
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

std::vector<std::string> Generate_King_Moves(const Gamestate& g, const int index)
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

		// Here we need to ignore moves that put us in check // todo
	
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

	// Here we need to check for castling moves // todo

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
			// Determine what kind of piece it is, and generate its moves
			// If piece is a pawn
			if (g.board[i] == 'P' || g.board[i] == 'p')
			{
				// Get the new moves, then add them to the existing list
				new_moves = Generate_Pawn_Moves(g, i);
				valid_moves.insert(valid_moves.end(), new_moves.begin(), new_moves.end());
			}

			// If piece is a bishop
			else if (g.board[i] == 'B' || g.board[i] == 'b')
			{
				new_moves = Generate_Bishop_Moves(g, i);
				valid_moves.insert(valid_moves.end(), new_moves.begin(), new_moves.end());
			}

			// If piece is a rook
			else if (g.board[i] == 'R' || g.board[i] == 'r')
			{
				new_moves = Generate_Rook_Moves(g, i);
				valid_moves.insert(valid_moves.end(), new_moves.begin(), new_moves.end());
			}

			// If piece is a knight
			else if (g.board[i] == 'N' || g.board[i] == 'n')
			{
				new_moves = Generate_Knight_Moves(g, i);
				valid_moves.insert(valid_moves.end(), new_moves.begin(), new_moves.end());
			}

			// If piece is a queen
			else if (g.board[i] == 'Q' || g.board[i] == 'q')
			{
				new_moves = Generate_Queen_Moves(g, i);
				valid_moves.insert(valid_moves.end(), new_moves.begin(), new_moves.end());
			}

			// If piece is a king
			else if (g.board[i] == 'K' || g.board[i] == 'k')
			{
				new_moves = Generate_King_Moves(g, i);
				valid_moves.insert(valid_moves.end(), new_moves.begin(), new_moves.end());
			}
		}
	}

	// Return the complete list
	return valid_moves;
}

// make a function to generate moves for a single piece // todo
// make functions to check for checks/checkmates/draws // todo
// make a function to update the gamestate given some move // todo
// 		(could use this to check for checks/mates/draws)


#endif