
#include "spring.h"
#include "../body.h"
#include <cmath>

Spring::Spring(Body* a, Body* b, double constant) {
    this->a = a;
    this->b = b;

    Vec2d d = a->getPos() - b->getPos();
    double dist = d.mag();
    // Set equilibrium length to initial distance between bodies
    equilibriumLength = dist;

    k = constant;
}

void Spring::update() {
    Vec2d v = a->getPos() - b->getPos();
    Vec2d dir = v.norm();
    double dist = v.mag();
    displacement = dist - equilibriumLength;

    double forceAmount = k * displacement;
    Vec2d springForce = dir * forceAmount/2;

    Vec2d relativeVelocity = b->getVel() - a->getVel();
    Vec2d dampingForce = relativeVelocity * -dampingRatio;

    Vec2d force = springForce + dampingForce;

    a->applyForce(force * -1);
    b->applyForce(force);
}

void Spring::setConstant(double constant) {
    if(constant > 0) {
        k = constant;
    }
}

void Spring::setEquilibriumLength(double length) {
    if(length > 0) {
        equilibriumLength = length;
    }
}

double Spring::getEnergy() const {
    if(displacement == 0) {
        return 0;
    }

    return 0.5 * k * pow(displacement, 2);
}
