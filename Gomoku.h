// Gomoku.h
//
// author: Tzai-Shuen Chen
// email: chen.tzai-shuen@wustl.edu
//
// purpose: declaration and definition about features related 
// to a Gomoku game

#ifndef GOMOKU_H
#define GOMOKU_H

#include "game.h"

class GomokuGame : public GameBase{
private:
	enum threat { connectInTwoSteps = -1, connectInOneStep = -2, connectForSure = -3 };
public:
	GomokuGame(int auto_player = 0, const unsigned int width = 19, const unsigned int height = 19,
		const unsigned int connect = 5);
	void initGomoku(int& auto_player, const unsigned int& width, const unsigned int& height,
		const unsigned int& connect);
	virtual ~GomokuGame(){}
	friend ostream & operator<<(ostream& output, const GomokuGame & game);
	virtual void print();
	virtual bool done();
	virtual bool stalemate();
	virtual bool done(const GameState& gameState);
	virtual bool stalemate(const GameState& gameState);
	virtual int turn();
	int count_connect(const GameState& gameState, pair<int, int> direct, int AI = 0);
	virtual int evaluationGameState(const GameState& gameState);
};

// The insersion operator of GomokuGame class
ostream & operator<<(ostream& output, const GomokuGame & game);

#endif