// Specification file for the Hash class
// Written By: A. Student
// Changed by: Huichan Cheng


#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "HashNode.h"
#include <cmath>
#include <fstream>

template<class ItemType>
class HashTable
{
private:
	HashNode<ItemType>* hashAry;
	int hashSize;
	int count;

public:
	HashTable() { count = 0; hashSize = 53; hashAry = new HashNode<ItemType>[hashSize]; }
	HashTable(int n)	{ count = 0; hashSize = n;	hashAry = new HashNode<ItemType>[hashSize]; }
	~HashTable(){ delete [] hashAry; }

	void setSize(int s) { hashSize = s; }
	int getCount() const	{ return count; }
	int getSize() const { return hashSize; }
	double getLoadFactor() const {return 100.0 * count / hashSize; }
	bool isEmpty() const	{ return count == 0; }
	bool isFull()  const	{ return count == hashSize; }

	bool insert( const ItemType &itemIn, int h(const ItemType &key, int size) );
	bool remove( ItemType &itemOut, const ItemType &key, int h(const ItemType &key, int size));
	bool search( ItemType &itemOut, const ItemType &key, int h(const ItemType &key, int size));

	int collisionRes(int index, int noCollision);
	int totCollision();
	int longestPath();
	void hpDisplay(std::ofstream &fout);
};

/*~*~*~*
   Insert an item into the hash table
   It does not reject duplicates
*~**/
template<class ItemType>
bool HashTable<ItemType>::insert( const ItemType &itemIn, int h(const ItemType &key, int size) )
{
	if ( count == hashSize)
        return false;

	HashNode<ItemType> newItem(itemIn);
	int bucket = h(itemIn, hashSize);
	int nc = 0;
	while (hashAry[bucket].getOccupied() == 1)
	{
		nc++;
		bucket = collisionRes(bucket, nc);
	}

	hashAry[bucket] = newItem;
	hashAry[bucket].setNoCollisions(nc);
	count++;

	return true;
}

/*~*~*~*
   Removes the item with the matching key from the hash table
     - copies data in the hash node to itemOut
     - replaces data in the hash node with an empty record
*~**/
template<class ItemType>
bool HashTable<ItemType>::remove( ItemType &itemOut, const ItemType &key, int h(const ItemType &key, int size))
{
	if (hashAry)
	{
		int bucket = h(key, hashSize), bucketProbed = 0;

		while (bucketProbed < hashSize)
		{
			if (hashAry[bucket].getItem() == key)
			{
				itemOut = hashAry[bucket].getItem();
				hashAry[bucket] = HashNode<ItemType>();
				hashAry[bucket].setOccupied(2);
				count--;
				return true;
			}
			bucketProbed++;
			bucket = collisionRes(bucket, bucketProbed);
		}
	}
	return false;
}

/*~*~*~*
   hash search - linear probe
   if found:
      - copy data to itemOut
      - returns true
   if not found, returns false
*~**/
template<class ItemType>
bool HashTable<ItemType>::search(ItemType &itemOut, const ItemType &key, int h(const ItemType &key, int size))
{
	int bucket = h(key, hashSize), bucketProbed = 0;

	while (bucketProbed < hashSize || hashAry[bucket].getOccupied() != 0)
	{
		if (hashAry[bucket].getItem() == key)
		{
			itemOut = hashAry[bucket].getItem();
			return true;
		}
		bucketProbed++;
		bucket = collisionRes(bucket, bucketProbed);
	}
	return false;
}

/*~*~*~*
   Collision resolution function
   input: index and nth collision;
   output: next index
*~**/
template<class ItemType>
int HashTable<ItemType>::collisionRes(int index, int nthCollision)
{
	int newIndex = static_cast<int>(index + pow(-1, nthCollision - 1) * nthCollision) % hashSize;

	if (newIndex < 0)
		newIndex += hashSize;

	return newIndex;
}

/*~*~*~*
   returning the total number of collisions.
*~**/
template<class ItemType>
int HashTable<ItemType>::totCollision()
{
	int sum = 0;

	if (!isEmpty())
	{
	 	for (int i = 0; i < hashSize; i++)
			sum += hashAry[i].getNoCollisions();
	}
	return sum;
}


/*~*~*~*
   returning the highest number of collisions.
*~**/
template<class ItemType>
int HashTable<ItemType>::longestPath()
{
	int max = hashAry[0].getNoCollisions();
	for (int i = 1; i < hashSize; i++)
	{
		if (hashAry[i].getNoCollisions() > max)
			max = hashAry[i].getNoCollisions();
	}
	return max;
}


/*~*~*~*
   displaying data in hashtable in output file format
*~**/
template<class ItemType>
void HashTable<ItemType>::hpDisplay(std::ofstream &fout)
{
	for (int i = 0; i < hashSize; i++)
	{
		if (hashAry[i].getOccupied() == 1)
			hashAry[i].getItem().fileOut(fout);
	}
}

#endif

