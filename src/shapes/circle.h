
#ifndef CIRCLE_H
#define CIRCLE_H

#include "../homarus.h"
#include "../physics/collision.h"
#include "shape.h"
#include "../util/vec2.h"

/**
 * Circle is a shape type that is defined by only a radius. It is a polymorphic child
 * of the Shape class.
 * 
 * Check Shape header file for documentation.
 */
class Circle : public Shape {

    public:
        Circle(float radius);
        Circle* clone() const override {
            return new Circle(*this);
        }

        short getType() const override { return TYPE_CIRCLE; }

        bool pointInside(Vec2d point) const override;
        bool checkOverlap(Shape *s) const override;
        bool checkCircleOverlap(Circle *s) const override;
        bool checkPolygonOverlap(Polygon *s) const override;
        std::pair<Vec2d, Vec2d> getBoundingBox() const override;

        Collision getCollision(Shape *s)  const override;
        Collision getCircleCollision(Circle *s) const override;
        Collision getPolygonCollision(Polygon *s) const override;

        void calcArea() override;
        void calcCentroid() override;
        void calcMoment() override; 

        void rotate(float angle) override { return; };

        float getRadius() { return r; }
    private:
        /** Radius of the circle */
        double r;


};

#endif // !CIRCLE_H
