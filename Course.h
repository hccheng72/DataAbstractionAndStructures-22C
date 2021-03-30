// Specification file for the College class
// Written By: A. Student
// Changed By: Anran Luo, Huichan Cheng
// IDE: Xcode

#ifndef COURSE_H
#define COURSE_H

#include <string>
using std::string;

class Course
{
protected:
	string    crn;
	string   code;

public:
	//constructors
	Course() { crn = ""; code = ""; }
	Course(string c, string cd) { crn = c; code = cd; }

	//setters
	void setCRN(string c) { crn = c; }
	void setCode(string tt) { code = tt; }

	//getters
	string getCRN() const { return crn; }
	string getCode() const { return code; }

	// overloaded operators
	virtual bool operator<(const Course &);
	virtual bool operator>(const Course &);
	virtual bool operator== (const Course &);
};

//***********************************************************
// Compare the node data with < and return true or false
//***********************************************************
bool Course::operator< (const Course &right)
{
	if (code < right.code)
		return true;
	else
		return false;
}

//***********************************************************
// Compare the node data with > and return true or false
//***********************************************************
bool Course::operator> (const Course &right)
{
	if (code > right.code)
		return true;
	else
		return false;
}

//***********************************************************
// Compare the node data with == and return true or false
//***********************************************************
bool Course::operator== (const Course & right)
{
	if (code == right.code)
		return true;
	else
		return false;
}

#endif
