
#include "body.h"
#include "../shapes/shape.h"
#include "fixture.h"
#include <iostream>
#include <stdexcept>

Body::Body() {
    // TODO: Defauls constructor
}

Body::Body(Fixture *f) {
    pos = Vec2d(0,0);
    vel = Vec2d(0, 0);
    acc = Vec2d(0, 0);
    ang = 0;

    last = nullptr;
    next = nullptr;

    setFixture(f);
}

void Body::setFixture(Fixture *f) {
    std::cout << "Fixture: " << f->getShape()->getType() << std::endl;
    if(f->attach(this)) {
        fixture = Fixture(*f);
        std::cout << fixture.getShape()->getType() << std::endl;
    } else {
        throw std::runtime_error("ERROR: Fixture already attached - cannot be reused");
    }
}

void Body::setPos(Vec2d p) {
    pos.x = p.x;
    pos.y = p.y;
}
