#pragma once
#include <math.h>

namespace math
{
	inline double sigm(double x)
	{
		return x / (1 + abs(x));
	}
	inline double sigmd(double x)
	{
		double sig = sigm(x);
		return sig * (1 - sig);
	}
}