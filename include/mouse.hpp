#ifndef MOUSE_HPP
#define MOUSE_HPP

#include "particle.hpp"

struct MouseMagnet {
    float m_radius;
    float m_strength;
    float m_damping;

    MouseMagnet(float r, float k, float b);
    void update(Particle& p, float dt) const;
    void draw() const;
};

#endif
