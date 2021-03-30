// Specification file for the College class
// Written By: A. Student
// Changed By: Anran Luo
// IDE: Xcode

#ifndef CLASSLIST_H
#define CLASSLIST_H

#include<string>
#include <iomanip>
#include <fstream>
#include "Course.h"

using namespace std;

class ClassList: public Course
{
protected:
	string title;
	string instructor;
	string time;
	string textbook;
	double rate;

public:
	//constructors
	ClassList() : Course() { title = ""; instructor = ""; time = "";
														textbook = ""; rate = -1; }
	ClassList(string c, string tt, string nm, string it, string dt,
				 							 string tb, double rt) : Course(c, tt)
	{ title = nm; instructor = it; time = dt; textbook = tb; rate = rt; }

	//setters
	void setTitle(string n) { title = n; }
	void setInstructor(string it) { instructor = it; }
	void setTime(string t) { time = t; }
	void setTextbook(string tb) { textbook = tb; }
	void setRate(double rt) { rate = rt; }

	//getters
	string getTitle() const { return title; }
	string getInstructor() const { return instructor; }
	string getTime() const { return time; }
	string getTextbook() const { return textbook; }
	double getRate() const { return rate; }

	//other functions
   void fileOut(ofstream &) const;

	// overloaded operators
	bool operator<(const ClassList &);
	bool operator>(const ClassList &);
	bool operator== (const ClassList &);
};

//***********************************************************
// Write out the values of the ClassList object member
// variables on one line (Horizontal display)
//***********************************************************
void ClassList::fileOut(ofstream &fout) const
{
    fout << crn << ';';
    fout << code << ';';
    fout << title << ';';
    fout << instructor << ';';
    fout << time << ';';
    fout << textbook << ';';
    fout << setprecision(2) << fixed << rate << '\n';
}

//***********************************************************
// Compare the node data with < and return true or false
//***********************************************************
bool ClassList::operator< (const ClassList &right)
{
	if (crn < right.crn)
		return true;
	else
		return false;
}

//***********************************************************
// Compare the node data with > and return true or false
//***********************************************************
bool ClassList::operator> (const ClassList &right)
{
	if (crn > right.crn)
		return true;
	else
		return false;
}

//***********************************************************
// Compare the node data with == and return true or false
//***********************************************************
bool ClassList::operator== (const ClassList & right)
{
	if (crn == right.crn)
		return true;
	else
		return false;
}
#endif
