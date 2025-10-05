#include "raylib-cpp.hpp"
#include "lineobstacle.hpp"

LineObstacle::LineObstacle(raylib::Vector2 v, raylib::Vector2 w) :
    m_v(v),
    m_w(w),
    m_bounciness(1.0)
{}

LineObstacle::LineObstacle(raylib::Vector2 v, raylib::Vector2 w, float bounciness) :
    m_v(v),
    m_w(w),
    m_bounciness(bounciness)
{}
