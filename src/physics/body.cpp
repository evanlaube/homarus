
#include "body.h"
#include "fixture.h"
#include <stdexcept>

Body::Body() {

}

Body::Body(Vec2d pos, double ang, Fixture f) {
    this->pos = pos;
    vel = Vec2d(0, 0);
    acc = Vec2d(0, 0);

    this->ang = ang;

    setFixture(f);
}

void Body::setFixture(Fixture f) {
    if(f.attach(this)) {
        fixture = f;
    } else {
        throw std::runtime_error("ERROR: Fixture already attached - cannot be reused");
    } 
}

void Body::setPos(Vec2d p) {
    pos.x = p.x;
    pos.y = p.y;
}
