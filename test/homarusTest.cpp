#include <chrono>
#include <cmath>
#include <cstdint>

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
#include <iostream>
#include <vector>

uint64_t getTime() {
    // Return integer of miliseconds since epoch
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

int main() {
    World world;
    
    Renderer renderer = Renderer(&world);

    renderer.init();

    // Simple test of Shape::calcArea and Shape::calcCentroid using a square
    //  Steps for creating a new body: 
    //      1. Create empty body with world
    //      2. Create a shape of some kind
    //      3. Create a fixture and attach the shape
    //      4. Attach fixture to body
    //      5. Set a position and angle to the body
    
    Circle c(75);
    Fixture* f = new Fixture(&c);
    Body* b1 = world.createBody(f);
    b1->setPos(Vec2d(400, 300));

    std::vector<Vec2d> verts;
    verts.push_back(Vec2d(0, -100));
    verts.push_back(Vec2d(100, 100));
    verts.push_back(Vec2d(-100, 100));

    Polygon p2(verts);
    Fixture* f2 = new Fixture(&p2);
    Body* b2 = world.createBody(f2);
    b2->setPos(Vec2d(540, 360));
    //b2->rotate(2* M_PI/3);

    uint64_t lastTime = getTime(); 

    while(renderer.close == false) {
        uint64_t currentTime = getTime();
        float elapsed = (currentTime-lastTime)/1000.0; // Convert milliseconds to seconds

        world.update(elapsed);
        renderer.draw();
        if(b1->isColliding(b2)) {
            renderer.drawCircle(50, 50, 48);
        }
        b2->rotate(0.01);
        renderer.update();
        lastTime = getTime(); 
    }

    return 0;
}
