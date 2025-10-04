#ifndef RECT_OBSTACLE_HPP
#define RECT_OBSTACLE_HPP
#include <optional>
#include <algorithm>
#include <array>
#include <cmath>

#include "raylib.h"

class RectObstacle {

private:
    float m_angle;
    Rectangle m_rect;

    std::array<Vector2, 4> m_points;
    std::array<Vector2, 4> m_normals;

public:

    Color m_color;

    RectObstacle(int left, int top, int width, int height, Color color):
        m_angle(0.0), m_color(color)
    { 
        m_rect = Rectangle{(float)left, (float)top, (float)width, (float)height};
        m_points = {
            (Vector2){(float)left, (float)top},
            (Vector2){(float)left, (float)top+height},
            (Vector2){(float)left+width, (float)top+height},
            (Vector2){(float)left+width, (float)top}
        };

        std::transform(m_points.begin(), m_points.end(), m_normals.begin(), 
            [](Vector2 v){
                return (Vector2) {-v.y, v.x};
            }
        );
    }

    RectObstacle(int left, int top, int width, int height, Color color, float angle):
        m_angle(angle), m_color(color)
    { 
        m_rect = Rectangle{(float)left, (float)top, (float)width, (float)height};
        rotate(angle);
    }

    void draw() const {
        DrawTriangleFan(m_points.data(), m_points.size(), BLACK);
    }

    void rotate(float angle) {
        m_angle = angle;
        auto& [a, b, c, d] = m_points;
        float coswidth = m_rect.width * std::cosf(angle * DEG2RAD);
        float sinwidth = m_rect.width * std::sinf(angle * DEG2RAD);
        float cosheight = m_rect.height * std::cosf(angle * DEG2RAD);
        float sinheight = m_rect.height * std::sinf(angle * DEG2RAD);

        a = {m_rect.x, m_rect.y};
        b = {m_rect.x - sinheight, m_rect.y + cosheight};
        c = {m_rect.x + coswidth - sinheight, m_rect.y + sinwidth + cosheight};
        d = {m_rect.x + coswidth, m_rect.y + sinwidth};

        std::transform(m_points.begin(), m_points.end(), m_normals.begin(), 
            [](Vector2 v){
                return (Vector2) {-v.y, v.x};
            }
        );
    }

    float get_angle() const {
        return m_angle;
    }

    Rectangle get_rect() const {
        return m_rect;
    }

    std::array<Vector2, 4> get_points() const {
        return m_points;
    }

    void move(Vector2 newpos) {
        m_rect.x = newpos.x;
        m_rect.y = newpos.y;

        m_points[0] = newpos;
        rotate(m_angle);
    }

};
#endif
