
#include "body.h"
#include "../shapes/shape.h"
#include "fixture.h"
#include "../homarus.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

Body::Body() {
    // TODO: Defauls constructor
}

Body::Body(Fixture *f) {
    pos = Vec2d(0,0);
    vel = Vec2d(0, 0);
    acc = Vec2d(0, 0);
    // Angle variable simply to keep track of angle for drawing purposes. The
    // actual rotation is stored within the shape by rotating its vertices.
    ang = 0;

    last = nullptr;
    next = nullptr;

    setFixture(f);
}

void Body::setFixture(Fixture *f) {
    if(f->attach(this)) {
        fixture = Fixture(*f);
    } else {
        throw std::runtime_error("ERROR: Fixture already attached - cannot be reused");
    }
}

void Body::setPos(Vec2d p) {
    pos.x = p.x;
    pos.y = p.y;
}

void Body::setOmega(double w) {
    omega = w;
}

void Body::setMass(double mass) {
    this->mass = mass;
    fixture.getShape()->calcMoment();
}

void Body::rotate(float angle) {
    fixture.getShape()->rotate(angle);
    this->ang += angle;
}

double Body::getMoment() const {
    if(type == BODY_STATIC) {
        return INFINITY;
    }
    return fixture.getShape()->getMoment();
}

double Body::getMass() const {
    if(type == BODY_STATIC) {
        return INFINITY;
    }
    return mass;
}
