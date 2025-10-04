#include "raylib.h"
#include "raymath.h"
#include "particle.hpp"
#include "util.hpp"

#include <optional>
#include <iostream>

Particle::Particle(raylib::Vector2 position, Color color = RED) :
    m_position(position),
    m_old_position(position),
    m_color(color),
    m_alive(true),
    m_radius(5)
{}

bool Particle::collide(const Particle& p, const Particle& q) {
    raylib::Vector2 diff = p.m_position - q.m_position;
    float dist {diff.Length()};
    return dist < (p.m_radius + q.m_radius);
}

void Particle::draw() const {
    DrawCircleV(m_position, m_radius, m_color);
}

void Particle::update(const float dt, const float g) {
    raylib::Vector2 displacement = m_position - m_old_position;
    m_old_position = m_position;
    m_position = m_position + displacement + Vector2({0, g}) * (dt * dt);
}

void Particle::give_velocity(const raylib::Vector2 v, const float dt) {
    m_old_position = Vector2Add(m_position, Vector2Scale(v, -dt));
    m_old_position = m_position + (v * -dt);
}

bool Particle::collide_with_rect(const RectObstacle& rect) {
    return true;
}

std::optional<raylib::Vector2> Particle::collide_with_line(const raylib::Vector2 v, const raylib::Vector2 w) const {
    raylib::Vector2 projection = v + project_point_to_line(m_position, v, w);

    // check projection is in the line segment. Colinearity is guaranteed
    if (!(fmin(v.x, w.x) <= projection.x &&
          projection.x   <= fmax(v.x, w.x) &&
          fmin(v.y, w.y) <= projection.y &&
          projection.y   <= fmax(v.x, w.y))) return {};

    raylib::Vector2 foot_to_center = m_position - projection;
    float foot_to_center_dist = foot_to_center.Length();

    // No intersection
    if (foot_to_center_dist > m_radius) return {};

    raylib::Vector2 foot_to_center_n = foot_to_center * (m_radius - foot_to_center_dist) / foot_to_center_dist;

    return foot_to_center_n;
}

