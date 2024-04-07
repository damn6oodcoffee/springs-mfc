// PlotArea2.cpp : implementation file
//

#include "pch.h"
#include "springs.h"
#include "PlotArea2.h"
using namespace Gdiplus;

// PlotArea2

IMPLEMENT_DYNAMIC(PlotArea2, CStatic)


PlotArea2::PlotArea2()
{
	GdiplusStartupInput input;
	Status s;
	s = GdiplusStartup(&token, &input, NULL);

	LogicalWidth = 200;
	LogicalHeight = 2;
	//	allObjects.push_back(&ball1);
	
}

void PlotArea2::Initialize(double xLeft, double xRight, double R, double x0, int size)
{
	MoveWindow(502, 270, 454, 204);
}

void PlotArea2::AddPoint(double t, double x) 
{

	delete[] pixelPoints;
	points.push_back(PointDb(t, x));
	pixelPoints = new Point[points.size()];

}
void PlotArea2::Reset() 
{

	points.clear();
}

PlotArea2::~PlotArea2()
{
	GdiplusShutdown(token);

}


BEGIN_MESSAGE_MAP(PlotArea2, CStatic)
	ON_WM_DRAWITEM()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// PlotArea2 message handlers

void PlotArea2::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  Add your code to draw the specified 
	if (points.size() == 0)	return;

	Matrix mtx;
	CRect rect;
	GetClientRect(&rect);


	if (bToOrigin)
	{
		offsetX =-rect.Width()/4; offsetY = 0;
		bToOrigin = false;
	}


	CDC* pDC = GetDC();
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	CBitmap* pOldBitmap = dcMem.SelectObject(&bitmap);
	Graphics gr(dcMem);

	Pen SimplePen(Color::Black, 0.0F);
	Pen GridPen(Color::LightGray, 0.0F);
	GridPen.SetDashStyle(DashStyleDash);
	Pen CurvePen(Color::Red, 0.0F);

	gr.Clear(Color::White);

	scaleX = rect.Width() / LogicalWidth;
	scaleY = rect.Height() / LogicalHeight;


	mtx.Translate(rect.Width() / 2 + offsetX, rect.Height() / 2 + offsetY);
	mtx.Scale(1.0, -1.0);
	gr.SetTransform(&mtx);


	/////////////////////////////////////////////////////
	//Рисование сетки и подписей

	Gdiplus::Font font(&FontFamily(L"Arial"), 8);
	WCHAR axisVals[60];
	double axisVal;
	Gdiplus::SolidBrush brush(Color::Black);

	RectF boundBox;
	double test;

	if (rect.Width() > GridLineCount && rect.Height() > GridLineCount)
	{
		int VertGridLineInterval = (rect.Width() / 10);
		int HorGridLineInterval = (rect.Height() / 10);

		gr.ResetTransform();
		int GridOffsetX = offsetX % VertGridLineInterval;
		int GridOffsetY = offsetY % HorGridLineInterval;
		for (int i = 0; i <= GridLineCount; i++)
		{
			gr.DrawLine(&GridPen, i * VertGridLineInterval + GridOffsetX, 0,
				i * VertGridLineInterval + GridOffsetX, rect.Height());
			gr.DrawLine(&GridPen, 0, (GridLineCount - i) * HorGridLineInterval + GridOffsetY,
				rect.Width(), (GridLineCount - i) * HorGridLineInterval + GridOffsetY);



			axisVal = (LogicalWidth / 10) * ((i - GridLineCount / 2) - int(offsetX / (rect.Width() / 10)));
			swprintf_s(axisVals, L"%.3f", axisVal);
			gr.MeasureString(axisVals, wcslen(axisVals), &font, PointF(0, 0), &boundBox); //Ширина текста(числовых подписей осей) в пикселях
			gr.DrawString(axisVals, -1, &font, PointF((i * rect.Width()) / 10 + GridOffsetX - boundBox.Width / 2, rect.Height() - 12), &brush);

			axisVal = -(LogicalHeight / 10) * ((i - GridLineCount / 2) - int(offsetY / (rect.Height() / 10)));
			swprintf_s(axisVals, L"%.3f", axisVal);
			gr.MeasureString(axisVals, wcslen(axisVals), &font, PointF(0, 0), &boundBox); //Ширина текста(числовых подписей осей) в пикселях
			gr.DrawString(axisVals, -1, &font, PointF(0, (i * rect.Height()) / 10 + GridOffsetY), &brush);


		}
		gr.SetTransform(&mtx);

	}
	/////////////////////////////////////////////////////

	//Отрисовка осей
	gr.DrawLine(&SimplePen, -rect.Width() - offsetX, 0, rect.Width() - offsetX, 0);
	gr.DrawLine(&SimplePen, 0, -rect.Height() + offsetY, 0, rect.Height() + offsetY);


	//Отрисовка точек кривой


	gr.ResetTransform();
	gr.TranslateTransform(rect.Width() / 2, rect.Height() / 2);
	gr.ScaleTransform(1.0f, -1.0f);

	//Преобразование вещественных координат в пиксели
	if (0)
	{
		for (int i = 0; i < iPtCount; i++)
		{
			pixelPoints[i].X = floor(pointsArray[i].x * scaleX);
			pixelPoints[i].Y = floor(pointsArray[i].y * scaleY);
		}
		bScalePoints = false;

		for (int i = 0; i < iPtCount; i++)
		{
			pixelPoints[i].X += offsetX;
			pixelPoints[i].Y -= offsetY;
		}

	}

	for (int i = 0; i < points.size(); i++)
	{
		pixelPoints[i].X = floor(points[i].x * scaleX);
		pixelPoints[i].Y = floor(points[i].y * scaleY);
	}
	bScalePoints = false;

	for (int i = 0; i < points.size(); i++)
	{
		pixelPoints[i].X += offsetX;
		pixelPoints[i].Y -= offsetY;
	}

	gr.SetSmoothingMode(SmoothingModeHighQuality);


	SolidBrush brush1(Color::DarkRed);
	//DrawPoints(&gr, &CurvePen, &brush1, iPtCount, true);

	gr.DrawLines(&CurvePen, pixelPoints, points.size());
	BitBlt(lpDrawItemStruct->hDC, 0, 0, rect.Width(), rect.Height(), dcMem.GetSafeHdc(), 0, 0, SRCCOPY);

	dcMem.SelectObject(pOldBitmap);
	ReleaseDC(pDC);

}
void PlotArea2::DrawPoints(Gdiplus::Graphics* gr, Gdiplus::Pen* pen, Gdiplus::Brush* brush, int size, bool bLines)
{
	if (bLines)
		gr->DrawLines(pen, pixelPoints, size);


	for (int i = 0; i < size; i++)
	{
		//gr->DrawEllipse(pen, pixelPoints[i].X - 3, pixelPoints[i].Y - 3, 6, 6);
		gr->FillEllipse(brush, pixelPoints[i].X - 3, pixelPoints[i].Y - 3, 6, 6);
	}

}




