// game.h
//
// author: Tzai-Shuen Chen
// email: chen.tzai-shuen@wustl.edu
//
// purpose: declaration and definition about features related to a game

#ifndef GAME_H
#define GAME_H

#include "Lab.h"

#include <iostream>
#include <map>
#include <memory>
#include <string>

using namespace std;

enum piece_color { no_color, red, black, white, invalid_color = -1 };
enum players { no_player, player1, last_player };

struct Game_piece{

	Game_piece(piece_color color, string name, string display, unsigned int x = 0, unsigned int y = 0) :
	color_(color), piece_name_(name), display_(display), x_pos_(x), y_pos_(y){}

	piece_color color_;
	string piece_name_;
	string display_;
	unsigned int x_pos_;
	unsigned int y_pos_;
};

struct GameState{
	GameState(){}
	GameState(vector<Game_piece> piece, vector<Game_piece> history, vector<pair<unsigned int, unsigned int>> valid_actions,
		unsigned int player, pair<unsigned int, unsigned int> action) :
		game_pieces_(piece), move_history_(history), valid_actions_(valid_actions), player_(player),
		action_(action) {}

	pair<unsigned int, unsigned int> action_;
	vector<Game_piece> game_pieces_;
	vector<Game_piece> move_history_;
	unsigned int player_;
	vector<pair<unsigned int, unsigned int>> valid_actions_;	
};

class GameBase{
protected:
	unsigned int board_width_;
	unsigned int board_height_;
	unsigned int connect_;
	unsigned int display_length_ = 1;
	string gameName_;
	static shared_ptr<GameBase> gamePtr_;
	GameState gamestate_;	
	map<unsigned int, string> player_type_;
	vector<string> tokens_;

public:
	GameBase(){}
	virtual ~GameBase(){}
	virtual void print() = 0;
	virtual bool done() = 0;
	virtual bool stalemate() = 0;
	// Overload done() and stalemaet() so that auto_player could evaluate its strategy 
	virtual bool done(const GameState& gameState) = 0;
	virtual bool stalemate(const GameState& gameState) = 0;
	virtual void prompt(unsigned int & x, unsigned int & y);
	virtual void prompt(unsigned int & x, unsigned int & y, unsigned int& digit);
	virtual int turn() = 0;
	int play();
	static void chooseGame(int argc, char * argv[]);
	static shared_ptr<GameBase> instance();

	bool is_valid(unsigned int & x, unsigned int & y);
	unsigned int next_player(unsigned int current_player);
	int auto_player(unsigned int & x, unsigned int & y);
	pair<unsigned int, unsigned int> alphaBetaValue(GameState& gameState, unsigned int opponent_num, int max_depth);
	int maxValue(GameState& gameState, unsigned int opponent_num, int depth, int alpha, int beta, int max_depth);
	int minValue(GameState& gameState, unsigned int opponent_num, int depth, int alpha, int beta, int max_depth);
	GameState generateSuccessor(GameState gameState, unsigned int player, pair<unsigned int, unsigned int>action);
	virtual int evaluationGameState(const GameState& gameState) = 0;
	virtual void save(string ans);
	virtual void load(string game);
};

// Function prototypes
int read_board_dim(ifstream & input_file, unsigned int& h_pos, unsigned int & v_pos);
int read_game_piece(ifstream & input_file, GameState & gamestate, unsigned int width, unsigned int height);

// Game exceptions
class QuitByUser : public std::runtime_error{
public:
	QuitByUser(string err_msg) :runtime_error(err_msg){}
};

class NoInstance : public std::runtime_error{
public:
	NoInstance(string err_msg) :runtime_error(err_msg){}
};

class ChooseGameFail : public std::runtime_error{
public:
	ChooseGameFail(string err_msg) :runtime_error(err_msg){}
};

class CmdException : public std::runtime_error{
public:
	CmdException(string err_msg) :runtime_error(err_msg){}
};


#endif