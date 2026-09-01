#include <OpenGL/Object/Object.h>

Object::Object() {

}

Object::~Object() {

}

Mat4 Object::modelMatrix(const Vec3 &position, const Vec3 &rotation, const Vec3 &scale)
{
    Mat4 model;
    Mat4 temp;

    model.setIdentity();
    temp.setIdentity(); temp.setScale(scale); model *= temp;
    temp.setIdentity(); temp.setRotationX(rotation.x); model *= temp;
    temp.setIdentity(); temp.setRotationY(rotation.y); model *= temp;
    temp.setIdentity(); temp.setRotationZ(rotation.z); model *= temp;
    temp.setIdentity(); temp.setTranslation(position); model *= temp;

    return model;
}