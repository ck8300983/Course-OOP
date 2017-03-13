// Sudoku.h
//
// author: Tzai-Shuen Chen
// email: chen.tzai-shuen@wustl.edu
//
// purpose: declaration and definition about features related 
// to a Sudoku game

#ifndef SUDOKU_H
#define SUDOKU_H

#include "game.h"

class SudokuGame : public GameBase{
private:
	unsigned int boxSize_;

public:
	SudokuGame();
	void initSudoku();
	virtual ~SudokuGame(){}
	friend ostream & operator<<(ostream& output, const SudokuGame & game);
	virtual void print();
	virtual bool done();
	virtual bool done(const GameState& gameState);
	virtual int turn();
	virtual bool stalemate();
	virtual bool stalemate(const GameState& gameState);
	virtual int evaluationGameState(const GameState& gameState);
};

// The insersion operator of SudokuGame class
ostream & operator<<(ostream& output, const SudokuGame & game);

#endif