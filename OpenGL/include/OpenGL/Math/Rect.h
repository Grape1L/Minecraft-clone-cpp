#pragma once

class Rect {
public:
    Rect() {}
    Rect(int width, int height) : width(width), height(height) {}
    Rect(int width, int height, int top, int left) : width(width), height(height), top(top), left(left) {}
    Rect(const Rect &rect) : width(rect.width), height(rect.height), top(rect.top), left(rect.left) {}

public:
    int width = 0, height = 0;
    int top = 0, left = 0;
};