#pragma once
#include <gdiplus.h>
#include <vector>

// PlotArea2
struct PointDb
{
	double x;
	double y;
	PointDb(double x, double y) { this->x = x; this->y = y; }
	PointDb() {};
};

class PlotArea2 : public CStatic
{
	DECLARE_DYNAMIC(PlotArea2)

public:

	void Initialize(double xLeft, double xRight, double R, double x0, int size);
	virtual ~PlotArea2();
	PlotArea2();
	void ZoomIn();
	void ZoomOut();
	void CoordToPixel(Gdiplus::Point&);
	void CoordToPixel(Gdiplus::Point*, int);
	void CalculatePoints(double (*func)(double, double), double xLeft, double xRight, int Size);
	void Rescale() { bScalePoints = true; }
	void ToOrigin() { bToOrigin = true; }
	void DrawPoints(Gdiplus::Graphics* gr, Gdiplus::Pen* pen, Gdiplus::Brush* brush, int size, bool lines);


	Gdiplus::REAL fZoom = 1.0F;

protected:
	DECLARE_MESSAGE_MAP()
public:

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

private:

	ULONG_PTR token;
	bool bCanDrag = false;
public:long long int offsetX, offsetY;
	  double scaleX, scaleY;
private:
	double LogicalWidth;
	double LogicalHeight;

	PointDb* pointsArray = NULL;
	Gdiplus::Point* pixelPoints = NULL;
	bool bToOrigin = true;
	bool bInitialized = false;
	bool bScalePoints = true;
	CPoint prevPt;
	short GridLineCount = 10;
	int iPtCount;
	double R;

	std::vector<PointDb> points;

public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void AddPoint(double t, double x);
	void Reset();

};

