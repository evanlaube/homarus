
#ifndef CIRCLE_H
#define CIRCLE_H

#include "../homarus.h"
#include "shape.h"
#include "../util/vec2.h"

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
        bool checkCircleOverlap(Circle *s) const override;
        bool checkPolygonOverlap(Polygon *s) const override;

        Vec2d getOverlap(Shape *s)  const override;
        Vec2d getCircleOverlap(Circle *s) const override;
        Vec2d getPolygonOverlap(Polygon *s) const override;

        void calcArea() override;
        void calcCentroid() override;

        void rotate(float angle) override { return; };

        float getRadius() { return r; }
    private:
        float r;


};

#endif // !CIRCLE_H
