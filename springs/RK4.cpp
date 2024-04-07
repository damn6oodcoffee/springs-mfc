#include "pch.h"
#include "RK4.h"

int sign(double x) { if (x > 0)return 1; else if (x < 0) return -1; else if (x == 0) return 0; }
double f(double x, double vx, double xprev, double xnext, double m, double visc, double u, double k1, double k2) 
{
	return 
		(-k1*x - k2*x + k1*xprev + k2*xnext - visc*vx - sign(vx)*u*m*G)/m;
		
}

