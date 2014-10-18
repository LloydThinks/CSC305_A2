//-------------------------------------------------------------------------------------------
//  Multiple viewports example
//-------------------------------------------------------------------------------------------
#ifndef WINDOW_H
#define WINDOW_H

#include <QDialog>
#include <QColorDialog>
#include "ui_MainWindow.h"
#include "glwidget.h"
#include "twod.h"
#include "catmull.h"

class GLWidget;

//Create a window and load the components from generated code ui_MainWindow.h
class Window : public QDialog, public Ui::frmMain
{
	Q_OBJECT;

public:
	//Constructor 
	Window(QWidget *parent = 0);
	
private:
        GLWidget *m_glWidget;
        twod *sideview1;
        twod *sideview2;
        twod *sideview3;
        catmull *catt;

    private slots:
        void endit(){cerr << "goodbye\n"; exit(0);};
        void clear();
        void aboutBut();
        void helpBut();
        void pushmebut();

        void xrot(int);
        void yrot(int);
        void zrot(int);

        void xFrom(int);
        void yFrom(int);
        void zFrom(int);

protected:
         void  resizeEvent(QResizeEvent * );

};


#endif
