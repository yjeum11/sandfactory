#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include "particle.hpp"
#include "raylib-cpp.hpp"
#include "rectobstacle.hpp"
#include "lineobstacle.hpp"

#include <cstdint>
#include <optional>

struct ParticleSystem {

    std::vector<Particle> m_particles;


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
