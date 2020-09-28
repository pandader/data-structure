#include <string>
#include <fstream>
#include "iostream"
#include "money.h"

using namespace std;
using namespace utilities;

int main() {

	string line;
	ifstream myFile("../../input/inputFile.txt");
	if (myFile.is_open()) {
		while (getline(myFile, line)) {
			vector<string> tempVec;
			vector<string> tokenize = strSplit(line, ' ');
			money m1(tokenize[0]);
			cout << tokenize[0] << " " << tokenize[1] << " " << tokenize[2] << " = ";
			if (tokenize[1] == "+") {
				money m2(tokenize[2]);
				cout << m1 + m2 << endl;
			} else if (tokenize[1] == "-") {
				money m2(tokenize[2]);
				cout << m1 - m2 << endl;
			} else if (tokenize[1] == "*") {
				tempVec = strSplit(tokenize[2], '.');
				cout << m1 * ((tempVec.size() == 2) ? stod(tokenize[2]) : stoi(tokenize[2])) << endl;
			} else if (tokenize[1] == "/") {
				tempVec = strSplit(tokenize[2], '.');
				cout << m1 / ((tempVec.size() == 2) ? stod(tokenize[2]) : stoi(tokenize[2])) << endl;
			} else if (tokenize[1] == "==") {
				money m2(tokenize[2]);
				cout << (m1 == m2) << endl;
			} else if (tokenize[1] == "!=") {
				money m2(tokenize[2]);
				cout << (m1 != m2) << endl;
			}
		}
	}

	// last test of >> operator
	money m3;
	cout << "Please enter:" << endl;
	cin >> m3;
	cout << m3;

	myFile.close();

}








































































