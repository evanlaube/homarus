
#ifndef BODY_H
#define BODY_H

#include "../util/vec2.h"
#include "fixture.h"

class Body {

    public:
        Body();
        Body(Fixture *fixture);

        Body* getNext() { return next; }
        Body* getLast() { return last; }

        Vec2d pos;
        double ang;
        Vec2d vel;
        Vec2d acc;

        void setFixture(Fixture *f);
        void setPos(Vec2d p);
        Shape* getShape() { return fixture.getShape(); }

        bool isColliding(Body b);
    private:
        friend class World;

        Fixture fixture;
        Body* next;
        Body* last;
};

#endif // !BODY_H
