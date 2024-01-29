
#include "renderer.h"
#include "util/vec2.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
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
}


void Renderer::drawCircle(float x, float y, float r) {
    Renderer::drawCircle(x, y, r, 32);
}

void Renderer::drawCircle(float x, float y, float r, int triCount) {
    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(x, y);
    
    for(int i = 0; i <= triCount; i++) {
        int cx = r * cosf(i * (M_PI*2) / triCount);
        int cy = r * sinf(i * (M_PI*2) / triCount);

        glVertex2f(x+cx, y+cy);
    }

    glEnd();
}

void Renderer::drawShape(Shape s, float x, float y, float a) {
    std::cout << "ERROR: Unknown shape passed into Renderer::drawShape()" << std::endl;
    // TODO: Check what type of shape is passed and draw it
}

void Renderer::drawShape(Circle c, float x, float y, float a) {
   drawCircle(x, y, c.getRadius()); 
}

void Renderer::drawShape(Polygon p, float x, float y, float a) {
    
    float cx = p.centroid.x;
    float cy = p.centroid.y;

    glBegin(GL_POLYGON);
    for(int i = 0; i < p.vertices.size(); i++) {
        Vec2d vert = p.vertices[i];
        glVertex2f(x - cx + vert.x, y - cy + vert.y);
    }

    glEnd();
}


void Renderer::drawBodies() {
    for(Body b : world->bodies) {
        drawBody(b);
    }
}

void Renderer::drawBody(Body b) {
    glColor3f(1, 0, 0);
    drawCircle(b.pos.x, b.pos.y, 32);
}
