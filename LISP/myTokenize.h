/* MANJARI AKELLA */

#include<vector>
#include<string>
using namespace std;

class myTokenize
{
	vector<string> tokens;
	vector<string>::iterator current;
public:
	myTokenize(void);
	string insertSpaces(string& input);
	void createTokens(string& spacesInput);
	vector<string> getTokens();
	~myTokenize(void);
};

