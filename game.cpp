// game.cpp : Defines the entry point for the console application.
//
// author: Tzai-Shuen Chen
// email: chen.tzai-shuen@wustl.edu
//
// purpose: define methods of game class


#include "stdafx.h"
#include "game.h"
#include "Lab.h"
#include "TicTacToe.h"
#include "Gomoku.h"
#include "Sudoku.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <time.h>
#include <unordered_set>

using namespace std;

shared_ptr<GameBase> GameBase::gamePtr_;
shared_ptr<GameBase> GameBase::instance() {
	if (gamePtr_.get() == nullptr){
		throw NoInstance("No instance exists!");
	}
	return gamePtr_;
}

// Helper function to decide whether an action is valid or not. If the action is valid,
// return true and erase it from gamestate's valid_actions_
bool GameBase::is_valid(unsigned int & x, unsigned int & y) {
	pair<unsigned int, unsigned int> move(x, y);

	auto ite = find(gamestate_.valid_actions_.begin(), gamestate_.valid_actions_.end(), move);

	if (ite != gamestate_.valid_actions_.end()) {
		return true;
	}
	else{
		return false;
	}
}


// Helper function to prompt the human player to input an action (coordinate)
void GameBase::prompt(unsigned int & x, unsigned int & y) {

	string input;
	int invalid_input = 1;

	while (invalid_input){

		cout << "Please enter a coordinate to place your token or enter 'quit' to exit the game: ";
		cin >> input;

		if (input == "quit"){
			cout << "Save the game? y/N" << endl;
			string ans;
			cin >> ans;
			save(ans);		
			throw QuitByUser("Quit by user!");
		}
		else{

			// convert coordinate "x, y" into string "x y"
			size_t comma = input.find(",");
			if (comma != string::npos){
				input.replace(comma, 1, " ");
			}

			istringstream in_coordinate(input);
			if (in_coordinate >> x >> y && is_valid(x, y)){
				invalid_input = 0;
			}
			else{
				cout << "Invalid coordinate! Please try again" << endl;
			}
		}
	} //end of while

}

// Helper function to prompt the player to input a coordinate and a number
void GameBase::prompt(unsigned int & x, unsigned int & y, unsigned int& digit){

	string input;
	int invalid_input = 1;

	while (invalid_input){

		cout << "Please enter a coordinate and a digit, separated by comma, such as 0,0,9" << endl;
		cout << "or enter 'quit' to exit the game: ";
		cin >> input;

		if (input == "quit"){
			cout << "Save the game? y/N" << endl;
			string ans;
			cin >> ans;
			save(ans);
			throw QuitByUser("Quit by user!");
		}
		else{

			// convert coordinate "x, y, digit" into string "x y digit"
			size_t comma = input.find(",");
			if (comma != string::npos){
				input.replace(comma, 1, " ");
			}
			comma = input.find(",");
			if (comma != string::npos){
				input.replace(comma, 1, " ");
			}

			istringstream in_coordinate(input);
			if (in_coordinate >> x >> y >> digit && is_valid(x, y) && (digit >= 1 && digit <= 9)){
				invalid_input = 0;
			}
			else{
				cout << "Invalid coordinate! Please try again" << endl;
			}
		}
	} //end of while

}

// Helper function to save a game
void GameBase::save(string ans){

	ofstream outfile(gameName_);

	if (outfile.is_open()){

		if (ans == "Y" || ans == "y"){

			// save board_height_, board_width_, connect_, and display_length_
			outfile << gameName_ << "\n"
				<< board_height_ << " " << board_width_ << " //board_size"<< "\n"
				<< connect_ << " //connect_requirement" << "\n"
				<< display_length_ << " //display_length" << "\n";

			// save player type
			for (auto ite = player_type_.begin(); ite != player_type_.end(); ++ite){
				outfile << ite->first << " " << ite->second << " ";
			}
			outfile << " //player_types"<< "\n";

			// save token
			copy(tokens_.begin(), tokens_.end(), ostream_iterator<string>(outfile, " "));
			outfile << " //tokens, the first one is space" << "\n";
				   
			// save last action and last player
			outfile << gamestate_.action_.first << " " << gamestate_.action_.second << " //last_action \n"
				<< gamestate_.player_ << " //last_player" <<"\n";

			// save valid actions
			outfile << " //valid_actions:\n";
			for (auto ite = gamestate_.valid_actions_.begin(); ite != gamestate_.valid_actions_.end(); ++ite){
				outfile << ite->first << " " << ite->second << "\n";
			}

			// save move history
			outfile << " //move_history:\n";
			for (auto ite = gamestate_.move_history_.begin(); ite != gamestate_.move_history_.end(); ++ite){
				outfile << ite->piece_name_ << " " << ite->display_ << " "
					<< ite->x_pos_ << " " << ite->y_pos_ << "\n";
			}
			outfile.close();
		}
		else{
			outfile << "No Data";
			outfile.close();
		}
	}
	else{ 
		throw ReadFileException("Open output file fail!"); 
	}
}

