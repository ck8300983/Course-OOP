// GamePlay.cpp : Defines the entry point for the console application.
//
// author: Tzai-Shuen Chen
// email: chen.tzai-shuen@wustl.edu
//
// purpose: The main program of game project

#include "stdafx.h"
#include "Lab.h"
#include "game.h"

#include <memory>
using namespace std;

int main(int argc, char* argv[])
{
	try{
		GameBase::chooseGame(argc, argv);
		return GameBase::instance()->play();
	}
	catch (bad_alloc &e){
		cout << "Allocation failed: " << e.what() << endl;
		return badAlloc;
	}
	catch (QuitByUser &e){
		cout << e.what() << endl;
		return quit_by_player;
	}
	catch (NoInstance &e){
		cout << e.what() << endl;
		return no_instance;
	}
	catch (CmdException &e){
		cout << e.what() << endl;
		string cmdInstruction = "Game [-a agent] [-b boardsize] [-c connect]";
		string options = "	Game   the game to play (TicTacToe, Gomoku, or Sudoku)\n";
		options += "	-a     specify the agent for TicTacToe or Gomoku (auto_player)\n";
		options += "	-b     the game board size of Gomoku\n";
		options += "	-c     the requirement of connection to win a Gomoku game";
		return usage(argv[program_name], cmdInstruction, options);
	}
	catch (ChooseGameFail &e){
		cout << e.what() << endl;
		return chooseGameFail;
	}
	catch (ConnectionException &e){
		cout << e.what() << endl;
		return requiredConnectionError;
	}
	catch (ReadFileException &e){
		cout << e.what() << endl;
		return initializedGameFail;
	}
	catch (...){
		cout << "caught exception" << endl;
		return unknown_exception;
	}
}