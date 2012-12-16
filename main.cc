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
#include "BSTree.h"

using namespace std;
BSTree<string> indexFile(string fileName);
int main(int argc, char **argv) {
	BSTree<string> tree = indexFile(argv[1]);
}

BSTree<string> indexFile(string fileName) {
	BSTree<string> tree;
	return tree;

}
