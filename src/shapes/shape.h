
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
       
        void attach(Fixture* f);
        Fixture* getFixture() { return fixture; }

        Vec2d getPos() const;
        float getAngle() const;
        double getArea() const { return area; };
        double getMoment() const { return moment; }
        float getMaxRadius() const { return maxRadius; }
        Vec2d getCentroid() const { return centroid; }
        double getMass() const;

        /**
         * Rotates the shape by a set angle.
         * @param angle The angle to rotate by in radians.
         */
        virtual void rotate(float angle) = 0;

        /**
         * Calculates the centroid of the shape. This is equivalent to the center of mass
         * of the shape assuming uniform density. The calculated centroid is stored in the 
         * private centroid variable.
         */
        virtual void calcCentroid() = 0;
        /**
         * Calculates the area of the shape and stores it in the private area variable
         */
        virtual void calcArea() = 0;
        /**
         * Calculates the moment of inertia of the shape about the centroid and stores
         * it in the private moment variable.
         */
        virtual void calcMoment() = 0;

        /**
         * Checks if a point is inside the shape, using the position of the body that the 
         * shape is attached to for the center of the shape. This function will not work
         * if the shape is not attached to a body.
         *
         * @param point The point to check if inside
         * @return true if the point is inside the shape, false otherwise
         */
        virtual bool pointInside(Vec2d point) const = 0;
        
        /**
         * Checks if a shape has any overlap with another shape. Both shapes must be attached
         * to a body that has a position in order to this function to work.
         *
         * @param s The shape to check overlap against
         * @return True if the shapes overlap, false otherwise
         */
        virtual bool checkOverlap(Shape *s)  const = 0;
        virtual bool checkCircleOverlap(Circle *s) const = 0;
        virtual bool checkPolygonOverlap(Polygon *s) const = 0;

        /**
         * Gets the smallest bounding box that can contain the entire shape. This box
         * is useful in rough collision detection.
         *
         * @return A std::pair of two Vec2d that mark the corners of the bounding box.
         */
        virtual std::pair<Vec2d, Vec2d> getBoundingBox() const = 0;

        /**
         * Gets the collision between two shapes. A collision is always returned, even if the shapes 
         * are not actually colliding. However, if the shapes don't collide, the 'colliding' field
         * of the Collision will be set to false
         *
         * @param s The shape to check collision against
         * @return a Collision with information about the collision of the two shapes
         */
        virtual Collision getCollision(Shape *s)  const = 0;
        virtual Collision getCircleCollision(Circle *s) const = 0;
        virtual Collision getPolygonCollision(Polygon *s) const = 0;

    protected:
        /** The geometric centroid of the shape, in local coordinates */
        Vec2d centroid;
        /** The radius of the smallest circle about the centroid that can enclose the entire shape */
        double maxRadius;
        /** The area of the shape */
        double area;
        /** The moment of inertia of the shape */
        double moment;
    private:
        /** Pointer to the fixture that the shape is attached to */
        Fixture* fixture = nullptr;

};

#endif // !SHAPE_H
