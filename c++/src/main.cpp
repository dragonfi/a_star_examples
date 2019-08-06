
#include <iostream>
#include <stddef.h>
#include <vector>
#include <algorithm>

#include <SDL2/SDL.h>

#include "pathing/a_star.hpp"
#include "pathing/randomPoints.hpp"
#include "pathing/graph.hpp"
#include "graphics/glWindow.hpp"
#include "graphics/shader.hpp"
#include "graphics/vao.hpp"

#include "ostream_helpers/ostream_helpers.hpp"


#include<fstream>
#include<sstream>
#include<string>

std::string load_file(std::string file_name) {
    std::ifstream f(file_name);
    std::stringstream stream;
    stream << f.rdbuf();

    return stream.str();
}

void render_square(graphics::Renderer& renderer) {
    graphics::ShaderProgram shader(
        load_file("shaders/basic.vert"), load_file("shaders/basic.frag")
    );

    const std::vector<graphics::Point> points = {
        { -0.5,  0.5,  0.5 }, // Top left
        {  0.5,  0.5,  0.5 }, // Top right
        {  0.5, -0.5,  0.5 }, // Bottom right 
        { -0.5, -0.5,  0.5 }, // Bottom left
    };


    const std::vector<graphics::Color> colors = {
        { 0.0, 1.0, 0.0, 1.0 }, // Top left
        { 1.0, 1.0, 0.0, 1.0  }, // Top right
        { 1.0, 0.0, 0.0, 1.0  }, // Bottom right 
        { 0.0, 0.0, 1.0, 1.0  }, // Bottom left
    };

    graphics::StaticVAO square(GL_TRIANGLE_FAN, points, colors);

    shader.use();
    // GL_LINES, GL_TRIANGLE_FAN, GL_LINES, GL_LINE_LOOP
    square.draw(GL_LINE_LOOP);

}

std::vector<graphics::StaticVAO> vaos_from_meta(pathing::PathWithMeta, pathing::IndexedGraph);

std::vector<graphics::StaticVAO> generate_new_graph() {
    auto points = pathing::randomPoints(1000, {-1.0, -1.0}, {2.0, 2.0});
    auto graph = pathing::connectPointsWithinThreshold(points, 0.08);
    pathing::AStar aStar(graph);
    pathing::PathWithMeta meta = aStar.shortest_path_with_metadata(500, 700);
    return vaos_from_meta(meta, graph);
}

bool handle_key_down(const SDL_Event& event, graphics::Scene& scene) {
    switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            return false;
            break;
        case SDLK_r:
            scene.vaos = generate_new_graph();
            break;
        default:
            break;
    }
    return true;
}

void handle_resize(const int width, const int height) {
    int viewport_size = width < height ? width : height;
    if (viewport_size < width) {
        int offset = (width - viewport_size) / 2;
        glViewport(offset, 0, viewport_size, viewport_size);
    } else {
        int offset = (height - viewport_size) / 2;
        glViewport(0, offset, viewport_size, viewport_size);
    }
}

void main_loop(graphics::Scene scene) {
    bool running = true;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    running = handle_key_down(event, scene);
                    break;
                case SDL_WINDOWEVENT:
                    switch(event.window.event) {
                        case SDL_WINDOWEVENT_RESIZED:
                            handle_resize(event.window.data1, event.window.data2);
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        //render_square(renderer);
        // renderer.swap();
        scene.renderer.clear(graphics::Color{0.0, 0.0, 0.0, 1.0});
        scene.shader.use();
        for(graphics::StaticVAO& vao : scene.vaos) {
            vao.draw();
        }
        scene.renderer.swap();
    }
}

