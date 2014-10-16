#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGroupBox>
#include "openGLwidget.h"
#include <QLabel>
#include <QGridLayout>
#include <scene.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    //Added layout elements
    QGridLayout * centralGridlayout;

    QGroupBox * perspViewBox;
    QBoxLayout * perspInnerLayout;
    openGLWidget * perspView;

    QGroupBox * topViewBox;
    QBoxLayout * topInnerLayout;
    openGLWidget * topView;

    QGroupBox * leftViewBox;
    QBoxLayout * leftInnerLayout;
    openGLWidget * leftView;

    QGroupBox * frontViewBox;
    QBoxLayout * frontInnerLayout;
    openGLWidget * frontView;


    QGLShaderProgram * shaderProgram;
    Scene mainScene;
protected:
    void timerEvent(QTimerEvent *);
};

#endif // MAINWINDOW_H
