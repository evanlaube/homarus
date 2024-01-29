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
    
    Polygon *p = new Polygon(verts);
    Fixture f;
    f.setShape(p);

    Body body(Vec2d(200, 100), 0, f);
    world.addBody(body);

    while(renderer.close == false) {
        renderer.draw();
        renderer.update();
    }

    return 0;
}
