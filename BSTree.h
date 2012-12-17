/**
 * Title: BSTTree.h
 * Name: Robert O'Connor
 * Created: 12/12/12
 * Description: a Binary Search Tree implementation
 *
 */

#ifndef BSTREE_H_
#define BSTREE_H_
#include <cstddef> // for NULL.
#include <new>
#include <iostream>
#include "TreeNode.h"
#include "TreeException.h"
using std::endl;
using std::cout;
template<typename T>
class BSTree {
		typedef void (*FunctionType)(T* node);
	public:
		// default ctor
		BSTree();

		// copy ctor
		BSTree(BSTree& other) throw (TreeException);

		// destuctor...destroys the tree and all its children...YAY for genocide.
		virtual ~BSTree();

		// rule of 3
		virtual BSTree<T>& operator=(BSTree<T> & rhs) throw (TreeException);

		/**
		 * Check is the tree is empty.
		 * @return true if the tree is empty, false otherwise
		 */
		virtual bool isEmpty();

		/** Inserts an item into a binary search tree.
		 * @pre The item to be inserted into the tree is newItem.
		 * @post newItem is in its proper order in the tree.
		 * @throw TreeException  If memory allocation fails. */
		virtual void searchTreeInsert(const T newItem) throw (TreeException);

		/** Deletes an item with a given search key from a binary search
		 *  tree.
		 * @pre searchKey is the search key of the item to be deleted.
		 * @post If the item whose search key equals searchKey existed
		 *       in the tree, the item is deleted. Otherwise, the tree
		 *       is unchanged.
		 * @throw TreeException  If searchKey is not found in the
		 *        tree. */
		virtual void searchTreeDelete(T searchKey) throw (TreeException);

		/** Retrieves an item with a given search key from a binary
		 *  search tree.
		 * @pre searchKey is the search key of the item to be
		 *      retrieved.
		 * @post If the retrieval was successful, treeItem contains the
		 *       retrieved item.
		 * @throw TreeException  If no such item exists.
		 */
		virtual T* searchTreeRetrieve(T item);
		/**
		 * Insert an item into the tree.
		 * @pre the item must exist
		 * @post the item is inserted into the tree correctly (e.g., left child if less-than, right child if greater-than)
		 * @param item the item to add to the tree
		 * @param root the root node (allows for recursive calls)
		 * @throws TreeException if memory allocation fails
		 *
		 */
		virtual void insert(TreeNode<T> *&treePtr, const T newItem) throw (TreeException);

		/**
		 * Delete an item from the tree
		 * @pre the item must exist and the root must not be NULL.
		 * @post the item is deleted if it exists.
		 * @param item the item to insert
		 * @param root the root node (allows for recursive calls)
		 * @throw TreeException if the root if the preconditions are not met.
		 */
		virtual void deleteNodeItem(T item, TreeNode<T>* node);

		virtual void deleteItem(T item, TreeNode<T> *& treePtr) throw (TreeException);
		/**
		 * Retrieve an item from the list
		 * @pre the item must exist and the root must not be NULL.
		 * @post the item is deleted if it exists.
		 * @param item the item to insert
		 * @param root the root node (allows for recursive calls)
		 * @return the item value or NULL if not found.
		 */
		virtual T* retrieve(TreeNode<T>* treePtr, T item);

		/**
		 * Find the findHeight to which our search key is located...
		 */
		int findHeight(TreeNode<T> *node, T key);

		/**
		 * Helper function...
		 * @key the key to start from...
		 */
		int findHeight(T key);

		void printTree(TreeNode<T> *node);
		void destroyTree();

	protected:
		/** Retrieves and deletes the leftmost descendant of a given
		 *  node.
		 * @pre node points to a node in a binary search tree;
		 *      node != NULL.
		 * @post returns the inorder successor (left-most node) and deletes it.
		 * @return the nodes value after its node has been deleted from the tree.
		 */
		// adapted from the book code to use templates and also not use pass-by-ref like its going out of style...
		virtual T inorderSuccessor(TreeNode<T>* node);
		/** Copies the tree rooted at treePtr into a tree rooted at
		 *  newTreePtr.
		 * @throw TreeException  If a copy of the tree cannot be
		 *        allocated.
		 */
		void copyTree(TreeNode<T> *treePtr, TreeNode<T> *& newTreePtr) throw (TreeException);

		/** Deallocates memory for a tree. */
		void destroyTree(TreeNode<T> *& treePtr);

	private:
		TreeNode<T> *root;
};

template<typename T>

BSTree<T>::BSTree() :
		root(NULL) {
}  // end default constructor

template<typename T>
BSTree<T>::BSTree(BSTree& other) throw (TreeException) {
	copyTree(root, other.root);
}

template<typename T>
BSTree<T>::~BSTree() {
	destroyTree(root);
}
template<typename T>
BSTree<T>& BSTree<T>::operator=(BSTree<T> & rhs) throw (TreeException) {
	if(this != &rhs) {
		destroyTree(root);  // deallocate left-hand side
		copyTree(rhs.root, root);  // copy right-hand side
	}  // end if
	return *this;
}  // end operator=

template<typename T>
bool BSTree<T>::isEmpty() {
	return root == NULL;
}

