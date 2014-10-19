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
	vel =0.5; // 0 to 1 scale
	numSteps=10;
	frames = 5;
	error=0;
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
	if (lastpt>2) makeArcLength();
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
    int i, windowX, windowY;

    if (window == XY)
    {
        windowX = x;
        windowY = y;
    }
    else if (window == XZ)
    {
        windowX = x;
        windowY = z;
    }
    else  // window == ZY
    {
        windowX = Z;
        windowY = y;
    }

    for (i=0; i<lastpt; i++) {
        if (   ( (pnts[i][0]-RADIUS) <windowX && (pnts[i][0]+RADIUS) >windowX )
            && ( (pnts[i][1]-RADIUS) <windowY && (pnts[i][1]+RADIUS) >windowY ) )
            return i;
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

void catmull::drawConstVelCurve(int p)
{
    int i,j;
    double d, ad, inc; // d is dist to next ad is accumulated distance inc is the increment per frame
    double totald;     // totald is the total distance for this segment
    double t = 0.0;
    double t2,t3;
    int x0,x1,y0,y1,z0,z1,k=0;
//	double step;
    int parc;
    x1 = pnts[p][0];
    y1 = pnts[p][1];
    z0=z1=0.0;
    d = ad = 0.0;
    parc = (p-1)+(p-1)*numSteps;  // starting point for this segment
    if (p==1) parc=0;
    cerr << "const vel. curve parc="<<parc<<"frames= " << frames<<" numSteps="<<numSteps<<" ";
    j = 0;
    totald = arc[parc+numSteps][1];
    inc = (double)totald/(double)frames;

    cerr << " total distance this segment is "<<totald<<" seg number p="<<p<<" inc="<<inc<<"\n";

    for (i=0; i<frames; i++) {
        d += inc; // first distance
        x0=x1;
        y0=y1;
        ad = arc[j][1];
        while (ad<d) {
            ad=arc[j][1];
            j++;
        }
        if (j>0) j--;  // go back to previous
        if (j+1 < numSteps) {
            if (nearzero( arc[j+1][1]-arc[j][1] ) ) cerr << " ***** zero error ****\n";
            t=arc[j][0]+(arc[j+1][0]-arc[j][0])*(d-arc[j][1])/(arc[j+1][1]-arc[j][1]);
        } else t=1.0;
        t2=t*t;
        t3=t*t2;
        x1 = pnts[p][0]*(2.0*t3-3.0*t2+1.0) + pnts[p+1][0]*(-2.0*t3+3.0*t2) + tv[p][0]*(t3-2.0*t2+t) + tv[p+1][0]*(t3-t2);
        y1 = pnts[p][1]*(2.0*t3-3.0*t2+1.0) + pnts[p+1][1]*(-2.0*t3+3.0*t2) + tv[p][1]*(t3-2.0*t2+t) + tv[p+1][1]*(t3-t2);
                //cerr <<"x1="<<x1<<" y1="<<y1<< " ad="<<ad<<" j = " <<j<<" t="<<t<<"\n";
		
        if (animatenow && nframe ==i && (seg+1)==p) {
            glColor3f(0.0f, 0.0f, 0.1f);
            drawPoint((double)x1, (double)y1, 0.0);

        }

        if (showsteps && !makeArcMode) {
            glColor3f(0.9f, 0.1f, 0.1f);
            drawPoint(double(x1), double(y1), double(z1));
        }

			
        glColor3f(0.0f, 0.0f, 1.0f);
        cerr << "line " SEP x0 SEP y0 SEP x1 SEP y1 NL;
        drawLine(x0, y0, z0, x1, y1, z1 );

	} // end for
}

void catmull::drawCurve(int p)
{
	// draw the curve between each pair of points
	double t = 0.0;
	double t2,t3;
	double d=0.0;
    int x0,x1,z0,y0,y1,z1, i;
	double step;
	int parc;
	x1 = pnts[p][0];
	y1 = pnts[p][1];
    z1 = pnts[p][2];

	step = 1.0 /(double)numSteps;
  //  cerr << "catmull:  drawCurve: numSteps="<<numSteps<<" showsteps="<<showsteps NL;
	// arc contains the following info for arc lengths
	// arc[arc count][0] contains the current parameter 0 - 1
	// arc[arc count][1] contains the current distance to the last point
	
	parc = (p-1)+(p-1)*numSteps;  // starting point for this segment
	if (p==1) parc=0;
        //cerr <<"MakeArc mode = "<<makeArcMode<< "setting parc to "<<parc<<" p="<<p<<"\n";
	for (i=0; i<=numSteps; i++) {
		if (!makeArcMode) t=getTvalue(i, parc);  // otherwise it is linear incremented by 1/numSteps

		t2=t*t;
		t3=t*t2;
		x0=x1;
		y0=y1;
        z0=z1;

		// this is essentially a Hermite spline with constraints on the tangent vectors
		x1 = pnts[p][0]*(2.0*t3-3.0*t2+1.0) + pnts[p+1][0]*(-2.0*t3+3.0*t2) + tv[p][0]*(t3-2.0*t2+t) + tv[p+1][0]*(t3-t2);
		y1 = pnts[p][1]*(2.0*t3-3.0*t2+1.0) + pnts[p+1][1]*(-2.0*t3+3.0*t2) + tv[p][1]*(t3-2.0*t2+t) + tv[p+1][1]*(t3-t2);
        z1 = pnts[p][2]*(2.0*t3-3.0*t2+1.0) + pnts[p+1][2]*(-2.0*t3+3.0*t2) + tv[p][2]*(t3-2.0*t2+t) + tv[p+1][2]*(t3-t2);
		// make the arc length calcs.
		if (makeArcMode ) {
			if (i>0) d += dist(x0, y0, x1, y1); 
			arc[lastArc][0]=t;  // always between 0 and 1
			arc[lastArc][1]=d; // distance to last point
			lastArc++;
		}
		t += step;


		if (showsteps && !makeArcMode) {
			glColor3f(0.9f, 0.5f, 0.1f);
            drawPoint(double(x1), double(y1), double(z1));
		}
        //cerr <<makeArcMode  SEP animatenow<<" catmull seg="<< seg << " p="<<p<<" param="<<param<<" i="<<i<<"\n";
		if (!makeArcMode && animatenow && param ==i && (seg+1)==p) {
			glColor3f(0.0f, 0.0f, 0.1f);
            drawPoint(double(x1), double(y1), double(z1));
		}
		glColor3f(0.0f, 0.0f, 1.0f);
       // cerr << "line " SEP x0 SEP y0 SEP x1 SEP y1 NL;
        if (!makeArcMode) drawLine(x0, y0, z0, x1, y1, z1 );
        x0 = x1;  y0 = y1; z0=z1;
	}
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
      //  cerr << "cat : " << lastpt NL;
	for (i=0; i<lastpt; i++) {
		x0=x1;
		y0=y1;
        z0=z1;
		x1 = pnts[i][0];
		y1 = pnts[i][1];
        z1 = pnts[i][2];
                glColor3f(0.2f, 0.8f, 0.1f);
        drawPoint(double(x1), double(y1), double(z1));
        glColor3f(1.0f, 0.0f, 0.1f);
        drawLine(x0, y0, z0, x1, y1, z1);
        x0 = x1;  y0 = y1; z0 = z1;
		// set the tangent vectors according the tvmethod
		if ( i>0 && i<(lastpt-1) ) {

			switch (tvmethod) {

			case 0:
				tv[i][0]=(pnts[i+1][0] - pnts[i-1][0])*tvscale;
				tv[i][1]=(pnts[i+1][1] - pnts[i-1][1])*tvscale;
				break;

			case 1:
				a[0]=(double)pnts[i-1][0];  a[1]=(double)pnts[i-1][1];
				b[0]=(double)pnts[i][0];    b[1]=(double)pnts[i][1];
				c[0]=(double)pnts[i+1][0];  c[1]=(double)pnts[i+1][1];
				tv[i]=(b-a) + (c-b);
				break;

			case 2:
				tv[i][0]=(pnts[i+1][0] - pnts[i-1][0])*tvscale;
				tv[i][1]=(pnts[i+1][1] - pnts[i-1][1])*tvscale;
				break;
			}
		}
	}
	glColor3f(0.0f, 0.0f, 1.0f);
	i = 1; // the first point is ignored as we need i-1th point to find curve
	
	while (i<(lastpt-2)) {
		if (makeArcMode || motionType==0) drawCurve(i);
		else if (motionType==1) drawConstVelCurve(i);
		i++;
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

bool catmull::step()
{
	int total;
	total = lastpt - 3; // total number of animatable segments
	tframes = total*frames;  // total number of frames, there are frames frames/segment
	// param is how far along each segment we are and 
	// seg is which segment we should do next
	if (seg>=total) return false; 
		param++;
		nframe++;
	if (param>=numSteps || nframe>=frames){
		param = 0;
		nframe=0;
		seg++;
	}
if (error)	cerr << " ********* error is " << error<<"\n";
	return true;
}

// called when steps changed or new points added
void catmull::makeArcLength()
{
if (lastpt<3) return;
	// numSteps is total steps per segment
	if (numSteps>numStepsMax) return; // flag error
	// for each curve segment - for each parameter value
	makeArcMode=true;
	lastArc=0;
	draw();
	makeArcMotion();
	makeArcMode=false;
//	printArcTab();

}

void catmull::printArcTab()
{
	int i;
	for (i=0; i<lastArc; i++) {
                        cerr << "arc "<< arc[i][0] << " " << arc[i][1]<<"\n";
		}
}

void catmull::makeArcMotion()
{
	// this routine builds the table
	switch (motionType) {
	case 0:  // 0 = parameter value 
		// copy parameter values to param table

		break;
	case 1: // constant velocity

		break;
	case 2: // ease in ease out
		break;
	}
}





