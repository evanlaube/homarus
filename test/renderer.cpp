
#include "homarus.h"
#include "renderer.h"
#include "shapes/polygon.h"
#include "util/vec2.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

Renderer::Renderer(World *w) {
    world = w;
    width = 1080;
    height = 720;
}

int Renderer::init() {
    if(!glfwInit()) {
        std::cerr << "Unable to initialize GLFW" << std::endl;
        return 0;
    } else {
        std::cout << "GLFW Initialized - Version: " << glfwGetVersionString() << std::endl;
    }

#ifdef GIF_EXPORT
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE); 
#endif

    window = glfwCreateWindow(width, height, "Homarus Test", NULL, NULL);

    if(window == NULL) {
        std::cerr << "Unable to create window" << std::endl;
        return 0;
    }

    glfwMakeContextCurrent(window);
    close = glfwWindowShouldClose(window);

    int glewErr = glewInit();

    if(glewErr != GLEW_OK) {
        std::cerr << "Unable to initialize GLEW" << std::endl;
    }

    glMatrixMode(GL_PROJECTION);
    glOrtho(0, width, height, 0, -1, 1);
   
    glClearColor(0.08627450980392157, 0.08627450980392157, 0.11372549019607843, 1);

    return 1;
}

void Renderer::update() {
    glfwSwapBuffers(window);
    glfwPollEvents();

    close = glfwWindowShouldClose(window);
}

void Renderer::draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawBodies();
    
    // Display FPS at end of draw loop
    const short fpsUpdateRate = 120;

    frameCount++;
    if(frameCount % fpsUpdateRate == 0) {
        double currentTime = glfwGetTime();
        fps = fpsUpdateRate / (currentTime - prevTime);
        prevTime = currentTime;
    }
}

void Renderer::drawCircle(float x, float y, float r) {
    Renderer::drawCircle(x, y, r, 64);
}

void Renderer::drawCircle(float x, float y, float r, int triCount) {
    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(x, y);

    for(int i = 0; i <= triCount; i++) {
        float cx = r * cosf(i * (M_PI*2) / triCount);
        float cy = r * sinf(i * (M_PI*2) / triCount);

        glVertex2f(x+cx, y+cy);
    }

    glEnd();
}

void Renderer::drawShape(Shape *s, float x, float y, float a) {
    if(s->getType() == TYPE_CIRCLE) {
        Circle *c = dynamic_cast<Circle*>(s);
        drawCircle(x, y, c->getRadius(), 32);
    } else if(s->getType() == TYPE_POLYGON) {
        Polygon *p = dynamic_cast<Polygon*>(s);
        drawShape(p, x, y, a);
    }

}

void Renderer::drawShape(Circle *c, float x, float y, float a) {
    drawCircle(x, y, c->getRadius(), 32);
}


void Renderer::drawShape(Polygon *p, float x, float y, float a) {
    
    float cx = p->centroid.x;
    float cy = p->centroid.y;

    glBegin(GL_POLYGON);

    std::vector<Vec2d> verts = p->getVertices();
    for(int i = 0; i < verts.size(); i++) {
        Vec2d vert = verts[i];
        glVertex2f(x - cx + vert.x, y - cy + vert.y);
    }

    glEnd();
}

void Renderer::drawBodies() {
    Body* b = world->bodyLink;

    int outOfBounds = 0;

    if(b == nullptr)
        return;
    do {
        drawBody(b);

        if(b->getPos().y > 1000) {
            outOfBounds++;
        }

        b = b->getNext();
    } while(b != nullptr);

    if(outOfBounds > 0) {
        std::cout << "Total Bodies out of bounds: " << outOfBounds << std::endl;
    }
}

void Renderer::saveImage(const char* path) {
}

float Renderer::getScalingFactor() const {
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return static_cast<float>(fbWidth) / width;
}

void Renderer::drawBody(Body *b) {
    if(b->getShape()->getType() == TYPE_CIRCLE) {
        glColor3f(1, 0, 0);
    } else if (b->getShape()->getType() == TYPE_POLYGON) {
        glColor3f(0, 0, 1); 
    } else {
        glColor3f(0, 1, 0);
    }

    Shape* s = b->getShape();
   
    drawShape(s, b->getPos().x, b->getPos().y, b->getAngle());
}

void Renderer::displayFps() {

    std::stringstream ss;
    ss << "Homarus Test - FPS: " << fps;
    glfwSetWindowTitle(window, ss.str().c_str());

}

void Renderer::setTitle(const char* title) {
    glfwSetWindowTitle(window, title);
}
