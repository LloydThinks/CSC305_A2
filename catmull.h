// catmull.h: interface for the catmull class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CATMULL_H__27EEFB10_E01C_4540_94B1_94F9D5CFC5DC__INCLUDED_)
#define AFX_CATMULL_H__27EEFB10_E01C_4540_94B1_94F9D5CFC5DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "foundation.h"
#include "stuff.h"


#define MAXP 1000
#define RADIUS 8
#define numStepsMax 100000

class catmull  
{
public:
	catmull();
    enum Mode{XY, XZ, ZY};
	virtual ~catmull();
    void mouseReleaseEvent();
    void mouseMoveEvent   (int x, int y, int z , Mode window);
    void mousePressEvent  (int x, int y, int z, int button , Mode window);
	void draw();

    void setxoff(int i){xoff=i;}
    void setyoff(int i){yoff=i;}
	void drawCurve(int p);
    void clear(){lastpt=0;}
    void setHull(bool b){hull=b;}
	void animate();
	bool step();
	void setSpeed(int v){
		if (v>numSteps) error=1;
		else frames=v;
    } // speed for now is no. of frames/segment

	void setSteps(int g){
		if (g<frames) {
			error=2;
			cerr << "************* Error Make steps ("<<g <<") >= frames (" <<  frames <<")\n";
		}
		else {
		numSteps=g;
		makeArcLength();
		}
	};

    void showSteps(bool b){showsteps=b; cerr << "showsteps\n";}
    void setTV(int j){tvmethod=j;}
    void setArcLength(bool b){arcLengthSwitch=b; cerr << "arc length is "<<b; }
    void setTVScale(double s){tvscale=s;}
    void setMotionType(int j){motionType=j;}


private:
	double arc[numStepsMax][2];  // the table holds 
	// 0 = t value linear  1 = distance to previous point
	double tvscale;
	int lastArc, error;
	int lastpt, cpt, seg, tvmethod;
    int pnts[MAXP][3];
    jVec2 tv[MAXP];  // must make this a 3D vector
	int mousex, mousey;
	bool mousedown, hull, animatenow, showsteps, arcLengthSwitch, makeArcMode;
	int button, frames, tframes, numSteps, nframe;
	int param; // t parameter
//	int iparam; // counts segments
	int xoff, yoff;

    void addPoint(int x, int y, int z);
    void movePoint(int x, int y, int z, Mode window);
    int select(int x, int y, int z, Mode window);
	bool nearzero(double x);
	void makeArcLength();
	int motionType; // 0 = parameter value 1 = const velocity
	double getTvalue(int p, int seg);
	void makeArcMotion();
	void printArcTab();
	double vel; // 0 to 1 range
	void drawConstVelCurve(int p);

};

#endif // !defined(AFX_CATMULL_H__27EEFB10_E01C_4540_94B1_94F9D5CFC5DC__INCLUDED_)




