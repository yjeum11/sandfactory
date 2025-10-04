#include <math.h>
#include <stdlib.h>
#include <unordered_set>
#include <random>
#include <iostream>
#include <cstdio>

#include "Window.hpp"
#include "raylib.h"
#include "raymath.h"
#include "particle.hpp"
#include "rectobstacle.hpp"
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

    const int SUBSTEPS = 2;

    std::unordered_set<std::unique_ptr<Particle>> particles;

    std::mt19937 rng;
    uint32_t seed = 100;
    rng.seed(seed);

    std::uniform_int_distribution<uint32_t> uint_dist;

    int counter = 0;
    bool paused = false;

    std::vector<RectObstacle> obstacles;

    RectObstacle rotated = RectObstacle(300, 300, 50, 30, BLACK, 0);

    obstacles.push_back(rotated);

    raylib::Vector2 v{100, 200};
    raylib::Vector2 w{466, 400};

    // Main game loop
    while (!exit_requested)
    {
        if (WindowShouldClose() || IsKeyDown(KEY_ESCAPE) || IsKeyDown(KEY_Q)) {
            exit_requested = true;
        }

        if (IsKeyDown(KEY_P)) {
            paused = true;
        }

        if (!paused || IsKeyPressed(KEY_PERIOD)) {
            float dt_sub = dt / SUBSTEPS;

            if (counter == 0) {
                auto n = std::make_unique<Particle>(Particle ({288.0, 154.0}, RED));
                n->give_velocity(raylib::Vector2(0, 0) * 4.0f, dt_sub);
                particles.insert(std::move(n));
                counter = 100;
            } else {
                counter--;
            }

            // Update
            //----------------------------------------------------------------------------------

            obstacles[0].rotate(obstacles[0].get_angle() + 1);
            obstacles[0].move({obstacles[0].get_rect().x + 1, obstacles[0].get_rect().y});

            for (int i{0}; i < SUBSTEPS; i++) {
                std::vector<decltype(particles)::iterator> to_erase;
                for (auto it{particles.begin()}; it != particles.end(); ++it) {
                    auto& p = *it;
                    p->update(dt_sub, g);
                    if (p->m_position.x > screenWidth || p->m_position.y > screenHeight) {
                        to_erase.push_back(it);
                    }
                }
                for (auto& e : to_erase) {
                    particles.erase(e);
                }

                // ball-to-ball collision
                for (auto p_it = particles.begin(); p_it != particles.end(); p_it++) {
                    auto& p = *p_it;
                    for (auto q_it = std::next(p_it); q_it != particles.end(); q_it++) {
                        auto& q = *q_it;
                        if (Particle::collide(*p, *q)) {
                            const raylib::Vector2 diff = p->m_position - q->m_position;
                            const float dist {diff.Length()};

                            const float delta {p->m_radius+q->m_radius - dist};

                            p->m_position += diff.Normalize() * (0.5*delta);
                            q->m_position -= diff.Normalize() * (0.5*delta);

                        }
                    }
                }

                // ball-to-line collision
                for (auto& p : particles) {
                    std::optional<raylib::Vector2> correction = p->collide_with_line(v, w);
                    if (correction.has_value()) {
                        raylib::Vector2 old_velocity = (p->m_position - p->m_old_position) * (1/dt_sub);

                        // break down old_velocity to parallel and perpendicular

                        raylib::Vector2 parallel = project_vector_to_line(old_velocity, v, w);
                        raylib::Vector2 perpendicular = old_velocity - parallel;

                        raylib::Vector2 new_perp = perpendicular * -0.8;
                        raylib::Vector2 new_velocity = parallel + new_perp;

                        p->m_position += correction.value();
                        p->give_velocity(new_velocity, dt_sub);
                    }
                }

                // const float constrain_radius = 200;
                // for (auto& p : particles) {
                //     Vector2 center = {screenWidth / 2.0, screenHeight / 2.0};
                //     Vector2 to_obj = Vector2Subtract(p->m_position, center);
                //     if (Vector2Length(to_obj) > constrain_radius - p->m_radius) {
                //         p->m_position = Vector2Add(center, Vector2Scale(Vector2Normalize(to_obj), constrain_radius - p->m_radius));
                //     }
                // }


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

            DrawLineV(v, w, BLACK);

            for (auto& p : particles) {
                DrawCircleV(p->m_old_position, p->m_radius, GRAY);
                p->draw();
            }

            for (auto& r : obstacles) {
                r.draw();
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
