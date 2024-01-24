
#include <iostream>

#include "homarus.h"

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

    Fixture f;

    Body b = Body(Vec2d(100, 100), 0, f);
    
    world.addBody(b);

    Renderer renderer = Renderer(&world);

    renderer.init();

    // Simple test of Shape::calcArea and Shape::calcCentroid using a square
    Vec2d tl(0, 40);
    Vec2d tr(40, 40);
    Vec2d br(40, 0);
    Vec2d bl(0, 0);

    std::vector<Vec2d> verts;
    verts.push_back(tl);
    verts.push_back(tr);
    verts.push_back(br);
    verts.push_back(bl);

    Polygon p(verts);
    
    while(renderer.close == false) {
        renderer.draw();
        renderer.drawShape(p, 50, 50, 0);
        renderer.update();
    }

    return 0;
}
