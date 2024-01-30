
#ifndef POLYGON_H
#define POLYGON_H

#include "../homarus.h"
#include "shape.h"
#include "../util/vec2.h"
#include <vector>

class Polygon : public Shape {

    public:
        Polygon(std::vector<Vec2d> vertices);
        Shape* clone() const override;

        short getType() const override {
            return TYPE_POLYGON;
        }
            
        std::vector<Vec2d> vertices;

        bool pointInside(Vec2d point) const override;
        bool checkOverlap(Shape *s) const override;

        void calcArea() override;
        void calcCentroid() override;

    protected:
        void orderVertices();
        static bool ccwCompare(Vec2d a, Vec2d b);

};

#endif // !POLYGON_H
