// Specification file for the HashNode class
// Written By: A. Student
// Changed by: Huichan Cheng

#ifndef _HASH_NODE
#define _HASH_NODE

template<class ItemType>
class HashNode
{
private:
	 ItemType item;
    int occupied;			// 0: empty since start, 1: occupied, 2: empty removal
    int noCollisions;

public:
    // constructors
    HashNode() {occupied = 0; noCollisions = 0;}
    HashNode(ItemType anItem) {item = anItem; occupied = 1; noCollisions = 0;}
    HashNode(ItemType anItem, int ocp, int nCol)
             {item = anItem; occupied = ocp; noCollisions = nCol;}
    // setters
    void setItem(const ItemType & anItem) {item = anItem;}
    void setOccupied(int ocp) {occupied = ocp;}
    void setNoCollisions(int nCol) {noCollisions = nCol;}

    // getters
	 ItemType getItem() const {return item;}
    int getOccupied() const {return occupied;}
    int getNoCollisions() const {return noCollisions;}
};

#endif
