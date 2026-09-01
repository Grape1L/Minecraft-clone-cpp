#pragma once

#include <OpenGL/Math/Vec3.h>
#include <OpenGL/Math/Mat4.h>
#include <OpenGL/Physics/Physics.h>
#include <chrono>
#include <OpenGL/Player/Player.h>

class Camera {
public:
    Camera(Player& _player);
    ~Camera();

public:
    float x=0, y=0, z=0;
    float yaw=90, pitch=0;
    bool firstMouse = true;
    const float cameraSpeed = 0.019f;
public:
    Vec3 getDataXYZ();
    void listenForInput(std::chrono::duration<float> deltaTime);
    void mouseInput(float mouseSensitivity, int width, int height);
    Mat4 lookAt(const Vec3 &eye, const Vec3 &target, const Vec3 &up);
    Vec3 getFront();
    Vec3 getFrontNotNormalized();

private:
	Player& player;
};