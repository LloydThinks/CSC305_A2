//-------------------------------------------------------------------------------------------
//   Qtgl for graphics class
//-------------------------------------------------------------------------------------------

#include "glwidget.h"

const double torad = M_PI/180.0;
const double lim=0.5;
const double RadPerPixel = - 0.01;
const double MovePerPixel = - 0.1;
GLfloat vertices[4][2] = {{100.0, 100.0}, {400.0, 100.0}, {400.0, 400.0}, {100.0, 400.0}}; //  vertex coords
GLubyte indices[] = {0,1,1,2,2,3,3,0};

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    startup();
}

GLWidget::~GLWidget()
{    

}

void GLWidget::startup()
{
    winw=width();  // width returns width of window
    winh=height();
    button = 0;
    cerr << "Glwidget\n";
    version=MYVERSION;
    xangle= yangle= zangle=0.0;
    scale = 1.5;// this will be reset by the 2dView
    object =0;
    xfrom=yfrom=zfrom=5.0;
    xto=yto=zto=0.0;
    animationTimer = new QTimer(this);
    // the timer will send a signal timeout at regular intervals.
    // whenever this timeout signal occurs, we want it to call our drawOpenGL
    // function

  connect(animationTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
    // we start the timer with a timeout interval of 20ms
  animationTimer->start(20);

}

void GLWidget::clear()
{
     updateGL();
}

void GLWidget::initializeGL()
{
    int i;
    QImage buf(256, 256, QImage::Format_RGB32);  // for texturing

//    GLfloat whiteDir[4] = {2.0, 2.0, 2.0, 1.0};
//    GLfloat whiteAmb[4] = {1.0, 1.0, 1.0, 1.0};
//    GLfloat lightPos[4] = {30.0, 30.0, 30.0, 1.0};

//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
//    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteAmb);

//    glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteDir);
//    glMaterialfv(GL_FRONT, GL_SPECULAR, whiteDir);
//    glMaterialf(GL_FRONT, GL_SHININESS, 20.0);

//    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDir);		// enable diffuse
//    glLightfv(GL_LIGHT0, GL_SPECULAR, whiteDir);	// enable specular
//    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

//    glShadeModel( GL_SMOOTH );

// Set up the textures

//    for (i=0; i<6; i++) {
//        tex[i] = QGLWidget::convertToGLFormat( buf );  // flipped 32bit RGBA
//    }

//    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
//    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//    glEnable( GL_TEXTURE_2D );

// Set up various other stuff
//    glClearColor( 0.5, 1.0, 0.75, 0.0 ); // Let OpenGL clear to black
//    glEnable( GL_CULL_FACE );  	// don't need Z testing for convex objects
//    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

// Make the object display list
    object = makeDice( );	// Generate an OpenGL display list
}

void GLWidget::redraw()
{
        updateGL();
}

void GLWidget::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT );

    glLoadIdentity();
    gluLookAt(xfrom,yfrom,zfrom, xto, yto, zto, 0.0, 1.0, 0.0);



    glColor3f(0.0f, 0.0f, 1.0f);
    drawLine(0.0, 0.0, 0.0, 10, 10, 10 );
    glCallList( object );
    glPushMatrix();
    glScalef( scale, scale, scale );
    // scale the catmul spline to fit into a 1 by 1
    if (catt!=NULL) catt->draw();
    glPopMatrix();

    /*
      glTranslatef( 0.0, 0.0, -10.0 );


    glRotatef( xangle, 1.0, 0.0, 0.0 );
    glRotatef( yangle, 0.0, 1.0, 0.0 );
    glRotatef( zangle, 0.0, 0.0, 1.0 );


    */
    sendUpdate();
}

