#ifndef UTIL_HPP
#define UTIL_HPP

#include "raylib-cpp.hpp"

raylib::Vector2 project_point_to_line(const raylib::Vector2 p, const raylib::Vector2 v, const raylib::Vector2 w);
raylib::Vector2 project_vector_to_line(const raylib::Vector2 p, const raylib::Vector2 v, const raylib::Vector2 w);

#endif
