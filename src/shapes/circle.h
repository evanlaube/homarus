
#ifndef CIRCLE_H
#define CIRCLE_H

#include "../homarus.h"
#include "shape.h"
class Circle : public Shape {

    public:
        Circle(float radius);

        short getType() override {
            return TYPE_CIRCLE;
        }

        void calcArea() override;

        float getRadius() { return r; }
    private:
        float r;


};

#endif // !CIRCLE_H
