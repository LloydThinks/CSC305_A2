#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cubeGeometry.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    centralGridlayout = new QGridLayout();

    perspViewBox = new QGroupBox("Perspective");
    perspViewBox->resize(420, 320);
    perspInnerLayout = new QBoxLayout(QBoxLayout::LeftToRight, perspViewBox);
    perspView = new openGLWidget();
    //get the context from this widget and share it among all viewports
    perspView->updateGL();
    QGLContext * mainContext = const_cast<QGLContext *>(perspView->context());
    perspInnerLayout->addWidget(perspView);
    perspViewBox->setLayout(perspInnerLayout);
    centralGridlayout->addWidget(perspViewBox, 0, 0, 1, 1);

    topViewBox = new QGroupBox("Top");
    topViewBox->resize(420, 320);
    topInnerLayout = new QBoxLayout(QBoxLayout::LeftToRight, topViewBox);
    topView = new openGLWidget(0, (QGLWidget *)perspView);
    topView->vpType = openGLWidget::Top;
    topInnerLayout->addWidget(topView);
    topViewBox->setLayout(topInnerLayout);
    centralGridlayout->addWidget(topViewBox, 0, 1, 1, 1);

    leftViewBox = new QGroupBox("Left");
    leftViewBox->resize(420, 320);
    leftInnerLayout = new QBoxLayout(QBoxLayout::LeftToRight, leftViewBox);
    leftView = new openGLWidget(0, (QGLWidget *)perspView);
    leftView->vpType = openGLWidget::Left;
    leftInnerLayout->addWidget(leftView);
    leftViewBox->setLayout(leftInnerLayout);
    centralGridlayout->addWidget(leftViewBox, 1, 0, 1, 1);

    frontViewBox = new QGroupBox("Front");
    frontViewBox->resize(420, 320);
    frontInnerLayout = new QBoxLayout(QBoxLayout::LeftToRight, frontViewBox);
    frontView = new openGLWidget(0, (QGLWidget *)perspView);
    frontView->vpType = openGLWidget::Front;
    frontInnerLayout->addWidget(frontView);
    frontViewBox->setLayout(frontInnerLayout);
    centralGridlayout->addWidget(frontViewBox, 1, 1, 1, 1);

    ui->centralwidget->setLayout(centralGridlayout);


    mainContext->makeCurrent();

    //Set up the scene graph: we only have 1 cube!
    CubeGeometry * cube = new CubeGeometry();
    cube->init();
    mainScene.Root.pObject = cube;
    mainScene.Root.LocalTransform.translate(0, 0, -5);

    //Link the scene to all views : they're different viewports but rendering the same scene!
    perspView->SetScene(&mainScene);
    topView->SetScene(&mainScene);
    leftView->SetScene(&mainScene);
    frontView->SetScene(&mainScene);
    mainContext->doneCurrent();

    //TODO: Start the timer.
    startTimer(33);
}


MainWindow::~MainWindow()
{
    delete ui;
}

//TODO: Implement timer Evenet
//Put the decl in "protected" section of the header file

void MainWindow::timerEvent(QTimerEvent *)
{
    mainScene.timeStep();

    perspView->update();
    topView->update();
    leftView->update();
    frontView->update();
}



















