/* MANJARI AKELLA */

#include "myParse.h"
#include<iostream>
#include<stdlib.h>
using namespace std;

string rule(string s1, string s2)
{
	string s;
	for(int i=0;i<5;i++)
	{
		if(!s1.compare(nont[i]))
		{
			for(int j=0;j<4;j++)
			{
				if(!s2.compare(term[j]))
				{
					s=parsetable[i][j];
					return s;
				}
			}
		}
	}
}

myParse::myParse(void)
{
}

mySexp* myParse :: parseTokens(vector<string> tok)
{
	//parse tokens by reducing using production rules
	//use treehelper to store pointers of the tree
	//stack
	helper.push("s");
	//insert into tree
	mySexp* s1 = new mySexp("");
	treehelper.push(s1);
	unsigned int pos=0;
	while(pos<tok.size())
	{
		//cout<<pos<<"\n";
		bool flag=false;
		for(int i=0;i<5;i++)
		{
			if(!helper.top().compare(nont[i]))
			{
				flag=true;
				break;
			}
		}
		if(flag==true)
		{
			string str;
			if ((tok[pos]=="(")||(tok[pos]==")")||(tok[pos]=="."))
				str=rule(helper.top(),tok[pos]);
			else
				str = rule(helper.top(),"atom");
			//cout<<"Rule: "<<helper.top()<<"->"<<str<<"\n";
			if(str=="")
			{
				//cout<<"str=empty\n";
				cout<<"ERROR: Invalid Expression. Parse tree cannot be generated\n";
				exit(0);
			}
			else if (!str.compare("atom"))
			{
				//insert into tree(check if token is literal or numeric)
				if((tok[pos][0]=='+')||(tok[pos][0]=='-')||(isdigit(tok[pos][0])!=0))
				{
					int value = atoi(tok[pos].c_str());
					treehelper.top()->setVal(value);
				}
				else
				{
					treehelper.top()->setVal(tok[pos]);
				}
				treehelper.pop();
				helper.pop();
				helper.push(tok[pos]);
			}
			else
			{
				//insert into tree
				helper.pop();
				str = string ( str.rbegin(), str.rend());
				for(int i=0;str[i]!='\0';i++)
				{
					helper.push(string(1,str[i]));
				}
				if(!(str.compare("ye"))||!(str.compare("re")))
				{
					mySexp* s2 = new mySexp("");
					mySexp* s3 = new mySexp("");
					treehelper.top()->parent(s2,s3);
					treehelper.pop();
					treehelper.push(s3);
					treehelper.push(s2);
				}
			}
		}
		else
		{
			if(!helper.top().compare("l"))
			{
				helper.pop();
				//set tree node to NIL
				treehelper.top()->setVal("NIL");
				treehelper.pop();
			}
			if(!helper.top().compare(tok[pos]))
			{
				helper.pop();
				pos++;
			}
		}
	}
	if(!helper.empty())
	{
			//cout<<"helper.empty()\n";
			cout<<"ERROR: Invalid Expression. Parse tree cannot be generated\n";
			exit(0);
	}
	//cout<<"String parsed successfully. Proceeding for evaluation.\n";
	return s1;
}
mySexp* myParse :: createSexp(string s)
{
	return (new mySexp(s));
}
mySexp* myParse :: createSexp(int a)
{
	return (new mySexp(a));
}

myParse::~myParse(void)
{
}
