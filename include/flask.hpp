#ifndef FLASK_HPP
#define FLASK_HPP

#include "Color.hpp"
#include "raylib-cpp.hpp"
#include "particle.hpp"
#include "util.hpp"
#include "rectobstacle.hpp"

struct Flask {
    int m_height;
    int m_width;
    float m_angle;
    const std::vector<RectObstacle> m_base_shapes;
    std::vector<RectObstacle> m_shapes;
    raylib::Vector2 m_position;
    RectObstacle m_bbox;

    Flask(int height, int width, int thickness):
        m_height(height),
        m_width(width),
        m_angle(0.0),
        m_base_shapes {
            RectObstacle(0, 0, thickness, height, raylib::BLACK, 0.7),
            RectObstacle(width, 0, thickness, height, raylib::BLACK, 0.7),
            RectObstacle(0, height, width+thickness, thickness, raylib::BLACK, 0.7),
        },
        m_shapes {m_base_shapes},
        m_position(raylib::Vector2(0.0)),
        m_bbox {
            0, 0, width, height, LIGHTGRAY, 0.0
        }
    { }

    void move (raylib::Vector2 dest) {
        m_position = dest;
        m_bbox.move(dest);
        for (int i = 0; i < m_base_shapes.size(); i++) {
            Rectangle rect = m_base_shapes[i].get_rect();
            m_shapes[i] = m_base_shapes[i];
            m_shapes[i].move({rect.x + m_position.x, rect.y + m_position.y});
        }
    }

    void rotate (float angle, raylib::Vector2 about) {
        m_angle = angle;
        m_bbox.rotate_about(angle, about);
        // about is relative to the topleft of the entire flask
        for (int i = 0; i < m_shapes.size(); i++) {
            // calculate the final place of the topleft corner of each rectangle
            m_shapes[i].rotate(angle);
            raylib::Vector2 topleft = m_shapes[i].get_points()[0];
            raylib::Vector2 newpos = (topleft - (m_position + about)).Rotate(angle * DEG2RAD) + m_position + about;
            m_shapes[i].move(newpos);
        }
    }

    bool within (raylib::Vector2 v) {
        auto [a, b, c, d] = m_bbox.get_points();
        return point_in_triangle(v, a, b, c) || point_in_triangle(v, a, c, d);
    }

    void draw() {
        for (auto& r : m_shapes) {
            r.draw();
        }
    }

    void collide_with_particle(Particle& p, float dt) {
        for (auto& r : m_shapes) {
            p.collide_with_rect(r, dt);
        }
    }

};

#endif
