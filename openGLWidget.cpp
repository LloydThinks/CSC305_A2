/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "openGLWidget.h"
#include <QMouseEvent>
#include <math.h>

openGLWidget::openGLWidget()
{
    program = NULL;
    scene = NULL;
    vpType = Persp;
}

openGLWidget::openGLWidget(QWidget *parent, QGLWidget * shareWidget)
    : QGLWidget(parent, shareWidget)
{
    program = NULL;
    scene = NULL;
    vpType = Persp;
}


//! [0]
void openGLWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->pos());
}

void openGLWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->pos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Pass velocity to the scene
    qreal acc = diff.length() / 100.0;
    scene->SetAngularSpeed(n, acc);

}
//! [0]

void openGLWidget::initializeGL()
{
    initializeGLFunctions();
    qglClearColor(Qt::black);
    initShaders();
    initTextures();


//! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
//! [2]    
}

//! [3]
void openGLWidget::initShaders()
{
    program = new QGLShaderProgram();
    // Compile vertex shader
    if (!program->addShaderFromSourceFile(QGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program->addShaderFromSourceFile(QGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program->link())
        close();

    // Bind shader pipeline for use
    if (!program->bind())
        close();

    // Use texture unit 0
    program->setUniformValue("texture", 0);

}
//! [3]

//! [4]
void openGLWidget::initTextures()
{
    // Load cube.png image
    glEnable(GL_TEXTURE_2D);

    // Set nearest filtering mode for texture minification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Set bilinear filtering mode for texture magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
//! [4]

//! [5]
void openGLWidget::resizeGL(int w, int h)
{
    // Set OpenGL viewport to cover whole widget
    glViewport(0, 0, w, h);

    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 3.0, zFar = 7.0, fov = 60.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection according to viewport type
    switch (vpType) {
    case Persp:
        projection.perspective(fov, aspect, zNear, zFar);
        break;
    case Front:
        projection.ortho(-3, 3, -3, 3, zNear, zFar);
        projection.scale(1, aspect, 1);
        break;
    case Top:
        projection.rotate(-90, QVector3D(1, 0, 0));
        projection.ortho(-3, 3, -3, 3, zNear, zFar);
        break;
    case Left:
        projection.rotate(-90, QVector3D(0, 1, 0));
        projection.ortho(-3, 3, -3, 3, zNear, zFar);
        projection.scale(1, aspect, 1/aspect);
        //////////NOTE:: WE OFFSETED THE CUBE BY 5 IN INITIALIZATION
        /// MOVE IT BACK NOW
        /// FOR A "GROUND TURTH" ORTHO PROJECTION, REMOVE FOLLOWING TRANSLATE
        projection.translate(0, 0, -2.333);
        break;
    default:
        break;
    }

}
//! [5]

void openGLWidget::paintGL()
{  // Use this to change the camera
//   if (vpType == Persp)
//   {
//       projection.lookAt(QVector3D(3,3,3), QVector3D(0,0,-5), QVector3D(0,1,0));
//   }
    // Clear color and depth buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   if (scene == NULL) return; //nothing to draw

   // Traverse the scene graph and draw geometries
   DrawSceneNode(&scene->Root, program, QMatrix4x4());
 }

void openGLWidget::SetScene(Scene * pScene_in)
{
    Q_ASSERT(pScene_in != NULL);
    scene = pScene_in;
}

void openGLWidget::DrawSceneNode(SceneGraphNode * node, QGLShaderProgram * program, QMatrix4x4 transform)
{
    QMatrix4x4 transform_current = node->LocalTransform * transform;
    if (node->pObject != NULL)
    {
        // Set modelview-projection matrix
        program->setUniformValue("mvp_matrix", projection * transform_current);
        SceneObject * obj = node->pObject;
        if (obj != NULL)
        {
            if (obj->hasTexture()) { texture = bindTexture(obj->getTexture()); }
            obj->drawGeometry(program);
            deleteTexture(texture);
        }
    }

    for (uint i = 0; i < node->children.size(); ++ i)
    {
        DrawSceneNode(node->children[i], program, transform_current);
    }
}
