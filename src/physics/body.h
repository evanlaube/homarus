
#ifndef BODY_H
#define BODY_H

#include "../homarus.h"
#include "../util/vec2.h"
#include "fixture.h"
#include "../shapes/shape.h"

/**
 * A Body is a phyiscal object in the World. All bodies must have a fixture that 
 * has a shape attached. Bodies store the linear and angular position, velocity, and acceleration
 * of an object, as well as properties like their mass. Bodies also store a pointer
 * to the next body in the world, acting like a linked list.
 * Bodies can be either static or dynamic, where a static body does not move and a
 * dynamic body does. Static bodies are good for tasks like making boundaries.
 */
class Body {

    public:
        Body();
        Body(Fixture *fixture);


        Body* getNext() const { return next; }
        Body* getLast() const { return last; }

        Vec2d getPos() const { return pos; };
        Vec2d getVel() const { return vel; };
        Vec2d getAcc() const { return acc; };
        
        double getAngle() const { return ang; };
        double getOmega() const { return omega; };
        double getAplha() const { return alpha; };

        double getMoment() const;
        double getMass() const;
        const Fixture* getFixture() const { return &fixture; }
        Shape* getShape() const { return fixture.getShape(); }
        short getType() const { return type; }

        /**
         * Sets the type of the body to either static or dynamic.
         * @param type Type to set body to. Use BODY_DYNAMIC or BODY_STATIC
         */
        void setType(short type) { this->type = type;}
        void setFixture(Fixture *f);
        void setMass(double mass);
        void setPos(Vec2d p);
        void setVel(Vec2d v) { vel = v; }
        void setOmega(double w);

        /**
         * Rotates the body by a set amount of radians.
         *
         * @param angle Amount to rotate the body in radians
         */
        void rotate(float angle);

        /**
         * Applies a force to the body according to the passed vector. The force is applied
         * over one phyisics step.
         *
         * @param force A vector that depicts the force applied in newtons
         */
        void applyForce(Vec2d force);
        void applyForce(double fx, double fy) { applyForce(Vec2d(fx, fy)); }

        /**
         * Checks if body is colliding with other.
         *
         * @param b Body to check collision against
         * @return True if bodies are colliding, False if not
         */
        bool isColliding(Body *b) const { return fixture.getShape()->checkOverlap(b->fixture.getShape()); };

        /** 
         * Checks if a point is contained inside the body.
         *
         * @param p Vector to check if contained inside body
         * @return True if point is inside body, false if not
         */
        bool pointInside(Vec2d p) const { return fixture.getShape()->pointInside(p); }
    private:
        friend class World;
        /** Fixture that attaches the body to a shape */
        Fixture fixture;
        
        /** linear position of the Body */
        Vec2d pos;
        /** Linear velociy of the Body */
        Vec2d vel;
        /** Linear acceleration of the Body */
        Vec2d acc;

        /** Angular position of the Body */
        double ang; 
        /** Angular velocity of the Body */
        double omega;
        /** Angular acceleration of the Body */
        double alpha;

        /** Mass of the body (in kg) */
        double mass;
        
        /** Pointer to the next body in linked list of bodies */
        Body* next;
        /** Pointer to previous body in linked list of bodies */
        Body* last;

        short type = BODY_DYNAMIC;
};

#endif // !BODY_H
