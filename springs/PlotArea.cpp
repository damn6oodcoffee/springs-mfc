// PlotArea.cpp : implementation file
//

#include "pch.h"
#include "springs.h"
#include "PlotArea.h"
#define BALLSCOUNT 40

using namespace std;
using namespace Gdiplus;
// PlotArea
double visc = 0;
double u = 0;
double dt = 1e-5;
int N = 0;
std::vector<PhysicalObject* > allObjects;
std::vector<Ball*> allBalls;
std::vector<Spring*> allSprings;


IMPLEMENT_DYNAMIC(PlotArea, CStatic)

PlotArea::PlotArea()
{
	GdiplusStartupInput input;
	Status s;
	s = GdiplusStartup(&token, &input, NULL);

//	allObjects.push_back(&ball1);

}
void PlotArea::Reset() 
{
	int sizeb = allBalls.size(), sizes = allSprings.size();

	for (int i = 0; i < sizeb; i++)
	{
		delete allBalls[allBalls.size()-1];
		allBalls.pop_back();
	}
	for (int i = 0; i < sizes; i++)
	{
		delete allSprings[allSprings.size() - 1];
		allSprings.pop_back();
	}
	allBalls.clear();
	allSprings.clear();
	
	for (int i = 0; i < N; i++)
	{
		allBalls.push_back(new Ball(1 + 2.3 * i, 0, 0.3));
	}
	for (int i = 0; i < N - 1; i++)
	{
		allSprings.push_back(new Spring(1, 1, 0, allBalls[i]->hitbox.X + allBalls[i]->hitbox.Width, allBalls[i + 1]->hitbox.X, 0.3));
	}
	for (int i = 1; i < N - 1; i++)
	{
		allBalls[i]->AttachSprings(allSprings[i - 1], allSprings[i]);
	}

}

PlotArea::~PlotArea()
{
	GdiplusShutdown(token);
}


BEGIN_MESSAGE_MAP(PlotArea, CStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// PlotArea message handlers




void PlotArea::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (N == 0) return;
	// TODO:  Add your code to draw the specified item
	CRect rect = lpDrawItemStruct->rcItem;
	CDC* pDC = GetDC();
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	CBitmap* pOldBitmap = dcMem.SelectObject(&bitmap);
	Graphics gr(dcMem);

	gr.SetSmoothingMode(SmoothingModeAntiAlias);
	gr.Clear(Color::White);
	gr.TranslateTransform(1, rect.Height() / 2);

	float scaleX = rect.Width() / (float)(2.3 * allBalls.size());
	float scaleY = scaleX;
	gr.ScaleTransform(scaleX, -scaleY);

	Pen pen(Color::Black,0);
	
	//RungeKutta(allBalls, allSprings);

	for (int i = 0; i < allSprings.size(); i++)
	{
		allSprings[i]->Show(&gr);
	}

	allBalls[0]->ShowWall(&gr);
	allBalls[allBalls.size() - 1]->ShowWall(&gr);
	for (int i = 1; i < allBalls.size()-1; i++)
	{
		allBalls[i]->Show(&gr);
	}


	BitBlt(lpDrawItemStruct->hDC, 0, 0, rect.Width(), rect.Height(), dcMem.GetSafeHdc(), 0, 0, SRCCOPY);
	dcMem.SelectObject(pOldBitmap);
	DeleteDC(dcMem);
	ReleaseDC(pDC);

	return;



}


void PlotArea::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	return;

	bLMButtonDown = true;
	for (int i = 0; i < allObjects.size(); i++) 
	{
		if (allObjects[i]->IsHit(point)) { physObj = allObjects[i]; ptClicked = curPoint = point;  break;
		}
	}
	Invalidate(0);
	CStatic::OnLButtonDown(nFlags, point);
}


void PlotArea::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	return;

	bLMButtonDown = false;
	physObj = NULL;
	curPoint = point;
	CStatic::OnLButtonUp(nFlags, point);
}


void PlotArea::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	return; 
	if (bLMButtonDown && physObj != NULL) 
	{
		physObj->Move(point.x - curPoint.x, point.y - curPoint.y);
		curPoint = point;
		//Invalidate(0);
	}
	

	CStatic::OnMouseMove(nFlags, point);
}


