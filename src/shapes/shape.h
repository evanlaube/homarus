
#ifndef SHAPE_H
#define SHAPE_H


#include "../util/vec2.h"

class Shape {

    public:

        Shape();
        //virtual ~Shape();

        virtual short getType() = 0;
        Vec2d centroid;
        float area;
        
        virtual void calcCentroid();
        virtual void calcArea();

        //virtual bool checkOverlap() const = 0;

};

#endif // !SHAPE_H
