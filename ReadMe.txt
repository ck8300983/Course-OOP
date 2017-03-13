Program: GamePlay
***************************************************************

	Author: Tzai-Shuen chen 
	Email: chen.tzai-shuen@wustl.edu
	Date: December 2, 2015


***************************************************************
-Lab.h: The common header file for all lab exercises. 
	Lab.h include result code enumeration, the prototypes of
        some common functions, and some general exception 
	classes for lab exercise.

-Lab.cpp: 
	The collection of all possible functions that 
        would be used for all lab exercises. 
	Lab.cpp includes "usage" function, 
 	"lowercase" function, etc.

-game.h: 
	Declarations of features related to a game.
        -- Game_piece, a structure to store game piece information.
	-- GameState, a structure to store the information of 
	   current game state, such move history, valid actions,
	   current player, last action being made, etc.
	-- GameBase, an base class of game.
	-- read_board_dim, a function to read game board dimension from input file
	-- read_game_piece, a function to read game pieces from input file
	-- Game level exceptions, such as QuitByUser, NoInstance, ChooseGameFail, etc.
	
-game.cpp: 
	Implementation of game.h, 

-TicTacToe.h
	Declarations of features related to a TicTacToe game.
	-- TicTacToeGame, a derived game class from GameBase.

-TicTacToe.cpp: 
	Implementation of TicTacToe.h, 

-Gomoku.h
	Declarations of features related to a Gomoku game.
	-- GomokuGame, a derived game class from GameBase.

-Gomoku.cpp: 
	Implementation of Gomoku.h, 

-Sudoku.h
	Declarations of features related to a Sudoku game.
	-- SudokuGame, a derived game class from GameBase.

-Sudoku.cpp: 
	Implementation of Sudoku.h, 

-GamePlay.cpp: 
	Main program for game project.


***************************************************************
Copy control features:

	I define the virtual destructor for GameBase, TicTacToeGame,
	GomokuGame, and SudokuGame classes. I do not define any other copy 
	control feature because almost all data members in those classes
        are either structure or build-in type, not pointer. The only 
        poiner in those classes is a static shared pointer, which means
	there is no need to do deep copy when constructing an object.

