#include <iostream>

#include "homarus.h"

#include "physics/fixture.h"
#include "renderer.h"
#include "shapes/shape.h"
#include "shapes/circle.h"
#include "shapes/polygon.h"
#include "util/vec2.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

int main() {
    World world;
    
    Renderer renderer = Renderer(&world);

    renderer.init();

    // Simple test of Shape::calcArea and Shape::calcCentroid using a square
    std::vector<Vec2d> verts;
    verts.push_back(Vec2d(50, 50));
    verts.push_back(Vec2d(150, 50));
    verts.push_back(Vec2d(100, 150));

    // Steps for creating a new body: 
    //      1. Create a New shape
    //      2. Create a fixture and attach shape
    //      3. Create a body and attach fixture
    //      4. Add body to world

    Polygon *p = new Polygon(verts);
    Fixture f(p);

    delete p;

    Body body(Vec2d(200, 100), 0, f);
    world.addBody(body);

    while(renderer.close == false) {
        renderer.draw();
        renderer.update();
    }

    return 0;
}
