// Lab.cpp : Defines the entry point for the console application.
//
// author: Tzai-Shuen Chen 
// email: chen.tzai-shuen@wustl.edu
//
// purpose: define all the common functions used in lab exercises

#include "stdafx.h"
#include "Lab.h"

#include <iostream> // For standard output stream and manipulators.
#include <sstream>  // For standard string streams.
#include <fstream>	// For standard file streams.
#include <cstring>  // For C-style string functions
#include <cctype>

using namespace std;

// Helper function to print out the program's usage message. 
int usage(char * program, string cmdInstruction, string options) {

	cout << program << " Usage: " << cmdInstruction << endl;
	cout << "Arguments: " << endl;
	cout << options << endl;
	return bad_command_line;
}

// Helper function to convert all characters in a string to lowercase. 
void lowercase(string & input){

	for (char &word : input){

		word = static_cast<char>(tolower(word));
	}
}

// Helper function to parse the strings in input_file and push them back in text. 
int parse(vector<string> & text, char * input_file) {

	ifstream infile(input_file);

	if (infile.is_open()) {

		string word;
		while (infile >> word) {

			text.push_back(word);
		}

		infile.close();
		infile.clear();
		return success;
	}
	else {

		string i = input_file;
		throw i;
	}
}


// Helper function to separate numeric strings from non-numeric strings and store them separately
int separate_integer_from_text(vector<string> & target, vector<int> & numeric_strings,
	vector<string> & nonnumeric_strings){

	for (vector<string>::iterator ite = target.begin(); ite != target.end(); ++ite) {

		bool is_numeric = true;
		const char * ite_ptr = (*ite).c_str();
		auto string_length = strlen((*ite).c_str());

		// collect nonnumeric strings 
		for (decltype(string_length) i = 0; i < string_length; ++i) {

			if (!isdigit(ite_ptr[i])) {
				nonnumeric_strings.push_back(*ite);
				is_numeric = false;
				break;
			}
		}

		// collect numeric strings
		if (is_numeric) {
			int tmp;
			istringstream integer_stream(*ite);
			integer_stream >> tmp;
			numeric_strings.push_back(tmp);
		}
	}

	return success;
}

