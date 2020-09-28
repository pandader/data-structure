#include "utilities.h"
#include <fstream>

using namespace mathUtils;

int main()
{
	// configs
	int numDist = 5;
	int sampleSize = 10000;

	// distribution type with parameters vector
  vector<string> distTypes = {"Uniform", "Normal","Poisson","Binomial","Gamma"};
  vector<vector<double>> parametersVec = {{0, 1}, {1, 1.5}, {4}, {4, 0.5}, {1, 2}};

  // output file
	ofstream outputFile;
	outputFile.open ("../../output/outputFile.txt");
	outputFile << left << setw(15) << "Dist Type" << "\t" << setw(15) << "Mean"
	           << "\t" << setw(15) << "S.T.D"  << "\t" << setw(15) << "Median" << "\n";
	// loop through dist
	for(int i = 0; i < numDist; i++){
    vector<double> tempSample = randomGen(parametersVec[i], sampleSize, distTypes[i]);
    vector<double> res = statistics(tempSample);
	  outputFile << left << setw( 15) << distTypes[i] << "\t" << setw(15) << res[0]
	             << "\t" << setw(15) << res[1]  << "\t" << setw(15)<< res[2] << "\n";
  }

	outputFile.close();
}






