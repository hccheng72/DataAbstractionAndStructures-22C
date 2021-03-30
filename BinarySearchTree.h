// Binary Search Tree ADT
// Written by: Anran Luo, Xinyi Guo

#ifndef _BINARY_SEARCH_TREE
#define _BINARY_SEARCH_TREE

#include "BinaryTree.h"
#include "HashTable.h"

template<class ItemType_1, class ItemType_2>
class BinarySearchTree : public BinaryTree<ItemType_1, ItemType_2>
{
public:
	// insert a node at the correct location
	bool insert(const ItemType_1 &item);

	// find a target node
	bool search(const ItemType_1 &target, ItemType_1 &returnedItem) const;

	// display the duplicate node
	void displayDuplicate(const ItemType_1 &target, HashTable<ItemType_2> &h, void visit(ItemType_1 &,
																							HashTable<ItemType_2> &)) const;

	// remove a node with assigned crn
	bool remove(const ItemType_1 & target, int compare(ItemType_1 , const ItemType_1 &));

	//get parent node of given node
	BinaryNode<ItemType_1>* getParent(BinaryNode<ItemType_1> &item, int compare(ItemType_1 ,
																										 const ItemType_1 &));

private:
	// internal insert node: insert newNode in nodePtr subtree
	BinaryNode<ItemType_1>* _insert(BinaryNode<ItemType_1>* nodePtr, BinaryNode<ItemType_1>* newNode);

	// search for target node
	BinaryNode<ItemType_1>* _search(BinaryNode<ItemType_1>* treePtr, const ItemType_1 &target) const;

	//find the parent node of given node
	BinaryNode<ItemType_1>* _getParent(BinaryNode<ItemType_1>* subtreeRoot, const ItemType_1 &item,
															 int compare(ItemType_1 , const ItemType_1 &)) const;
	//remove node by a given target
	BinaryNode<ItemType_1>* _remove(BinaryNode<ItemType_1>* nodePtr, const ItemType_1 target,
											  bool & success, int compare(ItemType_1, const ItemType_1& ));
	//delete node - called by remove node
	BinaryNode<ItemType_1>* _deleteNode(BinaryNode<ItemType_1>* nodePtr, int compare(ItemType_1,
																										const ItemType_1 &));
	//delete full node with 2 children
	BinaryNode<ItemType_1>* _deleteFullNode(BinaryNode<ItemType_1>* nodePtr,  int compare(ItemType_1 ,
																											const ItemType_1& ));
	//remove the left most node of a given tree
	BinaryNode<ItemType_1>* _removeLeftmostNode(BinaryNode<ItemType_1>* nodePtr, ItemType_1 &successor,
															  int compare(ItemType_1, const ItemType_1 &));

};


///////////////////////// public function definitions ///////////////////////////

//Wrapper for _insert - Inserting items within a tree
// - it calls the private _insert function that returns a Node pointer or NULL
// - if inserted, it copies data from that node and sends it back to the caller
//   via the output parameter, and returns true, otherwise it returns false.
template<class ItemType_1, class ItemType_2>
bool BinarySearchTree<ItemType_1, ItemType_2>::insert(const ItemType_1  &newEntry)
{
	BinaryNode<ItemType_1>* newNodePtr = new BinaryNode<ItemType_1>(newEntry);
	this->rootPtr = _insert(this->rootPtr, newNodePtr);
	this->count += 1;
	return true;
}

//Wrapper for _search
// - it calls the private _search function that returns a Node pointer or NULL
// - if found, it copies data from that node and sends it back to the caller
//   via the output parameter, and returns true, otherwise it returns false.
template<class ItemType_1, class ItemType_2>
bool BinarySearchTree<ItemType_1, ItemType_2>::search(const ItemType_1& anEntry,
																		ItemType_1 & returnedItem) const
{
	BinaryNode<ItemType_1>* temp = nullptr;
	temp = _search(this->rootPtr, anEntry);
	if(temp)
	{
		returnedItem = temp->getItem();
		return true;
	}

	return false;
}

//Wrapper for _remove - Removing items within a tree.
// - it calls the private _remove function that returns a Node pointer or NULL
// - if removed, it copies data from that node and sends it back to the caller
//   via the output parameter, and returns true, otherwise it returns false.
template<class ItemType_1, class ItemType_2>
bool BinarySearchTree<ItemType_1, ItemType_2>::remove(const ItemType_1 & target,
												 int compare(ItemType_1 , const ItemType_1 &))
{
	bool isSuccessful = false;

	this->rootPtr = _remove(this->rootPtr, target, isSuccessful, compare);
	if(isSuccessful)
		this->count--;
	return isSuccessful;
}

