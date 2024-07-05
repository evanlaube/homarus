
#ifndef RENDERER_H
#define RENDERER_H

#include "physics/world.h"
#include "GL/glew.h"
#include "shapes/circle.h"
#include "shapes/polygon.h"
#include "shapes/shape.h"
#include "util/quadtree.h"
#include <GLFW/glfw3.h>
#include <cstdint>

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

        /**
         * Sets the scale of the renderer in pixel per world meter (unit of length)
         * Note that when creating bodies and choosing their location, these values are specified in world
         * units.
         *
         * @param scale scale in pixels per meter.
         */
        void setScale(int scale);
        
        void setTitle(const char* title);
        void displayFps();

        void saveImage(const char* path);

        uint32_t getFrameCount() { return frameCount; }

        float getScalingFactor() const;

        void drawShape(Shape *s, float x, float y, float a);
        void drawShape(Circle *c, float x, float y, float a);
        void drawShape(Polygon *p, float x, float y, float a);

        void drawCircle(float x, float y, float r);
        void drawCircle(float x, float y, float r, int triCount);
       
        void drawBodies();
        void drawBody(Body *b);

        void drawJoints();
        void drawJoint(Joint* j);

        void drawQuadtree(Quadtree* tree);
        void drawQuadtreeNode(Node* node);
    private:
        int fps = 0;
        int scale = 1;
        double prevTime = 0;
        uint32_t frameCount = 0;

}; 

#endif // RENDERER_H
