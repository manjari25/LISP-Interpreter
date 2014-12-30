/* MANJARI AKELLA */

#include "myEvaluator.h"

myEvaluator::myEvaluator(void)
{
	//create s-expressions for all primitives
	t = mp.createSexp("T");
	nil = mp.createSexp("NIL");
	car = mp.createSexp("CAR");
	cdr = mp.createSexp("CDR");
	cons = mp.createSexp("CONS");
	atom = mp.createSexp("ATOM");
	eq = mp.createSexp("EQ");
	null = mp.createSexp("NULL");
	_int = mp.createSexp("INT");
	plus = mp.createSexp("PLUS");
	minus = mp.createSexp("MINUS");
	times = mp.createSexp("TIMES");
	quotient = mp.createSexp("QUOTIENT");
	remainder = mp.createSexp("REMAINDER");
	less = mp.createSexp("LESS");
	greater = mp.createSexp("GREATER");
	cond = mp.createSexp("COND");
	quote = mp.createSexp("QUOTE");
	defun = mp.createSexp("DEFUN");
	//also set dlist to NIL
	dlist=nil;
}

mySexp* myEvaluator :: eval(mySexp* s1)
{
	return eval(s1, nil);
}
mySexp* myEvaluator :: eval(mySexp* s1, mySexp* alist)
{
	//is it an atom
	if(s1->isAtom()) 
	{
		//is it just an INT atom
		if(s1->isInt()) 
		{
			return s1;
		}	
		//is it the atom T
		 if(s1->isEq(t) == 1) 
		{
			return t;
		}
		 //is it the atom NIL
		else if(s1->isEq(nil) == 1) 
		{
			return nil;
		}
		//is it a bound atom, check in alist
		else if(bound(s1, alist)) 
		{
			return getVal(s1, alist);
		}
		//if none of the above, error
		else 
		{
			cout << "ERROR: Unbound Variable\n";
			return NULL;
		}
	}
	//is it a list
	else if(s1->car()->isIdentifier()) 
	{
		//is it quote
		if(s1->car()->isEq(quote) == 1) 
		{
			//check syntax of QUOTE (one argument only)
			if(s1->cdr()->isNull()) 
			{
				cout << "ERROR: Missing arguments for QUOTE\n";
				return NULL;
			}
			else if(s1->cdr()->cdr()->isNull()) 
			{
				return s1->cdr()->car();
			}
			else 
			{
				cout << "ERROR: Only one argument allowed for QUOTE\n";
				return NULL;
			}
		}
		//is it cond
		else if(s1->car()->isEq(cond) == 1) 
		{
			//call helper evcon
			mySexp* evconans = evcon(s1->cdr(), alist);
			if(evconans == NULL)							
				return NULL;
			return evconans;
		}
		//is it defun, if so add to dlist 
		else if(s1->car()->isEq(defun) == 1) 
		{	// check defun before adding to dlist
			/* FUNCTION NAME CHECK */
			//function name missing
			if(s1->cdr()->isNull()) {
				cout << "ERROR: Function name missing\n";
				return NULL;
			}
			//function name is not an atom
			else if(!s1->cdr()->car()->isAtom())
			{
				cout << "ERROR: Function name must be a literal atom\n";
				return NULL;
			}
			//function name is a keyword
			else if(s1->cdr()->car()->isKeyword()!=0)
			{
				cout << "ERROR: Function name cannot be a keyword\n";
				return NULL;
			}
			//function name is an integer
			else if(s1->cdr()->car()->isInt()) 
			{
				cout << "ERROR: Function name should be an identifier\n";
				return NULL;
			}
			/* FUNCTION PARAMETERS CHECK */
			//list of parameters missing
			else if(s1->cdr()->cdr()->isNull()) 
			{
				cout << "ERROR: Function parameters missing\n";
				return NULL;
			}
			//parameters not specifed as list
			else if(!s1->cdr()->cdr()->car()->isList()) 
			{
				cout << "ERROR: Function parameters must be specified as a list\n";
				return NULL;
			}
			//parameters not distinct
			else if(!isDistinct(s1->cdr()->cdr()->car())) 
			{
				cout << "ERROR: Function parameters must be distinct\n";
				return NULL;
			}
			/* FUNCTION BODY CHECK */
			//function body missing
			else if(s1->cdr()->cdr()->cdr()->isNull()) 
			{
				cout << "ERROR: Function body missing\n";
				return NULL;
			}
			//if everything is fine, entry into dlist
			mySexp* funcname = s1->cdr()->car();
			mySexp* parm = s1->cdr()->cdr()->car();
			mySexp* body = s1->cdr()->cdr()->cdr()->car();			
			mySexp* entry = funcname->cons(parm->cons(body));
			dlist = entry->cons(dlist);
			//return created function name
			return funcname;
		}
		//function call, call helper apply
		else
		{							
			mySexp* evlistans = evlist(s1->cdr(), alist);
			if(evlistans == NULL)
				return NULL;
			return apply(s1->car(), evlistans, alist);
		}
	}
	//if none of the above, error
	else 
	{
		cout << "ERROR: Invalid function call ";
		return NULL;
	}
}
bool myEvaluator :: bound(mySexp* var, mySexp* adlist)
{
	//binding not found, var not in alist/dlist
	if(adlist->isNull())	
	{						
		return false;
	}
	//binding found, var in alist
	if(adlist->car()->car()->isEq(var) == 1) 
	{
		return true;
	}
	return bound(var, adlist->cdr());
}
mySexp* myEvaluator :: getVal(mySexp* var, mySexp* adlist)
{
	//couldnt't find var in adlist
	if(adlist->isNull())	
	{						
		return nil;
	}
	//recursively check adlist for var
	if(adlist->car()->car()->isEq(var) == 1) 
	{	
		return adlist->car()->cdr();
	}
	return getVal(var, adlist->cdr());
}
mySexp* myEvaluator :: evcon(mySexp* x, mySexp* alist)
{
	if(x->isNull()) 
	{
		cout << "ERROR: No conditions for COND\n";
		return NULL;
	}
	mySexp* evalans = eval(x->car()->car(), alist);
	if(evalans == NULL)								
		return NULL;
	else if(evalans->isEq(t) == 1) 
		return eval(x->car()->cdr()->car(), alist);
	return evcon(x->cdr(), alist);	
}
mySexp* myEvaluator :: evlist(mySexp* x, mySexp* alist)
{
	if(x->isNull()) 
		return nil;

	if(!x->isList()) 
	{						
		cout << "ERROR: evlist() evaluates only a list\n";
		return NULL;
	}

	mySexp* evans = eval(x->car(), alist);
	if(evans == NULL) 
	{
		return NULL;
	}
	mySexp* evlans = evlist(x->cdr(), alist);
	if(evlans == NULL)							
		return NULL;

	return evans->cons(evlans);
}
mySexp* myEvaluator :: apply(mySexp* f, mySexp* x, mySexp* alist)
{
		if(f->isIdentifier() && x->isList()) 
		{
			//PRIMITIVES
			//car
			if(f->isEq(car) == 1) 
			{
				if(!x->isNull()) 
				{
					if(!x->car()->isAtom()) 
					{
						if(x->cdr()->isNull())
							return x->car()->car();
						else 
						{
							cout << "ERROR: CAR takes only one argument";
							return NULL;
						}
					}
					else 
					{
						cout << "ERROR: CAR is not defined for atoms";
						return NULL;
					}
				}
				else 
				{
					cout << "ERROR: Missing arguments for CAR";
					return NULL;
				}
			}
			//cdr
			else if(f->isEq(cdr) == 1) 
			{
				if(!x->isNull()) 
				{
					if(!x->car()->isAtom()) 
					{
						if(x->cdr()->isNull())
							return x->car()->cdr();
						else 
						{
							cout << "ERROR: CDR takes only one argument";
							return NULL;
						}
					}
					else 
					{
						cout << "ERROR: CDR is not defined for atoms";
						return NULL;
					}
				}
				else 
				{
					cout << "ERROR: Missing arguments for CDR";
					return NULL;
				}
			}
			//cons
			else if(f->isEq(cons) == 1) 
			{
				if(!x->isNull()) 
				{
					if(!x->cdr()->isNull()) 
					{
						if(x->cdr()->cdr()->isNull())
							return x->car()->cons(x->cdr()->car());
						else 
						{
							cout << "ERROR: CONS takes only two arguments";
							return NULL;
						}
					}
					else 
					{
						cout << "ERROR: CONS takes two arguments";
						return NULL;
					}
				}
				else 
				{
					cout << "ERROR: Missing arguments for CONS";
					return NULL;
				}
			}
			//atom
			else if(f->isEq(atom) == 1) 
			{
				if(!x->isNull()) 
				{
					if(x->cdr()->isNull()) 
					{
						if(x->car()->isAtom())
							return t;
						else
							return nil;
					}
					else 
					{
						cout << "ERROR: ATOM takes only one argument";
						return NULL;
					}
				}
				else 
				{
					cout << "ERROR: Missing arguments for ATOM";
					return NULL;
				}
			}
			//equal
			else if(f->isEq(eq) == 1) 
			{
				if(!x->isNull()) 
				{
					if(!x->cdr()->isNull()) 
					{
						if(x->cdr()->cdr()->isNull()) 
						{
							int rv = x->car()->isEq(x->cdr()->car());
							if(rv == 1)
								return t;
							else if(rv == -1)
								return nil;
							else			// error 
								return NULL;
						}
						else 
						{
							cout << "ERROR: EQ takes only two arguments";
							return NULL;
						}
					}
					else 
					{
						cout << "ERROR: EQ requires two arguments";
						return NULL;
					}
				}
				else 
				{
					cout << "ERROR: Missing arguments for EQ";
					return NULL;
				}
			}
			//int
			else if(f->isEq(_int) == 1) 
			{
				if(!x->isNull()) 
				{
					if(x->cdr()->isNull()) 
					{
						if(x->car()->isInt())
							return t;
						else
							return nil;
					}
					else 
					{
						cout << "ERROR: INT takes only one argument";
						return NULL;
					}
				}
				else 
				{
					cout << "ERROR: Missing arguments for INT";
					return NULL;
				}
			}
			//null
			else if(f->isEq(null) == 1) 
			{
				if(!x->isNull()) 
				{
					if(x->cdr()->isNull()) 
					{
						if(x->car()->isNull())
							return t;
						else
							return nil;
					}
					else 
					{
						cout << "ERROR: NULL takes only one argument";
						return NULL;
					}
				}
				else 
				{
					cout << "ERROR: Missing arguments for NULL";
					return NULL;
				}
			}
			//ARITHMETIC
			//plus
			else if(f->isEq(plus) == 1) 
			{
				if(!x->isNull()) 
				{
					if(!x->cdr()->isNull()) 
					{
						if(x->cdr()->cdr()->isNull()) 
						{
							return x->car()->plus(x->cdr()->car());
						}
						else 
						{
							cout << "ERROR: PLUS takes only two arguments";
							return NULL;
						}
					}
					else 
					{
						cout << "ERROR: PLUS requires two arguments";
						return NULL;
					}
				}
				else
				{
					cout << "ERROR: Missing arguments for PLUS";
					return NULL;
				}
			}
			//minus
			else if(f->isEq(minus) == 1) 
			{
				if(!x->isNull()) 
				{
					if(!x->cdr()->isNull()) 
					{
						if(x->cdr()->cdr()->isNull()) 
						{
							return x->car()->minus(x->cdr()->car());
						}
						else 
						{
							cout << "ERROR: MINUS takes only two arguments";
							return NULL;
						}
					}
					else 
					{
						cout << "ERROR: MINUS requires two arguments";
						return NULL;
					}
				}
				else 
				{
					cout << "ERROR: Missing arguments for MINUS";
					return NULL;
				}
			}
			//times
			else if(f->isEq(times) == 1) 
			{
				if(!x->isNull()) 
				{
					if(!x->cdr()->isNull()) 
					{
						if(x->cdr()->cdr()->isNull()) 
						{
							return x->car()->times(x->cdr()->car());
						}
						else 
						{
							cout << "ERROR: TIMES takes only two arguments";
							return NULL;
						}
					}
					else 
					{
						cout << "ERROR: TIMES requires two arguments";
						return NULL;
					}
				}
				else 
				{
					cout << "ERROR: Missing arguments for TIMES";
					return NULL;
				}
			}
			//quotient
			else if(f->isEq(quotient) == 1) 
			{
				if(!x->isNull()) 
				{
					if(!x->cdr()->isNull()) 
					{
						if(x->cdr()->cdr()->isNull()) 
						{
							return x->car()->quotient(x->cdr()->car());
						}
						else 
						{
							cout << "ERROR: QUOTIENT takes only two arguments";
							return NULL;
						}
					}
					else 
					{
						cout << "ERROR: QUOTIENT requires two arguments";
						return NULL;
					}
				}
				else 
				{
					cout << "ERROR: Missing arguments for QUOTIENT";
					return NULL;
				}
			}
			//remainder
			else if(f->isEq(remainder) == 1) 
			{
				if(!x->isNull()) 
				{
					if(!x->cdr()->isNull()) 
					{
						if(x->cdr()->cdr()->isNull()) 
						{
							return x->car()->remainder(x->cdr()->car());
						}
						else 
						{
							cout << "ERROR: REMAINDER takes only two arguments";
							return NULL;
						}
					}
					else 
					{
						cout << "ERROR: REMAINDER requires two arguments";
						return NULL;
					}
				}
				else 
				{
					cout << "ERROR: Missing arguments for REMAINDER";
					return NULL;
				}
			}
			//RELATIONAL
			//less
			else if(f->isEq(less) == 1) 
			{
				if(!x->isNull()) 
				{
					if(!x->cdr()->isNull()) 
					{
						if(x->cdr()->cdr()->isNull()) 
						{
							int ans = x->car()->less(x->cdr()->car());
							if(ans == 1)
								return t;
							else if(ans == -1)
								return nil;
							else			
								return NULL;
						}
						else 
						{
							cout << "ERROR: LESS takes only two arguments";
							return NULL;
						}
					}
					else 
					{
						cout << "ERROR: LESS requires two arguments";
						return NULL;
					}
				}
				else 
				{
					cout << "ERROR: Missing arguments for LESS";
					return NULL;
				}
			}
			//greater
			else if(f->isEq(greater) == 1) 
			{
				if(!x->isNull()) 
				{
					if(!x->cdr()->isNull()) 
					{
						if(x->cdr()->cdr()->isNull()) 
						{
							int ans = x->car()->greater(x->cdr()->car());
							if(ans == 1)
								return t;
							else if(ans == -1)
								return nil;
							else			
								return NULL;
						}
						else 
						{
							cout << "ERROR: GREATER takes only two arguments";
							return NULL;
						}
					}
				else 
				{
					cout << "ERROR: GREATER requires two arguments";
					return NULL;
				}
			}
			else 
			{
				cout << "ERROR: Missing arguments for GREATER";
				return NULL;
			}
		}
		//user defined function
		else 
		{
			mySexp* defn = getVal(f, dlist);
			if(defn->isNull()) 
			{
				cout << "ERROR: Undefined function\n";
				return NULL;
			}
			mySexp* body = defn->cdr();
			mySexp* parameters = defn->car();
			alist = addPairs(parameters, x, alist);
			if(alist == NULL)
				return NULL;
	
			return eval(body, alist);
		}
	}
	//if none of the above, error
	else 
	{
		cout << "ERROR: Invalid function call\n";
		return NULL;
	}
}
mySexp* myEvaluator :: addPairs(mySexp* xlist, mySexp* ylist, mySexp* alist)
{
	if(xlist->isNull()) 
	{
		if(ylist->isNull())
			return alist;
		else 
		{
			cout << "ERROR: ylist is NIL";
			return NULL;
		}
	}
	//xlist and ylist must be lists
	if(!xlist->isAtom() && !ylist->isAtom()) 
	{		
		mySexp* newentry = xlist->car()->cons(ylist->car());
		alist = newentry->cons(alist);				
	}
	else 
	{
		cout << "ERROR: xlist and ylist in addpairs() must be lists\n";
		return NULL;
	}
	return addPairs(xlist->cdr(), ylist->cdr(), alist);
}
bool myEvaluator :: isDistinct(mySexp* s)
{
	mySexp* s1=s;
	while(s1->cdr()->isNull()!=1)
	{
		while(s->cdr()->isNull()!=1)
		{
			if((s1->car()->isEq(s->cdr()->car()))==1)
				return false;
			else
				s=s->cdr();
		}
		s1=s1->cdr();
		s=s1;
	}
	return true;
}

myEvaluator::~myEvaluator(void)
{
}
