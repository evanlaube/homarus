
#ifndef BODY_H
#define BODY_H

#include "../util/vec2.h"
#include "fixture.h"
#include "../shapes/shape.h"

class Body {

    public:
        Fixture fixture;
        Body();
        Body(Fixture *fixture);

        Body* getNext() { return next; }
        Body* getLast() { return last; }

        Vec2d getPos() { return pos; };
        Vec2d getVel() { return vel; };
        Vec2d getAcc() { return acc; };
        
        float getAngle() { return ang; };

        void setFixture(Fixture *f);
        void setPos(Vec2d p);
        void rotate(float angle);
        Shape* getShape() { return fixture.getShape(); }

        bool isColliding(Body *b) const { return fixture.getShape()->checkOverlap(b->fixture.getShape()); };
        bool pointInside(Vec2d p) const { return fixture.getShape()->pointInside(p); }
    private:
        friend class World;
        
        Vec2d pos;
        double ang; // Angle stored in radians
        Vec2d vel;
        Vec2d acc;

        Body* next;
        Body* last;
};

#endif // !BODY_H
