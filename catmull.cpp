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
    cerr << "building catmull\n";

	lastpt=0;
	hull = true;
	mousedown=false;
	animatenow=false;
	tvmethod=0;
	lastArc=0;
	makeArcMode=false;
	tvscale = 0.5;
    motionType =0;
	numSteps=10;
	frames = 5;
	error=0;
    tension = 0.5;
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

double catmull::getTvalue(int j, int seg)
{
	// seg is the segment index currently being drawn, index in the arc table

	// I want the parameter value corresponding to constant velocity
		
	switch (motionType) {
	case 0:  // 0 = parameter value do nothing
		if (seg+j > lastArc) cerr << " ***** arrgghh *****\n";
		else {
                        //cerr << arc[seg+j][0]<<" seg="<<seg<<" j="<<j<<"\n";
			return	arc[seg+j][0];
		}
		break;
	case 1: // constant velocity
		// total distance is arc[seg+j][1]
		cerr << "total distance is "<< arc[seg+j][1]<<"\n"; // in this segment
		// we want to travel at a speed of total distance / frames*nsegs as frames/seg is the setting
		// there are numSteps (number of frames per segment) in the parametric space 
		// and frames frames/segment tframes total in the object space 
		// which step is this velocity?
		// total dist/time =  arc[seg+p][1] / speed     but 1/numSteps is the 0-1 velocity
		// distance to be travelled at each step = 

		break;
	case 2: // ease in ease out
		break;
	}
	return 0.0; // shows there is an error
}

void catmull::drawCurve(int pnt1[], int pnt2[], int pnt3[], int pnt4[])
{
    glColor3f(0.129f, 0.850f, 0.768f);
    double stepX0 = double(pnt2[0]);
    double stepY0 = double(pnt2[1]);
    double stepZ0 = double(pnt2[2]);
    double stepX1, stepY1, stepZ1;
    double t = tension;
    double step = 1.0/double(numSteps);
    double u = step;

    for (int i = 0; i < numSteps; i++)
    {
        qDebug() << u;
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
        glColor3f(0.2f, 0.8f, 0.1f);
        //drawPoint(double(x1), double(y1), double(z1));
        //glColor3f(1.0f, 0.0f, 0.1f);
        //drawLine(x0, y0, z0, x1, y1, z1);
        x0 = x1;  y0 = y1; z0 = z1;

        // Draw the curve between each segment
        if (i > 1 && i < (lastpt - 1))
        {
            drawCurve(pnts[i-2], pnts[i-1], pnts[i], pnts[i+1]);
        }
	}

}

void catmull::animate()
{
	cerr << "setting animate to true\n";
	animatenow=true;
	param =  0;
	nframe=0;
	seg = 0;
}





