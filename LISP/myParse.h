/* MANJARI AKELLA */

#include "mySexp.h"
#include<stack>
#include<string>
#include<queue>
using namespace std;

const string nont [5] = {"e","r","s","x","y"};
const string term [4] = {"(",")",".","atom"};
const string parsetable[5][4] = {{"(x","","","atom"},
					{"er","l","","er"},
					{"e","","","e"},
					{"ey",")","","ey"},
					{"r)","r)",".e)","r)"}};

string rule(string s1, string s2);

class myParse
{		
	stack<string> helper;
	stack<mySexp*> treehelper;
public:
	myParse(void);
    	mySexp*  parseTokens(vector<string>);
	mySexp* createSexp(string);
	mySexp* createSexp(int);
	~myParse(void);
};

