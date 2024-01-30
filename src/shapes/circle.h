
#ifndef CIRCLE_H
#define CIRCLE_H

#include "../homarus.h"
#include "shape.h"

class Circle : public Shape {

    public:
        Circle(float radius);
        Circle* clone() const override {
            return new Circle(*this);
        }

        short getType() const override {
            return TYPE_CIRCLE;
        }

        bool pointInside(Vec2d point) const override;
        bool checkOverlap(Shape *s) const override;

        void calcArea() override;
        void calcCentroid() override;

        float getRadius() { return r; }
    private:
        float r;


};

#endif // !CIRCLE_H
