/* MANJARI AKELLA */

#include "myTokenize.h"
#include <iostream>
#include <stdlib.h>
#include <cstring>

myTokenize::myTokenize(void)
{
}

string myTokenize:: insertSpaces(string& input)
{
		//count open and close parentheses	
		int countop=0;
		int countcp=0;
		for(unsigned int i=0;i<input.size();i++)
		{
			if((input[i]=='('))
				countop++;
			else if(input[i]==')')
				countcp++;
		}
		string str = string((input.size()+(2*countop)+(2*countcp)),' ');
		unsigned int cs=0;
		unsigned int ct=0;
		//Insert whitespace before and after a ( or )
		while(ct<input.size())
		{
			if(input[ct] == '(')
			{
					str[cs+1] = input[ct];
					cs+=3;
					ct+=1;
			}
			else if(input[ct] == ')')
			{
				str[cs+1] = input[ct];
				cs+=3;
				ct+=1;
			}
			else
			{
				str[cs]=input[ct];
				cs+=1;
				ct+=1;
			}
		}
return str;
}

void myTokenize::createTokens(string& spacesInput)
{
		tokens.clear();
		char *pch;
		char *s=new char[spacesInput.size()+1];
		s[spacesInput.size()]=0;
		memcpy(s,spacesInput.c_str(),spacesInput.size());
		//printf ("Splitting string \"%s\" into tokens:\n",s);
		pch = strtok (s, " \t");
		while (pch != NULL)
		{
			tokens.push_back(pch);
			//convert everything to uppercase
			for(unsigned int i=0;i<tokens.back().size();i++)
			{
				if(isalpha(tokens.back()[i])!=0)
				{
					char c = toupper(tokens.back()[i]);
					tokens.back()[i]=c;
				}
			}
			//std::cout<<"Token:"<<tokens.back()<<"\n";
			pch = strtok (NULL, " \t");
		}
		//Check validity of tokens
		for(unsigned int i=0;i<tokens.size();i++)
		{
			if((tokens[i]=="(")||(tokens[i]==")")||(tokens[i]=="."))
				continue;
			else
			//validity of atomic tokens
			{
				//dot or ) or ( token followed by anything is invalid
				if((tokens[i][0]=='(')||(tokens[i][0]==')')||(tokens[i][0]=='.'))
				{
					std::cout<<"ERROR: INVALID TOKEN\n";
					exit(0);
				}
				//starts with a +,- or digit
				else if ((tokens[i][0]=='+')||(tokens[i][0]=='-'))
				{
					if(tokens[i].size()==1)
					{
								std::cout<<"ERROR: INVALID TOKEN";
								exit(0);
					}
					//only digits allowed
					for(unsigned int j=1;j<tokens[i].size();j++)
					{
							if(isdigit(tokens[i][j])!=0)
								continue;
							else
							{
								std::cout<<"ERROR: INVALID TOKEN";
								exit(0);
							}
					}
				}
				else if(isdigit(tokens[i][0])!=0)
				{
					//only digits allowed
					for(unsigned int j=1;j<tokens[i].size();j++)
					{
							if(isdigit(tokens[i][j])!=0)
								continue;
							else
							{
								std::cout<<"ERROR: INVALID TOKEN";
								exit(0);
							}
				}
				}
				//starts with an alphabet
				else if(isalpha(tokens[i][0])!=0)
				{
					//only digit or alphabet allowed
					for(unsigned int j=1;j<tokens[i].size();j++)
					{
						if ((isalpha(tokens[i][j])!=0)||(isdigit(tokens[i][j])!=0))
							continue;
						else
						{
							std::cout<<"ERROR: INVALID TOKEN";
							exit(0);
						}
					}
				}
				//any other char is invalid
				else
				{
					std::cout<<"ERROR: INVALID TOKEN";
					exit(0);
				}
		}
	}
		//reset iterator to begin
		current = tokens.begin();
		//cout<<"All tokens valid. Proceeding with parsing\n";
}

vector<string> myTokenize :: getTokens() 
{ 
	return tokens; 
}

myTokenize::~myTokenize(void)
{
}
