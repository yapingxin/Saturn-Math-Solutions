#include "common_functions.h"
#include <math.h>


double precision_round(const double x, const uint8_t k)
{
	double factor = pow(10, k);

	double A;
	double V;
	double fixedV;

	if (x > 0)
	{
		A = factor * x + 0.5;
		V = floor(A);
		fixedV = V / factor;

		return fixedV;

		//return floor(factor * x + 0.5) / factor;
	}
	else
	{
		return ceil(factor * x - 0.5) / factor;
	}
}


void get_precision_round(double* p_Value, const double x, const uint8_t k)
{
	double factor = pow(10, k);

	double A;
	double V;

	if (x > 0)
	{
		A = factor * x + 0.5;
		V = floor(A);
		*p_Value = V / factor;
	}
	else
	{
		*p_Value = ceil(factor * x - 0.5) / factor;
	}
}