
#include "body.h"
#include "fixture.h"

Body::Body(Vec2d pos, double ang, Fixture fixture) {
    this->pos = pos;
    vel = Vec2d(0, 0);
    acc = Vec2d(0, 0);

    this->ang = ang;
    this->fixture = fixture;
}
