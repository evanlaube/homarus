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
#include <cstdlib>
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
    world.setGravity(0, 9800*2);
    
    Renderer renderer = Renderer(&world);

    renderer.init();

    // Simple test of Shape::calcArea and Shape::calcCentroid using a square
    //  Steps for creating a new body: 
    //      1. Create empty body with world
    //      2. Create a shape of some kind
    //      3. Create a fixture and attach the shape
    //      4. Attach fixture to body
    //      5. Set a position and angle to the body
    
    /*
    Circle c = Circle(50);
    Fixture* f = new Fixture(&c);
    Body* b1 = world.createBody(f);
    b1->setPos(Vec2d(540, 360));
    b1->setVel(Vec2d(0, 0));
    b1->setMass((25*25));
    b1->rotate(M_PI/4);
    b1->setType(BODY_DYMANIC); 
    */


    for(int i = 0; i < 500; i++) {
        float r = 4;//20 + ((float)rand()/RAND_MAX) * 15;

        float x = 2 + r + ((float)rand()/RAND_MAX) * (1080-r-r-4);
        float y = r + ((float)rand()/RAND_MAX) * (720-r);

        Circle c(r);
        Fixture* f = new Fixture(&c);
        Body* b1 = world.createBody(f);
        b1->setPos(Vec2d(x, y));
        b1->setVel(Vec2d(200 - ((float)rand()/RAND_MAX) * 400, 200 - ((float)rand()/RAND_MAX) * 400));
        //b1->setVel(Vec2d(200, 200));
        b1->setMass(r*r*3.14);
        b1->setType(BODY_DYMANIC);
    }

    std::vector<Vec2d> floorVerts;
    floorVerts.push_back(Vec2d(0, 0));
    floorVerts.push_back(Vec2d(1080, 0));
    floorVerts.push_back(Vec2d(1080, 10));
    floorVerts.push_back(Vec2d(0, 10));

    Polygon topFloorShape(floorVerts);
    Fixture* topFloorFixture = new Fixture(&topFloorShape);
    Body* s = world.createBody(topFloorFixture);
    s->setType(BODY_STATIC);
    s->rotate(0.00001); // To prevent having 0 or infinite slope
    s->setPos(Vec2d(540, 5));

    Fixture* bottomFloorFixture = new Fixture(&topFloorShape);
    Body* bottomFloor = world.createBody(bottomFloorFixture);
    bottomFloor->setType(BODY_STATIC);
    bottomFloor->rotate(0.00001);
    bottomFloor->setPos(Vec2d(540, 720-5));

    std::vector<Vec2d> wallVerts;
    wallVerts.push_back(Vec2d(0, 10));
    wallVerts.push_back(Vec2d(10, 10));
    wallVerts.push_back(Vec2d(10, 710));
    wallVerts.push_back(Vec2d(0, 710));

    Polygon wallShape(wallVerts);
    Fixture* leftWallFixture = new Fixture(&wallShape);
    Body* leftWall = world.createBody(leftWallFixture);
    leftWall->setType(BODY_STATIC);
    leftWall->rotate(0.00001);
    leftWall->setPos(Vec2d(5, 360));

    Fixture* rightWallFixture = new Fixture(&wallShape);
    Body* rightWall = world.createBody(rightWallFixture);
    rightWall->setType(BODY_STATIC);
    rightWall->rotate(0.00001);
    rightWall->setPos(Vec2d(1080-5, 360));


    uint64_t currentTime = getTime();
    uint64_t lastTime = getTime(); 

    double totalUpdateTime = 0;
    double totalRenderTime = 0;

    while(renderer.close == false) {
        double elapsed = (currentTime-lastTime)/(double)1000.0; // Convert milliseconds to seconds
        
        if(renderer.getFrameCount() % 60 == 0) {
            std::stringstream ss;
            ss << "Homarus Test - Total KE: " << world.getTotalKE();
            renderer.setTitle(ss.str().c_str());

            std::cout << "Average render time per frame (s): " << totalRenderTime/60 << std::endl;
            std::cout << "Average update time per frame (s): " << totalUpdateTime/60 << std::endl;
            std::cout << "Average FPS: " << 1 / (totalRenderTime/60 + totalUpdateTime/60) << std::endl;

            totalUpdateTime = 0;
            totalRenderTime = 0;
        }

        double t = getTime() / (double)1000.0;
        world.update(elapsed);

        totalUpdateTime += ((double)getTime() / (double)1000.0) - t;

        t = getTime() / (double)1000.0;
        renderer.draw();
        renderer.update();
        totalRenderTime += ((double)getTime() / (double)1000.0) - t;

        lastTime = currentTime;
        currentTime = getTime();
    }

    return 0;
}
