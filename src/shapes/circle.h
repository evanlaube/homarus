
#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"
class Circle : public Shape {

    public:
        Circle(float radius);

        const static short type = 1;

        void calcArea() override;

        float getRadius() { return r; }
    private:
        float r;


};

#endif // !CIRCLE_H
