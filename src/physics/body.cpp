
#include "body.h"
#include "fixture.h"
#include <stdexcept>

Body::Body() {
    Fixture f;

    Body(Vec2d(0,0), 0, f);
}

Body::Body(Vec2d pos, double ang, Fixture f) {
    this->pos = pos;
    vel = Vec2d(0, 0);
    acc = Vec2d(0, 0);

    this->ang = ang;
    
    if(f.attached) {
        throw std::runtime_error("Unable to attach fixture - Fixture already attached");
    }

    f.attached = true;
    fixture = Fixture(f);
}