//Find and display node with same secondary key
template<class ItemType_1, class ItemType_2>
void BinarySearchTree<ItemType_1, ItemType_2>::displayDuplicate(const ItemType_1 &anEntry,
																					  HashTable<ItemType_2> &h,
											        void visit(ItemType_1 &, HashTable<ItemType_2> &)) const
{
	 BinaryNode<ItemType_1>*  found = _search(this->rootPtr, anEntry);
	 while(found)
	 {
		  //print data
		  ItemType_1 item = found->getItem();
		  visit(item, h);
		  found = _search(found->getRightPtr(), anEntry);
	 }
}

//Wrapper for _getParent - Getting parent node of a given node.
// - it calls the private _getParent function that returns a Node pointer or NULL
// - if found, it copies data from that node and sends it back to the caller
//   via the output parameter, and returns true, otherwise it returns false.
template<class ItemType_1, class ItemType_2>
BinaryNode<ItemType_1>* BinarySearchTree<ItemType_1, ItemType_2>::getParent(BinaryNode<ItemType_1>
															  &item, int compare(ItemType_1 , const ItemType_1 &))
{
	ItemType_1 target = item.getItem();
	return _getParent(this->rootPtr, target, compare);
}


//////////////////////////// private functions ////////////////////////////////////////////

//Implementation of the insert operation
// - returns NULL if the insertion is failed
// - returns a pointer to the node which has been successfully inserted
template<class ItemType_1, class ItemType_2>
BinaryNode<ItemType_1>* BinarySearchTree<ItemType_1, ItemType_2>::_insert(BinaryNode<ItemType_1>* nodePtr,
																							  BinaryNode<ItemType_1>* newNodePtr)
{
    BinaryNode<ItemType_1>* pWalk = nodePtr, *parent = nullptr;

    if( !nodePtr) // == NULL
    {
        nodePtr = newNodePtr;
        return nodePtr;
    }
    else
    {
        while(pWalk) // != NULL
        {
            parent = pWalk;
            if (pWalk->getItem() > newNodePtr->getItem())
                pWalk = pWalk->getLeftPtr();
            else
                pWalk = pWalk->getRightPtr();
        }
        if (parent->getItem() > newNodePtr->getItem())
            parent->setLeftPtr(newNodePtr);
        else
            parent->setRightPtr(newNodePtr);
    }

    return nodePtr;
}

//Implementation for the search operation
// - return NULL if target not found, otherwise
// - returns a pointer to the node that matched the target
template<class ItemType_1, class ItemType_2>
BinaryNode<ItemType_1>* BinarySearchTree<ItemType_1, ItemType_2>::_search(BinaryNode<ItemType_1>* nodePtr,
                                                           const ItemType_1 &target) const
{
	BinaryNode<ItemType_1>* found = nullptr;

	if(nodePtr) // != NULL
	{
	  if (nodePtr->getItem() == target)
		  found = nodePtr;
	  else if (nodePtr->getItem() > target)
			return _search( nodePtr->getLeftPtr(), target);
	  else
			return _search(nodePtr->getRightPtr(),target);
	}

	return found;
}

//Implementation to the getParent operation
// - return the parent node of a given node in tree
template<class ItemType_1, class ItemType_2>
BinaryNode<ItemType_1>* BinarySearchTree<ItemType_1, ItemType_2>::_getParent(BinaryNode<ItemType_1>* subtreeRoot,
												        const ItemType_1 &item, int compare(ItemType_1 , const ItemType_1 &)) const
{
	if(!subtreeRoot)
		  return NULL;

	if(subtreeRoot->getLeftPtr())
		if(compare(subtreeRoot->getLeftPtr()->getItem(), item) == 0)
		  return subtreeRoot;

	if(subtreeRoot->getRightPtr())
		if(compare(subtreeRoot->getRightPtr()->getItem(), item) == 0)
			return subtreeRoot;

	if(subtreeRoot->getItem() > item)
		  return _getParent(subtreeRoot->getLeftPtr(), item, compare);

	return _getParent(subtreeRoot->getRightPtr(), item, compare);
}

