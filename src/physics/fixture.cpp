
#include "fixture.h"
#include "../shapes/shape.h"

Fixture::Fixture() {
    shape = nullptr;
}

Fixture::Fixture(Shape* s) {
    shape = s;
}
