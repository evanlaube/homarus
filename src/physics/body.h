
#ifndef BODY_H
#define BODY_H

#include "../util/vec2.h"
#include "fixture.h"

class Body {

    public:
        Vec2d pos;
        double ang;
        Vec2d vel;
        Vec2d acc;

        Fixture fixture;

        Body(Vec2d pos, double ang, Fixture fixture);
        bool isColliding(Body b);
};

#endif // !BODY_H
