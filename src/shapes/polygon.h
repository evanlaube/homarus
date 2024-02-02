
#ifndef POLYGON_H
#define POLYGON_H

#include "../homarus.h"
#include "shape.h"
#include "../util/vec2.h"
#include <vector>

class Polygon : public Shape {

    public:
        Polygon(std::vector<Vec2d> vertices);
        Polygon* clone() const override {
            return new Polygon(*this);
        }

        short getType() const override {
            return TYPE_POLYGON;
        }
            
        std::vector<Vec2d> getVertices() { return vertices; }
        std::vector<Vec2d> rotateVertices(float angle);

        bool pointInside(Vec2d point) const override;
        bool checkOverlap(Shape *s) const override;

        void calcArea() override;
        void calcCentroid() override;

    protected:
        std::vector<Vec2d> vertices;
        void orderVertices();
        static bool ccwCompare(Vec2d a, Vec2d b);

};

#endif // !POLYGON_H
