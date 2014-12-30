/* MANJARI AKELLA */

#include "myParse.h"
class myEvaluator
{
	mySexp* t;
	mySexp* nil;
	mySexp* car;
	mySexp* cdr;
	mySexp* cons;
	mySexp* atom;
	mySexp* eq;
	mySexp* null;
	mySexp* _int;
	mySexp* plus;
	mySexp* minus;
	mySexp* times;
	mySexp* quotient;
	mySexp* remainder;
	mySexp* less;
	mySexp* greater;
	mySexp* cond;
	mySexp* quote;
	mySexp* defun;
	myParse mp;
	mySexp* dlist;
public:
	myEvaluator(void);
	mySexp* eval(mySexp*);
	mySexp* eval(mySexp*, mySexp*);
	bool bound(mySexp*, mySexp*);
	mySexp* getVal(mySexp*, mySexp*);
	mySexp* evcon(mySexp*, mySexp*);
	mySexp* evlist(mySexp*, mySexp*);
	mySexp* apply(mySexp*, mySexp*, mySexp*);
	mySexp* addPairs(mySexp*, mySexp*, mySexp*);
	//checks if function parameters in defun are distinct
	bool isDistinct(mySexp*);
	~myEvaluator(void);
};

