/******************************************************************************
 Title: main.cc
 Author: Robert O'Connor
 Created on: July 22, 2012
 Description: Entry point for the application which indexes an input file into a BST.
 Build with: make
 Modifications:
 *******************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <cstring>
#include <algorithm>
#include <string>
#include <vector>
#include "BSTree.h"
#include "TreeNode.h"

using namespace std;

/**
 * Index the file...
 */
bool indexFile(BSTree<string> &tree, string fileName, bool &done);
bool my_predicate(char c);
void help();
void prompt(string &command, BSTree<string> &tree, bool &done);
std::vector<std::string> split(const std::string &s, char delim);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

int main(int argc, char **argv) {
	bool done = false;
	do {
		BSTree<string> tree;
		string command;
		prompt(command, tree, done);
	} while(!done);
}

bool indexFile(BSTree<string> &tree, string fileName, bool &done) {
	ifstream input(fileName.c_str());
	if(!input) {
		cout << "Error: cannot open file \"" << fileName << "\"" << endl;
		return false;
	}
	string line;
	while(input && getline(input, line)) {
		line.erase(std::remove_if(line.begin(), line.end(), my_predicate), line.end()); // strip punctuation
		transform(line.begin(), line.end(), line.begin(), ::tolower); // to lower case...
		stringstream ss(line);
		string token;
		tree.destroyTree();
		while(ss >> token) {
			tree.searchTreeInsert(token);
		}
	}
	if(input) {
		input.close();

		return true;
	}
	return true;
}

void prompt(string &command, BSTree<string> &tree, bool &done) {
	cout << "Command: ";
	cin >> command;
	// Mark Stein helped a bit -- we aren't going in a group but I used his code for
	// this routine as inspiration.
	if(command == "load") {
		string fileName;
		cin >> fileName;
		cout << "Indexing file " << fileName << "..." << endl;
		tree.destroyTree();
		bool res = indexFile(tree, fileName, done);
		if(res) {
			cout << "File \"" << fileName << "\"read & index generated" << endl;
		}
	} else if(command == "find") {
		string choice;
		cin >> choice;
		int height = tree.findHeight(choice);
		if(height < 0) {
			cout << endl << "Word \"" << choice << "\" does not appear." << endl;
		} else {
			cout << endl << "Word \"" << choice << "\" appears at height : " << height << endl;
		}
	} else if(command == "remove") {
		string choice;
		cin >> choice;
		if(tree.findHeight(choice) == 0) {
			cout << endl << "Word \"" << choice << "\" does not appear." << endl;
		} else {
			tree.searchTreeDelete(choice);
			cout << endl << "Word \"" << choice << "\" removed from index." << endl;
		}
	} else if(command == "help" && command.length() == 4) {
		cout << endl;
		cout << "****** HELP *******" << endl;
		cout << "load file_path --- To load a file and create indexes." << endl;
		cout << "find keyword   --- To search height of a keyword" << endl;
		cout << "remove keyword --- To remove the index for a keyword" << endl;
		cout << "help           --- To find help" << endl;
		cout << "quit           --- To quit the program" << endl;
	} else if(command == "quit" && command.length() == 4) {
		done = true;
	} else {
		cout << endl << "Please enter a valid choice. Type 'help' for available commands or 'quit' to exit anytime." << endl;
	}

}

void help() {
	cout << "Help" << endl;
	cout << "1) to load type \"load <filename>\" (replacing <filename with the name of a text file in the current directory). " << endl;
	cout << "2) to locate a given word in the index, type \"find <word>\" (again, replacing <word> with the word to find)." << endl;
	cout << "3) to delete a word from the index (but not the file itself) type: \"remove <word>\" (replacing <word> with the word you wish to delete)." << endl;
	cout << "4) to quit simply type \"q q\" (without quotes)." << endl;
	cout << "5) To display this message again type: \"? ?\"" << endl;
	cout << endl << endl;
	cout << "Example dialogue:" << endl << endl;
	cout << "Command: load poetry.txt" << endl;
	cout << "File \"poetry.txt\" read & index generated" << endl;
	cout << "Command: find reading" << endl;
	cout << "Word \"reading\" appears at height: 3" << endl;
	cout << "Command: find it" << endl;
	cout << "Word \"it\" appears at height: 3" << endl;
	cout << "Command: remove the" << endl;
	cout << "Word \"the\" removed from index" << endl;
	cout << "Command: find the" << endl;
	cout << "Word \"the\" does not appear." << endl;
	cout << "***PLEASE ALWAYS provide the two parts (e.g., load <filename>)***" << endl;
	cout << "Code no workies otherwise..." << endl;
}

// utility functions...mostly grabbed from stackoverflow somewhere...
bool my_predicate(char c) {
	return ispunct(c) && !isspace(c);
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while(std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	return split(s, delim, elems);
}
