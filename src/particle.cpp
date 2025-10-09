#include "raylib.h"
#include "raymath.h"
#include "particle.hpp"
#include "util.hpp"

#include <optional>
#include <iostream>

Particle::Particle(raylib::Vector2 position, Color color = RED) :
    m_position(position),
    m_old_position(position),
    m_acceleration(raylib::Vector2(0.0)),
    m_color(color),
    m_alive(true),
    m_radius(2),
    m_bounciness(0.7)
{}

Particle::Particle(raylib::Vector2 position, Color color, float bounciness) :
    m_position(position),
    m_old_position(position),
    m_acceleration(raylib::Vector2(0.0)),
    m_color(color),
    m_alive(true),
    m_radius(2),
    m_bounciness(bounciness)
{}

void Particle::draw() const {
    DrawCircleV(m_position, m_radius, m_color);
}

void Particle::update(const float dt) {
    raylib::Vector2 displacement = m_position - m_old_position;
    m_old_position = m_position;
    m_position += displacement + m_acceleration * (dt * dt);
}

void Particle::give_acceleration(const raylib::Vector2 a) {
    m_acceleration = a;
}

void Particle::give_velocity(const raylib::Vector2 v, const float dt) {
    m_old_position = m_position + (v * -dt);
}

raylib::Vector2 Particle::get_velocity(const float dt) const {
    return (m_position - m_old_position) / dt;
}

void Particle::collide_with_particle(Particle& other) {
    raylib::Vector2 diff = m_position - other.m_position;
    float dist {diff.Length()};
    if (dist < (m_radius + other.m_radius)) {
        const raylib::Vector2 diff = m_position - other.m_position;
        const float dist {diff.Length()};

        const float delta {m_radius + other.m_radius - dist};

        m_position += diff.Normalize() * (0.5*m_bounciness*delta);
        other.m_position -= diff.Normalize() * (0.5*m_bounciness*delta);
    }
}

void Particle::collide_with_rect(const RectObstacle& rect, const float dt) {
    using raylib::Vector2;
    // collide with edges
    for (auto& l : rect.get_edges()) {
        collide_with_line(l, dt);
    }
    // prevent from being inside the rectangle
    // auto [a, b, c, d] = rect.get_points();
    // if (point_in_triangle(m_position, a, b, c) || point_in_triangle(m_position, c, d, a)) {
    //     for (int i = 0; i < 4; i++) {
    //         LineObstacle line = rect.get_edges()[i];
    //         Vector2 v = line.m_v;
    //         Vector2 w = line.m_w;
    //         Vector2 projection = project_point_to_line(m_position, v, w);
    //         // check projection is in the line segment. Colinearity is guaranteed
    //         if (!(fmin(v.x, w.x) <= projection.x &&
    //               projection.x   <= fmax(v.x, w.x) &&
    //               fmin(v.y, w.y) <= projection.y &&
    //               projection.y   <= fmax(v.y, w.y))) continue;
    //         Vector2 foot_to_center = m_position - projection;
    //         bool is_inside = foot_to_center.DotProduct(rect.get_normals()[i]) < 0;
    //         float foot_to_center_dist = foot_to_center.Length();
    //
    //     }
    // }
}

void Particle::collide_with_line(const LineObstacle& line, const float dt) {
    raylib::Vector2 v = line.m_v;
    raylib::Vector2 w = line.m_w;
    raylib::Vector2 projection = project_point_to_line(m_position, v, w);

    // check projection is in the line segment. Colinearity is guaranteed
    if (!(fmin(v.x, w.x) <= projection.x &&
          projection.x   <= fmax(v.x, w.x) &&
          fmin(v.y, w.y) <= projection.y &&
          projection.y   <= fmax(v.y, w.y))) return;

    raylib::Vector2 foot_to_center = m_position - projection;
    float foot_to_center_dist = foot_to_center.Length();

    // No intersection
    if (foot_to_center_dist > m_radius) return;

    raylib::Vector2 foot_to_center_n = foot_to_center * (m_radius - foot_to_center_dist) / foot_to_center_dist;

    raylib::Vector2 old_velocity = get_velocity(dt);

    raylib::Vector2 parallel = project_vector_to_line(old_velocity, v, w);
    raylib::Vector2 perpendicular = old_velocity - parallel;

    raylib::Vector2 new_perp = perpendicular * -(1-(m_bounciness * line.m_bounciness));
    raylib::Vector2 new_velocity = parallel + new_perp;

    m_position += foot_to_center_n;
    give_velocity(new_velocity, dt);
}