//Implementation to the delete operation
// - deletes given node in tree
// - return the root node of tree
template<class ItemType_1, class ItemType_2>
BinaryNode<ItemType_1>* BinarySearchTree<ItemType_1, ItemType_2>::_deleteNode(BinaryNode<ItemType_1>* nodePtr,
																						int compare(ItemType_1 , const ItemType_1 &))
{
	if (nodePtr->isLeaf())
	{
		BinaryNode<ItemType_1>* parentNode = getParent(*nodePtr, compare);
		ItemType_1 target = nodePtr->getItem();
		if(parentNode->getLeftPtr())
			if(compare(parentNode->getLeftPtr()->getItem(),target) == 0)
				parentNode->setLeftPtr(NULL);

		if(parentNode->getRightPtr())
			if(compare(parentNode->getRightPtr()->getItem(),target) == 0)
				parentNode->setRightPtr(NULL);

		delete nodePtr;
		nodePtr = 0;
		return this->rootPtr;
	}
	else if (nodePtr->getLeftPtr() == 0)
	{
		BinaryNode<ItemType_1>* nodeToConnectPtr = nodePtr->getRightPtr();
		BinaryNode<ItemType_1>* parentNode = getParent(*nodePtr, compare);
		if(!parentNode)
		{
			delete nodePtr;
			nodePtr = 0;
			return nodeToConnectPtr;
		}
		parentNode->setRightPtr(nodeToConnectPtr);
		delete nodePtr;
		nodePtr = 0;
		return this->rootPtr;
	}
	else if (nodePtr->getRightPtr() == 0)
	{
		BinaryNode<ItemType_1>* nodeToConnectPtr = nodePtr->getLeftPtr();
		BinaryNode<ItemType_1>* parentNode = getParent(*nodePtr, compare);
		if(!parentNode)
		{
			delete nodePtr;
			nodePtr = 0;
			return nodeToConnectPtr;
		}
		parentNode->setLeftPtr(nodeToConnectPtr);
		delete nodePtr;
		nodePtr = 0;
		return this->rootPtr;
	}
	else
	{
		_deleteFullNode(nodePtr, compare);
		return this->rootPtr;
	}
}

//Implementation of the delete full node operation
// - deletes given node with 2 children
// returns node pointer that is needed to be connected to other nodes
template<class ItemType_1, class ItemType_2>
BinaryNode<ItemType_1>* BinarySearchTree<ItemType_1, ItemType_2>::_deleteFullNode(BinaryNode<ItemType_1>* nodePtr,
																							 int compare(ItemType_1 , const ItemType_1 &))
{
	if (nodePtr->isLeaf())
	{
		delete nodePtr;
		nodePtr = 0;
		return nodePtr;
	}
	else if (nodePtr->getLeftPtr() == 0)
	{
		BinaryNode<ItemType_1>* nodeToConnectPtr = nodePtr->getRightPtr();
		delete nodePtr;
		nodePtr = 0;
		return nodeToConnectPtr;
	}
	else if (nodePtr->getRightPtr() == 0)
	{
		BinaryNode<ItemType_1>* nodeToConnectPtr = nodePtr->getLeftPtr();
		delete nodePtr;
		nodePtr = 0;
		return nodeToConnectPtr;
	}
	else
	{
		ItemType_1 *newNodeValue = new ItemType_1;
		nodePtr->setRightPtr(_removeLeftmostNode(nodePtr->getRightPtr(), *newNodeValue, compare));
		nodePtr->setItem(*newNodeValue);
		return nodePtr;
	}
}

//Implementation of the delete left most node operation
// - deletes the left most node of the tree
// - returns node pointer that is successfully deleted
template<class ItemType_1, class ItemType_2>
BinaryNode<ItemType_1>* BinarySearchTree<ItemType_1, ItemType_2>::_removeLeftmostNode(BinaryNode<ItemType_1>* nodePtr,
																	ItemType_1 &successor, int compare(ItemType_1 , const ItemType_1 &))
{
	if (nodePtr->getLeftPtr() == 0)
	{
		successor = nodePtr->getItem();
		return _deleteFullNode(nodePtr, compare);
	}
	else
	{
		nodePtr->setLeftPtr(_removeLeftmostNode(nodePtr->getLeftPtr(), successor, compare));
		return nodePtr;
	}
}

//Implementation of the remove node operation
// - remove node of the tree
// - call delete node function
// - returns node pointer that is successfully removed
template<class ItemType_1, class ItemType_2>
BinaryNode<ItemType_1>* BinarySearchTree<ItemType_1, ItemType_2>::_remove(BinaryNode<ItemType_1>* nodePtr,
                                                          const ItemType_1 target,
                                                          bool & success, int compare(ItemType_1 , const ItemType_1 &))
{
	if (nodePtr == 0)
		success = false;
	else
	{
		nodePtr = _search(this->rootPtr, target);
		while(nodePtr)
		{
			//print data
			ItemType_1 item = nodePtr->getItem();
			if(compare(item, target) == 0)
			{
				nodePtr = _deleteNode(nodePtr, compare);
				success = true;
				break;
			}
			nodePtr = _search(nodePtr->getRightPtr(), target);
		}
	}
	return nodePtr;
}

#endif
