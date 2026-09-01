#pragma once

#include <cstdint>
#include <functional>

class Vec2i {
public:
    Vec2i() {};
    Vec2i(int x, int y) : x(x), y(y) {}
public:
    int x = 0, y = 0;

public:
    bool operator==(const Vec2i& other) const {
		return x == other.x && y == other.y;
    }
};

namespace std {
    template<>
    struct hash<Vec2i> {
        size_t operator()(const Vec2i& v) const {
            size_t h1 = std::hash<int>()(v.x);
            size_t h2 = std::hash<int>()(v.y);

            return h1 ^ (h2 << 1);
        }
    };
}