/* 2D */
void GLWidget::resizeGL( int w, int h )
{
    glViewport( 0, 0, (GLint)w, (GLint)h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum( -1.0, 1.0, -1.0, 1.0, 5.0, 1500.0 );
    glMatrixMode( GL_MODELVIEW );
}

void GLWidget::about()
{
    QString vnum;
    QString mess, notes;
    QString title="QtOpenGl-1 ";

    vnum.setNum (version );
    mess="Simple OpenGl and Qt by Brian Wyvill Release Version: ";
    mess = mess+vnum;
    notes = "\n\n News: No News.";
    mess = mess+notes;
    QMessageBox::information( this, title, mess, QMessageBox::Ok );
}

void GLWidget::help()
{
    QString vnum;
    QString mess, notes;
    QString title="Flowsnake ";

    vnum.setNum ( version );
    mess="Simple Interface to openGl and Qt by Brian Wyvill Release Version: ";
    mess = mess+vnum;
    notes = "\nThis version driven from the GLwidget. \n \
            Just draws a circle that's all \n   ";
    mess = mess+notes;
    QMessageBox::information( this, title, mess, QMessageBox::Ok );
}

void GLWidget::initLight()
{
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 128.0 };
   //   GLfloat light_position[] = { 0.3, 0.3, 0.8, 0.0 };
   GLfloat light_position[] = { 0.2, 0.2, 0.9, 0.0 };
   GLfloat light_ambient[] = { 0.05, 0.05, 0.05, 1.0 };
//   GLfloat light_specular[] = { 0.1, 0.1, 0.1, 1.0 };
   GLfloat light_specular[] = { 0.99, 0.99, 0.99, 1.0 };
   GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };

// glClearColor(1.0, 1.0, 0.9, 1.0);
     glShadeModel (GL_SMOOTH);
   //   glShadeModel (GL_FLAT);

// MATERIAL
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

// LIGHT0
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

   glColorMaterial(GL_FRONT, GL_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   //   glEnable(GL_DEPTH_TEST);
   //   glDisable(GL_LIGHTING);

}

GLuint GLWidget::makeDice( )
{
    GLuint list;
    float w = 0.8;

    list = glGenLists( 1 );

    glNewList( list, GL_COMPILE );


    // one
    drawFace(0,  w);

    // six
    glPushMatrix();
    glRotatef( 180.0, 1.0, 0.0, 0.0 );
    drawFace(5, w);
    glPopMatrix();

    // four on left
    glPushMatrix();
    glRotatef( -90.0, 0.0, 1.0, 0.0 );
    drawFace(3, w);
    glPopMatrix();

    // three on right
    glPushMatrix();
    glRotatef( 90.0, 0.0, 1.0, 0.0 );
    drawFace(2, w);
    glPopMatrix();

    // two
    glPushMatrix();
    glRotatef( 90.0, 1.0, 0.0, 0.0 );
    drawFace(1, w);
    glPopMatrix();

    // five
    glPushMatrix();
    glRotatef( -90.0, 1.0, 0.0, 0.0 );
    drawFace(4, w);
    glPopMatrix();



    glEndList();

    return list;
}

void GLWidget::drawtFace( int tim, float w)
{
    // textured face
    glTexImage2D( GL_TEXTURE_2D, 0, 3, tex[tim].width(), tex[tim].height(), 0,
                  GL_RGBA, GL_UNSIGNED_BYTE, tex[tim].bits() );

    glBegin( GL_POLYGON );
    glTexCoord2f(0.0, 0.0);
    glVertex3f(  -w,  -w, w );
    glTexCoord2f(0.0, 1.0);
    glVertex3f(   w,  -w, w );
    glTexCoord2f(1.0, 1.0);
    glVertex3f(   w,   w, w );
    glTexCoord2f(1.0, 0.0);
    glVertex3f(  -w,   w, w );
    glEnd();

}

void GLWidget::drawFace( int tim, float w)
{
    // wire frame

   // glBegin( GL_POLYGON );
    glBegin( GL_LINE_LOOP);
    glVertex3f(  -w,  -w, w );
    glVertex3f(   w,  -w, w );
    glVertex3f(   w,   w, w );
    glVertex3f(  -w,   w, w );
    glEnd();

}

// communication with the window widget
void GLWidget::rotx(int a)
{
    xangle =  (double)a;
//	std::cerr << " x angle "<<xangle<<"\n";
    updateGL();
}
void GLWidget::roty(int a)
{
    yangle =  (double)a;
    updateGL();
}

void GLWidget::rotz(int a)
{
    zangle =  (double)a;
    updateGL();
}


