
#ifndef POLYGON_H
#define POLYGON_H

#include "shape.h"
#include "../util/vec2.h"
#include <vector>

class Polygon : public Shape {

    public:
        Polygon(std::vector<Vec2d> vertices);

        const static short type = 2;
            
        std::vector<Vec2d> vertices;

        void calcArea() override;
        void calcCentroid() override;

    protected:
        void orderVertices();
        static bool ccwCompare(Vec2d a, Vec2d b);

};

#endif // !POLYGON_H
