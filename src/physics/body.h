
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

        Body* getNext() const { return next; }
        Body* getLast() const { return last; }

        Vec2d getPos() const { return pos; };
        Vec2d getVel() const { return vel; };
        Vec2d getAcc() const { return acc; };
        
        float getAngle() const { return ang; };
        double getMoment() const;

        void setType(short type) { this->type = type;}
        void setFixture(Fixture *f);
        void setPos(Vec2d p);
        void setVel(Vec2d v) { vel = v; }
        void rotate(float angle);
        Shape* getShape() { return fixture.getShape(); }
        short getType() const { return type; }

        double getMass() { return mass; }
        void setMass(double mass);

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

        double mass;
        
        Body* next;
        Body* last;

        short type = BODY_DYNAMIC;
};

#endif // !BODY_H
