
#include "body.h"
#include "fixture.h"
#include "../shapes/shape.h"

Fixture::Fixture() {
    shape = nullptr;
}

Fixture::Fixture(Shape* s) {
    shape = s->clone();
    shape->attach(this);
}

bool Fixture::attach(Body* b) {
    if(attached) {
        return false;
    }

    body = b;
    attached = true;
    return true;
}
