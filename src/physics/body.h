
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


        void setFixture(Fixture f);
        Shape* getShape() { return fixture.getShape(); }

        Body();
        Body(Vec2d pos, double ang, Fixture fixture);
        bool isColliding(Body b);
    private:
        Fixture fixture;
};

#endif // !BODY_H
