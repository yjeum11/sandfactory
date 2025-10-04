#include "util.hpp"
#include "raylib-cpp.hpp"

raylib::Vector2 project_point_to_line(const raylib::Vector2 p, const raylib::Vector2 v, const raylib::Vector2 w) {
    raylib::Vector2 s = w - v;
    float t = (s.DotProduct(p - v)) / s.DotProduct(s);
    raylib::Vector2 res = s * t;
    return res;
}

raylib::Vector2 project_vector_to_line(const raylib::Vector2 p, const raylib::Vector2 v, const raylib::Vector2 w) {
    raylib::Vector2 s = w - v;
    float t = (s.DotProduct(p)) / s.DotProduct(s);
    raylib::Vector2 res = s * t;
    return res;
}

