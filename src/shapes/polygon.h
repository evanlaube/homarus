
#ifndef POLYGON_H
#define POLYGON_H

#include "shape.h"
#include "../util/vec2.h"
#include <vector>

class Polygon : public Shape {

    public:
        Polygon(std::vector<Vec2d> vertices);
 
        std::vector<Vec2d> vertices;

        void calcArea() override;
        void calcCentroid() override;

    protected:


};

#endif // !POLYGON_H