void RungeKutta(std::vector<Ball*> balls, std::vector<Spring*> springs)
{
	
	for (int i = 0; i < springs.size(); i++) 
	{
		springs[i]->CalcK(balls[i+1]->x0 - balls[i]->x0);
	}

	for (int i = 1; i < balls.size()-1; i++)
	{
	
		balls[i]->rk4.k1 = balls[i]->vx0;
		balls[i]->rk4.dk1 = f(balls[i]->x0, balls[i]->vx0, balls[i - 1]->x0, balls[i + 1]->x0, balls[i]->m, visc, u, springs[i-1]->k, springs[i]->k);
	}

	for (int i = 1; i < balls.size() - 1; i++)
	{

		balls[i]->rk4.k2 = balls[i]->vx0 + balls[i]->rk4.dk1 * dt/2;
		balls[i]->rk4.dk2 = f(balls[i]->x0 + balls[i]->rk4.k1 * dt/2,
								balls[i]->vx0 + balls[i]->rk4.dk1 * dt/2,
								balls[i - 1]->x0 + balls[i - 1]->rk4.k1 * dt/2,
								balls[i + 1]->x0 + balls[i + 1]->rk4.k1 * dt/2,
								balls[i]->m, visc, u, springs[i-1]->k, springs[i]->k);
	}

	for (int i = 1; i < balls.size() - 1; i++)
	{
		balls[i]->rk4.k3 = balls[i]->vx0 + balls[i]->rk4.dk2 * dt / 2;
		balls[i]->rk4.dk3 = f(balls[i]->x0 + balls[i]->rk4.k2 * dt / 2,
			balls[i]->vx0 + balls[i]->rk4.dk2 * dt / 2,
			balls[i - 1]->x0 + balls[i - 1]->rk4.k2 * dt / 2,
			balls[i + 1]->x0 + balls[i + 1]->rk4.k2 * dt / 2,
			balls[i]->m, visc, u, springs[i-1]->k, springs[i]->k);
	}

	for (int i = 1; i < balls.size() - 1; i++)
	{
		balls[i]->rk4.k4 = balls[i]->vx0 + balls[i]->rk4.dk3 * dt;
		balls[i]->rk4.dk4 = f(balls[i]->x0 + balls[i]->rk4.k3 * dt,
			balls[i]->vx0 + balls[i]->rk4.dk3 * dt,
			balls[i - 1]->x0 + balls[i - 1]->rk4.k3 * dt,
			balls[i + 1]->x0 + balls[i + 1]->rk4.k3 * dt,
			balls[i]->m, visc, u, springs[i-1]->k, springs[i]->k);
	}

	for (int i = 1; i < balls.size() - 1; i++)
	{
		balls[i]->x0 += (balls[i]->rk4.k1 + 2 * balls[i]->rk4.k2 + 2 * balls[i]->rk4.k3 + balls[i]->rk4.k4) * dt / 6;
		balls[i]->Move((balls[i]->rk4.k1 + 2 * balls[i]->rk4.k2 + 2 * balls[i]->rk4.k3 + balls[i]->rk4.k4) * dt / 6);
	
		balls[i]->vx0 += (balls[i]->rk4.dk1 + 2 * balls[i]->rk4.dk2 + 2 * balls[i]->rk4.dk3 + balls[i]->rk4.dk4) * dt / 6;
	}

	

}

void PlotArea::RKIter() 
{
	RungeKutta(allBalls, allSprings);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Phyiscal class implementation

void PhysicalObject::Move(double dx, double dy)
{

}
bool PhysicalObject::IsHit(CPoint pt)
{
	return 0;
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Ball class implementation


Ball::Ball(double x, double y, double Rad, Gdiplus::Color clr)
{
	
	hitbox.X = x - Rad;  myhitbox.x = x - Rad;
	hitbox.Y = y - Rad;  myhitbox.y = y - Rad;
	hitbox.Width = 2 * Rad; 		myhitbox.width = 2 * Rad;
	hitbox.Height = 2 * Rad;		myhitbox.height = 2 * Rad;
	this->clr = clr;
	x = vx = x0 = vx0 = 0;
	m = 0.01;

	rk4.k1 = 0;
	rk4.k2 = 0;
	rk4.k3 = 0;
	rk4.k4 = 0;
	rk4.dk1 = 0;
	rk4.dk2 = 0;
	rk4.dk3 = 0;
	rk4.dk4 = 0;

	leftSpring = NULL;
	rightSpring = NULL;
}
void Ball::Show(Gdiplus::Graphics* gr)
{
	Gdiplus::SolidBrush brush(clr);
	gr->FillEllipse(&brush, hitbox.X, hitbox.Y, hitbox.Width, hitbox.Height);
}

void Ball::ShowWall(Gdiplus::Graphics* gr)
{
	Gdiplus::SolidBrush brush(Color::Black);
	gr->FillRectangle(&brush, hitbox.X, 4*hitbox.Y, hitbox.Width, 4*hitbox.Height);
}



void Ball::AttachSprings(Spring* left, Spring* right)
{
	leftSpring = left;
	rightSpring = right;
}

void Ball::Move(double dx, double dy) 
{
	myhitbox.x += dx; myhitbox.y += dy;
	hitbox.X = myhitbox.x; if (leftSpring != NULL)leftSpring->myhitbox.width += dx; if (rightSpring != NULL) { rightSpring->myhitbox.x += dx; rightSpring->myhitbox.width -= dx; }
	hitbox.Y = myhitbox.y;
}

void Ball::Set(double x, double y)
{
	Move(-x0);
	x0 = x;
	Move(x0);
	

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Spring class implementation

Spring::Spring(double k0, double param, int type, double x1, double x2, double height)
{
	this->k0 = k = k0; this->param = param; this->type = type;
	hitbox.X = x1; myhitbox.x = x1;
	hitbox.Y = height / 2; myhitbox.y = height / 2;
	hitbox.Height = height; myhitbox.height = height;
	hitbox.Width = x2 - x1; myhitbox.width = x2 - x1;
	//pen = new Gdiplus::Pen(Gdiplus::Color::Black, 0);
}
void Spring::CalcK(double x)
{
	if (type == 0)k = k0 + param * abs(x);
	else k = k0 + param * x * x;
}
void Spring::Move(double x1, double x2)
{
	hitbox.X = x1;
	hitbox.Width = x2 - x1;
}
void Spring::Show(Gdiplus::Graphics* gr)
{
	hitbox.X = myhitbox.x; hitbox.Width = myhitbox.width;
	float step = hitbox.Width / 5;

	Gdiplus::PointF pts[6] = { Gdiplus::PointF(hitbox.X, 0),
	Gdiplus::PointF(hitbox.X + step, hitbox.Y),
	Gdiplus::PointF(hitbox.X + 2 * step, -hitbox.Y),
	Gdiplus::PointF(hitbox.X + 3 * step, hitbox.Y),
	Gdiplus::PointF(hitbox.X + 4 * step, -hitbox.Y),
	Gdiplus::PointF(hitbox.X + 5 * step, 0) };
	gr->DrawLines(&Pen(Gdiplus::Color::Black, 0), pts, 6);
	return;
}










