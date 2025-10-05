#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "raylib-cpp.hpp"
#include "rectobstacle.hpp"
#include "lineobstacle.hpp"

#include <cstdint>
#include <optional>

struct Particle {
    raylib::Vector2 m_position;
    raylib::Vector2 m_old_position;
    raylib::Vector2 m_acceleration;
    raylib::Color m_color;
    bool m_alive;
    uint32_t m_radius;
    float m_bounciness;

    Particle(raylib::Vector2 position, Color color);

    bool operator==(const Particle& other) const;

    static bool collide(const Particle& p, const Particle& q);
    void draw() const;
    void update(const float);

    void give_acceleration(const raylib::Vector2 a);
    raylib::Vector2 get_velocity(const float dt) const;
    void give_velocity(const raylib::Vector2 v, const float dt);

    void collide_with_particle(Particle& other);
    void collide_with_rect(const RectObstacle&, const float dt);
    void collide_with_line(const LineObstacle&,  const float dt);
};

#endif
