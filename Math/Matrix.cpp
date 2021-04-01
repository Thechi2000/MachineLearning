#include "Matrix.h"
#include "macro.hpp"
#include <string>
#include "math.h"

#define _size unsigned int width, unsigned int height
#define _pos unsigned int i, unsigned int j

// TODO Define methods

math::Matrix::Matrix(_size, double** values) :
    m_values(values),
    m_width(width),
    m_height(height)
{
}

double** math::Matrix::new_array(_size)
{
    double** array = NEWARR(double*, height);
    for_in_range(i, 0, height)
        array[i] = NEWARR(double, width);
    return array;
}
void math::Matrix::delete_array()
{
    if (m_values == nullptr) return;

    for_in_range(i, 0, m_height)
        DELARR(m_values[i]);
    DELARR(m_values);
}

math::Matrix::Matrix() :
    Matrix(0, 0)
{
}
math::Matrix::Matrix(const Matrix& other) :
    Matrix(other.width(), other.height(), other.m_values)
{
}
math::Matrix::Matrix(Matrix&& other) noexcept :
    Matrix(other.width(), other.height(), other.m_values)
{
    other.m_values = nullptr;
}
math::Matrix::~Matrix()
{
        delete_array();
}

math::Matrix::Matrix(_size) :
    m_values(width && height ? new_array(width, height) : nullptr),
    m_width(width),
    m_height(height)
{
}
math::Matrix::Matrix(_size, double initialValue) :
    Matrix(width, height)
{
    for_in_range(i, 0, height)
        for_in_range(j, 0, width)
        at(i, j) = initialValue;
}
math::Matrix::Matrix(_size, double* initialValuesArray) :
    m_values(new_array(width, height)),
    m_width(width),
    m_height(height)
{
    for_in_range(i, 0, m_height)
        for_in_range(j, 0, m_width)
        m_values[i][j] = initialValuesArray[i * m_width + j];
}

math::Matrix math::Matrix::random(_size)
{
    Matrix m(width, height);

    for_in_range(i, 0, height)
        for_in_range(j, 0, width)
        m.at(i, j) = 1.0 / (rand() % 9 + 1);

    return m;
}

double& math::Matrix::at(_pos)
{
    return m_values[i][j];
}
double& math::Matrix::operator()(_pos)
{
    return m_values[i][j];
}
double math::Matrix::get(_pos) const
{
    return m_values[i][j];
}
void math::Matrix::set(_pos, double val)
{
    m_values[i][j] = val;
}

unsigned int math::Matrix::width() const
{
    return m_width;
}
unsigned int math::Matrix::height() const
{
    return m_height;
}

math::Matrix math::Matrix::subMatrix(_pos, _size)
{
    double** array = NEWARR(double*, height);
    for_in_range(k, 0, height)
        array[k] = m_values[i + k];
    return Matrix(width, height, array);
}
math::Matrix math::Matrix::subMatrixConst(_pos, _size) const
{
    Matrix m(width, height);

    for_in_range(k, 0, height)
        for_in_range(l, 0, width)
        m(k, l) = get(i + k, j + l);

    return m;
}

void math::Matrix::multiplyLine(unsigned int i, double factor)
{
    for_in_range(j, 0, m_width)
        at(i, j) *= factor;
}
void math::Matrix::swapLines(unsigned int i1, unsigned int i2)
{
    double* temp = m_values[i1];
    m_values[i1] = m_values[i2];
    m_values[i2] = temp;
}
void math::Matrix::combineLines(unsigned int lineToModify, unsigned int lineToBeAdded, double factor)
{
    for_in_range(j, 0, m_width)
        at(lineToModify, j) += at(lineToBeAdded, j) * factor;
}

void math::Matrix::multiplyColumn(unsigned int j, double factor)
{
    for_in_range(i, 0, m_height)
        at(i, j) *= factor;
}
void math::Matrix::swapColumns(unsigned int j1, unsigned int j2)
{
    for_in_range(i, 0, m_height)
    {
        double temp = at(i, j1);
        at(i, j2) = at(i, j1);
        at(i, j2) = temp;
    }
}
void math::Matrix::combineColumns(unsigned int columnToModify, unsigned int columnToBeAdded, double factor)
{
    for_in_range(i, 0, m_height)
        at(i, columnToModify) += at(i, columnToBeAdded) * factor;
}

double math::Matrix::determinant() const
{
    Matrix m(*this);
    double det = 1;

    for_in_range(i, 0, m.height())
    {
        if (m.get(i, i) == 0)
        {
            for_in_range(k, i + 1, m.height())
                if (m.get(k, i) != 0)
                {
                    m.swapLines(i, k);
                    det = -det;
                    break;
                }
        }

        if (m.get(i, i) != 0)
        {
            for_in_range(k, i + 1, m.height())
                m.combineLines(k, i, -m.get(k, i) / m.get(i, i));
        }
        else
            return 0;
    }

    for_in_range(i, 0, m_height)
        det *= m.get(i, i);

    return det;
}

void math::Matrix::applySigmoid()
{
    for_in_range(i, 0, m_height)
        for_in_range(j, 0, m_width)
        at(i, j) = sigm(at(i, j));
}

