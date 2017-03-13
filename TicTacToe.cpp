// TicTacToe.cpp : Defines the entry point for the console application.
//
// author: Tzai-Shuen Chen
// email: chen.tzai-shuen@wustl.edu
//
// purpose: define methods of TicTacToeGame class

#include "stdafx.h"
#include "game.h"
#include "Lab.h"
#include "TicTacToe.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <time.h>
#include <unordered_set>

// Constructor of TicTacToeGame class. This would create a game with (width*height) number of game pieces
TicTacToeGame::TicTacToeGame(int auto_player, const unsigned int width, const unsigned int height, const unsigned int connect){

	try{

		ifstream infile("TicTacToe");
		if (!infile.is_open()) {
			infile.clear();
			throw ReadFileException("Open input file fail!");
		}

		for (unsigned i = 0; i < width*height; ++i){
			gamestate_.game_pieces_.push_back(Game_piece(no_color, "", " "));
		}
		load("TicTacToe");
	}
	catch (ReadDimensionException &e){
		cout << e.what() << endl;
		cout << "Start a new game" << endl;
		initTicTacToe(auto_player, width, height, connect);
	}
	catch (ReadFileException &e){
		cout << e.what() << endl;
		cout << "Start a new game" << endl;
		initTicTacToe(auto_player, width, height, connect);
	}
	catch (NoPieceException &e){
		cout << e.what() << endl;
		cout << "Start a new game" << endl;
		initTicTacToe(auto_player, width, height, connect);
	}
	catch (...){ // start a new game if load methog throw any error
		cout << "Unknown error. Start a new game" << endl;
		initTicTacToe(auto_player, width, height, connect);
	}
}

void TicTacToeGame::initTicTacToe(int& auto_player, const unsigned int& width, const unsigned int& height, const unsigned int& connect){
	board_width_ = width;
	board_height_ = height;
	connect_ = connect;
	display_length_ = static_cast<unsigned int>(log10(board_height_)) + 1;
	gameName_ = "TicTacToe";

	for (unsigned i = 0; i < width*height; ++i){
		gamestate_.game_pieces_.push_back(Game_piece(no_color, "", " "));
	}

	for (unsigned i = 1; i < 4; ++i){
		for (unsigned j = 1; j < 4; ++j){
			gamestate_.valid_actions_.push_back(pair<unsigned int, unsigned int>(i, j));
		}
	}
	gamestate_.player_ = no_player;

	// The default auto player is the last player. Chagne the following line 
	// to set auto player to other player.
	player_type_.insert({ player1, "keyboard" });
	if (auto_player){
		player_type_.insert({ last_player, "auto_player" });
	}
	else{
		player_type_.insert({ last_player, "keyboard" });
	}
	tokens_ = { " ", "X", "O" };

}

// Insertion operator of TicTacToe game
ostream & operator<<(ostream& output, const TicTacToeGame & game){

	unsigned int v_index;
	unsigned int h_index;
	unsigned int display_length = game.display_length_;
	const vector<Game_piece> & piece = game.gamestate_.game_pieces_;

	// Print out the game from up to down, from left to right
	for (unsigned int y_coordinate = game.board_height_ + 1; y_coordinate-- > 0;){

		if (y_coordinate > 0){

			v_index = y_coordinate - 1;
			for (unsigned int x_coordinate = 0; x_coordinate < game.board_width_ + 1; ++x_coordinate){

				if (x_coordinate == 0){ // print out the vertical coordinate axis
					output << v_index << " ";
				}
				else{
					h_index = x_coordinate - 1;
					output.fill(' ');
					output.width(display_length + 1);
					output << left << piece[game.board_width_*v_index + h_index].display_;
				}

			}

		}
		else{

			// print out the horizontal coordinate axis
			for (unsigned int x_coordinate = 0; x_coordinate < game.board_width_ + 1; ++x_coordinate){

				if (x_coordinate == 0){
					output << " " << " ";
				}
				else{
					h_index = x_coordinate - 1;
					output << h_index
						<< setw(display_length) << " ";
				}

			}
		} // end of (if v_index > 0)

		output << endl;

	} // end of vertical index loop

	return output;
}

void TicTacToeGame::print(){
	cout << *this << endl;
}

// Helper function to decide whether a player win a Tic-Tac-Toe game
bool TicTacToeGame::done(){
	return done(gamestate_);
}

