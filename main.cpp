/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 CIS 22C: Team Project - De Anza Computer Science Courses Database
 *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 TEAM MEMBERS' NAME: Anran Luo, Huichan Chan, Xinyi Guo
 IDE: Xcode, Visual Studio
 *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 main.cpp
 Screen output: Anran Luo
 File I/O: Huichan Cheng, Anran Luo
 *~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 PROJECT PURPOSE:
 This project is integrating the function of course register related websites
 which De Anza students usually use. For instance, when searching for a class
 by CRN or class code, The basic information about class will be printed out
 on the screen. Such as, lecture time, name of the instructor, the rating on
 class, and related textbook information.
 *~**/


#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>
#include "HashTable.h"
#include "BinarySearchTree.h"
#include "ClassList.h"
#include "Course.h"
using namespace std;

// Functions in main
void buildBstHash(BinarySearchTree<Course, ClassList> &bst, HashTable<ClassList> &hash,
																					 ifstream &);
void displayMenu();
void insertNode(BinarySearchTree<Course, ClassList> &bst, HashTable<ClassList> &hash);
void deleteNode(BinarySearchTree<Course, ClassList> &bst, HashTable<ClassList> &hash);
void searchManager(BinarySearchTree<Course, ClassList> &bst, HashTable<ClassList> &hash,
						 															char choice);
void searchByCrn(HashTable<ClassList> &hash);
void searchByCode(BinarySearchTree<Course, ClassList> &bst, HashTable<ClassList> &hash);
void displayList(BinarySearchTree<Course, ClassList> &bst, HashTable<ClassList> &hash);
void displayStatistics(HashTable<ClassList> &hash);
void writeData(HashTable<ClassList> &hash);
void displayDevelopers();

// Display functions - used as display function pointers
void hDisplay(Course &item, HashTable<ClassList> &hash);
void vDisplay(ClassList &);
void iDisplay(Course &, int);
void zDisplay(Course &item, HashTable<ClassList> &hash);

// Other functions
int compareByCRN(Course left, const Course &target);
int key_to_index(const ClassList &key, int size);
int numOfLines(ifstream &inFile);
int findNextPrime(int n);
int isPrime(int nn);
bool isParam(string crn);
void welcome();
void farewell();

