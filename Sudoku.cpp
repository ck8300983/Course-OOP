// Sudoku.cpp : Defines the entry point for the console application.
//
// author: Tzai-Shuen Chen
// email: chen.tzai-shuen@wustl.edu
//
// purpose: define methods of SudokuGame class


#include "stdafx.h"
#include "game.h"
#include "Sudoku.h"
#include "Lab.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <time.h>
#include <unordered_set>

// Constructor of SudokuGame class. This would create a game with (width*height) number of game pieces
SudokuGame::SudokuGame(){

	try{
		// Check the existence of Sudoku
		string line;
		string tmpStr;
		ifstream infile("Sudoku");
		if (!infile.is_open()) {
			infile.clear();
			throw ReadFileException("Saved file does not exist!");
		}

		board_width_ = 9;
		board_height_ = 9;
		boxSize_ = 3;
		for (unsigned i = 0; i < board_width_*board_height_; ++i){
			gamestate_.game_pieces_.push_back(Game_piece(no_color, "", " "));
		}

		load("Sudoku");
	}
	catch (ReadDimensionException &e){
		cout << e.what() << endl;
		cout << "Start a new game" << endl;
		initSudoku();
	}
	catch (ReadFileException &e){
		cout << e.what() << endl;
		cout << "Start a new game" << endl;
		initSudoku();
	}
	catch (NoPieceException &e){
		cout << e.what() << endl;
		cout << "Start a new game" << endl;
		initSudoku();
	}
	catch (...){ // start a new game if load methog throw any error
		cout << "Unknown error. Start a new game" << endl;
		initSudoku();
	}
}

// Set up the initial Sudoku game
void SudokuGame::initSudoku(){

	// Check the existence of sudoku0
	string line;
	string tmpStr;
	ifstream infile("sudoku0");

	if (!infile.is_open()) {
		infile.clear();
		throw ReadFileException("sudoku0 does not exist!");
	}

	board_width_ = 9;
	board_height_ = 9;
	boxSize_ = 3;
	connect_ = 0;
	display_length_ = static_cast<unsigned int>(log10(board_height_)) + 1;
	gameName_ = "Sudoku";

	for (unsigned i = 0; i < board_width_*board_height_; ++i){
		gamestate_.game_pieces_.push_back(Game_piece(no_color, "", " "));
	}

	for (unsigned i = 0; i < board_width_; ++i){
		for (unsigned j = 0; j < board_height_; ++j){
			gamestate_.valid_actions_.push_back(pair<unsigned int, unsigned int>(i, j));
		}
	}


	gamestate_.player_ = no_player;
	player_type_.insert({ player1, "keyboard" });
	tokens_ = { " ", "1", "2", "3", "4", "5", "6", "7", "8", "9" };

	// Input the default Sudoku game from sudoku0
	unsigned int y_pos = board_height_;
	int tmpInt = 0;
	string display = "";

	while (getline(infile, line) && y_pos-- > 0){

		istringstream temp(line);
		temp.clear();
		temp.str(line);

		for (unsigned int x_pos = 0; x_pos < board_width_; ++x_pos){

			if (temp >> tmpInt && (tmpInt >= 1 && tmpInt <= 9)){
				display = to_string(tmpInt);
				// update game piece, move history, and valid actions 
				// (Here I add the default game piece to move history)
				gamestate_.game_pieces_[board_width_*y_pos + x_pos] =
					Game_piece(no_color, "default", display, x_pos, y_pos);
				gamestate_.move_history_.push_back(gamestate_.game_pieces_[board_width_*y_pos + x_pos]);
				pair<unsigned int, unsigned int> move(x_pos, y_pos);
				auto ite = find(gamestate_.valid_actions_.begin(), gamestate_.valid_actions_.end(), move);
				gamestate_.valid_actions_.erase(ite);
			}
			else{
				continue;
			}
		}
	}

	if (y_pos != 0){
		throw NoPieceException("No enough game pieces!");
	}
	infile.close();

}

