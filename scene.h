#ifndef SCENE_H
#define SCENE_H
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <vector>
#include "SceneObject.h"
#include <math.h>

class SceneGraphNode
{
public:
    SceneGraphNode();
    SceneObject * pObject;
    QMatrix4x4 LocalTransform;
    std::vector<SceneGraphNode *> children;
    void release();
};

class Scene : public QObject
{
    Q_OBJECT

public:
    Scene();
    ~Scene();

public:
    void initialize();
    void timeStep();
    SceneGraphNode Root;
    //This method is specific to the lab project
    void SetAngularSpeed(QVector3D axis, qreal velocity);

private:
    QVector3D rot_axis;
    qreal rot_velocity;
    QQuaternion rotation;

    double offsetPara;
};

#endif // SCENE_H
