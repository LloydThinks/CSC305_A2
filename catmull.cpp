// catmull.cpp: implementation of the catmull Rom spline class.
//
//////////////////////////////////////////////////////////////////////

#include "catmull.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

catmull::catmull()
{
	lastpt=0;
	hull = true;
	mousedown=false;
	animatenow=false;
	tvmethod=0;
	lastArc=0;
    makeArcMode=false;
    motionType =0;
	numSteps=10;
	frames = 5;
	error=0;
    tensionValue = 50;

    showControlPoints = true;
    showControlLines = true;
    showCatmullRom = true;
}

catmull::~catmull()
{

}

void catmull::mousePressEvent( int x, int y, int z, int button, Mode window )
{
    cpt= -1;

	if (mousedown) return;
    mousedown = true;
	// left to select and move right to add new point
    if (button == Qt::RightButton) addPoint(x, y, z);
    if (button == Qt::LeftButton)  movePoint(x, y, z, window);
}

void catmull::mouseReleaseEvent()
{
	mousedown = false;
}

void catmull::mouseMoveEvent ( int x, int y, int z, Mode window )
{
    if (mousedown) movePoint(x, y, z, window);
}

void catmull::addPoint(int x, int y, int z)
{
	if (lastpt<MAXP) {
        pnts[lastpt][X]=x;
        pnts[lastpt][Y]=y;
        pnts[lastpt][Z]=z;
		lastpt++;
    }
}

void catmull::movePoint(int x, int y, int z, Mode window)
{	
    if (cpt < 0) cpt = select(x, y, z, window);
    if (cpt>=0)
    {
        if (window == XY)
        {
            pnts[cpt][0]=x;
            pnts[cpt][1]=y;
        }
        else if (window == XZ)
        {
            pnts[cpt][0]=x;
            pnts[cpt][2]=z;
        }
        else  // window == ZY
        {
            pnts[cpt][1]=y;
            pnts[cpt][2]=z;
        }
	}
}

int catmull::select(int x, int y, int z, Mode window)
{
    int i;

    if (window == XY)
    {
        for (i=0; i<lastpt; i++) {
            if (   ( (pnts[i][0]-RADIUS) <x && (pnts[i][0]+RADIUS) >x )
                && ( (pnts[i][1]-RADIUS) <y && (pnts[i][1]+RADIUS) >y ) )
                return i;
        }
    }
    else if (window == XZ)
    {
        for (i=0; i<lastpt; i++) {
            if (   ( (pnts[i][0]-RADIUS) <x && (pnts[i][0]+RADIUS) >x )
                && ( (pnts[i][2]-RADIUS) <z && (pnts[i][2]+RADIUS) >z ) )
                return i;
        }
    }
    else  // window == ZY
    {
        for (i=0; i<lastpt; i++) {
            if (   ( (pnts[i][1]-RADIUS) <y && (pnts[i][1]+RADIUS) >y )
                && ( (pnts[i][2]-RADIUS) <z && (pnts[i][2]+RADIUS) >z ) )
                return i;
        }
    }
    return -1;
}

bool catmull::nearzero(double x)
{
	if ( fabs(x) > -0.01  && fabs(x) < 0.01) return true;
	else return false;
}

void catmull::drawCurve(int pnt1[], int pnt2[], int pnt3[], int pnt4[])
{
    glColor3f(0.129f, 0.850f, 0.768f);
    // Save the original points per segment
    double stepX0 = double(pnt2[0]);
    double stepY0 = double(pnt2[1]);
    double stepZ0 = double(pnt2[2]);
    double stepX1, stepY1, stepZ1;
    double t = double(tensionValue)/50.0;
    double step = 1.0/double(numSteps);
    double u = step;
    // Each time through the loop, use a calculation on u to find the next step
    // Each step is a point to be drawn
    for (int i = 0; i < numSteps; i++)
    {
        stepX1 = (double(pnt1[0]) * (-t*u + 2*t*u*u - t*u*u*u) + \
                  double(pnt2[0]) * (1 + (t-3)*u*u + (2-t)*u*u*u) + \
                  double(pnt3[0]) * (t*u + (3-2*t)*u*u + (t-2)*u*u*u) + \
                  double(pnt4[0]) * (-t*u*u + t*u*u*u));
        stepY1 = (double(pnt1[1]) * ((-t*u + 2*t*u*u - t*u*u*u)) + \
                  double(pnt2[1]) * (1 + (t-3)*u*u + (2-t)*u*u*u) + \
                  double(pnt3[1]) * (t*u + (3-2*t)*u*u + (t-2)*u*u*u) + \
                  double(pnt4[1]) * (-t*u*u + t*u*u*u));
        stepZ1 = (double(pnt1[2]) * ((-t*u + 2*t*u*u - t*u*u*u)) + \
                  double(pnt2[2]) * (1 + (t-3)*u*u + (2-t)*u*u*u) + \
                  double(pnt3[2]) * (t*u + (3-2*t)*u*u + (t-2)*u*u*u) + \
                  double(pnt4[2]) * (-t*u*u + t*u*u*u));
        drawLine(stepX0, stepY0, stepZ0, stepX1, stepY1, stepZ1);
        stepX0 = stepX1;
        stepY0 = stepY1;
        stepZ0 = stepZ1;

        u += step;
    }
    // Draw the final line per segment, to connect with the next segment
    drawLine(stepX1, stepY1, stepZ1, double(pnt3[0]), double(pnt3[1]), double(pnt3[2]));

}

void catmull::draw()
{
	int i;
    int x0,x1,y0,y1,z0,z1;
	jVec2 a,b,c;
	    
	glColor3f(1.0f, 0.0f, 0.1f);
	x1 = pnts[0][0];
    y1 = pnts[0][1];
    z1 = pnts[0][2];
    for (i=0; i<lastpt; i++) {
		x0=x1;
		y0=y1;
        z0=z1;
		x1 = pnts[i][0];
		y1 = pnts[i][1];
        z1 = pnts[i][2];
        if (showControlPoints)
        {
            glColor3f(0.2f, 0.8f, 0.1f);
            drawPoint(double(x1), double(y1), double(z1));
        }
        if (showControlLines)
        {
            glColor3f(1.0f, 0.0f, 0.1f);
            drawLine(x0, y0, z0, x1, y1, z1);
        }
        x0 = x1;  y0 = y1; z0 = z1;


        // Draw the curve between each segment
        if (showCatmullRom)
        {
            if (i > 1 && i < (lastpt - 1))
            {
                drawCurve(pnts[i-2], pnts[i-1], pnts[i], pnts[i+1]);
            }
        }
	}

}

void catmull::animate()
{
	animatenow=true;
	param =  0;
	nframe=0;
	seg = 0;
}

void catmull::tensionSlider(int tensionValueL)
{
    tensionValue = tensionValueL;
}

void catmull::controlPoints(bool showControlPointsL)
{
    showControlPoints = showControlPointsL;
}

void catmull::controlLines(bool showControlLinesL)
{
    showControlLines = showControlLinesL;
}

void catmull::catmullRom(bool showCatmullRomL)
{
    showCatmullRom = showCatmullRomL;
}