void GLWidget::setxFrom(int a)
{
    xfrom=a;
    updateGL();
}

void GLWidget::setyFrom(int a)
{
    yfrom=a;
    updateGL();
}
void GLWidget::setzFrom(int a)
{
    zfrom=a;
    updateGL();
}

void GLWidget::setView(int w, int h)
{
    // sets up scale
    if (w>h) scale = (double)h;
    else scale = (double)w;
    scale = 1.0/scale;
    cerr<< scale NL;
}

// mouse routines for camera control to be implemented
//****************************************************

void GLWidget::mousePressEvent( QMouseEvent *e )
{
    //TODO:When a button is pressed!
    if ( e->button() == Qt::LeftButton)
    {
        Rotating = true;
        lastMousePoint = e->pos();
    }

    if ( e->button() == Qt::RightButton)
    {
        Scaling = true;
        lastMousePoint = e->pos();
    }

}

void GLWidget::mouseReleaseEvent( QMouseEvent *e)
{
    //TODO:When some pressed mouse button is release!
    if ( e->button() == Qt::LeftButton)
    {
        Rotating = false;
        DoRotate(e->pos(), lastMousePoint);
    }

    if ( e->button() == Qt::RightButton)
    {
        Scaling = false;
        DoRotate(e->pos(), lastMousePoint);
    }
    updateGL();

}

void GLWidget::mouseMoveEvent( QMouseEvent *e )
{
    //TODO: when the mouse is moved!
    if ( (e->buttons() & Qt::LeftButton) && Rotating)
    {
        DoRotate(e->pos(), lastMousePoint);
        lastMousePoint = e->pos();
    }

    if ( (e->buttons() & Qt::RightButton) && Scaling)
    {
        DoScale(e->pos(), lastMousePoint);
        lastMousePoint = e->pos();
    }
    updateGL();

}

void GLWidget::RotateY(double rad)
{
    //TODO: rotate pVec around Y axis by the angle rad
    double cosPhi = cos(rad);
    double sinPhi = sin(rad);

    Matrix33d m;
    m.M11 = cosPhi;
    m.M13 = sinPhi;
    m.M22 = 1;
    m.M31 = -sinPhi;
    m.M33 = cosPhi;

    Vector3d pVec = Vector3d();
    pVec.x = xfrom;
    pVec.y = yfrom;
    pVec.z = zfrom;

    pVec = MultiplyMatrix33Vec3(pVec, m);

    xfrom = pVec.x;
    yfrom = pVec.y;
    zfrom = pVec.z;
}

void GLWidget::RotateZ(double rad)
{
    //TODO: rotate pVec around Z axis by the angle rad
    double cosPhi = cos(rad);
    double sinPhi = sin(rad);

    Matrix33d m;
    m.M11 = cosPhi;
    m.M12 = -sinPhi;
    m.M21 = sinPhi;
    m.M22 = cosPhi;
    m.M33 = 1;

    Vector3d pVec = Vector3d();
    pVec.x = xfrom;
    pVec.y = yfrom;
    pVec.z = zfrom;

    pVec = MultiplyMatrix33Vec3(pVec, m);

    xfrom = pVec.x;
    yfrom = pVec.y;
    zfrom = pVec.z;
}

void GLWidget::DoRotate(QPoint desc, QPoint orig)
{
    //TODO: adjust the camera position so the viewport is rotated
    double YRot = (desc.x() - orig.x()) * RadPerPixel;
    double ZRot = -1 * (desc.y() - orig.y()) * RadPerPixel;

    RotateY(YRot);
    RotateZ(ZRot);
}

void GLWidget::DoScale(QPoint desc, QPoint orig)
{
    //TODO: adjust the camera position so the viewport is scaled.
    double length = sqrt(CameraPos.x * CameraPos.x + CameraPos.y * CameraPos.y + CameraPos.z * CameraPos.z);

    double newLength = length + (desc.y() - orig.y()) * MovePerPixel;

    if (newLength > 6)
    {
        double ratio = newLength / length;
        CameraPos.x *= ratio;
        CameraPos.y *= ratio;
        CameraPos.z *= ratio;
    }
}
