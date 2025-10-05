#include "mouse.hpp"
#include "raylib-cpp.hpp"

MouseMagnet::MouseMagnet(float r, float k, float b) {
    m_radius = r;
    m_strength = k;
    m_damping = b;
}

void MouseMagnet::update(Particle& p, float dt) const {
    if (raylib::Mouse::IsButtonDown(MOUSE_BUTTON_LEFT)) {
        raylib::Vector2 mouse_pos = raylib::Mouse::GetPosition();
        float mouse_dist = (mouse_pos - p.m_position).Length();
        if (mouse_dist < m_radius) {
            raylib::Vector2 n = (mouse_pos - p.m_position).Normalize();
            raylib::Vector2 undamped = (n * (m_strength / mouse_dist * mouse_dist));
            raylib::Vector2 damping = p.get_velocity(dt) * (-m_damping); 
            p.give_acceleration(undamped + damping);
        }
    }
}

void MouseMagnet::draw() const {
    if (raylib::Mouse::IsButtonDown(MOUSE_BUTTON_LEFT)) {
        raylib::Vector2 mouse_pos = GetMousePosition();
        DrawCircleLines(mouse_pos.x, mouse_pos.y, m_radius, BLACK);
    }
}
