#include <math.h>
#include <stdlib.h>
#include <unordered_set>
#include <random>
#include <iostream>
#include <cstdio>

#include "raylib-cpp.hpp"
#include "mouse.hpp"
#include "particle.hpp"
#include "rectobstacle.hpp"
#include "lineobstacle.hpp"
#include "util.hpp"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    raylib::Window window(screenWidth, screenHeight, "raylib [core] example - basic window");
    // InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetExitKey(KEY_NULL);
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    bool exit_requested = false;

    // int num_particles = 3;
    const float dt = 0.05;
    const float g = 80.0f;

    const int SUBSTEPS = 8;

    std::unordered_set<std::unique_ptr<Particle>> particles;

    std::mt19937 rng;
    uint32_t seed = 100;
    rng.seed(seed);

    std::uniform_int_distribution<uint32_t> uint_dist;

    int counter = 0;
    bool paused = false;

    std::vector<RectObstacle> obstacles;

    RectObstacle rotated = RectObstacle(300, 300, 50, 30, BLACK, 0.8);

    obstacles.push_back(rotated);

    raylib::Vector2 t{426, 300};
    raylib::Vector2 u{326, 300};
    raylib::Vector2 v{326, 358};
    raylib::Vector2 w{426, 358};

    std::vector<LineObstacle> lines = {
        LineObstacle(v, w),
        LineObstacle(u, v),
        LineObstacle(t, w),
    };

    MouseMagnet magnet = MouseMagnet(30, 500, 2);

    // Main game loop
    while (!exit_requested)
    {
        if (WindowShouldClose() || IsKeyDown(KEY_ESCAPE) || IsKeyDown(KEY_Q)) {
            exit_requested = true;
        }

        if (IsKeyPressed(KEY_P)) {
            paused = !paused;
        }

        if (!paused || IsKeyPressed(KEY_PERIOD)) {
            float dt_sub = dt / SUBSTEPS;

            if (counter == 0) {
                auto n = std::make_unique<Particle>(Particle ({288.0, 154.0}, RED));
                n->give_velocity(raylib::Vector2(0, 0) * 4.0f, dt_sub);
                n->give_acceleration(raylib::Vector2(0.0, g));
                particles.insert(std::move(n));
                counter = 20;
            } else {
                counter--;
            }

            // Update
            //----------------------------------------------------------------------------------

            obstacles[0].rotate(obstacles[0].get_angle() + 1);

            for (int i{0}; i < SUBSTEPS; i++) {
                // Mouse interaction
                for (auto& p : particles) {
                    p->give_acceleration(raylib::Vector2(0.0, g));
                    magnet.update(*p, dt_sub);
                }

                // ball-to-ball collision
                for (auto p_it = particles.begin(); p_it != particles.end(); p_it++) {
                    auto& p = *p_it;
                    for (auto q_it = std::next(p_it); q_it != particles.end(); q_it++) {
                        auto& q = *q_it;
                        p->collide_with_particle(*q);
                    }
                }

                // ball-to-line collision
                for (auto& p : particles) {
                    for (auto& l : lines) {
                        p->collide_with_line(l, dt_sub);
                    }
                    for (auto& r : obstacles) {
                        p->collide_with_rect(r, dt_sub);
                    }
                }

                const float constrain_radius = 200;
                for (auto& p : particles) {
                    raylib::Vector2 center = {screenWidth / 2.0, screenHeight / 2.0};
                    raylib::Vector2 to_obj = p->m_position - center;
                    if (to_obj.Length() > constrain_radius - p->m_radius) {
                        p->m_position = center + (to_obj * (constrain_radius - p->m_radius) / to_obj.Length());
                    }
                }

                std::vector<decltype(particles)::iterator> to_erase;
                for (auto it{particles.begin()}; it != particles.end(); ++it) {
                    auto& p = *it;
                    p->update(dt_sub);
                    if (p->m_position.x > screenWidth || p->m_position.y > screenHeight) {
                        to_erase.push_back(it);
                    }
                }
                for (auto& e : to_erase) {
                    particles.erase(e);
                }
            }
        }

        //----------------------------------------------------------------------------------
        //
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GRAY);

            raylib::Vector2 center = {screenWidth / 2.0, screenHeight / 2.0};
            center.DrawCircle(200, RAYWHITE);

            raylib::Vector2 mousePos = GetMousePosition();
            char mouseText[512];
            snprintf(mouseText, 512, "X: %d, Y: %d", (int)mousePos.x, (int)mousePos.y);

            DrawText(mouseText, 190, 200, 20, LIGHTGRAY);
            DrawFPS(190, 150);

            for (auto& l : lines) {
                DrawLineV(l.m_v, l.m_w, BLACK);
            }

            for (auto& p : particles) {
                p->draw();
            }

            for (auto& r : obstacles) {
                r.draw();
            }

            magnet.draw();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    return 0;
}
