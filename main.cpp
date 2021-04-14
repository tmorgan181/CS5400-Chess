#include <iostream>

#include "gamestate.hpp"
#include "game_logic.hpp"


int main()
{	
	std::string start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	std::string move_2 = "rnbqkbnr/ppp1pppp/8/3pP3/8/5N2/PPPP1PPP/RNBQKB1R b KQkq d6 1 2";
	std::string pawn_promo = "rnbqkb1r/p1pppp1p/8/2n3P1/8/2N5/PpPP1PPP/R1BQKBNR w KQkq - 0 1";
	std::string lots_pawns = "8/2PPPPPP/1P6/1P6/8/8/8/8 w - - 0 1";

	std::string bisop = "8/7N/8/6N1/3N4/8/8/8 w - - 0 1";
	std::string rooks = "1Q5B/8/8/8/8/8/8/R7 w - - 0 1";
	std::string kings = "8/8/4K3/8/8/8/8/3K4 w - - 0 1";


	std::string move_12 = "2krr3/1ppq1ppp/p1pbb2n/8/3PP3/2N1BN2/PP3PPP/R2QR1K1 w - - 7 12";

	std::string pawn_attacked = "rnbqkbnr/1ppppppp/8/pP6/8/8/P1PPPPPP/RNBQKBNR b KQkq - 0 1";

	std::string q_2_n = "3qq3/8/6N1/8/8/4R3/4K3/8 w - - 0 1";

	std::string castl = "8/8/8/8/8/5q2/8/R3K2R w KQkq - 0 1";

	std::string check = "8/8/8/8/8/5q2/R7/5B1K w - - 0 1";

	Gamestate from_fen(start_fen);
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


	// std::vector<std::string> a2_moves = Generate_Piece_Moves(from_fen, Convert_to_Index("b5"), false);
	// for (int i = 0; i < a2_moves.size(); i++)
	// {
	// 	std::cout << a2_moves[i] << "\n";
	// }

	// bool attack = Square_Under_Attack(from_fen, Convert_to_Index("d2"), 'w');
	// std::cout << "d2 under attack by black? " << attack << std::endl;

	// Gamestate n_st = Simulate_Move(from_fen, "e2e4");
	// n_st.Print();

	// Gamestate n_stt = Simulate_Move(n_st, "e4e2");
	// n_stt.Print();


	return 0;
}