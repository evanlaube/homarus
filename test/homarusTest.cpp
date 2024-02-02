#include <cmath>
#include <iostream>

#include "homarus.h"

#include "physics/fixture.h"
#include "physics/world.h"
#include "renderer.h"
#include "shapes/shape.h"
#include "shapes/circle.h"
#include "shapes/polygon.h"
#include "util/vec2.h"
#include "util/blockallocator.h"

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

    //  Steps for creating a new body: 
    //      1. Create empty body with world
    //      2. Create a shape of some kind
    //      3. Create a fixture and attach the shape
    //      4. Attach fixture to body
    //      5. Set a position and angle to the body
    
    Polygon p(verts);
    Fixture* f = new Fixture(&p);
    Body* b1 = world.createBody(f);
    float x = 100;
    b1->setPos(Vec2d(x, 100));
    std::cout << "BodyLink: " << world.bodyLink << std::endl;

    Polygon p2(verts);
    Fixture* f2 = new Fixture(&p2);
    Body* b2 = world.createBody(f2);
    b2->setPos(Vec2d(x, 400));
    b2->rotate(M_PI/2);
    std::cout << "BodyLink: " << world.bodyLink << std::endl;
    
    while(renderer.close == false) {
        b2->rotate(M_PI/1200);
        renderer.draw();
        renderer.update();
    }


    return 0;
}