bool math::Matrix::isSquare() const
{
    return m_width == m_height;
}
bool math::Matrix::isDiagonal() const
{
    for_in_range(i, 0, m_height)
        for_in_range(j, 0, m_width)
            if (get(i, j) != 0 && i != j)
                return false;
    return true;
}
bool math::Matrix::isUpperTriangular() const
{
    for_in_range(i, 1, m_height)
        for_in_range(j, 0, i)
            if (get(i, j) != 0)
                return false;
    return true;
}
bool math::Matrix::isBottomTriangular() const
{
    for_in_range(j, 1, m_width)
        for_in_range(i, 0, j)
        if (get(i, j) != 0)
            return false;
    return true;
}
bool math::Matrix::isSymetric() const
{
    for_in_range(i, 1, m_height)
        for_in_range(j, 0, i)
        if (get(i, j) != get(j, i))
            return false;
    return true;
}
bool math::Matrix::isNull() const
{
    for_in_range(i, 0, m_height)
        for_in_range(j, 0, m_width)
            if (!get(i, j))
                return false;
    return true;
}

void math::Matrix::transpose()
{
    for_in_range(i, 1, m_height)
        for_in_range(j, 0, i)
    {
        double temp = get(i, j);
        at(i, j) = at(j, i);
        at(j, i) = temp;
    }
}
void math::Matrix::echelon()
{
    for_in_range(i, 0, m_height)
    {
        if (get(i, i) == 0)
        {
            for_in_range(k, i + 1, m_height)
                if (get(k, i) != 0)
                    swapLines(i, k);
        }

        if (get(i, i) != 0)
        {
            for_in_range(k, i + 1, m_height)
                combineLines(k, i, -get(k, i) / get(i, i));
        }
    }
}
void math::Matrix::echelonAndReduce()
{
    echelon();
    for_in_range(i, 0, m_height)
        multiplyLine(i, 1 / get(i, i));
}
void math::Matrix::invert()
{
}

math::Matrix math::Matrix::getTransposed() const
{
    Matrix m(*this);
    m.transpose();
    return m;
}
math::Matrix math::Matrix::getEchelonned() const
{
    Matrix m(*this);
    m.echelon();
    return m;
}
math::Matrix math::Matrix::getEchelonnedAndReducted() const
{
    Matrix m(*this);
    m.echelonAndReduce();
    return m;
}
math::Matrix math::Matrix::getInverse() const
{
    // TODO
    return Matrix();
}

math::Matrix& math::Matrix::operator=(const Matrix& other)
{
    delete_array();

    m_values = new_array(other.width(), other.height());
    m_width = other.width();
    m_height = other.height();

    for_in_range(i, 0, m_width)
        for_in_range(j, 0, m_height)
            at(i, j) = other.get(i, j);

    return *this;
}
math::Matrix& math::Matrix::operator=(Matrix&& other) noexcept
{
    delete_array();

    m_values = other.m_values;
    m_width = other.m_width;
    m_height = other.m_height;

    other.m_values = nullptr;

    return *this;
}

math::Matrix& math::Matrix::operator+=(const Matrix& other)
{
    THROW_IF(m_width != other.width() || m_height != other.height());

    for_in_range(i, 0, m_height)
        for_in_range(j, 0, m_width)
        at(i, j) += other.get(i, j);

    return *this;
}
math::Matrix& math::Matrix::operator-=(const Matrix& other)
{
    THROW_IF(m_width != other.width() || m_height != other.height());

    for_in_range(i, 0, m_height)
        for_in_range(j, 0, m_width)
        at(i, j) += other.get(i, j);

    return *this;
}

math::Matrix& math::Matrix::operator*=(double lambda)
{
    for_in_range(i, 0, m_height)
        for_in_range(j, 0, m_width)
        at(i, j) *= lambda;

    return *this;
}
math::Matrix& math::Matrix::operator/=(double lambda)
{
    for_in_range(i, 0, m_height)
        for_in_range(j, 0, m_width)
        at(i, j) /= lambda;

    return *this;
}

math::Matrix math::operator+(const Matrix& a, const Matrix& b)
{
    Matrix m(a);
    m += b;
    return m;
}
math::Matrix math::operator-(const Matrix& a, const Matrix& b)
{
    Matrix m(a);
    m -= b;
    return m;
}

math::Matrix math::operator*(const Matrix& a, const Matrix& b)
{
    THROW_IF(a.width() != b.height());

    Matrix m(b.width(), a.height());

    for_in_range(i, 0, m.height())
        for_in_range(j, 0, m.width())
    {
        double val = 0;
        for_in_range(k, 0, a.width())
            val += a.get(i, k) * b.get(k, j);
        m(i, j) = val;
    }

    return m;
}
math::Matrix math::operator*(const Matrix& matrix, double lambda)
{
    Matrix m(matrix);
    m *= lambda;
    return m;
}
math::Matrix math::operator*(double lambda, const Matrix& matrix)
{
    return matrix * lambda;
}

math::Matrix math::operator/(const Matrix& matrix, double lambda)
{
    Matrix m(matrix);
    m /= lambda;
    return m;
}
math::Matrix math::operator/(double lambda, const Matrix& matrix)
{
    return matrix / lambda;
}

std::ostream& math::operator<<(std::ostream& os, const Matrix& m)
{
    for_in_range(i, 0, m.height()) 
    {
        for_in_range(j, 0, m.width())
            os << m.get(i, j) << ' ';

        os << std::endl;
    }

    return os;
}
