
#ifndef RENDERER_H
#define RENDERER_H

#include "physics/world.h"
#include "GL/glew.h"
#include "shapes/circle.h"
#include "shapes/polygon.h"
#include "shapes/shape.h"
#include <GLFW/glfw3.h>

class Renderer {

    public:
        World *world;

        bool close = false;

        GLFWwindow *window;
        int width;
        int height;

        Renderer(World *w);
        int init();
        void draw();
        void update();
        void exit();

        void drawShape(Shape *s, float x, float y, float a);
        void drawShape(Circle *c, float x, float y, float a);
        void drawShape(Polygon *p, float x, float y, float a);

        void drawCircle(float x, float y, float r);
        void drawCircle(float x, float y, float r, int triCount);
       
        void drawBodies();
        void drawBody(Body b);
    private:

}; 

#endif // RENDERER_H
