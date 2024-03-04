#include <chrono>
#include <cstdint>

#include "homarus.h"

#include "physics/fixture.h"
#include "physics/world.h"
#include "renderer.h"
#include "shapes/shape.h"
#include "shapes/circle.h"
#include "shapes/polygon.h"
#include "util/vec2.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <vector>

uint64_t getTime() {
    // Return integer of miliseconds since epoch
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

int main() {
    World world;
    //world.setGravity(0, 45000);
    
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
    b1->setPos(Vec2d(540, 350));
    b1->setVel(Vec2d(0, 150));
    b1->setType(BODY_DYMANIC);

    std::vector<Vec2d> verts;
    verts.push_back(Vec2d(0, 0));
    verts.push_back(Vec2d(880, 0));
    verts.push_back(Vec2d(880, 40));
    verts.push_back(Vec2d(0, 40));

    Polygon floorPolygon(verts);
    Fixture* floorFixture = new Fixture(&floorPolygon);
    Body* floor = world.createBody(floorFixture);
    floor->setPos(Vec2d(540, 620));
    floor->setType(BODY_STATIC);
    floor->rotate(0.01);

    std::vector<Vec2d> verts2;
    verts2.push_back(Vec2d(0, 0));
    verts2.push_back(Vec2d(50, 0));
    verts2.push_back(Vec2d(50, 50));
    verts2.push_back(Vec2d(0, 50));

    Polygon square(verts2);
    Fixture* squareFixture = new Fixture(&square);
    Body* s = world.createBody(squareFixture);
    s->setType(BODY_DYMANIC);
    s->rotate(0.1);
    s->setPos(Vec2d(540, 0));
    s->setVel(Vec2d(0, 150));

    //Fixture* f3 = new Fixture(new Circle(30));
    //Body *b2 = world.createBody(f3);
    //b2->setPos(Vec2d(540, 400));
    //b2->setVel(Vec2d(0, 200));
    //b2->setType(BODY_STATIC);
    

    uint64_t currentTime = getTime();
    uint64_t lastTime = getTime(); 

    while(renderer.close == false) {
        float elapsed = (currentTime-lastTime)/1000.0; // Convert milliseconds to seconds
        
        if(renderer.getFrameCount() % 60 == 0) {
            std::stringstream ss;
            ss << "Homarus Test - FPS: " << 1/elapsed;
            renderer.setTitle(ss.str().c_str());
        }

        world.update(elapsed);
        renderer.draw();
        renderer.update();
        lastTime = currentTime;
        currentTime = getTime();
    }

    return 0;
}
