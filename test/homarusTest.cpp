// #define GIF_EXPORT

#include <chrono>
#include <cmath>
#include <cstdint>

#include "homarus.h"

#include "physics/fixture.h"
#include "physics/joints/spring.h"
#include "physics/joints/joint.h"
#include "physics/world.h"
#include "renderer.h"
#include "shapes/shape.h"
#include "shapes/circle.h"
#include "shapes/polygon.h"
#include "util/vec2.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ostream>
#include <sstream>
#include <vector>

#ifdef GIF_EXPORT
#include "../lib/gif.h"
#endif

uint64_t getTime() {
    // Return integer of miliseconds since epoch
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

int main() {
    srand(time(NULL));
        
    World world;
    world.setGravity(0, -9.8);
    
    Renderer renderer = Renderer(&world);
    // Set renderer to have 36 pixels per world meter
    // 1080x720 (px) -> 30 x 20 (meters)
    renderer.setScale(36);
    renderer.init();

    std::vector<Vec2d> floorVerts;
    floorVerts.push_back(Vec2d(0, 0));
    floorVerts.push_back(Vec2d(0, 0.5));
    floorVerts.push_back(Vec2d(30, 0.5));
    floorVerts.push_back(Vec2d(30, 0));
    Polygon topFloorShape(floorVerts);
    Fixture* topFloorFixture = new Fixture(&topFloorShape);
    Body* s = world.createBody(topFloorFixture, Vec2d(15, 0.25));
    s->setType(BODY_STATIC);
    Fixture* bottomFloorFixture = new Fixture(&topFloorShape);
    Body* bottomFloor = world.createBody(bottomFloorFixture, Vec2d(15, 19.75));
    bottomFloor->setType(BODY_STATIC);

    std::vector<Vec2d> wallVerts;
    wallVerts.push_back(Vec2d(0, 0));
    wallVerts.push_back(Vec2d(0.5, 0));
    wallVerts.push_back(Vec2d(0.5, 19));
    wallVerts.push_back(Vec2d(0, 19));
    Polygon wallShape(wallVerts);
    Fixture* leftWallFixture = new Fixture(&wallShape);
    Body* leftWall = world.createBody(leftWallFixture, Vec2d(0.25, 10));
    leftWall->setType(BODY_STATIC);
    Fixture* rightWallFixture = new Fixture(&wallShape);
    Body* rightWall = world.createBody(rightWallFixture, Vec2d(29.75, 10));
    rightWall->setType(BODY_STATIC);

    std::vector<Vec2d> squareVerts;
    squareVerts.push_back(Vec2d(0, 0));
    squareVerts.push_back(Vec2d(3, 0));
    squareVerts.push_back(Vec2d(3, 3));
    squareVerts.push_back(Vec2d(0, 3));

    Polygon *squarePoly = new Polygon(squareVerts);
    Fixture *squareFix = new Fixture(squarePoly);
    Body* square = world.createBody(squareFix, Vec2d(10, 10));
    square->setOmega(3);
    square->rotate(0);
    square->setMass(25);
    //square->setVel(Vec2d(100, 50));
    square->setType(BODY_DYNAMIC);
    
    Circle *circleShape = new Circle(1);
    Fixture *circleFix = new Fixture(circleShape);
    Body *circle = world.createBody(circleFix, Vec2d(20, 10));
    circle->rotate(1);
    circle->setMass(25); 
    //circle->setVel(Vec2d(-100, -150));
    circle->setType(BODY_DYNAMIC);

    Spring* spring = world.createSpring(square, circle, 200);

    uint64_t currentTime = getTime();
    uint64_t lastTime = getTime(); 

    double totalUpdateTime = 0;
    double totalRenderTime = 0;

#ifdef GIF_EXPORT
    GifWriter gifWriter;
    GifBegin(&gifWriter, "output.gif", 1080, 720, 2); // Adjust the frame rate as needed
#endif

    while(renderer.close == false) {
        
        if(renderer.getFrameCount() % 60 == 0) {
            std::stringstream ss;
            ss << "Homarus Test - Total Energy: " << world.getTotalEnergy();
            renderer.setTitle(ss.str().c_str());

            std::cout << "Average render time per frame (s): " << totalRenderTime/60 << std::endl;
            std::cout << "Average update time per frame (s): " << totalUpdateTime/60 << std::endl;
            std::cout << "Average FPS: " << 1 / (totalRenderTime/60 + totalUpdateTime/60) << std::endl;

            totalUpdateTime = 0;
            totalRenderTime = 0;
        }

        double t = getTime() / (double)1000.0;
        currentTime = getTime();
        double elapsed = (currentTime-lastTime)/(double)1000.0; // Convert milliseconds to seconds
        world.update(elapsed, 2);

        totalUpdateTime += ((double)getTime() / (double)1000.0) - t;

        t = getTime() / (double)1000.0;
        renderer.draw();
        renderer.update();
        totalRenderTime += ((double)getTime() / (double)1000.0) - t;

#ifdef GIF_EXPORT // defined in renderer.h
        if(renderer.getFrameCount() > 30*60 && renderer.getFrameCount() < 2*30*60) {
            std::vector<uint8_t> pixels(renderer.width * renderer.height * 4); // Assuming RGBA format
            glReadPixels(0, 0, renderer.width, renderer.height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
            GifWriteFrame(&gifWriter, pixels.data(), renderer.width, renderer.height, 2); // Adjust the frame delay as needed
        } else if(renderer.getFrameCount() > 2*30*60) {
            break;
        }
#endif

        lastTime = currentTime;
    }

#ifdef GIF_EXPORT
    GifEnd(&gifWriter);
#endif

    return 0;
}
