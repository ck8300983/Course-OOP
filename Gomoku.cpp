// Gomoku.cpp : Defines the entry point for the console application.
//
// author: Tzai-Shuen Chen
// email: chen.tzai-shuen@wustl.edu
//
// purpose: define methods of GomokuGame class


#include "stdafx.h"
#include "game.h"
#include "Gomoku.h"
#include "Lab.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <time.h>
#include <unordered_set>



// Constructor of GomokuGame class. This would create a game with (width*height) number of game pieces
GomokuGame::GomokuGame(int auto_player, const unsigned int width, const unsigned int height, const unsigned int connect){

	try{

		ifstream infile("Gomoku");
		if (!infile.is_open()) {
			infile.clear();
			throw ReadFileException("Open input file fail!");
		}

		for (unsigned i = 0; i < (width + 1)*(height + 1); ++i){
			gamestate_.game_pieces_.push_back(Game_piece(no_color, "", " "));
		}
		load("Gomoku");
	}
	catch (ReadDimensionException &e){
		cout << e.what() << endl;
		cout << "Start a new game" << endl;
		initGomoku(auto_player, width, height, connect);
	}
	catch (ReadFileException &e){
		cout << e.what() << endl;
		cout << "Start a new game" << endl;
		initGomoku(auto_player, width, height, connect);
	}
	catch (NoPieceException &e){
		cout << e.what() << endl;
		cout << "Start a new game" << endl;
		initGomoku(auto_player, width, height, connect);
	}
	catch (...){ // start a new game if load methog throw any error
		cout << "Unknown error. Start a new game" << endl;
		initGomoku(auto_player, width, height, connect);
	}
}

// Set up the initial Gomoku game
void GomokuGame::initGomoku(int& auto_player, const unsigned int& width, const unsigned int& height, const unsigned int& connect){
	board_width_ = width;
	board_height_ = height;
	connect_ = connect;
	display_length_ = static_cast<unsigned int>(log10(board_height_)) + 1;
	gameName_ = "Gomoku";

	for (unsigned i = 0; i < (width + 1)*(height + 1); ++i){
		gamestate_.game_pieces_.push_back(Game_piece(no_color, "", " "));
	}

	for (unsigned i = 1; i <= width; ++i){
		for (unsigned j = 1; j <= height; ++j){
			gamestate_.valid_actions_.push_back(pair<unsigned int, unsigned int>(i, j));
		}
	}
	gamestate_.player_ = no_player;

	player_type_.insert({ player1, "keyboard" });
	if (auto_player){
		player_type_.insert({ last_player, "auto_player" });
	}
	else{
		player_type_.insert({ last_player, "keyboard" });
	}
	tokens_ = { " ", "B", "W" };
}

// Insertion operator of Gomoku game
ostream & operator<<(ostream& output, const GomokuGame & game){

	unsigned int display_length = game.display_length_;
	const vector<Game_piece> & piece = game.gamestate_.game_pieces_;

	// Print out the game from up to down, from left to right
	// The coordinate of Gomoku is displayed as 1 to width (height) instead of 0 to width - 1 (height - 1)
	for (unsigned int y_coordinate = game.board_height_ + 1; y_coordinate-- > 0;){

		if (y_coordinate > 0){

			for (unsigned int x_coordinate = 0; x_coordinate < game.board_width_ + 1; ++x_coordinate){

				if (x_coordinate == 0){ // print out the vertical coordinate axis
					output.fill(' ');
					output.width(display_length + 1);
					output << left << y_coordinate;
				}
				else{
					output.fill(' ');
					output.width(display_length + 1);
					output << left << piece[game.board_width_*y_coordinate + x_coordinate].display_;
				}

			}

		}
		else{

			// print out the horizontal coordinate axis
			for (unsigned int x_coordinate = 0; x_coordinate < game.board_width_ + 1; ++x_coordinate){

				if (x_coordinate == 0){
					output.fill(' ');
					output.width(display_length + 1);
					output << left << "X";
				}
				else{
					output.fill(' ');
					output.width(display_length + 1);
					output << left << x_coordinate;
				}

			}
		} // end of (if v_index > 0)

		output << endl;

	} // end of vertical index loop

	return output;
}

void GomokuGame::print(){
	cout << *this << endl;
}

// Helper function to decide whether a player win a Gomoku game
bool GomokuGame::done(){
	return done(gamestate_);
}

// Helper function to decide whether a player win a Gomoku game
bool GomokuGame::done(const GameState& gameState){

	// Let the current action (x, y) be the center, we search eight directions: 
	// east, west, north, south, southwest, northeast, northwest, and southeast to
	// check whether the current action win a game or not
	pair<int, int> horizontal(1, 0);
	pair<int, int> vertical(0, 1);
	pair<int, int> diagonal(1, 1);
	pair<int, int> antidiag(1, -1);
	int connect = static_cast<int>(connect_);

	if (count_connect(gameState, horizontal) >= connect){
		return true;
	}
	if (count_connect(gameState, vertical) >= connect){
		return true;
	}
	if (count_connect(gameState, diagonal) >= connect){
		return true;
	}
	if (count_connect(gameState, antidiag) >= connect){
		return true;
	}

	return false;

}

