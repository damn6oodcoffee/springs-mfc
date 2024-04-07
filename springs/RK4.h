#pragma once
#include <vector>
#define G 10.0
double f(double x, double vx, double xprev, double xnext, double m, double visc, double u, double k1, double k2);
int sign(double x);

struct RK4 
{
	double k1;
	double k2;
	double k3;
	double k4;
	double dk1;
	double dk2;
	double dk3;
	double dk4;
};


