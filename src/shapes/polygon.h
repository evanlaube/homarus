
#ifndef POLYGON_H
#define POLYGON_H

#include "../homarus.h"
#include "../physics/collision.h"
#include "shape.h"
#include "../util/vec2.h"
#include <vector>


/**
 * A polygon shape is defined by an array of vertices that are connected with straight
 * lines. It is a polymorphic child of the Shape class.
 *
 * Check Shape header for documentation.
 */
class Polygon : public Shape {

    public:
        Polygon(std::vector<Vec2d> vertices);
        Polygon* clone() const override {
            return new Polygon(*this);
        }

        short getType() const override { return TYPE_POLYGON; }

        std::vector<Vec2d> getVertices() { return vertices; }
        void rotate(float angle) override;

        /**
         * Loops through all vertices and rotates them around the centroid of the shape by a set angle
         * @param angle Amount to rotate vertices by in radians.
         */
        void rotateVertices(float angle);

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

    protected:
        /** List of all of the vertices in the shape */
        std::vector<Vec2d> vertices;
        /** Sorts vertices array so that vertices are in counter clockwise order. */
        void orderVertices();
        
        /**
         * Sorting method to check whether a is clockwise or counter-clockwise of b around the 
         * centroid. This function is used to sort vertices in counter-clockwise order.
         *
         * @param a Point to check.
         * @param b Point to compare to.
         * @return True if a is more counter-clockwise than b, false if otherwise
         */
        static bool ccwCompare(Vec2d a, Vec2d b);

};

#endif // !POLYGON_H
