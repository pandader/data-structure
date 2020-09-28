#include "std_lib_facilities.h"

#include <string>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

namespace mathUtils{

		vector<double> randomGen(vector<double> const& parameters, int const& sampleSize, string const& distType);

		vector<double> statistics(vector<double> & sample);

}