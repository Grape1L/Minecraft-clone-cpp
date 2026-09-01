#include <OpenGL/Camera/Camera.h>
#include <windows.h>
#include <iostream>
#include <OpenGL/Debug.h>

Camera::Camera(Player& _player) : player(_player) {
}

Camera::~Camera() {
}

Vec3 Camera::getDataXYZ() {
    return Vec3(x, y, z);
}

void Camera::listenForInput(std::chrono::duration<float> deltaTime) {
    float speed = this->cameraSpeed * deltaTime.count();
    float cameraSpeedForward = speed;

    Vec3 frontNoPitch(
        cosf(calcRAD(yaw)), 
        0.0f, 
        sinf(calcRAD(yaw))
    );
    frontNoPitch = Vec3::normalize(frontNoPitch);

    Vec3 rightNoPitch(
        -(sinf(calcRAD(yaw))), 
        0.0f, 
        cosf(calcRAD(yaw))
    );

    rightNoPitch = Vec3::normalize(rightNoPitch);

    if (GetAsyncKeyState(VK_LSHIFT) & 0x8000) {
        cameraSpeedForward *= 2;
    }

    else if (GetAsyncKeyState(VK_LCONTROL) & 0x8000) {
        cameraSpeedForward /= 2;
        speed /= 2;
    }

    if (GetAsyncKeyState('W') & 0x8000) {
        x -= frontNoPitch.x * cameraSpeedForward;
        z -= frontNoPitch.z * cameraSpeedForward;
    }
    if (GetAsyncKeyState('S') & 0x8000) {
        x += frontNoPitch.x * speed;
        z += frontNoPitch.z * speed;
    }

    if (GetAsyncKeyState('A') & 0x8000) {
        x += rightNoPitch.x * speed;
        z += rightNoPitch.z * speed;
    }
    if (GetAsyncKeyState('D') & 0x8000) {
        x -= rightNoPitch.x * speed;
        z -= rightNoPitch.z * speed;
    }

    if (GetAsyncKeyState(VK_SPACE) & 0x8000 && player.inAir == false) {
        player.vy = 0.075f;
        player.inAir = true;
    }

    if (GetAsyncKeyState(VK_BACK) & 0x8000) {
        y = 3;

        x = 512 / 2;
        z = 512 / 2;

        player.playerHealth = 100;
        player.vy = 0;
    }

    int hotBarKeys[] = {
        '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };

    for (int i = 0; i < std::size(hotBarKeys); i++) {
        if (GetAsyncKeyState(hotBarKeys[i]) & 0x8000) {
            player.m_inventory->currentSlot = i;
            player.itemInHand = &player.m_inventory->getItemAtSlot(i);
        }
    }
}

void Camera::mouseInput(float mouseSensitivity, int width, int height) {
    POINT mousePos;
    GetCursorPos(&mousePos);
    float xScreenCenter = width / 2;
    float yScreenCenter = height / 2;

    float lastMousePosX = xScreenCenter;
    float lastMousePosY = yScreenCenter;

    if (firstMouse) {
        lastMousePosX = (float)mousePos.x;
        lastMousePosY = (float)mousePos.y;
        firstMouse = false;
    }

    float xOffset = (float)mousePos.x - lastMousePosX;
    float yOffset = (float)mousePos.y - lastMousePosY;

    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;

    SetCursorPos(xScreenCenter, yScreenCenter);
    lastMousePosX = (float)xScreenCenter;
    lastMousePosY = (float)yScreenCenter;
}

Mat4 Camera::lookAt(const Vec3 &eye, const Vec3 &target, const Vec3 &up) {
    Vec3 f = Vec3::normalize(target - eye);
    Vec3 s = Vec3::normalize(Vec3::cross(f, up));
    Vec3 u = Vec3::cross(s, f);

    Mat4 result;
    result.mat[0][0] = s.x;
    result.mat[1][0] = s.y;
    result.mat[2][0] = s.z;

    result.mat[0][1] = u.x;
    result.mat[1][1] = u.y;
    result.mat[2][1] = u.z;

    result.mat[0][2] = -f.x;
    result.mat[1][2] = -f.y;
    result.mat[2][2] = -f.z;

    result.mat[3][0] = -Vec3::dot(s, eye);
    result.mat[3][1] = -Vec3::dot(u, eye);
    result.mat[3][2] =  Vec3::dot(f, eye);

    return result;
}

Vec3 Camera::getFront() {
    Vec3 front(
        cosf(calcRAD(yaw)) * cosf(calcRAD(pitch)), 
        sinf(calcRAD(pitch)), 
        sinf(calcRAD(yaw)) * cosf(calcRAD(pitch))
    );

    return Vec3::normalize(front);
}

Vec3 Camera::getFrontNotNormalized() {
    return Vec3(
        cosf(calcRAD(yaw)) * cosf(calcRAD(pitch)), 
        sinf(calcRAD(pitch)), 
        sinf(calcRAD(yaw)) * cosf(calcRAD(pitch))
    );
}