// Helper function to count the consecutive token of a player. 
// This function could be used to check whether a player wins or to report possilbe threats
int GomokuGame::count_connect(const GameState& gameState, pair<int, int> direct, int AI){

	// The game piece with coordinate (h_index, v_index) is stored in a vector<Game_piece> 
	// by using the formula width*v_index + h_index to index it.
	const vector<Game_piece>& pieces = gameState.game_pieces_;
	unsigned int x = gameState.action_.first;
	unsigned int y = gameState.action_.second;
	unsigned int player = gameState.player_;

	string  display = tokens_[player];
	int height = static_cast<int>(board_height_);
	int width = static_cast<int>(board_height_);
	unsigned int connect = 1;
	int colum = static_cast<int>(x)+direct.first;
	int row = static_cast<int>(y)+direct.second;
	int emptySpace_forward = 0;
	int emptySpace_backward = 0;

	// Note: the board here is from 1 to width (height)
	while (row > 0 && row <= height && colum > 0 && colum <= width){

		unsigned int read_pieces = static_cast<unsigned int>(width*row + colum);
		if (pieces[read_pieces].display_ != display){

			// If there is an auto player, then we want to check two more space ahead
			// and use this information to identify a "three", "frour", or "straight fout"
			if (AI && emptySpace_forward < 2){
				if (pieces[read_pieces].display_ == " "){
					++emptySpace_forward;
				}
				else{
					break;
				} // break if a line is stopped by opponent's token
			}
			else{
				break;
			}
		}
		else if (emptySpace_forward < 1){
			++connect; // increase connect only if a line is consecutive
		}
		colum += direct.first;
		row += direct.second;
	}

	// search opposite direction
	colum = static_cast<int>(x)-direct.first;
	row = static_cast<int>(y)-direct.second;

	while (row > 0 && row <= height && colum > 0 && colum <= width){

		unsigned int read_pieces = static_cast<unsigned int>(width*row + colum);
		if (pieces[read_pieces].display_ != display){
			if (AI && emptySpace_backward < 2){
				if (pieces[read_pieces].display_ == " "){
					++emptySpace_backward;
				}
				else{
					break;
				}
			}
			else{
				break;
			}
		}
		else if (emptySpace_backward < 1){
			++connect;
		}
		colum -= direct.first;
		row -= direct.second;

	}

	if (AI){
		if (connect == (connect_ - 2) && ((emptySpace_backward + emptySpace_forward) > 2)){ return connectInTwoSteps; }
		if (connect == (connect_ - 1) && (emptySpace_backward > 0) && (emptySpace_forward > 0)){
			return connectForSure;
		}
		else if (connect == (connect_ - 1) && ((emptySpace_backward > 0) || (emptySpace_forward) > 0)){
			return connectInOneStep;
		}
	}

	return static_cast<int>(connect);

}

bool GomokuGame::stalemate(){

	if (done() || gamestate_.valid_actions_.size() > 0){
		return false;
	}
	else{
		return true;
	}
}

// Helper function to decide whether a game is draw
bool GomokuGame::stalemate(const GameState& gameState){

	if (done(gameState) || gameState.valid_actions_.size() > 0){
		return false;
	}
	else{
		return true;
	}
}


int GomokuGame::turn(){

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
int GomokuGame::evaluationGameState(const GameState& gameState){

	pair<int, int> horizontal(1, 0);
	pair<int, int> vertical(0, 1);
	pair<int, int> diagonal(1, 1);
	pair<int, int> antidiag(1, -1);

	int score = 0;
	int AI = 1;
	int result_h = count_connect(gameState, horizontal, AI);
	int result_v = count_connect(gameState, vertical, AI);
	int result_d = count_connect(gameState, diagonal, AI);
	int result_a = count_connect(gameState, antidiag, AI);

	bool loseForSure = (result_h == connectForSure || result_v == connectForSure ||
		result_d == connectForSure || result_a == connectForSure);
	int OneStepToLose = (result_h == connectInOneStep) + (result_v == connectInOneStep) +
		(result_d == connectInOneStep) + (result_a == connectInOneStep);
	int TowStepsToLose = (result_h == connectInTwoSteps) + (result_v == connectInTwoSteps) +
		(result_d == connectInTwoSteps) + (result_a == connectInTwoSteps);

	if (loseForSure || OneStepToLose > 1){
		score += game_result::lose; // lose for sure
	}
	if (TowStepsToLose > 1 || (OneStepToLose > 0 && TowStepsToLose > 0)){
		score += (game_result::lose) / 2; // going to lose in next two step
	}
	if (OneStepToLose == 1){
		score += (game_result::lose) / 3;
	}
	if (TowStepsToLose == 1){
		score += (game_result::lose) / 4;
	}

	score += static_cast<int>(gameState.move_history_.size());
	return score;
}
