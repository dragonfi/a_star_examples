
#include <iostream>
#include <stddef.h>
#include <vector>
#include <exception>

#include <SDL2/SDL.h>

#include "pathing/a_star.hpp"
#include "pathing/randomPoints.hpp"
#include "pathing/graph.hpp"
#include "graphics/glWindow.hpp"

bool handle_key_down(const SDL_Event& event, graphics::Renderer& renderer) {
    const graphics::Color red(1.0, 0.0, 0.0);
    const graphics::Color green(0.0, 1.0, 0.0);
    const graphics::Color blue(0.0, 0.0, 1.0);
    switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            return false;
            break;
        case SDLK_r:
            renderer.clear(red);
            renderer.swap();
            break;
        case SDLK_g:
            renderer.clear(green);
            renderer.swap();
            break;
        case SDLK_b:
            renderer.clear(blue);
            renderer.swap();
            break;
        default:
            break;
    }
    return true;
}

void main_loop(graphics::Renderer renderer) {
    bool running = true;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    running = handle_key_down(event, renderer);
                    break;
                default:
                    break;
            }
        }
        renderer.swap();
    }
}

int main() {
    // initi video
    graphics::Window window;
    graphics::Renderer renderer(window);
    graphics::Color black(0.0, 0.0, 0.0);
    renderer.clear(black);
    renderer.swap();

    main_loop(renderer);

    // main
    auto points = pathing::randomPoints(1000, {0, 0}, {100, 100});
    auto graph = pathing::connectPointsWithinThreshold(points, 5);
    pathing::AStar aStar(graph);
    std::cout << aStar.shortest_path(700, 500) << std::endl;
    for(size_t j = 0; j < points.size(); j++) {
        //std::cout << j << " ";
        auto path = aStar.shortest_path(0, j);
        //std::cout << path.nodes.size() << std::endl;
    }

    size_t sum = 0;
    for(size_t i = 0; i < points.size(); i++) {
        sum += graph.edgesFrom(i).size();
    }
    std::cout << points.size() << " " << sum << std::endl;

    return 0;
}