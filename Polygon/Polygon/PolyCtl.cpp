// PolyCtl.cpp : Implementation of CPolyCtl
#include "stdafx.h"
#include "PolyCtl.h"


// CPolyCtl


STDMETHODIMP CPolyCtl::get_Sides(SHORT* pVal)
{
	*pVal = m_nSides;

	return S_OK;
}


STDMETHODIMP CPolyCtl::put_Sides(SHORT newVal)
{
	if (2 < newVal && newVal < 101)
	{
		m_nSides = newVal;

		FireViewChange();

		return S_OK;
	}
	else
	{
		return Error(_T("Shape must have between 3 and 100 sides"));
	}
}


void CPolyCtl::DrawTriangle(HDC hdc, POINT radius, POINT opposite, POINT adjacent)
{
	POINT array[3];
	array[0] = radius;
	array[1] = opposite;
	array[2] = adjacent;
	Polygon(hdc, &array[0], 3);
}

void CPolyCtl::CalcPoints(HDC hdc, const RECT& rc)
{
	const double pi = 3.14159265358979;
	POINT   ptCenter;
	// The radius is the hypotenuse 
	double  dblRadiusx = (rc.right - rc.left) / 2;
	double  dblRadiusy = (rc.bottom - rc.top) / 2;
	// angle is in radians, or radius lengths. 
	double  dblAngle = 4 * pi / 2;          // Start at the top
	double  dblDiff = 2 * pi / m_nSides;   // Angle each side will make

	ptCenter.x = (rc.left + rc.right) / 2;
	ptCenter.y = (rc.top + rc.bottom) / 2;

	POINT radianStart[2];
	radianStart[0].x = ptCenter.x;
	radianStart[0].y = ptCenter.y;
	radianStart[1].x = (long)(dblRadiusx * cos(0) + ptCenter.x + 0.5);
	radianStart[1].y = (long)(dblRadiusy * sin(0) + ptCenter.y + 0.5);

	Polyline(hdc, &radianStart[0], 2);

	// Calculate the points for each side
	int i = 0;
	for (; i < m_nSides; i++)
	{
		// When executing in the Visual C++ Debugger the output isn't directed to DebugView.
		ATLTRACE(_T("Angle in radians: %f\n"), dblAngle);

		m_arrPoint[i].x = (long)(dblRadiusx * cos(dblAngle) + ptCenter.x + 0.5);
		m_arrPoint[i].y = (long)(dblRadiusy * sin(dblAngle) + ptCenter.y + 0.5);

		if ( i > 0)
			DrawTriangle(hdc, ptCenter, m_arrPoint[i-1], m_arrPoint[i]);

		dblAngle += dblDiff;
	}
	DrawTriangle(hdc, ptCenter, m_arrPoint[i-1], m_arrPoint[0]);
}