// Insertion operator of Sudoku game
ostream & operator<<(ostream& output, const SudokuGame& game){

	// Print out the game from up to down, from left to right
	unsigned int v_index;
	unsigned int h_index;
	const unsigned int& display_length = game.display_length_;
	const vector<Game_piece> & piece = game.gamestate_.game_pieces_;

	// Print out the game from up to down, from left to right
	for (unsigned int y_coordinate = game.board_height_ + 1; y_coordinate-- > 0;){

		if (y_coordinate > 0){

			v_index = y_coordinate - 1;
			for (unsigned int x_coordinate = 0; x_coordinate < game.board_width_ + 1; ++x_coordinate){

				if (x_coordinate == 0){ // print out the vertical coordinate axis
					output << v_index << "||";
				}
				else{
					h_index = x_coordinate - 1;
					output.fill(' ');
					output.width(display_length + 1);
					output << left << piece[game.board_width_*v_index + h_index].display_ << "|";
					if (h_index % 3 == 2){
						output << "|";
					}
				}
			}
			output << "\n";
			// Print the grid between each line
			for (unsigned int x_coordinate = 0; x_coordinate < game.board_width_ + 1; ++x_coordinate){

				string grid = "-";
				if (v_index % 3 == 0){ grid = "="; }

				if (x_coordinate == 0){
					output << grid << "||";
				}
				else{
					h_index = x_coordinate - 1;
					output.fill(' ');
					output.width(display_length + 1);
					output << left << grid << "|";
					if (h_index % 3 == 2){
						output << "|";
					}
				}
			}
		}
		else{

			// print out the horizontal coordinate axis
			for (unsigned int x_coordinate = 0; x_coordinate < game.board_width_ + 1; ++x_coordinate){

				if (x_coordinate == 0){
					output << "#" << "||";
				}
				else{
					h_index = x_coordinate - 1;
					output.fill(' ');
					output.width(display_length + 1);
					output << left << h_index << "|";
					if (h_index % 3 == 2){
						output << "|";
					}
				}

			}
		} // end of (if v_index > 0)

		output << endl;

	} // end of vertical index loop

	return output;
}

void SudokuGame::print(){
	cout << *this << endl;
}

bool SudokuGame::done(){
	return done(gamestate_);
}

bool SudokuGame::done(const GameState& gameState){

	unordered_set<string> checkList = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	vector<unordered_set<string>> rowList, columList, boxList;
	const vector<Game_piece>& piece = gameState.game_pieces_;

	for (unsigned int i = 0; i < board_height_; ++i){
		rowList.push_back(checkList);
		columList.push_back(checkList);
		boxList.push_back(checkList);
	}

	for (unsigned int height = 0; height < board_height_; ++height){
		for (unsigned int width = 0; width < board_width_; ++width){

			string currentNum = piece[board_width_*height + width].display_;
			unsigned int box = width / boxSize_ + (height / boxSize_)*boxSize_;

			// If there is no number or duplicate number in each row, colum, or box, return false.
			if (!rowList[height].erase(currentNum)){
				return false;
			}
			if (!columList[width].erase(currentNum)){
				return false;
			}
			if (!boxList[box].erase(currentNum)){
				return false;
			}
		}
	}

	return true;
}

int SudokuGame::turn(){

	unsigned int x_pos = 0;
	unsigned int y_pos = 0;
	unsigned int digit = 0;
	prompt(x_pos, y_pos, digit);
	string display = to_string(digit);

	// If the player input a valid action, then update game_pieces_, move_history_, valid_actions_, player_, and action_
	gamestate_.game_pieces_[board_width_*y_pos + x_pos] =
		Game_piece(no_color, player_type_[player1], display, x_pos, y_pos);
	gamestate_.move_history_.push_back(gamestate_.game_pieces_[board_width_*y_pos + x_pos]);
	pair<unsigned int, unsigned int> move(x_pos, y_pos);
	gamestate_.player_ = player1;
	gamestate_.action_ = move;
	display_length_ = max(display_length_, display.size());

	// Print the current game 
	print();
	cout << endl;
	return success;

}

bool SudokuGame::stalemate(){
	return false;
}
bool SudokuGame::stalemate(const GameState&){
	return false;
}

int SudokuGame::evaluationGameState(const GameState&){
	return game_result::tie;
}
