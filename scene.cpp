#include "scene.h"

SceneGraphNode::SceneGraphNode()
{
    pObject = NULL;
    LocalTransform.setToIdentity();
}

void SceneGraphNode::release()
{
    for (uint i = 0; i < children.size(); ++ i)
    {
        SceneGraphNode * pChild = children[i];
        if (pChild != NULL)
        {
            pChild->release();
            delete pChild;
        }
    }

    if (pObject != NULL)
    {
        pObject->release();
        delete pObject;
    }
}


Scene::Scene()
{
    rot_velocity = 0;
}

Scene::~Scene()
{
    Root.release();
}

void Scene::initialize()
{
    //some extra initialization
}

void Scene::timeStep()
{
    //TODO: update rotation for the current time step

    //Root.LocalTransform.rotate(5, QVector3D(0,1,0));

    rot_velocity *= 0.99;

    if (rot_velocity < 0.001) { rot_velocity = 0; }
    else
    {
        rotation = QQuaternion::fromAxisAndAngle(rot_axis, rot_velocity);
        Root.LocalTransform.rotate(rotation);
    }
    //offsetPara += 0.01;
    //Root.LocalTransform.translate(sin(offsetPara) * 0.01, cos(offsetPara) * 0.01, 0);

}

void Scene::SetAngularSpeed(QVector3D axis, qreal velocity)
{
    //TODO: Update the rot_axis and rot_velocity according to mouse input

    rot_axis = (rot_axis * rot_velocity + axis * velocity).normalized();
    rot_velocity += velocity;
}
