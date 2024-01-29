

#ifndef FIXTURE_H
#define FIXTURE_H

#include "../shapes/shape.h"
class Fixture {

    public:
        Shape shape;
        float mass;
        // TODO: make this private by some method
        bool attached = false;

        void setShape(Shape s);
        Shape* getShape() { return &shape; }
    private:

};

#endif // !FIXTURE_H
