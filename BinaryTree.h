// Binary tree abstract base class
// Created by A. Student
// Modified by: Xinyi Guo

#ifndef _BINARY_TREE
#define _BINARY_TREE

#include "BinaryNode.h"

template<class ItemType_1, class ItemType_2>
class BinaryTree
{
protected:
	BinaryNode<ItemType_1>* rootPtr;		// ptr to root node
	int count;							// number of nodes in tree
	
public:
	// "admin" functions
 	BinaryTree() {rootPtr = 0; count = 0;}
	BinaryTree(const BinaryTree<ItemType_1, ItemType_2> & tree){ }
	virtual ~BinaryTree() { destroyTree(rootPtr); }

	// common functions for all binary trees
 	bool isEmpty() const {return count == 0;}
	int getCount() const {return count;}
	void clear() {destroyTree(rootPtr); rootPtr = 0; count = 0;}
   void inOrder(void visit(ItemType_1 &, HashTable<ItemType_2> &), HashTable<ItemType_2> &h)
																			const  {_inorder(visit, h, rootPtr);}
	void printTree(void visit(ItemType_1 &, int)) const{_printTree(visit, rootPtr, 1);}

	// abstract functions to be implemented by derived class
	virtual bool insert(const ItemType_1 &newData) = 0;
	virtual bool search(const ItemType_1 &target, ItemType_1 & returnedItem) const = 0;

private:
	// delete all nodes from the tree
	void destroyTree(BinaryNode<ItemType_1>* nodePtr);

	// internal traverse
	void _inorder(void visit(ItemType_1 &, HashTable<ItemType_2> &), HashTable<ItemType_2> &h,
																				 BinaryNode<ItemType_1>* nodePtr) const;
   void _printTree(void visit(ItemType_1 &, int), BinaryNode<ItemType_1>* nodePtr, int level) const;

};

//Destroy the entire tree
template<class ItemType_1, class ItemType_2>
void BinaryTree<ItemType_1, ItemType_2>::destroyTree(BinaryNode<ItemType_1>* nodePtr)
{
    if(nodePtr) // != NULL
    {
        destroyTree(nodePtr->getLeftPtr());
        destroyTree(nodePtr->getRightPtr());
        delete nodePtr;
    }
}

//Inorder Traversal
template<class ItemType_1, class ItemType_2>
void BinaryTree<ItemType_1, ItemType_2>::_inorder(void visit(ItemType_1 &, HashTable<ItemType_2> &),
												     HashTable<ItemType_2> &h, BinaryNode<ItemType_1>* nodePtr) const
{
    if (nodePtr) // != NULL
    {
        ItemType_1 item = nodePtr->getItem();
        _inorder(visit, h, nodePtr->getLeftPtr());
		  visit(item, h);
        _inorder(visit, h, nodePtr->getRightPtr());
    }
}

//Prints tree as an indented list
template<class ItemType_1, class ItemType_2>
void BinaryTree<ItemType_1, ItemType_2>::_printTree(void visit(ItemType_1 &, int), BinaryNode<ItemType_1>* nodePtr,
																																  int level) const
{
	 if(nodePtr)
	 {
		  ItemType_1 item = nodePtr->getItem();
		  visit(item, level);
		  level++;

		  _printTree(visit, nodePtr->getRightPtr(), level);
		  _printTree(visit, nodePtr->getLeftPtr(), level);
	 }
}

#endif
