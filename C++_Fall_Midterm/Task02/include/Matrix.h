#pragma once

#include "std_lib_facilities.h"

class Matrix {
private:
    vector<vector<int>> m_data;
    int m_rows;
    int m_columns;

public:
    Matrix(int rows, int columns);
    Matrix(const vector<vector<int>>& data);

    int rows() const;
    int columns() const;
    int position(int row, int column) const;
    int& position(int row, int column);
		Matrix operator-(Matrix &m);
		Matrix operator*(int const& scalar);
		Matrix operator+(Matrix const &m);
};

ostream& operator<<(ostream& os, const Matrix& m);
