//-------------------------------------------------------------------------------------------
//  This is a demo. program for open gl vertex arrays
//-------------------------------------------------------------------------------------------
#include <QtGui>
#include <QtOpenGL>
#include "window.h"
//#include "dfuncs.h"

#define BIT0 0x00000001   // alpha
#define BIT1 0x00000002   // no buttons
#define BIT2 0x00000004   // set don't use native dialog

//extern dfuncs *df;

//------------------------------------------------------------------------------------
// Creates and initializes the main window for application
//------------------------------------------------------------------------------------
Window::Window(QWidget *parent):QDialog(parent)
{
    //We create an instance of GLWidget component we built in glwidget.h
    m_glWidget = new GLWidget;

    //Setup application interface. Creates all the required components and sliders.
    setupUi(this);

    //We need to attach our m_glWidget to glWidgetArea
    //All our drawings will be on glWidgetArea
    glWidgetArea->setWidget(m_glWidget); // gl widget perspective view

    sideview1 = new twod(catmull::XY);  // this is the first sideview
    sideview2 = new twod(catmull::XZ);  // this is the first sideview
    sideview3 = new twod(catmull::ZY);  // this is the first sideview
    catt = new catmull();

    sideview1->setCatt(catt);
    sideview2->setCatt(catt);
    sideview3->setCatt(catt);
    m_glWidget->setCatt(catt);
    sideview1->setgl(m_glWidget);
    sideview2->setgl(m_glWidget);
    sideview3->setgl(m_glWidget);
    sideview1->show();
    sideview2->show();
    sideview3->show();
    // set up the scale for the sideview
    m_glWidget->setView(sideview1->width(), sideview1->height());
    m_glWidget->setView(sideview2->width(), sideview2->height());
    m_glWidget->setView(sideview3->width(), sideview3->height());


    connect(m_glWidget, SIGNAL(sendUpdate()), sideview1, SLOT(myUpdate()));
    connect(m_glWidget, SIGNAL(sendUpdate()), sideview2, SLOT(myUpdate()));
    connect(m_glWidget, SIGNAL(sendUpdate()), sideview3, SLOT(myUpdate()));
}

void Window::resizeEvent( QResizeEvent * )
{
  cerr << "new size "<< width() SEP height() NL;
}

void Window::clear()
{
    m_glWidget->clear();
}

void Window::aboutBut()
{
    m_glWidget->about();
}

void Window::helpBut()
{
    m_glWidget->help();
}

void Window::pushmebut()
{
    cerr << "pushme pushed\n";
}

void Window::xrot(int x)
{
    m_glWidget->rotx(x);
}

void Window::yrot(int y)
{
    m_glWidget->roty(y);
}

void Window::zrot(int z)
{
    m_glWidget->rotz(z);
}

void Window::xFrom(int a)
{
    m_glWidget->setxFrom(a);
}

void Window::yFrom(int a)
{
    m_glWidget->setyFrom(a);
}

void Window::zFrom(int a)
{
    m_glWidget->setzFrom(a);
}
