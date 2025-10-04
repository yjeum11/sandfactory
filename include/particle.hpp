#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "raylib-cpp.hpp"
#include "rectobstacle.hpp"

#include <cstdint>
#include <optional>

struct Particle {
    raylib::Vector2 m_position;
    raylib::Vector2 m_old_position;
    raylib::Color m_color;
    bool m_alive;
    uint32_t m_radius;

    Particle(raylib::Vector2 position, Color color);

    bool operator==(const Particle& other) const;

    static bool collide(const Particle& p, const Particle& q);
    void draw() const;
    void update(const float, const float);

    void give_velocity(const raylib::Vector2 v, const float dt);

    bool collide_with_rect(const RectObstacle&);
    std::optional<raylib::Vector2> collide_with_line(const raylib::Vector2, const raylib::Vector2) const;
};

#endif
