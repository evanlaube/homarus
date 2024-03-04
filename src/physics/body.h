
#ifndef BODY_H
#define BODY_H

#include "../homarus.h"
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

        void setType(short type) { this->type = type;}
        void setFixture(Fixture *f);
        void setPos(Vec2d p);
        void setVel(Vec2d v) { vel = v; }
        void rotate(float angle);
        Shape* getShape() { return fixture.getShape(); }
        short getType() const { return type; }

        bool isColliding(Body *b) const { return fixture.getShape()->checkOverlap(b->fixture.getShape()); };
        bool pointInside(Vec2d p) const { return fixture.getShape()->pointInside(p); }
    private:
        friend class World;
        
        Vec2d pos;
        Vec2d vel;
        Vec2d acc;

        double ang; // Angle stored in radians
        double angVel;
        double angAcc;
        
        Body* next;
        Body* last;

        short type = BODY_DYMANIC;
};

#endif // !BODY_H
