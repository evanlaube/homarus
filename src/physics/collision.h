
#ifndef COLLISION_H
#define COLLISION_H

#include "../util/vec2.h"
#include <iostream>
#include <ostream>
struct Collision {
    public:
        bool colliding = false;
        Vec2d overlap;
        Vec2d tangent;
        Vec2d normal;
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
