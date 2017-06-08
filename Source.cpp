/*Lecture 3.0
Building parse tree for simple math exersize by rulles:
S → E
E → E + T
E → E * T
E → T
T → id 
example for input: a+b*c
T+b*c
E+b*c
E+T*c
E*c
E*T
E
S
*/
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class parseTree {
public:
	parseTree* left = nullptr;
	parseTree*  right = nullptr;
	//zero if T, 1 if E, 2 if S
	int typeOfNode = 0;
	//For sign
	bool ifsign = false;
	char value = '0';
};



string PrintTree(parseTree* root, string str) 
{
	if (root == nullptr)
		return str;

	str = PrintTree(root->left, str);
	str = PrintTree(root->right, str);

	std::string temp;
	if (root->typeOfNode == 0)
	{
	//If first note
		if (str.at(0) != 'E' && str.at(0) != 'T')
		{
			str.at(0) = 'T';
			cout << str << endl;
		}
		else if (str.at(0) == 'E')
		{
			int i = str.find(root->value);
			str.at(i) = 'T';
			cout << str << endl;
		}
	}
	//If first E
	else if (str.at(0) == 'T')
	{
		str.at(0) = 'E';
		cout << str << endl;
	}
	//If sign
	else if (root->ifsign && str.length()>2)
	{
		int i = str.find('T');
		str.assign(str.begin() + i + 1, str.end());
		temp.append("E");
		temp.append(str);
		str = temp;
		cout << str << endl;
	}
	else
		cout << "S" << endl;
		return str;

}

//Building parse tree

int main()
{
	//Initializing
	parseTree* S = new (parseTree);
	S->typeOfNode = 2;
	
	parseTree* E = new (parseTree);
	E->typeOfNode = 1;

	parseTree* T = new (parseTree);

	//Input test flag
	bool inputTest = true;

	bool firstVal = true;

	std::string str;
	int tempNum = 0;
	cout << "Enter your string of sum of numbers" << endl;
	getline(cin, str);

	//Building parse tree
	for (int i = 0; i<str.length(); i++)
	{
		if (str.at(i) >= 97 && str.at(i) <= 122)
		{
			inputTest = true;
			//if first note value
			if (firstVal)
			{
				T->value = str.at(i);
				E->left = T;
				E->ifsign = false;
				firstVal = false;
			}
			else
			{
				parseTree* T = new (parseTree);
				T->value = str.at(i);
				E->right = T;
			}
			}
		else if (str.at(i) == '+' || str.at(i) == '*')
		{
			inputTest = false;
			parseTree* tempE = new (parseTree);
			tempE->typeOfNode = 1;
			tempE->ifsign = true;

			tempE->left = E;

			E = tempE;
		}
		else if (str.at(i) != ' ')
		{
			inputTest = false;
		}
		else {}
	}


	if (!inputTest)
	{
		cout << "Wrong Input" << endl;
		return 0;
	}

	else
	{
		S->left = E;
		str = PrintTree(S, str);
		cout << "FINISH" << endl;
	}
}

