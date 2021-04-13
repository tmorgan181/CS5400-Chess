#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <vector>
#include <queue>
#include <cstdlib> // isdigit
#include <sstream> // stringstream


class Gamestate
{
public:
	std::vector<char> board;		// squares are indexed 0-63 from bottom left to top right, each piece 
										// is represented by a different character ex) 'k' = black king, 'N' = white knight

	char next_turn;						// 'w' = white, 'b' = black
	std::string castles;				// castles available to each player ex) "KQkq" or "K--q"
	std::string en_passant_target;		// where en passant is possible ex) "c3" or "-"

	int halfmove_clock;					// halfmoves since last capture, promotion, or pawn move
	int fullmove_counter;				// current turn

	std::queue<std::string> last_eight_moves;	// last eight full moves made (UCI format)

	// Default constructor uses start state FEN
	Gamestate()
	{
		std::string start_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
		Gamestate temp(start_FEN);

		*this = temp;
	}

	// Copy constructor
	Gamestate(const Gamestate& g)
	{
		board = g.board;

		next_turn = g.next_turn;
		castles = g.castles;
		en_passant_target = g.en_passant_target;

		halfmove_clock = g.halfmove_clock;
		fullmove_counter = g.fullmove_counter;

		last_eight_moves = g.last_eight_moves;
	}

	// Constructor from given FEN string
	Gamestate(const std::string fen_string)
	{
		// Example FEN for the start state
		// start_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

		// splits[0] = position data
		// splits[1] = next turn
		// splits[2] = castling availabilities
		// splits[3] = en passant target
		// splits[4] = halfmove clock
		// splits[5] = current turn

		// Start the board off with all spaces
		for (int i = 0; i < 64; i++)
		{
			board.push_back(' ');
		}

		// Split the string at each space character
		std::vector<std::string> splits;
		std::stringstream ss(fen_string);
		std::string word;
		while (ss >> word)
		{
			splits.push_back(word);
		}

		// Start at top left square
		int rank = 7;
		int file = 0;

		// Fill in the piece positions
		for (int i = 0; i < splits[0].length(); i++)
		{
			char symbol = splits[0][i];		// the current character of the string

			// If the character is forward slash, go to next row down
			if (symbol == '/')
			{
				rank--;
				file = 0;
			}
			else
			{
				// If the character is a digit, then skip that number of empty squares
				if (isdigit(symbol))
				{
					file += symbol - '0';	// converts character digit to int using ASCII equivalents
				}
				// Otherwise the character must represent a piece
				else
				{
					// Add the piece's character to the board data and move to the next square over
					board[rank * 8 + file] = symbol;
					file++;
				}
			}
		}

		// FEN contains information for other state variables as well
		next_turn = splits[1][0];
		castles = splits[2];
		en_passant_target = splits[3];
		halfmove_clock = atoi(splits[4].c_str());
		fullmove_counter = atoi(splits[5].c_str());
	}

	// Output the board data and other state variables to the console
	void Print()
	{
		// Print board from top left to bottom right
		std::cout << " -------------------------------------\n";
		std::cout << " |   | a | b | c | d | e | f | g | h |\n";
		std::cout << " -------------------------------------\n";
		for (int rank = 7; rank >= 0; rank--)
		{
			std::cout << " | " << rank + 1 << " |";
			for (int file = 0; file < 8; file++)
			{
				int index = rank * 8 + file;
				std::cout << " " << this->board[index] << " |";
			}
			std::cout << "\n -------------------------------------\n";
		}
		std::cout << "\n";

		// Output other variables
		std::cout << "Current turn: " << fullmove_counter << "/" << next_turn << "\n";
		std::cout << "Halfmove Clock: " << halfmove_clock << "\n";
		std::cout << "Castles: " << castles << "\n";
		std::cout << "En Passant: " << en_passant_target << "\n";
		std::cout << "\n";

		return;
	}
};

#endif