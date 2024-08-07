// #define GIF_EXPORT

#include <chrono>
#include <cstdint>

#include "../../src/homarus.h"

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
#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>

#include "../lib/gif.h"

uint64_t getTime() {
    // Return integer of miliseconds since epoch
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

int main() {
    srand(time(NULL));
        
    World world;
    world.setGravity(0, -1);
    
    Renderer renderer = Renderer(&world);
    renderer.setScale(36);
    renderer.init();

    Circle *circle = new Circle(2.5);
    Fixture *fix = new Fixture(circle);
    Body *circleBody = world.createBody(fix, Vec2d(15, 10));
    circleBody->setMass(2.5); 
    circleBody->setType(BODY_DYNAMIC);

    for(int i = 0; i < 3000; i++) {
        float r = 0.1;

        float x = 3 + r + ((float)rand()/RAND_MAX) * (30-r-r-6);
        float y = 3 + r + ((float)rand()/RAND_MAX) * (20-r-6);

        Circle c(r);
        Fixture* f = new Fixture(&c);
        Body* b1 = world.createBody(f, Vec2d(x,y));
        b1->setVel(Vec2d((2 - ((float)rand()/RAND_MAX) * 4), (2 - ((float)rand()/RAND_MAX) * 4)));
        b1->setMass(r*r*3.14); 
        b1->setType(BODY_DYNAMIC);
    }

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
            ss << "Homarus Test - Total KE: " << world.getTotalKE();
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
        world.update(0.01, 2);

        totalUpdateTime += ((double)getTime() / (double)1000.0) - t;

        t = getTime() / (double)1000.0;
        renderer.draw();
        renderer.update();
        totalRenderTime += ((double)getTime() / (double)1000.0) - t;

#ifdef GIF_EXPORT 
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
