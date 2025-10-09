#include <math.h>
#include <stdlib.h>
#include <unordered_set>
#include <random>
#include <iostream>
#include <cstdio>

#include "Camera2D.hpp"
#include "mouse.hpp"
#include "raylib-cpp.hpp"
#include "particle.hpp"
#include "rectobstacle.hpp"
#include "lineobstacle.hpp"
#include "util.hpp"
#include "flask.hpp"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    using raylib::Vector2;
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    raylib::Window window(screenWidth, screenHeight, "raylib [core] example - basic window");

    raylib::Camera2D camera = raylib::Camera2D( Vector2(screenWidth / 2.0, screenHeight / 2.0) , Vector2(screenWidth / 2.0, screenHeight / 2.0));

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

    // obstacles.push_back(rotated);

    Vector2 t{426, 300};
    Vector2 u{326, 300};
    Vector2 v{326, 358};
    Vector2 w{426, 358};

    std::vector<LineObstacle> lines = {
        // LineObstacle(v, w),
        // LineObstacle(u, v),
        // LineObstacle(t, w),
    };

    MouseMagnet magnet = MouseMagnet(30, 500, 2);

    Vector2 flask_location {255, 323};
    float flask_angle = 5.0f;

    Flask test_flask = Flask(100, 70, 5);

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
                auto n = std::make_unique<Particle>(Particle ({288.0, 154.0}, RED, 0.9));
                uint32_t r = uint_dist(rng);
                float angle = 10 * sinf((float)r) + 90.0;
                n->give_velocity(Vector2(cosf(angle), sinf(angle)) * 4.0f, dt_sub);
                n->give_acceleration(Vector2(0.0, g));
                particles.insert(std::move(n));
                counter = 10;
            } else {
                counter--;
            }


            // Update
            //----------------------------------------------------------------------------------

            for (int i{0}; i < SUBSTEPS; i++) {
                test_flask.move(flask_location);
                test_flask.rotate(flask_angle, Vector2 {35, 50});

                // Mouse interaction
                for (auto& p : particles) {
                    p->give_acceleration(Vector2(0.0, g));
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
                    test_flask.collide_with_particle(*p, dt_sub);
                }

                if (raylib::Keyboard::IsKeyDown(KEY_D)) {
                    flask_location += {2.0/SUBSTEPS, 0};
                } else if (raylib::Keyboard::IsKeyDown(KEY_A)) {
                    flask_location -= {2.0/SUBSTEPS, 0};
                } else if (raylib::Keyboard::IsKeyDown(KEY_W)) {
                    flask_location -= {0, 2.0/SUBSTEPS};
                } else if (raylib::Keyboard::IsKeyDown(KEY_S)) {
                    flask_location += {0, 2.0/SUBSTEPS};
                }

                if (raylib::Keyboard::IsKeyDown(KEY_R)) {
                    flask_angle += 1.0/SUBSTEPS;
                } else if (raylib::Keyboard::IsKeyDown(KEY_T)) {
                    flask_angle -= 1.0/SUBSTEPS;
                }

                // constrain to circle
                const float constrain_radius = 200;
                for (auto& p : particles) {
                    Vector2 center = {screenWidth / 2.0, screenHeight / 2.0};
                    Vector2 to_obj = p->m_position - center;
                    if (to_obj.Length() > constrain_radius - p->m_radius) {
                        p->m_position = center + (to_obj * (constrain_radius - p->m_radius) / to_obj.Length());
                    }
                }

                // Run Verlet integration and erase other particles
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

            camera.BeginMode();

                Vector2 center = {screenWidth / 2.0, screenHeight / 2.0};
                center.DrawCircle(200, RAYWHITE);


                for (auto& l : lines) {
                    DrawLineV(l.m_v, l.m_w, BLACK);
                }

                for (auto& p : particles) {
                    p->draw();
                }

                for (auto& r : obstacles) {
                    r.draw();
                }

                test_flask.draw();

                magnet.draw();

            camera.EndMode();
            Vector2 mousePos = GetMousePosition();
            char mouseText[512];
            snprintf(mouseText, 512, "X: %d, Y: %d", (int)mousePos.x, (int)mousePos.y);

            DrawText(mouseText, 190, 200, 20, LIGHTGRAY);

            char particleText[512];
            snprintf(particleText, 512, "Particles: %d", (int)particles.size());
            DrawText(particleText, 190, 100, 20, LIGHTGRAY);
            DrawFPS(190, 150);


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    return 0;
}
