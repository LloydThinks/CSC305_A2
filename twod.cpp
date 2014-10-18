#include "twod.h"
#include "ui_twod.h"

twod::twod(catmull::Mode mode) :
    ui(new Ui::twod)
{
    ui->setupUi(this);
    window = mode;
}

twod::~twod()
{
    delete ui;
}

void twod::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
void twod::initializeGL()
{
    //Background color will be white
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glShadeModel( GL_FLAT );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glPointSize(5);
}


void twod::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT );
    glLoadIdentity();

    if (window == catmull::XZ) {
        glRotatef(-90, 1, 0, 0);
    }
    else if (window == catmull::ZY) {
        glRotatef(90, 0, 1, 0);
    }
    else {
        ;  // Do nothing
    }

    catt->draw();

  // send signal to m_glWidget->updateGL
}

void twod::redraw()
{
   updateGL();
   if (m_glWidget!=NULL) {
       m_glWidget->update();
       m_glWidget->repaint();
   }
}

/* 2D */
void twod::resizeGL( int w, int h )
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho(0.0,GLdouble(w),0,GLdouble(h),-1000.0,1000.0);
    glFlush();
    glMatrixMode(GL_MODELVIEW);
    glViewport( 0, 0, (GLint)w, (GLint)h );
}

void twod::mousePressEvent( QMouseEvent *e )
{
    if (window == catmull::XY)
        catt->mousePressEvent(e->x(), height()-e->y(), 0, e->button() );
    else if (window == catmull::XZ)
        catt->mousePressEvent(e->x(), 0, height()-e->y(), e->button() );
    else  // window == ZY
        catt->mousePressEvent(0, height()-e->y(), e->x(), e->button() );
    updateGL();
}

void twod::mouseReleaseEvent( QMouseEvent *e)
{
    catt->mouseReleaseEvent(e->x(), height()-e->y(), e->button() );
    updateGL();
}

void twod::mouseMoveEvent ( QMouseEvent *e )
{
    catt->mouseMoveEvent(e->x(), height()-e->y(), e->button() );
    updateGL();
}
