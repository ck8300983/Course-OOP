// Lab.h
//
// author: Tzai-Shuen Chen
// email: chen.tzai-shuen@wustl.edu
//
// purpose: Declarations for all lab exercises

#ifndef LAB_H
#define LAB_H

#include <cstring>  // For C-style string functions
#include <exception>
#include <fstream>
#include <iostream> // For standard output stream and manipulators.
#include <set>
#include <sstream>  // For standard string streams.
#include <string>
#include <vector>

using namespace std;

enum cmdArg_index {
	program_name, game_name, setting1, setting2, setting3,
	expected_min_argument = 2, expected_max_argument = 5
};
enum result_codes {
	success, bad_command_line, read_file_fail, read_line_fail,
	board_dimension_failure, no_valid_piece, pos_does_not_match_dim,
	no_winner, quit_by_player, badAlloc, no_instance, 
	requiredConnectionError, initializedGameFail, chooseGameFail,unknown_exception = -1
};

enum game_result { tie, win = 500, lose = -500 };

// Function prototypes
int usage(char * program_name, string cmdInstruction, string purpose);
int parse(vector<string> & text, char * input_file);
int separate_integer_from_text(vector<string> & target, vector<int> & numeric_character, vector<string> & nonnumeric_char);
void lowercase(string & input);

// Exception classes
class ReadLineException :public std::runtime_error{
public:
	ReadLineException(string err_msg) :runtime_error(err_msg){}
};

class ReadFileException :public std::runtime_error{
public:
	ReadFileException(string err_msg) :runtime_error(err_msg){}
};

class ReadDimensionException :public std::runtime_error{
public:
	ReadDimensionException(string err_msg) :runtime_error(err_msg){}
};

class NoPieceException : public std::runtime_error{
public:
	NoPieceException(string err_msg) :runtime_error(err_msg){}
};

class ConnectionException : public std::runtime_error{
public:
	ConnectionException(string err_msg) :runtime_error(err_msg){}
};


#endif /* LAB_H */


