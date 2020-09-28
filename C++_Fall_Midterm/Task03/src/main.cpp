#include "std_lib_facilities.h"
#include <fstream>
#include <iostream>
#include <chrono>

void swapMethod01(vector<double> A, vector<double> B)
{
    vector<double> tmp = A;
    A = B;
    B = tmp;
}

// You should modify this one:
void swapMethod02(vector<double>& A, vector<double>& B)
{
    vector<double> tmp = A;
    A = B;
    B = tmp;
}

int main()
{
    cout << "Task 3:" << '\n';
	//output file
	ofstream outputFile;
	outputFile.open ("../../output/result.txt");
	outputFile << left << setw(15) << "Time used by swapMethod01: " << "\t" << setw(15) << "Time used by swapMethod02: "
	           << "\n";

    vector<double> dataSetA(10000000);
    vector<double> dataSetB(10000000);

    fill(dataSetA.begin(), dataSetA.end(), 1.0); // fill A with 1.0's
    fill(dataSetB.begin(), dataSetB.end(), 2.0); // fill B with 2.0's

    // Calculate & output the time used to run swapMethod01
    auto start = chrono::system_clock::now();
    for (int i = 0; i < 50; i++) {
        swapMethod01(dataSetA, dataSetB);
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> elapsedSeconds = end - start;
    cout << "Time used by swapMethod01: " << elapsedSeconds.count() << "s" << endl;

    // Calculate & output the time used to run swapMethod02
    start = chrono::system_clock::now();
    for (int i = 0; i < 50; i++) {
        swapMethod02(dataSetA, dataSetB);
    }
    end = chrono::system_clock::now();
    elapsedSeconds = end - start;
    cout << "Time used by swapMethod02: " << elapsedSeconds.count() << "s" << endl;


		outputFile << left << setw( 15) << elapsedSeconds.count() << "\t" << setw(15) << elapsedSeconds.count() << "\n";

		outputFile.close();

    keep_window_open();

    return 0;
}