std::vector<graphics::StaticVAO> vaos_from_meta(pathing::PathWithMeta meta, pathing::IndexedGraph graph) {
    std::vector<graphics::StaticVAO> vaos;
    // edges
    {
        std::vector<graphics::Point> positions;
        std::vector<graphics::Color> colors;
        for(pathing::Edge edge: graph.edges()) {
            pathing::Vec2 source = graph.nodes()[edge.source];
            pathing::Vec2 dest = graph.nodes()[edge.dest];
            positions.push_back({static_cast<GLfloat>(source.x), static_cast<GLfloat>(source.y), 0.0});
            positions.push_back({static_cast<GLfloat>(dest.x), static_cast<GLfloat>(dest.y), 0.0});
            colors.push_back({0.3, 0.3, 0.3, 1.0});
            colors.push_back({0.3, 0.3, 0.3, 1.0});
        }

        vaos.push_back(
            graphics::StaticVAO(GL_LINES, positions, colors)
        );
    }

    // path
    {
        std::vector<graphics::Point> positions;
        std::vector<graphics::Color> colors;
        for(pathing::Index index: meta.path.nodes) {
            pathing::Vec2 node = graph.nodes()[index];
            positions.push_back({static_cast<GLfloat>(node.x), static_cast<GLfloat>(node.y), 0.0});
            colors.push_back({1.0, 1.0, 0.0, 1.0});
        }

        vaos.push_back(
            graphics::StaticVAO(GL_LINE_STRIP, positions, colors)
        );
    }

    // nodes
    {
        std::vector<graphics::Point> positions;
        std::vector<graphics::Color> colors;

        const std::vector<pathing::Vec2> nodes = graph.nodes();
        for(size_t i = 0; i < nodes.size(); i++) {
            positions.push_back({static_cast<GLfloat>(nodes[i].x), static_cast<GLfloat>(nodes[i].y), 0.0});
            if (i == 500) {
                colors.push_back({0.0, 1.0, 0.0, 1.0});
            } else if (i == 700) {
                colors.push_back({1.0, 0.0, 0.0, 1.0});
            } else if (std::find(meta.path.nodes.begin(), meta.path.nodes.end(), i) != meta.path.nodes.end()) {
                colors.push_back({1.0, 1.0, 1.0, 1.0});
            } else if (std::find(meta.candidates.begin(), meta.candidates.end(), i) != meta.candidates.end()) {
                colors.push_back({0.0, 0.8, 0.8, 1.0});
            } else if (std::find(meta.explored.begin(), meta.explored.end(), i) != meta.explored.end()) {
                colors.push_back({0.8, 0.0, 0.8, 1.0});
            } else {
                colors.push_back({0.5, 0.5, 0.5, 1.0});
            }
        }

        vaos.push_back(
            graphics::StaticVAO(GL_POINTS, positions, colors)
        );
    }

    return vaos;
}

int main() {
    graphics::Window window;
    graphics::Renderer renderer(window);
    graphics::Color black{0.0, 0.0, 0.0, 1.0};
    renderer.clear(black);
    renderer.swap();

    srand (time(NULL));

    auto points = pathing::randomPoints(1000, {-1.0, -1.0}, {2.0, 2.0});
    auto graph = pathing::connectPointsWithinThreshold(points, 0.08);
    pathing::AStar aStar(graph);
    pathing::PathWithMeta meta = aStar.shortest_path_with_metadata(500, 700);
    std::cout << meta.path << std::endl;

    graphics::ShaderProgram shader(
        load_file("shaders/basic.vert"), load_file("shaders/basic.frag")
    );

    std::vector<graphics::StaticVAO> vaos = vaos_from_meta(meta, graph);

    glPointSize(3);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    graphics::Scene scene{renderer, shader, vaos};

    main_loop(scene);

    // stress test

    /* for(size_t j = 0; j < points.size(); j++) {
        //std::cout << j << " ";
        auto path = aStar.shortest_path(0, j);
        //std::cout << path.nodes.size() << std::endl;
    }

    size_t sum = 0;
    for(size_t i = 0; i < points.size(); i++) {
        sum += graph.edgesFrom(i).size();
    }
    std::cout << points.size() << " " << sum << std::endl;
*/
    return 0;
}