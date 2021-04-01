#pragma once
#include "Matrix.h"

namespace math
{
	typedef unsigned int ui;

	class Vector
	{
	private:
		double* m_values;
		ui m_height;

	public:
		Vector();
		Vector(const Vector& other);
		Vector(Vector&& other) noexcept;
		~Vector();

		Vector(ui height);
		Vector(ui height, double initialValue);
		Vector(ui height, double* values);

		static Vector random(ui height);

		double& at(ui pos);
		double& operator[](ui pos);
		void set(ui pos, double val);
		double get(ui pos) const;
		ui height() const;

		void applySigmoid();

		Vector& operator=(const Vector& other);
		Vector& operator=(Vector&& other) noexcept;

		Vector& operator+=(const Vector& other);
		Vector& operator-=(const Vector& other);
		Vector& operator*=(double lambda);
		Vector& operator*=(const Matrix& matrix);
		Vector& operator/=(double lambda);
	};

	Vector operator+(const Vector& a, const Vector& b);
	Vector operator-(const Vector& a, const Vector& b);
	
	double operator*(const Vector& a, const Vector& b);
	Vector operator*(const Vector& vector, double lambda);
	Vector operator*(double lambda, const Vector& vector);
	Vector operator*(const Vector& vector, const Matrix& matrix);
	Vector operator*(const Matrix& matrix, const Vector& vector);

	Vector operator/(const Vector& vector, double lambda);
	Vector operator/(double lambda, const Vector& vector);
}