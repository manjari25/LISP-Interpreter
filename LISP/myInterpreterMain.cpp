/* MANJARI AKELLA */

#include "myTokenize.h"
#include "myEvaluator.h"
#include<iostream>
#include<string>
#include<stdlib.h>

using namespace std;


int main()
{
	myTokenize mT;
	myParse mP;
	myEvaluator mE;
	vector<string> tok;	
	char ch;
	string fullinput;
	string str;
	int countp;
	unsigned int position=0;	
	
	while(!cin.eof())
	{
		getline(cin,str);
		fullinput = fullinput+str;
	}
	//cout<<fullinput.size()<<"\n";
	//ensure only top level expression sent each time to the tokenizer-parser-evaluator phases
	while(position<fullinput.size())
	{
		string input;
		char f = fullinput[position];
		//if first char of new top level expression is a space continue till a digit/alphabet/+/-/(/) before terminating the top level expression
		if((f==' ')||(f=='\t'))
		{
			++position;
			f=fullinput[position];
			while((isdigit(f)==0)&&(isalpha(f)==0)&&(f!='+')&&(f!='-')&&(f!='(')&&(f!=')'))
			{
				++position;
				f=fullinput[position];
				continue;
			}
		}
		//if first char is a (
		if(f=='(')
		{
			input.append(1,f);
			++position;
			ch=fullinput[position];
			countp=1;
			//break when '(' and ')' become equal
			while(position<fullinput.size())
			{

				if(ch=='(')
				{
					countp++;
					input.append(1,ch);
					++position;
					ch=fullinput[position];
				}
				else if(ch==')')
				{
					countp--;
					input.append(1,ch);
					if((countp==0))
						break;
					++position;
					ch=fullinput[position];
				}
				else 
				{
					input.append(1,ch);
					++position;
					ch=fullinput[position];
				}
			}
			++position;
		}
		//if first char is a ), look for next white space before terminating the top level expression
		else if(f==')')
		{
			while((f!=' ')&&(f!='\t'))
			{
				input.append(1,f);
				++position;
				f=fullinput[position];
				continue;
			}

		}
		//if first char is a digit/alphabet/+/-/., look for next white space before terminating the top level expression
		else if((isdigit(f)!=0)||(isalpha(f)!=0)||(f=='+')||(f=='-')||(f=='.'))
		{
			input.append(1,f);
			++position;
			f=fullinput[position];
			while((f!=' ')&&(f!='\t'))
			{
				input.append(1,f);
				++position;
				f=fullinput[position];
				continue;
			}
		
		}
		//cout<<input<<" "<<position<<"\n";
		//tokenize the top level expression
		//string input = "(int 2)";
		string s = mT.insertSpaces(input);
		mT.createTokens(s);
		tok = mT.getTokens();
		//parse the top level expression
		mySexp *p = mP.parseTokens(tok);
		//evaluate the top level expression
		mySexp *finalans = mE.eval(p);
		//error in evaluation 
		if(finalans==NULL)
		{
			exit(0);
		}
		//else display output
		else
			finalans->print();
		std::cout<<"\n";
	}//look for next top level expression
	return 0;
}
