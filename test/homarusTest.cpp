// #define GIF_EXPORT

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
    //world.setGravity(0, 100);
    
    Renderer renderer = Renderer(&world);

    renderer.init();

    std::vector<Vec2d> verts;
    verts.push_back(Vec2d(0, 0));
    verts.push_back(Vec2d(100, 0));
    verts.push_back(Vec2d(100, 500));
    verts.push_back(Vec2d(0, 500));

    Polygon *s2 = new Polygon(verts);
    Fixture *f2 = new Fixture(s2);
    Body* b2 = world.createBody(f2, Vec2d(580, 360));
    b2->rotate(0.2);
    b2->setMass(25*500);
    b2->setType(BODY_DYNAMIC);
    
    Circle *circle = new Circle(30);
    Fixture *fix = new Fixture(circle);
    Body *circleBody = world.createBody(fix, Vec2d(200, 280));
    circleBody->setVel(Vec2d(200, 0));
    circleBody->setMass(30*30*3.14); // One half (ish) unit density
    circleBody->setType(BODY_DYNAMIC);

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
        world.step(0.008, 2);

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