template<typename T>
void BSTree<T>::insert(TreeNode<T>*& treePtr, const T newItem) throw (TreeException) {
	if(treePtr == NULL) {  // position of insertion found; insert after leaf

		// create a new node
		try {
			treePtr = new TreeNode<T>(newItem, NULL, NULL);
		} catch(bad_alloc &e) {
			throw TreeException("TreeException: insertItem cannot allocate memory");
		}  // end try
	}
	// else search for the insertion position
	else if(newItem < treePtr->item) {
		// search the left subtree
		insert(treePtr->left, newItem);
	}

	else {
		// search the right subtree
		insert(treePtr->right, newItem);
	}
}
template<typename T>
void BSTree<T>::searchTreeInsert(T newItem) throw (TreeException) {
	insert(root, newItem);
}

template<typename T>
void BSTree<T>::deleteItem(T item, TreeNode<T>*& treePtr) throw (TreeException) {
	if(treePtr == NULL)
		throw TreeException("TreeException: delete failed");  // empty tree

	else if(item == treePtr->item)
		// item is in the root of some subtree
		deleteNodeItem(item, treePtr);  // delete the item

	// else search for the item
	else if(item < treePtr->item)
		// search the left subtree
		deleteItem(item, treePtr->left);

	else
		// search the right subtree
		deleteItem(item, treePtr->right);
}

template<typename T>
void BSTree<T>::deleteNodeItem(T item, TreeNode<T>* nodePtr) {
	TreeNode<T> *delPtr;
	T replacementItem;

	// test for a leaf
	if((nodePtr->left == NULL) && (nodePtr->right == NULL)) {
		delete nodePtr;
		nodePtr = NULL;
	}  // end if leaf

	// test for no left child
	else if(nodePtr->left == NULL) {
		delPtr = nodePtr;
		nodePtr = nodePtr->right;
		delPtr->right = NULL;
		delete delPtr;
	}  // end if no left child

	// test for no right child
	else if(nodePtr->right == NULL) {
		delPtr = nodePtr;
		nodePtr = nodePtr->left;
		delPtr->left = NULL;
		delete delPtr;
	}  // end if no right child

	// there are two children:
	// retrieve and delete the inorder successor
	else {
		replacementItem = inorderSuccessor(nodePtr->right);
		nodePtr->item = replacementItem;
	}  // end if two children
}
template<typename T>
void BSTree<T>::searchTreeDelete(T item) throw (TreeException) {
	deleteItem(item, root);
}

template<typename T>
T* BSTree<T>::retrieve(TreeNode<T>* treePtr, T item) {
	if(treePtr == NULL)
		return NULL;
	else if(item == treePtr->item)
		// item is in the root of some subtree
		return &(treePtr->item);
	else if(item < treePtr->item)
		// search the left subtree
		return retrieve(treePtr->left, item);
	else
		// search the right subtree
		return retrieve(treePtr->right, item);
}

template<typename T>
T* BSTree<T>::searchTreeRetrieve(T item) {
	return retrieve(root, item);
}

template<typename T>
T BSTree<T>::inorderSuccessor(TreeNode<T>* node) {
	if(node->left == NULL) {
		T ret = node->item;
		TreeNode<T> *delPtr = node;
		node = node->right;
		delPtr->right = NULL;  // defense
		delete delPtr;
		return ret;
	} else
		return inorderSuccessor(node->left);
}

template<typename T>
void BSTree<T>::copyTree(TreeNode<T>* treePtr, TreeNode<T> *&newTreePtr) throw (TreeException) {
	if(treePtr != NULL) {
		try {
			newTreePtr = new TreeNode<T>(newTreePtr->item, NULL, NULL);
			copyTree(treePtr->left, newTreePtr->left);
			copyTree(treePtr->right, newTreePtr->right);

		} catch(bad_alloc &ex) {
			throw new TreeException("Memory allocated for a TreeNode has failed...");
		}
	} else {
		newTreePtr = NULL;
	}

}
// inspired in part by Mark Stein's code -- except I used templates...so needed to tweak a bit
template<typename T>
int BSTree<T>::findHeight(TreeNode<T>* node, T key) {
	int height = 1;
	if(node == NULL) {
		return -1;
	} else if(node->item == key) {
		return height;
	} else if(node->item < key) {
		cout << "less than..." << endl;
		return findHeight(node->left, key);
	} else if(node->item > key) {
		cout << endl;
		cout << "greater than..." << endl;
		return findHeight(node->right, key);
	} else {
		++height;
	}

}

template<typename T>
int BSTree<T>::findHeight(T key) {
	return findHeight(root, key);
}

template<typename T>
void BSTree<T>::destroyTree(TreeNode<T> *& treePtr) {
// postorder traversal
	if(treePtr != NULL) {
		destroyTree(treePtr->left);
		destroyTree(treePtr->right);
		delete treePtr;
		treePtr = NULL;
	}  // end if
}
template<typename T>
void BSTree<T>::destroyTree() {
	if(root != NULL) {
		destroyTree(root->left);
		destroyTree(root->right);
		delete root;
		root = NULL;
	}
}

template<typename T>
void BSTree<T>::printTree(TreeNode<T> *node) {
	using namespace std;
	if(node == NULL) {
		cout << "NULL" << endl;
		return;
	}
	printTree(node->left);
	cout << (node->item);
	cout << endl;
	printTree(node->right);
}

#endif /* BSTREE_H_ */
