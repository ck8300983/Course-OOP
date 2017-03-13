// TicTacToe.h
//
// author: Tzai-Shuen Chen
// email: chen.tzai-shuen@wustl.edu
//
// purpose: declaration and definition about features related 
// to a TicTacToe game

#ifndef TICTACTOE_H
#define TICTACTOE_H

#include "game.h"

class TicTacToeGame : public GameBase{
public:
	TicTacToeGame(int auto_player = 0, const unsigned int width = 5, const unsigned int height = 5,
		const unsigned int connect = 3);
	void initTicTacToe(int& auto_player, const unsigned int& width, const unsigned int& height,
		const unsigned int& connect);
	virtual ~TicTacToeGame(){}
	friend ostream & operator<<(ostream& output, const TicTacToeGame & game);
	virtual void print();
	virtual bool done();
	virtual bool stalemate();
	virtual bool done(const GameState& gameState);
	virtual bool stalemate(const GameState& gameState);
	virtual int turn();
	virtual int evaluationGameState(const GameState& gameState);
};

// The insersion operator of TicTacToeGame class
ostream & operator<<(ostream& output, const TicTacToeGame & game);

#endif