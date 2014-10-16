#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QGLFunctions>
#include <QGLShaderProgram>

class SceneObject : protected QGLFunctions
{
public:
    virtual ~SceneObject() {}
    virtual void init() = 0;
    virtual void drawGeometry(QGLShaderProgram * program) = 0;
    virtual bool hasTexture() = 0;
    virtual const QImage & getTexture() = 0;
    virtual void release() = 0;
};

#endif // SCENEOBJECT_H
