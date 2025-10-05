#ifndef LINE_OBSTACLE_HPP
#define LINE_OBSTACLE_HPP

#include "raylib-cpp.hpp"

struct LineObstacle {
    raylib::Vector2 m_v;
    raylib::Vector2 m_w;
    float m_bounciness;

    LineObstacle(raylib::Vector2 v, raylib::Vector2 w);
    LineObstacle(raylib::Vector2 v, raylib::Vector2 w, float bounciness);
};

#endif
