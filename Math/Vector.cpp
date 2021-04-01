#include "Vector.h"
#include "macro.hpp"
#include "math.h"

using namespace math;

math::Vector::Vector() :
	m_values(nullptr),
	m_height(0)
{
}
math::Vector::Vector(const Vector& other) :
	m_values(NEWARR(double, other.height())),
	m_height(other.height())
{
	for_in_range(i, 0, other.height())
		m_values[i] = other.get(i);
}
math::Vector::Vector(Vector&& other) noexcept :
	m_values(other.m_values),
	m_height(other.height())
{
	other.m_values = nullptr;
}
math::Vector::~Vector()
{
	if (m_values != nullptr)
		DELARR(m_values);
}

math::Vector::Vector(ui height) :
	m_values(NEWARR(double, height)),
	m_height(height)
{
}
math::Vector::Vector(ui height, double initialValue) :
	Vector(height)
{
	for_in_range(i, 0, m_height)
		at(i) = initialValue;
}
math::Vector::Vector(ui height, double* values) :
	Vector(height)
{
	for_in_range(i, 0, m_height)
		m_values[i] = values[i];
}

Vector math::Vector::random(ui height)
{
	Vector v(height);
	for_in_range(i, 0, height)
		v[i] = 1.0 / (rand() % 9 + 1);
	return v;
}

double& math::Vector::at(ui pos)
{
	return m_values[pos];
}
double& math::Vector::operator[](ui pos)
{
	return m_values[pos];
}
void math::Vector::set(ui pos, double val)
{
	m_values[pos] = val;
}
double math::Vector::get(ui pos) const
{
	return m_values[pos];
}
ui math::Vector::height() const
{
	return m_height;
}

void math::Vector::applySigmoid()
{
	for_in_range(i, 0, m_height)
		m_values[i] = sigm(m_values[i]);
}

Vector& math::Vector::operator=(const Vector& other)
{
	DELARR(m_values);
	m_values = NEWARR(double, other.height());
	m_height = other.height();

	for_in_range(i, 0, m_height)
		m_values[i] = other.get(i);

	return *this;
}
Vector& math::Vector::operator=(Vector&& other) noexcept
{
	DELARR(m_values);
	m_values = other.m_values;
	m_height = other.m_height;
	other.m_values = nullptr;

	return *this;
}

Vector& math::Vector::operator+=(const Vector& other)
{
	THROW_IF(m_height != other.height());

	for_in_range(i, 0, m_height)
		at(i) += other.get(i);

	return *this;
}
Vector& math::Vector::operator-=(const Vector& other)
{
	THROW_IF(m_height != other.height());

	for_in_range(i, 0, m_height)
		at(i) -= other.get(i);

	return *this;
}

Vector& math::Vector::operator*=(double lambda)
{
	for_in_range(i, 0, m_height)
		at(i) *= lambda;

	return *this;
}
Vector& math::Vector::operator*=(const Matrix& matrix)
{
	THROW_IF(matrix.width() != m_height);

	double* values = NEWARR(double, matrix.height());

	for_in_range(i, 0, matrix.height())
	{
		double& val = values[i];

		val = 0;
		for_in_range(j, 0, m_height)
			val += matrix.get(i, j) * get(j);
	}

	DELARR(m_values);
	m_values = values;
	m_height = matrix.height();

	return *this;
}
Vector& math::Vector::operator/=(double lambda)
{
	for_in_range(i, 0, m_height)
		at(i) *= lambda;

	return *this;
}

Vector math::operator+(const Vector& a, const Vector& b)
{
	return Vector(a) += b;
}
Vector math::operator-(const Vector& a, const Vector& b)
{
	return Vector(a) -= b;
}

double math::operator*(const Vector& a, const Vector& b)
{
	THROW_IF(a.height() != b.height());

	double scalarProduct = 0;
	for_in_range(i, 0, a.height())
		scalarProduct += a.get(i) * b.get(i);

	return scalarProduct;
}

Vector math::operator*(const Vector& vector, double lambda)
{
	return Vector(vector) *= lambda;
}
Vector math::operator*(double lambda, const Vector& vector)
{
	return Vector(vector) *= lambda;
}

Vector math::operator*(const Vector& vector, const Matrix& matrix)
{
	return Vector(vector) *= matrix;
}
Vector math::operator*(const Matrix& matrix, const Vector& vector)
{
	return Vector(vector) *= matrix;
}

Vector math::operator/(const Vector& vector, double lambda)
{
	return Vector(vector) /= lambda;
}
Vector math::operator/(double lambda, const Vector& vector)
{
	return Vector(vector) /= lambda;
}
