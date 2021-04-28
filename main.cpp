#include <iostream>

#include "gamestate.hpp"
#include "game_logic.hpp"
#include "algorithms.hpp"


int main()
{	

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




	// Generate pawn moves from a2
	// std::cout << Convert_to_Algebraic(8) << "\n";
	// std::cout << Convert_to_Index("a2") << "\n";
	// std::cout << "Pawn moves:\n";
	// std::vector<std::string> pawns = Generate_Pawn_Moves(from_fen, Convert_to_Index("b2"));
	// for (int i = 0; i < pawns.size(); i++)
	// {
	// 	std::cout << pawns[i] << "\n";
	// }




	std::string start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	std::string draw = "7k/8/6Q1/8/8/8/8/7K b - - 1 1";
	std::string white_mate = "7k/8/6QK/8/8/8/8/8 w - - 1 1";
	std::string black_mate = "8/8/8/8/8/5q1k/8/7K w - - 1 1";

	std::string bish_draw = "6bk/8/8/8/8/8/5N2/7K w - - 0 1";

	std::string mate_in_2 = "7k/8/8/8/8/5q1r/8/5K2 w - - 1 1";

	std::string rook_weird = "3B4/8/5R2/4kN1p/P5p1/8/6p1/3RKN2 w - - 0 1";

	std::string mate_in_3 = "6nk/8/2Q4p/6R1/8/7K/8/8 w - - 0 2";

	Gamestate game_state(start_fen);
	game_state.Print();
	std::cout << "\n-------STARTING THE GAME!!!-------\n\n";

	// Take turns playing
	bool stop = false;
	int count = 0;
	while (!stop)
	{
		std::string new_move = "";

		// Check if we have reached the end of the game
		if (Game_Draw(game_state))	// Draw
		{
			std::cout << "\n-------GAME OVER!!!-------\n";
			std::cout << "It's a Draw! - " << Draw_Type(game_state) << "\n";
			break;
		}
		else if (White_Checkmated(game_state))	// Black wins
		{
			std::cout << "\n-------GAME OVER!!!-------\n";
			std::cout << "Black wins by Checkmate!\n";
			break;
		}
		else if (Black_Checkmated(game_state))	// White wins
		{
			std::cout << "\n-------GAME OVER!!!-------\n";
			std::cout << "White wins by Checkmate!\n";
			break;
		}
		else	// Else keep going
		{
			std::cout << "\n---------- TURN: " << game_state.fullmove_counter << " / " << game_state.next_turn << " ----------\n";

			// // Get moves for white via ID_DL_Minimax
			// if (game_state.next_turn == 'w')	// White's move
			// {
			// 	new_move = ID_DL_Minimax(game_state);
			// }

			// // Use a set of given moves for white
			// if (game_state.next_turn == 'w')
			// {
			// 	new_move = "g6g7";
			// }

			// // Get moves for white randomly
			// if (game_state.next_turn == 'w')
			// {
			// 	std::vector<std::string> all_white = Generate_Player_Moves(game_state, 'w');
			// 	new_move = Get_Random_Move(all_white);
			// }

			// Get moves for white via DL_Minimax
			if (game_state.next_turn == 'w')
			{
				new_move = ID_DL_Minimax(game_state);
				// new_move = DL_Minimax_Choice(game_state, 1);
			}

			// // Use a set of given moves for black
			// else
			// {
			// 	new_move = "f3g2";
			// }

			// // Get moves for black randomly
			// else
			// {
			// 	std::vector<std::string> all_black = Generate_Player_Moves(game_state, 'b');
			// 	new_move = Get_Random_Move(all_black);
			// }

			// Get moves for black via DL_Minimax
			else
			{
				new_move = ID_DL_Minimax(game_state);
				// new_move = DL_Minimax_Choice(game_state, 1);
			}
		}

		// // Figure out what the best move is for this state
		// std::string best_move = DL_Minimax_Choice(game_state, 1);
		// std::cout << "The best move for " << game_state.next_turn << " is " << best_move << "\n";

		// Update the game state with the new move
		std::cout << "Player " << game_state.next_turn << " choses move " << new_move << "\n";
		game_state = Simulate_Move(game_state, new_move);
		game_state.Print();

		// if (game_state.next_turn == 'w')
		// {
		// 	std::cout << "Min: " 
		// }

		int score = hValue_Material(game_state);
		std::cout << "Material: " << (score > 0 ? "+" : "") << score << "\n";

		int utility = Utility_Value(game_state);
		if (utility == 1)
			std::cout << "Utility: N/A\n";
		else
			std::cout << "Utility: " << utility << "\n";

		std::cout << "\n\n";

		// Only run a number of turns
		count++;
		if (count/2 >= 300)
		{
			stop = true;
			std::cout << "\nReached the turn limit!\n";
		}
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