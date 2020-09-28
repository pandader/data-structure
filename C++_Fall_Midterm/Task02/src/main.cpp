#include "Matrix.h"
#include "std_lib_facilities.h"

int main()
{
    cout << "Task 2:" << '\n';

    vector<vector<int>> A = {
        { 1, 5, 9, 4, 12 },
        { 7, 12, 3, 5, 8 },
        { 8, 4, 15, 9, 5 },
        { 6, 3, 12, 8, 1 }
    };

    vector<vector<int>> B = {
        { 3, 2, 9, 8 },
        { 1, 2, 3, 4 },
        { 9, 8, 7, 6 },
        { 2, 3, 4, 5 },
        { 9, 7, 5, 3 }
    };

    Matrix MA{ A };
    Matrix MB{ B };

    cout << MA << '\n';
    cout << MB << '\n';

    keep_window_open();

    return 0;
}