// Helper function to load a game
void GameBase::load(string game){

	ifstream infile(game);
	int retrieveSuccess = 0;
	int gameParamNum = 10;

	if (!infile.is_open()) {
		infile.clear();
		throw ReadFileException("Open input file fail!");
	}

	string line;
	string tmpStr;
	getline(infile, line);
	istringstream temp(line);
	temp >> tmpStr;
	if (tmpStr != "TicTacToe" && tmpStr != "Gomoku" && tmpStr != "Sudoku"){
		throw ReadFileException("No saved file");
	}
	gameName_ = tmpStr;
	++retrieveSuccess;

	// Read the game board dimension. If the dimension is not stored in the first line, then read the following
	// lines until getting the width and height. If there is no such information through the whole file, throw
	// an ReadDimensionException.
	int read_index = read_board_dim(infile, board_width_, board_height_);
	while (read_index == board_dimension_failure){
		read_index = read_board_dim(infile, board_width_, board_height_);
	}
	++retrieveSuccess;

	// Read connection requirement
	if (getline(infile, line)){
		temp.clear();
		temp.str(line);			
		if (temp >> connect_ >> tmpStr && tmpStr =="//connect_requirement"){
			++retrieveSuccess;
		}
	}
	
	// Read display length
	if (getline(infile, line)){
		temp.clear();
		temp.str(line);			
		if (temp >> display_length_ >> tmpStr && tmpStr == "//display_length"){
			++retrieveSuccess;
		}
	}

	// Read player type
	unsigned int tmpInt;
	if (getline(infile, line)){
		temp.clear();
		temp.str(line);
		while (temp >> tmpInt >> tmpStr){
			player_type_[tmpInt] = tmpStr;
		}
		++retrieveSuccess;
	}

	// Read game tokens
	tokens_.push_back(" ");
	if (getline(infile, line)){
		temp.clear();
		temp.str(line);
		while (temp >> tmpStr && *(tmpStr.begin()) != '/'){
			tokens_.push_back(tmpStr);
		}
		++retrieveSuccess;
	}
	
	// Read last action
	unsigned int tmpInt2;
	if (getline(infile, line)){
		temp.clear();
		temp.str(line);
		if (temp >> tmpInt >> tmpInt2 >> tmpStr && tmpStr =="//last_action"){
			pair<unsigned int, unsigned int> move(tmpInt, tmpInt2);
			gamestate_.action_ = move;
			++retrieveSuccess;
		}
	}
	
	// Read last playertT
	if (getline(infile, line)){
		temp.clear();
		temp.str(line);
		if (temp >> gamestate_.player_ >> tmpStr && tmpStr == "//last_player"){
			++retrieveSuccess;
		}
	}
	
	// Read valid actions
	if (getline(infile, line)){

		temp.clear();
		temp.str(line);
		if (temp >> tmpStr && tmpStr == "//valid_actions:"){
			while (getline(infile, line)){

				temp.clear();
				temp.str(line);

				if (temp >> tmpInt >> tmpInt2){
					pair<unsigned int, unsigned int> move(tmpInt, tmpInt2);
					gamestate_.valid_actions_.push_back(move);
				}
				else{ 
					break;
				}
			}
			++retrieveSuccess;
		}
	}
		
	// Read the game pieces.
	read_index = read_game_piece(infile, gamestate_, board_width_, board_height_);
	++retrieveSuccess;
	infile.close();

	if (retrieveSuccess != gameParamNum){
		throw ReadFileException("error: parameters do not match!");
	}
}

