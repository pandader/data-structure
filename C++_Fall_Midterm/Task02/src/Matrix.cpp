#include "Matrix.h"
#include "std_lib_facilities.h"

Matrix::Matrix(int rows, int columns)
    : m_rows{ rows }
    , m_columns{ columns }
{
    m_data = vector<vector<int>>(m_rows, vector<int>(m_columns));
}

Matrix::Matrix(const vector<vector<int>>& data)
{
    m_data = data;
    m_rows = m_data.size();
    m_columns = m_data[0].size();
}

int Matrix::rows() const
{
    return m_rows;
}

int Matrix::columns() const
{
    return m_columns;
}

int Matrix::position(int row, int column) const
{
    if (0 > row && m_rows <= row && 0 > column && m_columns <= column)
        error("out of bounds");

    return m_data[row][column];
}

int& Matrix::position(int row, int column)
{
    if (0 > row && m_rows <= row && 0 > column && m_columns <= column)
        error("out of bounds");

    return m_data[row][column];
}

Matrix Matrix::operator+(Matrix const &m) {
	vector<vector<int>> resM;
	if(m.rows() != m_rows || m.columns() != m_columns)
		throw std::invalid_argument("number of columns/rows of two matrices should be consistent!");
	for(size_t i = 0; i < m_rows; i++){

		vector<int> tempRow;

		for(size_t j = 0; j < m_columns; j++){

			tempRow.push_back(m.position(i, j) + m_data[i][j]);

		}

		resM.push_back(tempRow);

	}

	Matrix resMatrix(resM);

	return resMatrix;

}

Matrix Matrix::operator*(int const& scalar) {
	vector<vector<int>> resM;
	for(size_t i = 0; i < m_rows; i++){
		vector<int> tempRow;
		for(size_t j = 0; j < m_columns; j++){
			tempRow.push_back(m_data[i][j] * scalar);
		}
		resM.push_back(tempRow);
	}

	Matrix resMatrix(resM);

	return resMatrix;
}

Matrix Matrix::operator-(Matrix &m) {

	if(m.rows() != m_rows || m.columns() != m_columns)
		throw std::invalid_argument("number of columns/rows of two matrices should be consistent!");
	return this->operator+(m * (-1));
}



ostream& operator<<(ostream& os, const Matrix& m)
{
    for (int i = 0; i < m.rows(); ++i) {
        for (int j = 0; j < m.columns(); ++j) {
            os << m.position(i, j) << '\t';
        }
        if (i < m.rows() - 1) {
            os << '\n';
        }
    }

    return os;
}