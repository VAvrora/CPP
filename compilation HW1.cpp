/*Programm: building parse tree for simple add exersize
input: string as simple math add exersize
output: sorce code using stack for asm, ccp, java 
*/

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class parseTree {
public:
	parseTree* left = nullptr;
	parseTree*  right = nullptr;
	int value = 0;
	//True if number, False if plus
	bool typeOfNode = true;
};

//Recursion postorder function for ASM outcome
void toasm(parseTree* root)
{

	if (root == nullptr)
		return;

	toasm(root->left);
	toasm(root->right);
		
	if (root->typeOfNode)
		cout << "PUSH " << to_string(root->value) << endl;

	else
		cout << "ADD" << endl;
}


//Recursion postorder function for CPP outcome
void rec2(parseTree* root)
{
	if (root == nullptr)
			return;

	rec2(root->left);
	rec2(root->right);

	if (root->typeOfNode)
		cout << "\tstack.push(" << to_string(root->value) << ");" << endl;
	else
		cout << "\tstack.push(topAndRemove(stack) + topAndRemove(stack));" << endl;
}


//CPP code
void tocpp(parseTree* root)
{
	std::string mainstring1;
	mainstring1.append("\n\n/*CPP CODE*/\n#include <stack>\n#include <iostream>\n\nint topAndRemove(std::stack<int>& _stack)\n{\n\tint val = _stack.top();\n\t_stack.pop();\n\treturn val;\n}");
	mainstring1.append("\nint main()\n{\n\t// Initialization\n\tint top = 0;\n\tstd::stack<int> stack;\n\n\t// Generated code");

	std::string mainstring3("\n\t// Print result\n\tstd::cout << \"The result is: \" << stack.top() << std::endl;\n}\n\n");
	
	cout << mainstring1 <<  endl;
	//Call to recursion function
	rec2(root);
	cout << mainstring3 << endl;

}

//Recursion postorder function for JAVA outcome
void rec3(parseTree* root)
{
	if (root == nullptr)
		return;

	rec3(root->left);
	rec3(root->right);

	if (root->typeOfNode)
		cout << "\t\tstack.push(" << to_string(root->value) << ");" << endl;

	else
		cout << "\t\tstack.push(stack.pop() + stack.pop());" << endl;
}

//Java code
void tojava(parseTree* root)
{
	std::string mainstring1;
	mainstring1.append("/*JAVA CODE*/\nimport java.util.*;\nimport java.lang.*;\nimport java.io.*;\n\nclass TheoryOfCompilation\n{\n\t");
	mainstring1.append("public static void main(String[] args) throws java.lang.Exception\n\t\t{\n");
	mainstring1.append("\t\t// Stack declaration\n\t\tStack<Integer> stack = new Stack<Integer>();\n\n\t\t// Generated code");

	std::string mainstring3("\n\t\t// Print result\n\t\tSystem.out.println(\"The result is: \" + stack.peek());\n\t}\n}");
	
	cout << mainstring1 << endl;
	//Call to recursion function
	rec3(root);
	cout << mainstring3 << endl;
}


int main()
{
	parseTree* root = new (parseTree);
	parseTree* templeft = new (parseTree);;
	parseTree* tempright = new (parseTree);;

	std::string str;
	int tempNum = 0;
	cout << "Enter your string of sum of numbers" << endl;
	getline(cin,str);
	
	/*Building parse tree - left tree*/
	for (int i = 0; i<str.length(); i++)
	{
		//Check if digit
		if (str.at(i)>47) {
			//Check if there is more than one digit in number
			tempNum = tempNum * 10;
			tempNum = tempNum + (int(str.at(i) - 48));
		}
		else if (str.at(i) == '+')
		{
			//if first number
			if (root->left == nullptr)
			{
				templeft->value = tempNum;
				root->left = templeft;
				root->typeOfNode = false;
				tempNum = 0;
			}
			else
			{

				tempright->value = tempNum;
				root->right = tempright;
				root->typeOfNode = false;//plus note
				//Create new root 
				parseTree* temproot = new (parseTree); 
				temproot->typeOfNode = false; 				
				temproot->left = root;
				root = temproot;
				tempright = new (parseTree); //Create new right node
				tempNum = 0;
			}
		}
	}
	//Enter last number
	tempright->value = tempNum;
	root->right = tempright;
	root->typeOfNode = false;

	//For ASM outcome
	toasm(root);
	//For CPP outcome
	tocpp(root);
	//For JAVA outcome
	tojava(root);
}