// The main procedure to play a game
int GameBase::play(){

	// Print the game board when a game begins
	print();

	int return_value = 0;
	bool isDone = 0;
	bool isStalemate = 0;
	while (!isDone && !isStalemate){
		return_value = turn();
		isDone = done();
		isStalemate = stalemate();
	}

	//Clear the saved file
	ofstream outfile(gameName_);
	if (outfile.is_open()){
		outfile << "No Data";
		outfile.close();
	}
	else{
		throw ReadFileException("Open output file fail!"); 
	}

	if (isDone){
		cout << "Player " << (gamestate_.move_history_.end() - 1)->display_ << " wins!" << endl;
		return success;
	}
	else{
		cout << "Draw! " << gamestate_.move_history_.size() << " turns were played. No winning moves remain." << endl;
		return no_winner;
	}

}

// The method would decide which game to play
/* If there is more than two arguments, then the program would check
* the following keywords: -a agent, -b gameboard, and -c connect
*/
void GameBase::chooseGame(int argc, char * argv[]){

	if (gamePtr_ != nullptr){
		throw ChooseGameFail("default game pointer is not null!");
	}
	
	// Case1: If less than two arguments, throw an exception
	if (argc < expected_min_argument){
		throw CmdException("Not enough arguments!");
	}

	string playGame = argv[game_name];

	// Case2: If exactly two arguments, dynamically construct a game
	if (argc == expected_min_argument){
		if (playGame == "TicTacToe"){
			gamePtr_ = make_shared<TicTacToeGame>();
		}
		else if (playGame == "Gomoku"){
			gamePtr_ = make_shared<GomokuGame>();
		}
		else if (playGame == "Sudoku"){
			gamePtr_ = make_shared<SudokuGame>();
		}
		else{ 
			throw CmdException("No such game exist!"); 
		}
	}

	//Case3: If there are more than two arguments, check the keywords and parameters
	if (argc > expected_min_argument){
		int keyword_index = expected_min_argument;
		int parameter = keyword_index;
		int auto_player = 0;
		int board_size = 19;
		int connect_requirement = 5;

		while (keyword_index + 1 <= argc){

			parameter = keyword_index + 1;
			if (strcmp(argv[keyword_index], "-a") == 0){

				// If there is no parameter after keyword or parameter is not valid
				if (parameter + 1 > argc || strcmp(argv[parameter], "auto_player") != 0){
					throw CmdException("Keyword grammar is not correct!");
				}
				else{
					auto_player = 1;
				}
			}
			else if (strcmp(argv[keyword_index], "-b") == 0){

				if (parameter + 1 > argc){
					throw CmdException("Keyword grammar is not correct!");
				}
				else{
					istringstream inString(argv[parameter]);
					inString >> board_size;
					if (board_size <= 0){
						throw ReadDimensionException("Board size could not be less than 1!");
					}
				}
			}
			else if (strcmp(argv[keyword_index], "-c") == 0){

				if (parameter + 1 > argc){
					throw CmdException("Keyword grammar is not correct!");
				}
				else{
					istringstream inString(argv[parameter]);
					inString >> connect_requirement;
					if (connect_requirement < 3){
						throw ConnectionException("Connection number has to be larger than three!");
					}
				}
			}
			else{
				throw CmdException("Keyword grammar is not correct!");
			}
			keyword_index = parameter + 1;
		}

		if (playGame == "TicTacToe"){
			board_size = 5;
			gamePtr_ = make_shared<TicTacToeGame>(auto_player, board_size, board_size);
		}
		else if (playGame == "Gomoku"){
			gamePtr_ = make_shared<GomokuGame>(auto_player, board_size, board_size, connect_requirement);
		}
		else{ 
			throw CmdException("No such game exist!"); 
		}
	}
}

// Decide who is next player
unsigned int GameBase::next_player(unsigned int current_player){

	if (current_player < last_player){
		++current_player;
	}
	else{
		current_player = player1;
	}
	return current_player;

}


