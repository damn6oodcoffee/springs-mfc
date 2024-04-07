#pragma once
#include <vector>
extern class PhysicalObject;
extern class Ball;
extern class Spring;
extern class PlotArea;

struct Settings 
{
	std::vector<Ball*> balls;
	std::vector<Spring*> springs;
	PlotArea* pplotarea;
};