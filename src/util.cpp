#include "util.hpp"
#include "raylib-cpp.hpp"

raylib::Vector2 project_point_to_line(const raylib::Vector2 p, const raylib::Vector2 v, const raylib::Vector2 w) {
    raylib::Vector2 s = w - v;
    float t = (s.DotProduct(p - v)) / s.DotProduct(s);
    raylib::Vector2 res = s * t;
    return res + v;
}

raylib::Vector2 project_vector_to_line(const raylib::Vector2 p, const raylib::Vector2 v, const raylib::Vector2 w) {
    raylib::Vector2 s = w - v;
    float t = (s.DotProduct(p)) / s.DotProduct(s);
    raylib::Vector2 res = s * t;
    return res;
}

bool point_in_triangle(const raylib::Vector2 p, const raylib::Vector2 a, const raylib::Vector2 b, const raylib::Vector2 c) {
    auto [x, y] = p;
    auto [x1, y1] = a;
    auto [x2, y2] = b;
    auto [x3, y3] = c;
    float denominator = ((y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3));
    float bary_a = ((y2 - y3)*(x - x3) + (x3 - x2)*(y - y3)) / denominator;
    float bary_b = ((y3 - y1)*(x - x3) + (x1 - x3)*(y - y3)) / denominator;
    float bary_c = 1.0 - bary_a - bary_b;

    return 0.0 <= bary_a && bary_a <= 1.0 &&
           0.0 <= bary_b && bary_b <= 1.0 &&
           0.0 <= bary_c && bary_c <= 1.0;
}