void PlotArea2::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (bCanDrag)
	{
		offsetX += point.x - prevPt.x;
		offsetY += point.y - prevPt.y;
		prevPt = point;
		Invalidate();
	}

	CStatic::OnMouseMove(nFlags, point);
}

void PlotArea2::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	prevPt = point;
	bCanDrag = true;
	CStatic::OnLButtonDown(nFlags, point);
}


void PlotArea2::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	bCanDrag = false;
	CStatic::OnLButtonUp(nFlags, point);
}

void PlotArea2::ZoomIn()
{
	LogicalWidth /= 1.5;
	LogicalHeight /= 1.5;

	bScalePoints = true;
}

void PlotArea2::ZoomOut()
{
	LogicalWidth *= 1.5;
	LogicalHeight *= 1.5;

	bScalePoints = true;

}

void PlotArea2::CoordToPixel(Point& pt)
{
	pt.X = pt.X * scaleX;
	pt.Y = pt.Y * scaleY;

}

void PlotArea2::CoordToPixel(Point* pt, int iSize)
{
	for (int i = 0; i < iSize; i++)
	{
		pt[i].X = pt[i].X * scaleX;
		pt[i].Y = pt[i].Y * scaleY;
	}
}




void PlotArea2::CalculatePoints(double(*func)(double, double), double xLeft, double xRight, int size)
{

	double Step = (xRight - xLeft) / (size - 1);
	for (int i = 1; i < size; i++)
	{
		pointsArray[i].x = i;
		pointsArray[i].y = func(pointsArray[i - 1].y, R);
	}

}




BOOL PlotArea2::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(&rect);
	ScreenToClient(&pt);
	Point ppt; ppt.X = pt.x; ppt.Y = pt.y;
	Matrix mtx;
	mtx.Translate(rect.Width() / 2 + offsetX, rect.Height() / 2 + offsetY);
	mtx.Scale(-1.0, -1.0);
	mtx.TransformPoints(&ppt);
	double val1, val2, val3;
	if (zDelta < 0)
	{
		val1 = (ppt.X / scaleX);
		val2 = (rect.Width() / LogicalWidth) / 1.5;
		val3 = val1 * val2;;
		offsetX += -(ppt.X - int(val3));
		val1 = (ppt.Y / scaleY);
		val2 = (rect.Height() / LogicalHeight) / 1.5;
		val3 = val1 * val2;;
		offsetY += -(ppt.Y - int(val3));

		ZoomOut();
	}
	else
	{

		val1 = (ppt.X / scaleX);
		val2 = (rect.Width() / LogicalWidth) * 1.5;
		val3 = val1 * val2;;
		offsetX += -(ppt.X - int(val3));
		val1 = (ppt.Y / scaleY);
		val2 = (rect.Height() / LogicalHeight) * 1.5;
		val3 = val1 * val2;;
		offsetY += -(ppt.Y - int(val3));
		//	offsetX += int(int(ppt.X * scaleX) * (rect.Width() / LogicalWidth) * 1.5);
		//	offsetY += int(int(ppt.Y * scaleY) * (rect.Height() / LogicalHeight) * 1.5);


		ZoomIn();

	}

	Invalidate();
	return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}
