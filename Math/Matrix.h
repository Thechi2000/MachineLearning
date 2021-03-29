#pragma once

#include "GarbageCollector.h"	

#define _size unsigned int width, unsigned int height
#define _pos unsigned int i, unsigned int j

namespace math
{
	class Matrix
	{
	private:
		double** m_values;
		unsigned int m_width, m_height;

		// Private simple constructor
		Matrix(_size, double** values, bool copy = false);

		static double** new_array(_size);
		static void delete_array(double** array);

	public:
		// Redefinition of classic constructors and destructor
		Matrix();
		Matrix(const Matrix& other);
		Matrix(Matrix&& other);
		~Matrix();

		// Basic constructors
		Matrix(_size);
		Matrix(_size, double initialValue);
		Matrix(_size, double* initialValuesArray);

		// Elements getters and setters
		double& at(_pos);
		double& operator()(_pos);
		double get(_pos) const;
		void set(_pos, double val);

		// Size getters
		unsigned int width() const;
		unsigned int height() const;

		// Submatrix getter
		Matrix subMatrix(_pos, _size);
		Matrix subMatrixConst(_pos, _size) const;

		// Line operations
		void multiplyLine(unsigned int line, double factor);
		void swapLines(unsigned int line1, unsigned int line2);
		void combineLines(unsigned int lineToModify, unsigned int lineToBeAdded, double factor);

		// Column operations
		void multiplyColumn(unsigned int column, double factor);
		void swapColumns(unsigned int column1, unsigned int column2);
		void combineColumns(unsigned int columnToModify, unsigned int columnToBeAdded, double factor);

		// Basic calculations
		double determinant() const;

		// Assignement operators
		Matrix& operator=(const Matrix& other);
		Matrix& operator=(Matrix&& other);

		// Arithmetic assigning operators
		Matrix& operator+=(const Matrix& other);
		Matrix& operator-=(const Matrix& other);
		Matrix& operator*=(double lamda);
		Matrix& operator/=(double lamda);
	};

	// Matrix Arithemetic operators
	Matrix operator+(const Matrix& a, const Matrix& b);
	Matrix operator-(const Matrix& a, const Matrix& b);

	Matrix operator*(const Matrix& a, const Matrix& b);
	Matrix operator*(const Matrix& matrix, double lamda);
	Matrix operator*(double lamda, const Matrix& matrix);

	Matrix operator/(const Matrix& matrix, double lamda);
	Matrix operator/(double lamda, const Matrix& matrix);
}

#undef _pos
#undef _size