***************************************************************
-Test:

 < Test for command line arguments >

 (1) command line arguments: GamePlay
     output: 
	GamePlay Usage: Game [-a agent] [-b boardsize] [-c connect]
	Arguments:
           Game   the game to play (TicTacToe, Gomoku, or Sudoku)
           -a     specify the agent for TicTacToe or Gomoku (auto_player)
           -b     the game board size of Gomoku
           -c     the requirement of connection to win a Gomoku game

     return value: 1 (bad_command_line)

 (2) command line arguments:  GamePlay ToeTacTic
     output: 
	No such game exist!
	GamePlay Usage: Game [-a agent] [-b boardsize] [-c connect]
	Arguments:
           Game   the game to play (TicTacToe, Gomoku, or Sudoku)
           -a     specify the agent for TicTacToe or Gomoku (auto_player)
           -b     the game board size of Gomoku
           -c     the requirement of connection to win a Gomoku game

     return value: 1 (bad_command_line)

 (3) command line arguments: GamePlay TicTacToe GomoKu
     output: 
	Keyword grammar is not correct!
	GamePlay Usage: Game [-a agent] [-b boardsize] [-c connect]
	Arguments:
           Game   the game to play (TicTacToe, Gomoku, or Sudoku)
           -a     specify the agent for TicTacToe or Gomoku (auto_player)
           -b     the game board size of Gomoku
           -c     the requirement of connection to win a Gomoku game

     return value: 1 (bad_command_line)

 (4) command line arguments: GamePlay TicTacToe 
     output: 

	No saved file
	Start a new game
	4
	3
	2
	1
	0	
	 01234
	
	Please enter a coordinate to place your mark or enter
	'quit' to exit the game:quit
	Save the game? y/N
	n
	Quit by user!

     return value: 8 (quit_by_player)

 (5) command line arguments: GamePlay Gomoku
     output: 

	No saved file
	Start a new game
	19
	18
	17
	16
	15
	14
	13
	12
	11
	10
	9
	8
	7
	6
	5
	4
	3
	2
	1
	X  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19

	Please enter a coordinate to place your token or enter 'quit' to exit the game:
	quit
	Save the game? y/N
	n
	Quit by user!

     return value: 8 (quit_by_player)

 (6) command line arguments: GamePlay Sudoku
     output: 

	No saved file
	Start a new game
	8||  |  |  ||  |8 |  ||  |7 |9 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	7||  |  |  ||4 |1 |9 ||  |  |5 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	6||  |6 |  ||  |  |  ||2 |8 |  ||
	=||= |= |= ||= |= |= ||= |= |= ||
	5||7 |  |  ||  |2 |  ||  |  |6 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	4||4 |  |  ||8 |  |3 ||  |  |1 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	3||8 |  |  ||  |6 |  ||  |  |3 ||
	=||= |= |= ||= |= |= ||= |= |= ||
	2||  |9 |8 ||  |  |  ||  |6 |  ||
	-||- |- |- ||- |- |- ||- |- |- ||
	1||6 |  |  ||1 |9 |5 ||  |  |  ||
	-||- |- |- ||- |- |- ||- |- |- ||
	0||5 |3 |  ||  |7 |  ||  |  |  ||
	=||= |= |= ||= |= |= ||= |= |= ||
	#||0 |1 |2 ||3 |4 |5 ||6 |7 |8 ||

	Please enter a coordinate and a digit, separated by comma, such as 0,0,9
	or enter 'quit' to exit the game: quit
	Save the game? y/N
	n
	Quit by user!

     return value: 8 (quit_by_player)

 <Test for user input of TicTacToe game>

 (7) Please enter a coordinate to place your mark or enter
     'quit' to exit the game: 

     input: 1 1
     output: 
	Invalid coordinate! Please try again
	Please enter a coordinate to place your mark or enter
	'quit' to exit the game: 	

     No return value (program still wait for input)

 (8) Please enter a coordinate to place your mark or enter
     'quit' to exit the game: 

     input: 0,0
     output: 
	Invalid coordinate! Please try again
	Please enter a coordinate to place your mark or enter
	'quit' to exit the game: 	

     No return value (program still wait for input)

 (9) Please enter a coordinate to place your mark or enter
     'quit' to exit the game: 

     input: 2,2
     output: 

	4
	3
	2  X
	1
	0	
	 01234

	Player X: 2, 2;
	
	Please enter a coordinate to place your mark or enter
	'quit' to exit the game:

     No return value (program still wait for input)

 (10) Follow test (9), input the second coordinate
     input: 1,1
     output: 

	4
	3
	2  X
	1 O
	0	
	 01234

	Player O: 1, 1;
	
	Please enter a coordinate to place your mark or enter
	'quit' to exit the game:

     No return value (program still wait for input)

 (11) Follow test (10), input the third coordinate
     input: 1,1 (the same as previous coordinate)
     output: 
	Invalid coordinate! Please try again
	Please enter a coordinate to place your mark or enter
	'quit' to exit the game: 	

     No return value (program still wait for input)

 (12) Follow test (11), input a draw game

	Please enter a coordinate to place your mark or 
	enter 'quit' to exit the game: 2,2
	4
	3
	2  X
	1
	0
	 01234

	Player X: 2, 2;

	Please enter a coordinate to place your mark or 
	enter 'quit' to exit the game: 1,1
	4
	3
	2  X
	1 O
	0
	 01234

	Player O: 1, 1;

	Please enter a coordinate to place your mark or 
	enter 'quit' to exit the game: 1,2
	4
	3
	2 XX
	1 O
	0
	 01234

	Player X: 2, 2; 1, 2;

	Please enter a coordinate to place your mark or 
	enter 'quit' to exit the game: 3,2
	4
	3
	2 XXO
	1 O
	0
	 01234

	Player O: 1, 1; 3, 2;

	Please enter a coordinate to place your mark or 
	enter 'quit' to exit the game: 1,3
	4
	3 X
	2 XXO
	1 O
	0
	 01234

	Player X: 2, 2; 1, 2; 1, 3;

	Please enter a coordinate to place your mark or 
	enter 'quit' to exit the game: 3,1
	4
	3 X
	2 XXO
	1 O O
	0
	 01234

	Player O: 1, 1; 3, 2; 3, 1;
	Please enter a coordinate to place your mark or 
	enter 'quit' to exit the game: 2,1
	4
	3 X
	2 XXO
	1 OXO
	0
	 01234

	Player X: 2, 2; 1, 2; 1, 3; 2, 1;

	Please enter a coordinate to place your mark or 
	enter 'quit' to exit the game: 2,3
	4
	3 XO
	2 XXO
	1 OXO
	0
	 01234

	Player O: 1, 1; 3, 2; 3, 1; 2, 3;

	Please enter a coordinate to place your mark or 
	enter 'quit' to exit the game: 3,3
	4
	3 XOX
	2 XXO
	1 OXO
	0
	 01234

	Player X: 2, 2; 1, 2; 1, 3; 2, 1; 3, 3;
	Draw! 9 turns were played. No winning moves remain.

     return value: 7 (no_winner)

 (13) Follow test (12), input a win game

	4
	3
	2
	1
	0
	 01234

	Please enter a coordinate to place your mark or 
	enter 'quit' to exit the game: 2,2
	4
	3
	2  X
	1
	0
	 01234

	Player X: 2, 2;
	
	Please enter a coordinate to place your mark or 
	enter 'quit' to exit the game: 2,1
	4
	3
	2  X
	1  O
	0
	 01234

	Player O: 2, 1;

	Please enter a coordinate to place your mark or 
	enter 'quit' to exit the game: 1,1
	4
	3
	2  X
	1 XO
	0
	 01234

	Player X: 2, 2; 1, 1;

	Please enter a coordinate to place your mark or 
	enter 'quit' to exit the game: 3,3
	4
	3   O
	2  X
	1 XO
	0
	 01234

	Player O: 2, 1; 3, 3;
	
	Please enter a coordinate to place your mark or 
	enter 'quit' to exit the game: 1,2
	4
	3   O
	2 XX
	1 XO
	0
	 01234

	Player X: 2, 2; 1, 1; 1, 2;
	
	Please enter a coordinate to place your mark or 
	enter 'quit' to exit the game: 1,3
	4
	3 O O
	2 XX
	1 XO
	0
	 01234

	Player O: 2, 1; 3, 3; 1, 3;
	
	Please enter a coordinate to place your mark or 
	enter 'quit' to exit the game: 3,2
	4
	3 O O
	2 XXX
	1 XO
	0
	 01234

	Player X: 2, 2; 1, 1; 1, 2; 3, 2;
	Player X wins!

     return value: 0 (success)

 (14) Resume a previous game (played with auto player).
      I did the following three steps:
	(a) I played a new game with an auto player, and then quitted the game 
	    and chose to save it.
	(b) Resume the game with auto_player (without keyword "-a auto_player").
        (c) After the game finished, start next one to see whether the program
	    construct a new game or not

      Output:

	Command line arguments: GamePlay TicTacToe -a auto_player
	Output: 
	No saved file
	Start a new game
	4
	3
	2
	1
	0
	  0 1 2 3 4
	
	Please enter a coordinate to place your token or enter 'quit' to exit the game:
	2,2
	4
	3
	2     X
	1
	0
	  0 1 2 3 4

	Player X: 2, 2;
	4
	3
	2     X
	1   O
	0
	  0 1 2 3 4
	
	Player O: 1, 1;
	Please enter a coordinate to place your token or enter 'quit' to exit the game:
	quit
	Save the game? y/N
	Y
	Quit by user!

	Command line arguments: GamePlay TicTacToe
	Output:

	4
	3
	2     X
	1   O
	0
	  0 1 2 3 4
	
	Please enter a coordinate to place your token or enter 'quit' to exit the game:
	1,2
	4
	3
	2   X X
	1   O
	0
	  0 1 2 3 4
	
	Player X: 2, 2; 1, 2;
	4
	3
	2   X X O
	1   O
	0
	  0 1 2 3 4

	Player O: 1, 1; 3, 2;
	Please enter a coordinate to place your token or enter 'quit' to exit the game:
	1,3
	4
	3   X
	2   X X O
	1   O
	0
	  0 1 2 3 4

	Player X: 2, 2; 1, 2; 1, 3;
	4
	3   X
	2   X X O
	1   O   O
	0
	  0 1 2 3 4

	Player O: 1, 1; 3, 2; 3, 1;
	Please enter a coordinate to place your token or enter 'quit' to exit the game:
	2,1
	4
	3   X
	2   X X O
	1   O X O
	0
	  0 1 2 3 4

	Player X: 2, 2; 1, 2; 1, 3; 2, 1;
	4
	3   X   O
	2   X X O
	1   O X O
	0
	  0 1 2 3 4

	Player O: 1, 1; 3, 2; 3, 1; 3, 3;
	Player O wins!
	
	Command line arguments: GamePlay TicTacToe
	Output: 
	No saved file
	Start a new game
	4
	3
	2
	1
	0
	  0 1 2 3 4
	
	Please enter a coordinate to place your token or enter 'quit' to exit the game:
	quit
	Save the game? y/N
	n
	Quit by user!

	Return value when the game finished: 0 (success)

 <Test for user input of Gomoku game>
 (15) Repeat the test (6) - (14) with Gomoku game. 
      The results are similar.

 <Test for user input of Sudoku game>
 (16) I would do the following four steps:
	(a) enter an invalid input
	(b) enter an valid input
	(c) enter an valid input at the same tile as (b) with a different number
	(d) enter an valid input at the tile that was part of the initial game board

      Command line arguments: GamePlay Sudoku
      Output:
	No saved file
	Start a new game

	8||  |  |  ||  |8 |  ||  |7 |9 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	7||  |  |  ||4 |1 |9 ||  |  |5 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	6||  |6 |  ||  |  |  ||2 |8 |  ||
	=||= |= |= ||= |= |= ||= |= |= ||
	5||7 |  |  ||  |2 |  ||  |  |6 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	4||4 |  |  ||8 |  |3 ||  |  |1 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	3||8 |  |  ||  |6 |  ||  |  |3 ||
	=||= |= |= ||= |= |= ||= |= |= ||
	2||  |9 |8 ||  |  |  ||  |6 |  ||
	-||- |- |- ||- |- |- ||- |- |- ||
	1||6 |  |  ||1 |9 |5 ||  |  |  ||
	-||- |- |- ||- |- |- ||- |- |- ||
	0||5 |3 |  ||  |7 |  ||  |  |  ||
	=||= |= |= ||= |= |= ||= |= |= ||
	#||0 |1 |2 ||3 |4 |5 ||6 |7 |8 ||

	Please enter a coordinate and a digit, separated by comma, such as 0,0,9
	or enter 'quit' to exit the game: 1,1,100
	Invalid coordinate! Please try again

	Please enter a coordinate and a digit, separated by comma, such as 0,0,9
	or enter 'quit' to exit the game: 1,1,9

	8||  |  |  ||  |8 |  ||  |7 |9 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	7||  |  |  ||4 |1 |9 ||  |  |5 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	6||  |6 |  ||  |  |  ||2 |8 |  ||
	=||= |= |= ||= |= |= ||= |= |= ||
	5||7 |  |  ||  |2 |  ||  |  |6 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	4||4 |  |  ||8 |  |3 ||  |  |1 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	3||8 |  |  ||  |6 |  ||  |  |3 ||
	=||= |= |= ||= |= |= ||= |= |= ||
	2||  |9 |8 ||  |  |  ||  |6 |  ||
	-||- |- |- ||- |- |- ||- |- |- ||
	1||6 |9 |  ||1 |9 |5 ||  |  |  ||
	-||- |- |- ||- |- |- ||- |- |- ||
	0||5 |3 |  ||  |7 |  ||  |  |  ||
	=||= |= |= ||= |= |= ||= |= |= ||
	#||0 |1 |2 ||3 |4 |5 ||6 |7 |8 ||


	Please enter a coordinate and a digit, separated by comma, such as 0,0,9
	or enter 'quit' to exit the game: 1,1,5

	8||  |  |  ||  |8 |  ||  |7 |9 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	7||  |  |  ||4 |1 |9 ||  |  |5 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	6||  |6 |  ||  |  |  ||2 |8 |  ||
	=||= |= |= ||= |= |= ||= |= |= ||
	5||7 |  |  ||  |2 |  ||  |  |6 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	4||4 |  |  ||8 |  |3 ||  |  |1 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	3||8 |  |  ||  |6 |  ||  |  |3 ||
	=||= |= |= ||= |= |= ||= |= |= ||
	2||  |9 |8 ||  |  |  ||  |6 |  ||
	-||- |- |- ||- |- |- ||- |- |- ||
	1||6 |5 |  ||1 |9 |5 ||  |  |  ||
	-||- |- |- ||- |- |- ||- |- |- ||
	0||5 |3 |  ||  |7 |  ||  |  |  ||
	=||= |= |= ||= |= |= ||= |= |= ||
	#||0 |1 |2 ||3 |4 |5 ||6 |7 |8 ||

	Please enter a coordinate and a digit, separated by comma, such as 0,0,9
	or enter 'quit' to exit the game: 0,0,1
	Invalid coordinate! Please try again
	Please enter a coordinate to place your mark or enter
	'quit' to exit the game: 	

     No return value (program still wait for input)

 (17) I would do the following two steps:
	(a) fill all the tiles but  one constraint was not satisfied
	(b) enter the solution for sudoku0
      
      For simplicity, I would only show the result of last input of step (a) and (b)

      Output:

	Please enter a coordinate and a digit, separated by comma, such as 0,0,9
	or enter 'quit' to exit the game: 6,8,9

	8||3 |4 |5 ||2 |8 |6 ||9 |7 |9 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	7||2 |8 |7 ||4 |1 |9 ||6 |3 |5 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	6||9 |6 |1 ||5 |3 |7 ||2 |8 |4 ||
	=||= |= |= ||= |= |= ||= |= |= ||
	5||7 |1 |3 ||9 |2 |4 ||8 |5 |6 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	4||4 |2 |6 ||8 |5 |3 ||7 |9 |1 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	3||8 |5 |9 ||7 |6 |1 ||4 |2 |3 ||
	=||= |= |= ||= |= |= ||= |= |= ||
	2||1 |9 |8 ||3 |4 |2 ||5 |6 |7 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	1||6 |7 |2 ||1 |9 |5 ||3 |4 |8 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	0||5 |3 |4 ||6 |7 |8 ||9 |1 |2 ||
	=||= |= |= ||= |= |= ||= |= |= ||
	#||0 |1 |2 ||3 |4 |5 ||6 |7 |8 ||

	Please enter a coordinate and a digit, separated by comma, such as 0,0,9
	or enter 'quit' to exit the game: 6,8,1

	8||3 |4 |5 ||2 |8 |6 ||1 |7 |9 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	7||2 |8 |7 ||4 |1 |9 ||6 |3 |5 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	6||9 |6 |1 ||5 |3 |7 ||2 |8 |4 ||
	=||= |= |= ||= |= |= ||= |= |= ||
	5||7 |1 |3 ||9 |2 |4 ||8 |5 |6 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	4||4 |2 |6 ||8 |5 |3 ||7 |9 |1 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	3||8 |5 |9 ||7 |6 |1 ||4 |2 |3 ||
	=||= |= |= ||= |= |= ||= |= |= ||
	2||1 |9 |8 ||3 |4 |2 ||5 |6 |7 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	1||6 |7 |2 ||1 |9 |5 ||3 |4 |8 ||
	-||- |- |- ||- |- |- ||- |- |- ||
	0||5 |3 |4 ||6 |7 |8 ||9 |1 |2 ||
	=||= |= |= ||= |= |= ||= |= |= ||
	#||0 |1 |2 ||3 |4 |5 ||6 |7 |8 ||


	Player 1 wins!

     Return value: 0 (success)
	
***************************************************************
