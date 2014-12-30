/* MANJARI AKELLA */

#include "mySexp.h"
#include<iostream>
using namespace std;

mySexp::mySexp(void)
{
	this->left=NULL;
	this->right=NULL;
}
//for numeric atom
mySexp::mySexp(int a)
{
	this->val=a;
	this->type=2;
	this->left=NULL;
	this->right=NULL;
}
//for literal atom(keyword or identifier)
mySexp::mySexp(string s)
{
	this->sval=s;
	this->type=1;
	this->left=NULL;
	this->right=NULL;
}
//to cons 2 s-expressions
mySexp::mySexp(mySexp* s1,mySexp* s2)
{
	this->type = 0;
	this->left = s1;
	this->right= s2;
} 

//S-Expression functions
//primitives
mySexp* mySexp :: car()
{
	if(this->type == 0)
		return this->left;
	else 
	{
		cout << "ERROR: CAR undefined for atoms\n";
		return NULL;
	}
}
mySexp* mySexp :: cdr()
{
	if(this->type == 0)
		return this->right;
	else 
	{
		cout << "ERROR: CDR undefined for atoms\n";
		return NULL;
	}
}
mySexp* mySexp :: cons(mySexp* s1)
{
	return new mySexp(this, s1);
}
bool mySexp :: isAtom()
{
	if((this->type == 1) || (this->type == 2))
		return true;
	else
		return false;
}
int mySexp :: isEq(mySexp* s1)
{
	if(this->type != 0 && s1->type != 0) 
	{
		if(this->type == 1 && s1->type == 1) 
		{
			if(this->sval == s1->sval)
				return 1;
			else
				return -1;
		}	
		else if(this->type == 2 && s1->type == 2) 
		{
			if(this->val == s1->val)
				return 1;
			else
				return -1;
		}
		else 
		{	
			cout << "ERROR: EQ defined only for ATOMS of same type\n";
			return 0;
		}
	}
	else 
	{	
		cout << "ERROR: EQ undefined for this type";
		return 0;
	}
}
bool mySexp :: isNull()
{
	if((this->type == 1) && (this->sval == "NIL"))
		return true;
	else
		return false;
}
bool mySexp :: isInt()
{
	if(this->type == 2)
		return true;
	else
		return false;
}

//utilities
bool  mySexp :: isIdentifier()
{
	if(this->type == 1)
		return true;
	else
		return false;
}
bool  mySexp :: isKeyword()
{
	if(this->type == 1)
	{
		if( this->sval == "T" || this->sval == "NIL" || this->sval == "CAR" || this->sval == "CDR" || this->sval == "CONS" ||
			this->sval == "ATOM" || this->sval == "EQ" || this->sval == "NULL" || this->sval == "INT" || this->sval == "PLUS" ||
			this->sval == "MINUS" || this->sval == "TIMES" ||this->sval == "QUOTIENT" || this->sval == "REMAINDER" || 
			this->sval == "LESS" || this->sval == "GREATER" || this->sval == "COND" || this->sval == "QUOTE" || this->sval == "DEFUN" )
			return true;
		else
			return false;
	}
	else
		return false;
}
bool  mySexp :: isList()
{
	if(this->isNull()) 
		return true;
	else if(this->isAtom()) 
		return false;
	else
		return this->cdr()->isList();
}
void  mySexp :: printList()
{
	cout << "(";
	mySexp* s1 = this;
	while(s1->type == 0) 
	{
		if(s1->car()->isAtom()) 
		{
			cout << " ";
			s1->car()->printSexp();
			cout << " ";
		}
		else 
		{
			s1->car()->print();
		}
		s1 = s1->cdr();
	}
	cout << ")";
}
void  mySexp :: printSexp()
{
	if(type == 0) 
	{
		cout << "(";
		left->print();
		cout << " . ";
		right->print();
		cout << ")";
	}
	if(type == 1)
	{
			cout << sval;
	}
	if(type == 2)
	{
		cout << val;
	}
}
void  mySexp :: print()
{
	if(isList() && !this->isNull())
		printList();
	else
		printSexp();
}
void mySexp :: parent(mySexp *s2, mySexp *s3)
{
	this->type = 0;
	this->left = s2;
	this->right= s3;
}
void mySexp :: setVal(string s)
{
	this->sval=s;
	if(s=="s"||s=="e"||s=="y")
	{
		this->type=2;
	}
	else
	{
		this->type=1;
	}
}	
void mySexp :: setVal(int a)
{
		this->sval="";
		this->val=a;
		this->type=2;
}
	
//arithmetic 
mySexp*  mySexp :: plus(mySexp* s1)
{
	if((this->type == 2) && (s1->type == 2))
	{
		return new mySexp(this->val + s1->val);
	}
	else 
	{
		cout << "ERROR: PLUS defined only for numeric atoms\n";
		return NULL;
	}
}
mySexp*  mySexp :: minus(mySexp* s1)
{
	if((this->type == 2) && (s1->type == 2))
	{
		return new mySexp(this->val - s1->val);
	}
	else 
	{
		cout << "ERROR: MINUS defined only for numeric atoms\n";
		return NULL;
	}
}
mySexp*  mySexp :: times(mySexp* s1)
{
	if((this->type == 2) && (s1->type == 2))
	{
		return new mySexp(this->val * s1->val);
	}
	else 
	{
		cout << "ERROR: TIMES defined only for numeric atoms\n";
		return NULL;
	}
}
mySexp*  mySexp :: quotient(mySexp* s1)
{
	if((this->type == 2) && (s1->type == 2))
	{
		return new mySexp(this->val / s1->val);
	}
	else 
	{
		cout << "ERROR: QUOTIENT defined only for numeric atoms\n";
		return NULL;
	}
}
mySexp*  mySexp :: remainder(mySexp* s1)
{
	if((this->type == 2) && (s1->type == 2))
	{
		return new mySexp(this->val % s1->val);
	}
	else 
	{
		cout << "ERROR: REMAINDER defined only for numeric atoms\n";
		return NULL;
	}
}

//relational
int  mySexp :: less(mySexp* s1)
{
	if(this->type == 2 && s1->type == 2) 
	{
		if(this->val < s1->val)
			return 1;
		else
			return -1;
	}
	else 
	{	
		cout << "ERROR: LESS undefined for this type";
		return 0;
	}
}
int  mySexp :: greater(mySexp* s1)
{
	if(this->type == 2 && s1->type == 2) 
	{
		if(this->val < s1->val)
			return 1;
		else
			return -1;
	}
	else 
	{	
		cout << "ERROR: GREATER undefined for this type";
		return 0;
	}
}

mySexp::~mySexp(void)
{
}
