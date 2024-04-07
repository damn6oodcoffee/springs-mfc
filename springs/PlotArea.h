#pragma once
#include <gdiplus.h>
#include <vector>
#include "RK4.h"
// PlotArea
struct MyRect
{
	double x, y, height, width;
};
class PhysicalObject;
class Ball;
class Spring;
void RungeKutta(std::vector<Ball*>, std::vector<Spring*>);
//extern std::vector<PhysicalObject* > allObjects;

class PlotArea : public CStatic
{
	ULONG_PTR token;
	bool bLMButtonDown = false;
	PhysicalObject* physObj = NULL;
	CPoint ptClicked;
	CPoint curPoint;
	DECLARE_DYNAMIC(PlotArea)
		
public:
	PlotArea();
	virtual ~PlotArea();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	void RKIter();
	void Reset();

};

class PhysicalObject 
{
public: Gdiplus::RectF hitbox; 	MyRect myhitbox;
public: 
	virtual void Show(Gdiplus::Graphics* gr) = 0;
	virtual void Move(double dx, double dy = 0);
	
	bool IsHit(CPoint pt);


};

class Ball: public PhysicalObject 
{

	Gdiplus::Color clr;
	bool bSelected = false;
	Spring* leftSpring;
	Spring* rightSpring;
public:
	Ball(double x, double y, double Rad, Gdiplus::Color clr = Gdiplus::Color::Red);
		
	void Show(Gdiplus::Graphics* gr);
	void ShowWall(Gdiplus::Graphics* gr);

	void AttachSprings(Spring* left, Spring* right);

	void Move(double dx, double dy = 0) override;

	void Set(double x, double y = 0);

	RK4 rk4;
	
	double vx;
	double x0;
	double vx0;
	double m;
};


class Spring : public PhysicalObject 
{
public:
	double k0; double param; int type;
	double k;
	Gdiplus::Pen* pen;
	Spring(double k0, double param, int type, double x1, double x2, double height);
	
	void CalcK(double x);
	
	void Move(double x1, double x2);
	
	void Show(Gdiplus::Graphics* gr);


};
