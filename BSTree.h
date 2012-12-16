/**
 * Title: BSTTree.cpp
 * Name: Robert O'Connor
 * Created: 12/12/12
 * Description: a Binary Search Tree implementation
 *
 */

#ifndef BSTREE_H_
#define BSTREE_H_
#include <cstddef> // for NULL.
#include <new>
#include "TreeNode.h"
#include "TreeException.h"

template<typename T>
class BSTree {
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

		/**
		 * Insert an item into the tree.
		 * @pre the item must exist
		 * @post the item is inserted into the tree correctly (e.g., left child if less-than, right child if greater-than)
		 * @param item the item to add to the tree
		 * @param root the root node (allows for recursive calls)
		 * @throws TreeException if memory allocation fails
		 *
		 */
		virtual void insert(T item, TreeNode<T>* treePtr) throw (TreeException);

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

		// getter & setter for the root node.
		TreeNode<T> getRoot();
		void setRoot(TreeNode<T> *root);

		// traversal functions adapted from the code from the book...
		void preorder(TreeNode<T> *node, T& visit);
		void inorder(TreeNode<T> *node, T& visit);
		void postorder(TreeNode<T> *node, T& visit);

		// helper function to get the children (as well as set them).
		// These have been adapted from the book's code.
		void getChildPtrs(TreeNode<T> *node, TreeNode<T> *& left, TreeNode<T> *& right) const;
		void setChildPtrs(TreeNode<T> *node, TreeNode<T> *left, TreeNode<T> *right);
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
		typedef void (*FunctionType)(T&);
		TreeNode<T> *root;
};

template<typename T>
BSTree<T>::BSTree() :
		root(NULL) {
}

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
	if (this != &rhs) {
		destroyTree(root);  // deallocate left-hand side
		copyTree(rhs->root, root);  // copy right-hand side
	}  // end if
	return *this;
}  // end operator=

template<typename T>
bool BSTree<T>::isEmpty() {
	return root == NULL;
}

template<typename T>
void BSTree<T>::insert(T item, TreeNode<T>* treePtr) throw (TreeException) {
	if (treePtr == NULL) {  // position of insertion found; insert after leaf

		// create a new node
		try {
			treePtr = new TreeNode<T>(item, NULL, NULL);
		} catch (bad_alloc e) {
			throw TreeException("TreeException: cannot allocate new memory for the new node to be inserted!");
		}  // end try
	}
	// else search for the insertion position
	else if (item < treePtr->item)
		// search the left subtree
		insert(item,treePtr.left);

	else
		// search the right subtree
		insert(item,treePtr.right);
}

template<typename T>
void BSTree<T>::deleteItem(T item, TreeNode<T>*& treePtr) throw (TreeException) {
	if (treePtr == NULL)
		throw TreeException("TreeException: delete failed");  // empty tree

	else if (item == treePtr->item)
		// item is in the root of some subtree
		deleteNodeItem(item, treePtr);  // delete the item

	// else search for the item
	else if (item < treePtr->item)
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
	if ((nodePtr->left == NULL) && (nodePtr->right == NULL)) {
		delete nodePtr;
		nodePtr = NULL;
	}  // end if leaf

	// test for no left child
	else if (nodePtr->left == NULL) {
		delPtr = nodePtr;
		nodePtr = nodePtr->right;
		delPtr->right = NULL;
		delete delPtr;
	}  // end if no left child

	// test for no right child
	else if (nodePtr->right == NULL) {
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
T* BSTree<T>::retrieve(TreeNode<T>* treePtr, T item) {
	T* ret = NULL;
	if (treePtr == NULL)
		return NULL;
	else if (item == treePtr->item)
		// item is in the root of some subtree
		return treePtr->item;
	else if (item < treePtr->item.getKey())
		// search the left subtree
		return retrieve(treePtr->left, item);
	else
		// search the right subtree
		return retrieve(treePtr->right, item);
}

template<typename T>
TreeNode<T> BSTree<T>::getRoot() {
	return this->root;
}
template<typename T>
void BSTree<T>::setRoot(TreeNode<T>* newRoot) {
	this->root = newRoot;
}

template<typename T>
void BSTree<T>::preorder(TreeNode<T>* node, T& visit) {
	if (node != NULL) {
		postorder(node->left, visit);
		postorder(node->right, visit);
		visit(node->item);
	}
}

template<typename T>
void BSTree<T>::inorder(TreeNode<T>* node, T& visit) {
	if (node != NULL) {
		inorder(node->left, visit);
		visit(node->item);
		inorder(node->right, visit);
	}  // end if
}

template<typename T>
void BSTree<T>::postorder(TreeNode<T>* node, T& visit) {
	if (node != NULL) {
		visit(node->item);
		preorder(node->left, visit);
		preorder(node->right, visit);
	}  // end if
}

template<typename T>
void BSTree<T>::getChildPtrs(TreeNode<T>* node, TreeNode<T>*& left, TreeNode<T>*& right) const {
	left = node->left;
	right = node->right;
}

template<typename T>
void BSTree<T>::setChildPtrs(TreeNode<T>* node, TreeNode<T>* left, TreeNode<T>* right) {

	node->left = root->left;
	node->right = root->right;
}

template<typename T>
T BSTree<T>::inorderSuccessor(TreeNode<T>* node) {
	if (node->left == NULL) {
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
	if (treePtr != NULL) {
		try {
			newTreePtr = new TreeNode<T>(newTreePtr->item, NULL, NULL);
			copyTree(treePtr->left, newTreePtr->left);
			copyTree(treePtr->right, newTreePtr->right);

		} catch (bad_alloc &ex) {
			throw new TreeException("Memory allocated for a TreeNode has failed...");
		}
	} else {
		newTreePtr = NULL;
	}

}

template<typename T>
void BSTree<T>::destroyTree(TreeNode<T> *& treePtr) {
// postorder traversal
	if (treePtr != NULL) {
		destroyTree(treePtr->left);
		destroyTree(treePtr->right);
		delete treePtr;
		treePtr = NULL;
	}  // end if
}

#endif /* BSTREE_H_ */
