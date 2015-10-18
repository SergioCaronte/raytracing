#ifndef MATH_MATRIX_H
#define MATH_MATRIX_H

#include "math.h"

#include <cstring>
#include <new>
#include <stdexcept>
#include <cfloat>
#include <string>
#include <sstream>

template<size_t NumRows, size_t NumColumns, typename DataType = float>
class Matrix {
    /// The data of the matrix.
    DataType _data[NumRows][NumColumns];

public:

	inline void identity()
	{
		 for(size_t i = 0; i < getNumRows(); ++i) 
		{
            for(size_t j = 0; j < getNumColumns(); ++j) 
			{
				if(i == j)
					_data[i][j] = DataType(1);
				else
					_data[i][j] = DataType(0);
            }
        }
	}

	inline void fill(DataType dt)
	{
		 for(size_t i = 0; i < getNumRows(); ++i) 
		{
            for(size_t j = 0; j < getNumColumns(); ++j) 
			{
                _data[i][j] = dt;
            }
        }
	}

    inline DataType *operator[](size_t index) 
	{
        return _data[index];
    }


    inline const DataType *operator[](size_t index) const 
	{
        return _data[index];
    }

   
    inline DataType &at(size_t row, size_t column) 
	{
        if(row >= NumRows || column >= NumColumns)
            throw std::out_of_range("Trying to access invalid matrix element");
        return _data[row][column];
    }

    inline const DataType &at(size_t row, size_t column) const 
	{
        if(row >= NumRows || column >= NumColumns)
            throw std::out_of_range("Trying to access invalid matrix element");
        return _data[row][column];
    }

    inline size_t getNumRows()
	{
        return NumRows;
    }

    inline size_t getNumColumns() 
	{
        return NumColumns;
    }

    Matrix &operator+=(const Matrix &right) 
	{
        if((getNumRows() != right.getNumRows())
                || getNumColumns() != right.getNumColumns()) {
            throw std::runtime_error("Invalid matrix size");
        }

        for(size_t i = 0; i < getNumRows(); ++i) {
            for(size_t j = 0; j < getNumColumns(); ++j) {
                _data[i][j] += right[i][j];
            }
        }
    }


    Matrix &operator-=(const Matrix &right) 
	{
        if((getNumRows() != right.getNumRows())
                || getNumColumns() != right.getNumColumns()) 
		{
            throw std::runtime_error("Invalid matrix size");
        }

        for(size_t i = 0; i < getNumRows(); ++i) 
		{
            for(size_t j = 0; j < getNumColumns(); ++j) 
			{
                _data[i][j] -= right[i][j];
            }
        }
    }

    Matrix &operator*=(float right)
	{
        for(size_t i = 0; i < getNumRows(); ++i) 
		{
            for(size_t j = 0; j < getNumColumns(); ++j) 
			{
                _data[i][j] *= right;
            }
        }
    }


    Matrix &operator/=(float right) 
	{
        if(std::abs(right) < FLT_EPSILON)
            throw std::runtime_error("Division by zero");

        for(size_t i = 0; i < getNumRows(); ++i) 
		{
            for(size_t j = 0; j < getNumColumns(); ++j) 
			{
                _data[i][j] /= right;
            }
        }
    }


    Matrix &operator*=(const Matrix &right)
	{
        if(getNumRows() != right.getNumColumns()
                || getNumColumns() != right.getNumRows())
            throw std::runtime_error("Invalid matrix size");

        for(size_t i = 0; i < getNumRows(); ++i)
            for(size_t j = 0; j < right.getNumColumns(); ++j)
                for(size_t k = 0; k < getNumColumns(); ++k)
                    _data[i][k] *= right[k][j];
    }

	std::string print() 
	{
		std::stringstream stream;
		for(size_t i = 0; i < getNumRows(); ++i)
		{
			for(size_t j = 0; j < getNumColumns(); ++j)
			{
				stream << _data[i][j] << "\t";
			}
			stream << "\n";
		}
		return stream.str();
	}
};

typedef Matrix<4,4,float> Matrix4x4;


template<int a, int b, int c>
Matrix<a, c> operator*( Matrix<a, b> &left,  Matrix<b, c> &right) 
{
    Matrix<a, c> m;
	m.fill(0);

    for(size_t i = 0; i < left.getNumRows(); ++i)
        for(size_t j = 0; j < right.getNumColumns(); ++j)
            for(size_t k = 0; k < left.getNumColumns(); ++k)
                m[i][k] += left[i][k] * right[k][j];

    return m;
}

#endif 
