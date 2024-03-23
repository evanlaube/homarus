
#ifndef SHAPE_H
#define SHAPE_H


#include "../util/vec2.h"
#include "../physics/collision.h"

class Fixture;
class Circle;
class Polygon;

class Shape {

    public:

        Shape();
        virtual Shape* clone() const = 0;

        virtual short getType() const = 0;
        Vec2d centroid;
        float area;
       
        void attach(Fixture* f);
        Fixture* getFixture() { return fixture; }

        Vec2d getPos() const;
        float getAngle();
        float getMaxRadius() const { return maxRadius; }
        virtual void rotate(float angle) = 0;

        virtual void calcCentroid() = 0;
        virtual void calcArea() = 0;

        virtual bool pointInside(Vec2d point) const = 0;
        virtual bool checkOverlap(Shape *s)  const = 0;
        virtual bool checkCircleOverlap(Circle *s) const = 0;
        virtual bool checkPolygonOverlap(Polygon *s) const = 0;
        virtual std::pair<Vec2d, Vec2d> getBoundingBox() const = 0;

        virtual Collision getCollision(Shape *s)  const = 0;
        virtual Collision getCircleCollision(Circle *s) const = 0;
        virtual Collision getPolygonCollision(Polygon *s) const = 0;

    protected:
        float maxRadius;
    private:
        Fixture* fixture = nullptr;

};

#endif // !SHAPE_H
