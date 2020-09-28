#include "EuropeanOption.h"
#include <map>

using namespace std;
using namespace utilities;


void lineParser(vector<string> & outputLine, string inputLine)
{
	vector<string> tokenized = strSplit(inputLine, ' ');
	for(size_t i = 0; i < tokenized.size(); i++)
	{
		if(!tokenized[i].empty())
			outputLine.push_back(tokenized[i]);
	}
}

int main() {

	string line;
	size_t numOfCols;
	vector<string> schema;
	bool firstLine = true;
	ifstream inputFile("../../Input/Options.txt");
  ofstream outputFile;
  outputFile.open ("../../Output/outputFile.txt");
	if (inputFile.is_open()) {
		while (getline(inputFile, line, '\r')) {
			if(firstLine) {
				lineParser(schema, line);
				firstLine = false;
				numOfCols = schema.size() + 2;
				for(auto& each : schema) outputFile << left << setw(18) << each;
				outputFile << left << setw(18) << "Vol(Bisec)"
				           << left << setw(18) << "Vol(Secant)" << endl;
			}else{
				string optType;
				vector<string> resRow;
				map<string, double> m;
				lineParser(resRow, line);
				for(size_t i = 0; i < resRow.size(); i++){
					if(i == 0) {
						vector<string> tempVec = strSplit(resRow[0], '\n');
						optType = (tempVec.size() == 2) ? tempVec[1] : tempVec[0];
					}else
						m.insert(pair<string, double>(schema[i], stod(resRow[i])));
				}
				EuropeanOption option(optType, m["Spot"], m["Strike"], m["Rate"], m["DaysToMaturity"]/365.);
			  double volBisec = option.getImpliedVol((m["Bid"] + m["Ask"]) / 2.,"bisection", 0, 0.5);
				double volSec = option.getImpliedVol((m["Bid"] + m["Ask"]) / 2.,"secant", 0, 0.5);
				for(size_t i = 0; i < numOfCols; i++) {
					if(i == 0) {
						outputFile << left << setw(18) << optType;
					}else if(i == numOfCols - 2) {
						outputFile << left << setw(18) << volBisec;
					}else if(i == numOfCols - 1){
						outputFile << left << setw(18) << volSec;
					}else outputFile << left << setw(18) << m[schema[i]];
				}
				outputFile << endl;
			}
		}
	}
	inputFile.close();
	outputFile.close();
}

