/**
 * Title: TreeNode.h
 * Name: Robert O'Connor
 * Created: 12/12/12
 * Description: TreeNode structure used by the BST.
 *
 */


#ifndef TREENODE_H_
#define TREENODE_H_
/** @file TreeNode.h */

#include <cstddef>


/** @class TreeNode
 * Node used in the ADT binary search tree.
 *
 */
template<typename T>
class TreeNode {
	private:
		TreeNode():left(NULL),right(NULL) {
		}

		TreeNode(T nodeItem, TreeNode *left = NULL, TreeNode *right = NULL) :
				item(nodeItem), left(left), right(right) {
		}

		/** A data item in the tree. */
		T item;
		/** Pointers to children. */
		TreeNode *left, *right;

		// friend class - can access private parts
		friend class BSTree;
};
// end TreeNode

#endif /* TREENODE_H_ */
