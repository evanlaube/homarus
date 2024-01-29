
#include "body.h"
#include "fixture.h"
#include <stdexcept>

Body::Body(Vec2d pos, double ang, Fixture fixture) {
    this->pos = pos;
    vel = Vec2d(0, 0);
    acc = Vec2d(0, 0);

    this->ang = ang;

    setFixture(fixture);
}

void Body::setFixture(Fixture fixture) {
    if(fixture.attached) {
        throw std::runtime_error("Unable to attach fixture - Fixture already attached");
    }

    fixture.attached = true;
    this->fixture = fixture;
}