// Helper function to decide whether a player win a Tic-Tac-Toe game
/* In order to implement auto player with alpha beta pruning,
* the done() method has to take gameState as input for the auto player
* to evaluate the result of each possible action
*/
bool TicTacToeGame::done(const GameState& gameState){

	// The game piece with coordinate (h_index, v_index) is stored in a vector<Game_piece> 
	// by using the formula width*v_index + h_index to index it.
	const vector<Game_piece>& game_pieces_ = gameState.game_pieces_;

	const Game_piece& piece_11 = game_pieces_[board_width_ * 1 + 1];
	const Game_piece& piece_12 = game_pieces_[board_width_ * 2 + 1];
	const Game_piece& piece_13 = game_pieces_[board_width_ * 3 + 1];
	const Game_piece& piece_21 = game_pieces_[board_width_ * 1 + 2];
	const Game_piece& piece_22 = game_pieces_[board_width_ * 2 + 2];
	const Game_piece& piece_23 = game_pieces_[board_width_ * 3 + 2];
	const Game_piece& piece_31 = game_pieces_[board_width_ * 1 + 3];
	const Game_piece& piece_32 = game_pieces_[board_width_ * 2 + 3];
	const Game_piece& piece_33 = game_pieces_[board_width_ * 3 + 3];

	// List all eight cases that a tic-tac-toe game has a winner
	bool win1 = piece_11.display_ != " " &&  piece_11.display_ == piece_21.display_ &&
		piece_21.display_ == piece_31.display_;
	bool win2 = piece_12.display_ != " " && piece_12.display_ == piece_22.display_ &&
		piece_22.display_ == piece_32.display_;
	bool win3 = piece_13.display_ != " " && piece_13.display_ == piece_23.display_ &&
		piece_23.display_ == piece_33.display_;
	bool win4 = piece_11.display_ != " " && piece_11.display_ == piece_12.display_ &&
		piece_12.display_ == piece_13.display_;
	bool win5 = piece_21.display_ != " " && piece_21.display_ == piece_22.display_ &&
		piece_22.display_ == piece_23.display_;
	bool win6 = piece_31.display_ != " " && piece_31.display_ == piece_32.display_ &&
		piece_32.display_ == piece_33.display_;
	bool win7 = piece_11.display_ != " " && piece_11.display_ == piece_22.display_ &&
		piece_22.display_ == piece_33.display_;
	bool win8 = piece_13.display_ != " " && piece_13.display_ == piece_22.display_ &&
		piece_22.display_ == piece_31.display_;

	if (win1 || win2 || win3 || win4 || win5 || win6 || win7 || win8){
		return true;
	}
	else{
		return false;
	}

}

// Helper function to decide whether a game is draw
bool TicTacToeGame::stalemate(){

	if (done() || gamestate_.valid_actions_.size() > 0){
		return false;
	}
	else{
		return true;
	}
}

// Helper function to decide whether a game is draw
bool TicTacToeGame::stalemate(const GameState& gameState){

	if (done(gameState) || gameState.valid_actions_.size() > 0){
		return false;
	}
	else{
		return true;
	}
}


int TicTacToeGame::turn(){

	unsigned int x_pos = 0;
	unsigned int y_pos = 0;
	unsigned int player = next_player(gamestate_.player_);

	if (player_type_[player] == "auto_player"){
		auto_player(x_pos, y_pos);
	}
	else{
		prompt(x_pos, y_pos);
	}

	// If the player input a valid action, then update game_pieces_, move_history_, valid_actions_, player_, and action_
	gamestate_.game_pieces_[board_width_*y_pos + x_pos] =
		Game_piece(no_color, player_type_[player], tokens_[player], x_pos, y_pos);
	gamestate_.move_history_.push_back(gamestate_.game_pieces_[board_width_*y_pos + x_pos]);
	pair<unsigned int, unsigned int> move(x_pos, y_pos);
	auto ite = find(gamestate_.valid_actions_.begin(), gamestate_.valid_actions_.end(), move);
	gamestate_.valid_actions_.erase(ite);
	gamestate_.player_ = player;
	gamestate_.action_ = move;
	display_length_ = max(display_length_, tokens_[player].size());

	// Print the current game and the action history of current player
	print();
	cout << "Player " << tokens_[player] << ": ";

	for (auto ite = gamestate_.move_history_.begin(); ite != gamestate_.move_history_.end(); ++ite){
		if (ite->display_ == tokens_[player]){
			cout << ite->x_pos_ << ", " << ite->y_pos_ << "; ";
		}
	}

	cout << endl;
	return success;

}

// Helper function to evaluation the game state for auto player using alpha beta pruning
int TicTacToeGame::evaluationGameState(const GameState&){
	return game_result::tie;
}
