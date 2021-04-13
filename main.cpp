#include <iostream>

#include "gamestate.hpp"
#include "game_logic.hpp"


int main()
{	
	// Gamestate defaulty;
	// defaulty.Print();

	std::string move_2 = "rnbqkbnr/ppp1pppp/8/3pP3/8/5N2/PPPP1PPP/RNBQKB1R b KQkq d6 1 2";
	std::string pawn_promo = "rnbqkb1r/p1pppp1p/8/2n3P1/8/2N5/PpPP1PPP/R1BQKBNR w KQkq - 0 1";
	std::string lots_pawns = "8/2PPPPPP/1P6/1P6/8/8/8/8 w - - 0 1";

	std::string bisop = "8/7N/8/6N1/3N4/8/8/8 w - - 0 1";
	std::string rooks = "1Q5B/8/8/8/8/8/8/R7 w - - 0 1";
	std::string kings = "8/8/4K3/8/8/8/8/3K4 w - - 0 1";


	std::string move_12 = "2krr3/1ppq1ppp/p1pbb2n/8/3PP3/2N1BN2/PP3PPP/R2QR1K1 w - - 7 12";

	Gamestate from_fen(move_12);
	from_fen.Print();


	// Generate pawn moves from a2
	// std::cout << Convert_to_Algebraic(8) << "\n";
	// std::cout << Convert_to_Index("a2") << "\n";
	// std::cout << "Pawn moves:\n";
	// std::vector<std::string> pawns = Generate_Pawn_Moves(from_fen, Convert_to_Index("b2"));
	// for (int i = 0; i < pawns.size(); i++)
	// {
	// 	std::cout << pawns[i] << "\n";
	// }

	// Generate all white moves from given position
	std::vector<std::string> all_white = Generate_Player_Moves(from_fen, 'w');
	for (int i = 0; i < all_white.size(); i++)
	{
		std::cout << all_white[i] << "\n";
	}

	std::vector<std::string> all_black = Generate_Player_Moves(from_fen, 'b');
	for (int i = 0; i < all_black.size(); i++)
	{
		std::cout << all_black[i] << "\n";
	}



	return 0;
}