// Helper funciton to paly an action by auto player instead of prompting player to input an action
int GameBase::auto_player(unsigned int & x, unsigned int & y){

	unsigned int opponent_num = last_player - 1;
	GameState gameState = gamestate_;
	pair<unsigned int, unsigned int> bestAction;
	vector<pair<unsigned int, unsigned int>> valid_actions = gamestate_.valid_actions_;
	int max_depth = 1;
	unsigned int threshold1 = 8;
	unsigned int threshold2 = 15;

	// In order to reduce the search range for a huge game, the program would divide the game phase into two periods.
	// In the first phase, the auto player would only search for best action around previous actions.
	// The program would search from all valid actions in the second phase.
	unsigned int numberOfTurns = (board_height_ * board_width_ - gamestate_.valid_actions_.size()) / 2 + 1;
	unsigned int firstPhase = (board_height_ * board_width_) / 4;

	vector<pair<unsigned int, unsigned int>> keep;
	unsigned int zero = 0;

	if (numberOfTurns < firstPhase){
		for (auto ite = gamestate_.move_history_.begin(); ite != gamestate_.move_history_.end(); ++ite){
			vector<pair<unsigned int, unsigned int>> check;
			pair<unsigned int, unsigned int> upleft(max(ite->x_pos_ - 1, zero), min(ite->y_pos_ + 1, board_height_ - 1));
			check.push_back(upleft);
			pair<unsigned int, unsigned int>downleft(max(ite->x_pos_ - 1, zero), max(ite->y_pos_ - 1, zero));
			check.push_back(downleft);
			pair<unsigned int, unsigned int>upright(min(ite->x_pos_ + 1, board_width_ - 1), min(ite->y_pos_ + 1, board_height_ - 1));
			check.push_back(upright);
			pair<unsigned int, unsigned int>downright(min(ite->x_pos_ + 1, board_width_ - 1), max(ite->y_pos_ - 1, zero));
			check.push_back(downright);
			pair<unsigned int, unsigned int> left(max(ite->x_pos_ - 1, zero), ite->y_pos_);
			check.push_back(left);
			pair<unsigned int, unsigned int> right(min(ite->x_pos_ + 1, board_width_ - 1), ite->y_pos_);
			check.push_back(right);
			pair<unsigned int, unsigned int> up(ite->x_pos_, min(ite->y_pos_ + 1, board_height_ - 1));
			check.push_back(up);
			pair<unsigned int, unsigned int> down(ite->x_pos_, max(ite->y_pos_ - 1, zero));
			check.push_back(down);

			// Keep the unique positions around previous actions as the candidate actions for auto player
			for (auto ite_ck = check.begin(); ite_ck != check.end(); ++ite_ck){
				if (find(keep.begin(), keep.end(), *ite_ck) == keep.end()){
					keep.push_back(*ite_ck);
				}
			}
		}// end of move history

		gameState.valid_actions_.clear();
		// Keep the valid actions that are around previous movements
		for (auto ite = keep.begin(); ite != keep.end(); ++ite){
			if (find(valid_actions.begin(), valid_actions.end(), *ite) != valid_actions.end()){
				gameState.valid_actions_.push_back(*ite);
			}
		}
	}

	// Change the alpha-beta pruning depth accordint to the number of actions
	if (gameState.valid_actions_.size() > threshold2){
		max_depth = 1;
	}
	else if (gameState.valid_actions_.size() > threshold1){
		max_depth = 2;
	}
	else{
		max_depth = 3;
	}

	bestAction = alphaBetaValue(gameState, opponent_num, max_depth);

	x = bestAction.first;
	y = bestAction.second;

	return success;

}


// Helper function to get the best action through alpha beta pruning algorithm
pair<unsigned int, unsigned int> GameBase::alphaBetaValue(GameState& gameState, unsigned int opponent_num, int max_depth){

	int depth = 1;
	// I set auto player to be the last player. If auto player is the first player, we need to modify this line
	unsigned int agentIndex = last_player;
	GameState successorState;
	int newValue = 0;
	pair<unsigned int, unsigned int> bestAction;

	int alpha = std::numeric_limits<int>::min();
	int beta = std::numeric_limits<int>::max();

	vector<pair<unsigned int, unsigned int>>& valid_actions = gameState.valid_actions_;

	for (auto action = valid_actions.begin(); action != valid_actions.end(); ++action){

		successorState = generateSuccessor(gameState, agentIndex, *action);
		newValue = minValue(successorState, opponent_num, depth, alpha, beta, max_depth);

		if (newValue > alpha){
			alpha = newValue;
			bestAction = *action;
		}
	}

	return bestAction;
}

// Helper function to maximize self's value
int GameBase::maxValue(GameState& gameState, unsigned int opponent_num, int depth, int alpha, int beta, int max_depth){

	if (done(gameState)){
		return game_result::lose + static_cast<int>(gameState.move_history_.size());
	}
	else if (stalemate(gameState)){
		return game_result::tie - static_cast<int>(gameState.move_history_.size());
	}
	else if (depth > max_depth){
		return evaluationGameState(gameState);
	}

	GameState successorState;
	unsigned int current_player = next_player(gameState.player_);
	int newValue = 0;

	vector<pair<unsigned int, unsigned int>>& valid_actions = gameState.valid_actions_;

	// maximizing self value
	int max_val = std::numeric_limits<int>::min();

	for (auto action = valid_actions.begin(); action != valid_actions.end(); ++action){

		successorState = generateSuccessor(gameState, current_player, *action);
		newValue = minValue(successorState, opponent_num, depth, alpha, beta, max_depth);
		if (newValue >= beta){
			return beta;
		}
		else if (newValue > max_val){
			max_val = newValue;
			alpha = max(max_val, alpha);
		}
	}

	return alpha;
}

