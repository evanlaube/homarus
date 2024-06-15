
#ifndef COLLISION_H
#define COLLISION_H

#include "../util/vec2.h"
#include <iostream>
#include <ostream>

/** 
 * The Collision struct is a data structure for storing information on a collision between
 * two bodies. It stores vectors for the overlap of the collision, the intersection point of 
 * the collision, the tangent along the point of collision, as well as the normal perpendicular
 * to the tangent. It also stores a boolean to check if the two bodies are actually colliding.
 */
struct Collision {
    public:
        bool colliding = false;
        /** A vector that stores the amount that the shapes of the two bodies are 'overlapping'.
         * In theory, this value should always be zero, but that is not possible due to limitations
         * of calculating movement of bodies in fixed steps.
         */
        Vec2d overlap;

        /**
         * Vector that stores the normalized tangent vector along the point of the collison
         */
        Vec2d tangent;

        /**
         * Vector that stores the orthoganal vector to the tangent
         */
        Vec2d normal;

        /**
         * Vector that stores the absolute point in space where the two bodies are colliding
         */
        Vec2d intersection;

        Collision(Vec2d o, Vec2d t, Vec2d i) {
            colliding = true;
            overlap = o;
            tangent = t;
            normal = Vec2d(-t.y, t.x);
            intersection = i;
        }

        Collision() {
            colliding = false;
        }

        friend std::ostream& operator<<(std::ostream& os, const Collision c) {
            os << "Collision: \n" <<
                "\tColliding: " << c.colliding << "\n" <<
                "\tOverlap: " << c.overlap << "\n" <<
                "\ttangent: " << c.tangent << "\n" <<
                "\tintersection: " << c.intersection << "\n";

            return os;
        }
        
};

#endif // !COLLISION_H