int main()
{
	welcome();

	// Build database
	ifstream fin("classList.txt");

	HashTable<ClassList> hash;
	BinarySearchTree <Course, ClassList> bst;

	buildBstHash(bst, hash, fin);
	displayMenu();

	// Selection menu manager
	char choice = ' ';
	while (choice != 'Q')
	{
		cout << "\n\t\t- Enter a selection: ";
		cin >> choice;
		cin.ignore(100, '\n');

		switch(choice)
		{
				// HiddEn option
				case 'B':
					displayDevelopers();
					break;

				// Insert by user
				case 'I':
					insertNode(bst, hash);
					break;

				// Delete by CRN
				case 'D':
					deleteNode(bst, hash);
					break;

				// Search and display node by CRN - Primary key
            case 'P':
					searchManager(bst, hash, choice);
               break;

				// Search and display node by Code - Secondary key
            case 'S':
					searchManager(bst, hash, choice);
               break;

				// Display list by Code as Table
            case 'T':
					displayList(bst, hash);
               break;

				// Special Option: display list as indented tree
            case 'O':
               bst.printTree(iDisplay);
               break;

				// Write out data
            case 'W':
               writeData(hash);
               break;

				// Display stAtistics of hashtable
            case 'A':
					displayStatistics(hash);
               break;

				// Help: display selection menu again
            case 'H':
               displayMenu();
               break;

				// Quit: farewell message
            case 'Q':
					farewell();
               break;

            default:
                cout << "\n\t\t=== Invalid Selection - "
							<< "Choose Again ===\n" << endl;
            }
	}

	// Default write out file
	writeData(hash);

	return 0;
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 Display the selection menu which include the operation for the progamme.
 Pre: None
 Post: Print out menu on screen
*~**/
void displayMenu()
{
	cout  << "\t\t[MENU]\n\n";
	cout  << "\t\t- [I] Add new data" << endl
			<< "\t\t- [D] Delete data by CRN" << endl
			<< "\t\t- [P] Search a course by CRN" << endl
			<< "\t\t- [S] Search courses by Code" << endl
			<< "\t\t- [T] List data sorted by the Code" << endl
			<< "\t\t- [O] Hidden print option" << endl
			<< "\t\t- [W] Write data to a file" << endl
			<< "\t\t- [A] Statistics about Hashing" << endl
			<< "\t\t- [H] Help - Print menu" << endl
			<< "\t\t- [Q] Exit" << endl << endl;
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 Built up the hash table and binary search tree. Read data from input file
 and calculate and setup the size of hash table.
 Pre: Binary search tree object, hash table object, file pointer.
 Post: Builted hash table and binary search tree.
*~**/
void buildBstHash(BinarySearchTree<Course, ClassList> &bst, HashTable<ClassList> &hash,
																				  ifstream &fin)
{
	string crn, code, title, instructor, time, textbook;
	double rate;
	int size;

	// input file validation
	if (!fin)
	{
		cout << "Error opening the input file" << endl;
		exit(EXIT_FAILURE);
	}

	// Set up size of hashtable
	size = findNextPrime(numOfLines(fin) * 2);
	hash.setSize(size);

	// Reading data from input file
	while(getline(fin, crn, ';'))
	{
		getline(fin, code, ';');
		getline(fin, title, ';');
		getline(fin, instructor, ';');
		getline(fin, time, ';');
		getline(fin, textbook, ';');
		fin >> rate;
		fin.ignore();

		ClassList aClass(crn, code, title, instructor, time, textbook, rate);

		if(hash.insert(aClass, key_to_index))
            bst.insert(aClass);
	}
	fin.close();
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 Insert new node into hashtable and binary search tree by user entering
 data from keyboard.
 Pre: Binary search tree object, hash table object
 Post: Updated BST and hashtable and status after insertion
*~**/
void insertNode(BinarySearchTree<Course, ClassList> &bst, HashTable<ClassList> &hash)
{
	cout << "\n\n\t\t[Insert by user]\n\n";
	string  crn, code, title, instructor, time, textbook;
	double  rate;
	ClassList target, found;

	cout << "\t\tEnter a CRN (e.g. 42345): ";
	cin >> crn;
	cin.ignore();

	// Test for the primary key input
	if(!isParam(crn = crn.substr(0, 5))) 					// Only taking five charcters at most
	{
		cout << "\n\t\t=== Please Enter digits ===\n";
		return;
	}
	target.setCRN(crn);

	if(!hash.search(found, target, key_to_index)) // Reject duplicate
	{
		cout << "\t\tEnter a class code (e.g. CIS 22A): ";
		getline(cin, code);
		cout << "\t\tEnter a class title (e.g. Java Programming): ";
		getline(cin, title);
		cout << "\t\tEnter an instructor (e.g. John Luna): ";
		getline(cin, instructor);
		cout << "\t\tEnter a time (e.g. MW 09:30 am-11:20 am): ";
		getline(cin, time);
		cout << "\t\tEnter a textbook (e.g. Database Management Systems): ";
		getline(cin, textbook);
		cout << "\t\tEnter the rating (e.g. 5.00): ";
		cin >> rate;

		ClassList aClass(crn, code, title, instructor, time, textbook, rate);

		if(hash.insert(aClass, key_to_index))
		{
			if(bst.insert(aClass))
				cout << "\n\t\t=== Insert success ===\n";
			else
				cout << "\n\t\t=== Insert failed ===\n";
		}
	}
	else
		cout << "\n\t\t=== Insert failed because of duplication ===\n"
			  << "\t\tNote: Please enter a different CRN.\n";
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 Delete a node by CRN. This function will delete the node in both hashtable
 and BST. If the there is duplicate key in BST, only the node correspodes
 to CRN will be deleted.
 Pre: Binary tree and hashtable object
 Post: Updated BST and hashtable
*~**/
void deleteNode(BinarySearchTree<Course, ClassList> &bst, HashTable<ClassList> &hash)
{
	cout << "\n\n\t\t[Delete by CRN]\n\n";

	// Get target
	string crn;
	cout << "\t\tEnter an CRN: ";
	cin >> crn;
	cin.ignore();

	// Set up target to base object
	ClassList temp, found;
	temp.setCRN(crn);

	// Search target in hashtable
	if(!hash.search(found, temp, key_to_index))
		cout << "\n\t\t=== Not found ===\n";
	else
	{
		// Remove target
		if(bst.remove(found, compareByCRN) && hash.remove(temp, found,
																		  key_to_index))
			cout << "\t\tCourse with CRN " << found.getCRN() << " is removed.\n";
		else
			cout << "\n\t\t=== Remove failed ===\n";
	}
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 This function will call search function for different keys.
 Pre: BST and hashtable objects, selection
 Post: None
*~**/
void searchManager(BinarySearchTree<Course, ClassList> &bst, HashTable<ClassList> &hash,
																					  char choice)
{
	if(choice == 'P')
		searchByCrn(hash);
	if(choice == 'S')
		searchByCode(bst, hash);
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 If CRN is found the node in hashtable and corresponded BST node will be
 displayed, otherwise print "not found" message on screen.
 Pre: Hashtable object
 Post: Display target node or "not found"
*~**/
void searchByCrn(HashTable<ClassList> &hash)
{
	cout << "\n\n\t\t[Search by CRN]\n\n";

	// Get target
	string  crn;
	cout << "\t\tEnter an CRN: ";
	getline(cin, crn, '\n');

	// Set up target to base object
	ClassList temp, searched;
	temp.setCRN(crn);

	// Search target in hash table
	if(!hash.search(searched, temp, key_to_index))
		 cout << "\n\t\t=== Not found ===\n";
	else
		 vDisplay(searched);
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 If class code is found the node in hashtable and corresponded BST node
 will be displayed, otherwise print "not found" message on screen. The node
 with same class code will also be displayed.
 Pre: Hashtable and BST object
 Post: Display target node or "not found"
*~**/
void searchByCode(BinarySearchTree<Course, ClassList> &bst, HashTable<ClassList> &hash)
{
	cout << "\n\n\t\t[Search by Code]\n\n";

	// Get target
	string  code;
	cout << "\t\tEnter a class code: ";
	getline(cin, code, '\n');

	// Set up target to base object
	ClassList temp, searched;
	temp.setCode(code);

	// Search target in BST
	if(!bst.search(temp, searched))
		cout << "\n\t\t=== Not found ===\n";
	else
		bst.displayDuplicate(temp, hash, zDisplay);
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 Displaying class list by ascending order of class code.
 Pre: Hashtable and BST object
 Post: Display class list as table
*~**/
void displayList(BinarySearchTree<Course, ClassList> &bst, HashTable<ClassList> &hash)
{
	cout << "\n\t\t[Display List]\n\n";

	// Print header
	cout <<  "\t\t========================================================"
		  <<  "============================================================\n";
	cout <<	"\t\t  CRN   Code        Title                               "
		  <<  "\t\t Instructor             Time\n";
	cout <<  "\t\t========================================================"
		  <<  "============================================================\n";

	// Print list in BST inorder
	bst.inOrder(hDisplay, hash);

	// Print Bottom
	cout <<  "\t\t========================================================"
	     <<  "============================================================\n";
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 Write out the input data into output file and update the output file name.
 Pre: Hashtable object
 Post: Create an output file
*~**/
void writeData(HashTable<ClassList> &hash)
{
	// Set up output file name
	static int count = 1;
	string outputFileName;
	outputFileName = "ClassList_output_" + to_string(count) + ".txt";

	// Create output file
	ofstream fout;
	fout.open(outputFileName.c_str());

	// Write out data
   hash.hpDisplay(fout);
   cout << "\n\t\t\t\t\t\t\t\t\t\t===Write out success===\n";

	count++;
	fout.close();
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 Display statistics about hashtable
 Pre: Hashtable object
 Post: Display statistics
*~**/
void displayStatistics(HashTable<ClassList> &hash)
{
	cout << "\n\t\t[Statistic about hashtable]\n\n";
	cout << "\t\tLoad factor: " << setprecision(2) << fixed
		  << hash.getLoadFactor() << "%" << endl
		  << "\t\tTotal number of collisions: "
		  << hash.totCollision() << " time(s)" << endl
		  << "\t\tLongest collision path: "
		  << hash.longestPath() << " time(s)" << endl << endl;
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 Display developers name when hidden menu is activated.
 Pre: None
 Post: Display the name of developers
*~**/
void displayDevelopers()
{
	cout  << "\n\t\t ==== This program is developed by ====\n"
			<< "\t\t\tHuichan Cheng\tXinyi Guo\tAnran Luo\n\n";
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 Horizontal display: all items on one line
 Pre: A course object and hashtable object
 Post: Print out data members in Item as a line
*~**/
void hDisplay(Course &item, HashTable<ClassList> &hash)
{
	ClassList temp, searched;
	temp.setCRN(item.getCRN());
	hash.search(searched, temp, key_to_index);
	cout <<  left;
	cout <<  "\t\t ";
	cout <<  setw(2)  << searched.getCRN() << "  ";
	cout <<  setw(10) << searched.getCode() << "  ";
	cout <<  setw(51) << searched.getTitle() << "  ";
	cout <<  setw(21) << searched.getInstructor() << "  ";
	cout <<  setw(1)  << searched.getTime() << "  ";
	cout <<  endl;
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 Hvertical display: one data member per line
 Pre: A ClassList object
 Post: Print out data members in Item
*~**/
void vDisplay(ClassList &item)
{
	cout << endl;
	cout << "\t\tCRN: " << item.getCRN() << endl;
	cout << "\t\tCode: " << item.getCode() << endl;
	cout << "\t\tInstructor: " << item.getInstructor() << endl;
	cout << "\t\tTime: " << item.getTime() << endl;
	cout << "\t\tTextbook: " << item.getTextbook() << endl;
	cout << "\t\tRate: " << setprecision(2) << fixed
								<< item.getRate() << endl << endl;
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 Vertical display with search: one data member per line
 Pre: A course object and hashtable object
 Post: Print out data members in Item
*~**/
void zDisplay(Course &item, HashTable<ClassList> &hash)
{
	ClassList temp, searched;
	temp.setCRN(item.getCRN());
	hash.search(searched, temp, key_to_index);
	cout << endl << endl;
	cout << "\t\tCRN: " << searched.getCRN() << endl;
	cout << "\t\tCode: " << searched.getCode() << endl;
	cout << "\t\tInstructor: " << searched.getInstructor() << endl;
	cout << "\t\tTime: " << searched.getTime() << endl;
	cout << "\t\tTextbook: " << searched.getTextbook() << endl;
	cout << "\t\tRate: " << setprecision(2) << fixed
								<< searched.getRate() << endl;
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 Indented tree display: one item per line, including the level number
 Pre: Course object and number of level in tree
 Post: Print out class code with indented dot and number of level
*~**/
void iDisplay(Course &item, int level)
{
	cout << "\t\t";
	for (int i = 1; i < level; i++)
		cout << "..";
	cout << level << "). " << item.getCode() << endl;
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 Hash function: takes the key and returns the index in the hash table
 Written By: Huichan Cheng
 Pre: ClassList object and size of hashtable
 Post: index in hashtable
*~**/
int key_to_index(const ClassList &key, int size)
{
	int k = stoi(key.getCRN());
	return k % size;
};

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 Compare CRN between Course objects
 Pre: Course objects
 Post: result of comparison
*~**/
int compareByCRN(Course left, const Course &target)
{
	if(left.getCRN() < target.getCRN())
        return 1;
	if(left.getCRN() > target.getCRN())
        return -1;
    return 0;
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 Obtain number of lines in file -- support creating size of hashtable
 Written By: Huichan Cheng
 Pre: file pointer
 Post: number of lines in input file
*~**/
int numOfLines(ifstream &inFile)
{
	int count = 0;
	string temp;

	while (getline(inFile, temp))
		count++;

	inFile.clear();
	inFile.seekg(0, ios::beg);

	return count;
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 Find the next prime number of n -- support creating size of hashtable
 call isPrime function.
 Written By: Huichan Cheng
 Pre: integer n
 Post: Next prime number for n
*~**/
int findNextPrime(int n)
{
	if (n <= 1)
		return 2;

	bool found = false;

	while (!found)
	{
		n++;
		if (isPrime(n))
			found = true;
	}
	return n;
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 Determine a number is prime -- support creating size of hashtable
 Written By: Huichan Cheng
 Pre: integer nn
 Post: result about nn
*~**/
int isPrime(int nn)
{
	if (nn < 2)
		return false;

	for (int i = 2; i <= ceil(sqrt(nn)); i++)
	{
		if (nn % i == 0 && nn != i)
			return false;
	}
	return true;
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 Print out welcome message and introduction about this programme.
 Pre: None
 Post: Print out welcome message on screen
*~**/
void welcome()
{
	cout << "\t\t*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~" << endl;
	cout << "\t\t\tWelcome to De Anza Computer Science Courses Database" 					 << endl;
	cout << "\t\t*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~" << endl;
	cout << "\t\tThis project is integrating the function of course register related websites" << endl
		  << "\t\twhich De Anza students usually use. For instance, when searching for a class" << endl
	     << "\t\tby CRN or class code, The basic information about class will be printed out"  << endl
		  << "\t\ton the screen. Such as, lecture time, name of the instructor, the rating on"  << endl
		  << "\t\tclass, and related textbook information." 								 << endl;
	cout << "\t\t*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~" << endl;
	cout << endl;
}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 Print out farewell message and name of developers and instructor.
 Pre: None
 Post: Print out farewell message on screen
*~**/
void farewell()
{
	cout << endl << endl;
	cout << "\t\t*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~" << endl;
	cout << "\t\t\tThank you for using De Anza Computer Science Courses Database" 				 << endl;
	cout << "\t\t*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~" << endl;
	cout << "\t\t\tThis programme is developed by Anran Luo, Huichan Cheng, Xinyi Guo" 		 << endl
		  << "\t\t\tSpeical Thanks:  Our instructor -- Delia Garbacea" 						 << endl
		  << "\t\t\tHave a great day and please check the output file" 						 << endl;
	cout << "\t\t*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~" << endl;
	cout << endl;

}

/*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~
 Test for a string is only contain digit characters
 Pre: A string variable
 Post: Return true if there is all digit chatacters, otherwise return false
*~**/
bool isParam(string crn)
{
	for(int i = 0; i < crn.length(); i++)
		if(!isdigit(crn[i]))
			return false;
	return true;
}