// Helper function to minimize opponent's value
int GameBase::minValue(GameState& gameState, unsigned int opponent_num, int depth, int alpha, int beta, int max_depth){

	if (done(gameState)){
		return game_result::win *(1 + 1 / (1 + static_cast<int>(gameState.move_history_.size())));
	}
	else if (stalemate(gameState)){
		return game_result::tie - static_cast<int>(gameState.move_history_.size());
	}

	GameState successorState;
	unsigned int current_player = next_player(gameState.player_);

	int newValue = 0;
	vector<pair<unsigned int, unsigned int>>& valid_actions = gameState.valid_actions_;

	//minimizing opponent's value
	int min_val = std::numeric_limits<int>::max();
	++depth;
	for (auto action = valid_actions.begin(); action != valid_actions.end(); ++action){

		successorState = generateSuccessor(gameState, current_player, *action);
		newValue = maxValue(successorState, opponent_num, depth, alpha, beta, max_depth);

		if (newValue <= alpha){
			return alpha;
		}
		else if (newValue < min_val){
			min_val = newValue;
			beta = min(min_val, beta);
		}
	}

	return beta;
}

// Helper function to generate next game state after taking an action
GameState GameBase::generateSuccessor(GameState gameState, unsigned int player, pair<unsigned int, unsigned int>action){

	vector<Game_piece>& pieces = gameState.game_pieces_;
	vector<Game_piece>& history = gameState.move_history_;
	vector<pair<unsigned int, unsigned int>>& valid_actions = gameState.valid_actions_;
	unsigned int x_pos = action.first;
	unsigned int y_pos = action.second;

	// update game state
	pieces[board_width_*y_pos + x_pos] = Game_piece(no_color, player_type_[player], tokens_[player], x_pos, y_pos);
	history.push_back(pieces[board_width_*y_pos + x_pos]);
	auto ite = find(valid_actions.begin(), valid_actions.end(), action);
	valid_actions.erase(ite);

	return GameState(pieces, history, valid_actions, player, action);

}


// Helper function to read the dimension of the game board, which should be defined in the first line of input file
int read_board_dim(ifstream & input_file, unsigned int& h_pos, unsigned int & v_pos){

	string line;
	if (getline(input_file, line)){

		istringstream temp(line);
		if (temp >> h_pos && temp >> v_pos){

			return success;
		}

		return board_dimension_failure;
	}
	else{
		throw ReadDimensionException("error: fail to read the game board dimension from input file");
	}
}

// Helper function to read the game piece, which should be defined from the second line of input file to the end
int read_game_piece(ifstream & input_file, GameState & gamestate, unsigned int width, unsigned int height){

	// extract the info of game pieces and store it in a game_piece vector
	vector<Game_piece> & piece = gamestate.game_pieces_;
	vector<Game_piece> & history = gamestate.move_history_;
	string line;
	string name;
	string display;
	unsigned int h_pos;
	unsigned int v_pos;
	unsigned int piece_index = 0;
	int num_of_piece_read = 0;

	while (getline(input_file, line)){

		istringstream temp(line);

		// If a game piece is well defined, store the information; otherwise, skip this game piece and check next one
		if (temp >> name && temp >> display && temp >> h_pos && temp >> v_pos){

			if (h_pos >= width || v_pos >= height){

				continue;
			}
			else{
				// retrieve game piece
				piece_index = width * v_pos + h_pos;
				piece[piece_index].piece_name_ = name;
				piece[piece_index].display_ = display;
				piece[piece_index].x_pos_ = h_pos;
				piece[piece_index].y_pos_ = v_pos;
				++num_of_piece_read;

				// retrieve move history
				history.push_back(piece[width*v_pos + h_pos]);
			}
		}
		else{

			continue;
		}
	}

	// check whether any game piece is read
	if (num_of_piece_read > 0){

		return success;
	}
	else{

		return no_valid_piece;
	}

}
