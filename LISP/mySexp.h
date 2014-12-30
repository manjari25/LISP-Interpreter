/* MANJARI AKELLA */

#include<string>
#include<iostream>
using namespace std;

class mySexp
{
	//string value for literal atom
	string sval;
	//integer value for numeric atom
	int val;
	//type of node in tree(0= tree internal node,1=literal atom,2=numeric atom)
	int type;
	//left pointer
	mySexp* left;
	//right pointer
	mySexp* right;
public:
	//constructors and destructor
	mySexp(void);
	//for numeric atom
	mySexp(int);
	//for literal atom(keyword or identifier)
	mySexp(string);
	//new S-expression with left and right children
	mySexp(mySexp*,mySexp*);
	~mySexp(void);

	//S-Expression functions
	//primitives
	mySexp* car();
	mySexp* cdr();
	mySexp* cons(mySexp*);
	bool isAtom();
	int isEq(mySexp*);
	bool isNull();
	bool isInt();

	//utilities
	bool isIdentifier();
	bool isKeyword();
	bool isList();
	void printList();
	void printSexp();
	void print();
	void parent(mySexp*, mySexp*);
	void setVal(string);
	void setVal(int);
		
	//arithmetic 
	mySexp* plus(mySexp*);
	mySexp* minus(mySexp*);
	mySexp* times(mySexp*);
	mySexp* quotient(mySexp*);
	mySexp* remainder(mySexp*);
	
	//relational
	int less(mySexp*);
	int greater(mySexp*);
};
