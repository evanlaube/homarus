
#ifndef SHAPE_H
#define SHAPE_H


#include "../util/vec2.h"

class Fixture;

class Shape {

    public:

        Shape();
        //virtual ~Shape();
        virtual Shape* clone() const = 0;

        virtual short getType() const = 0;
        Vec2d centroid;
        float area;
       
        void attach(Fixture* f);

        virtual void calcCentroid() = 0;
        virtual void calcArea() = 0;

        virtual bool pointInside(Vec2d point) const = 0;
        virtual bool checkOverlap(Shape *s)  const = 0;
    private:
        Fixture* fixture = nullptr;

};

#endif // !SHAPE_H
