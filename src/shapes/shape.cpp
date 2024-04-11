
#include "../physics/fixture.h"
#include "../physics/body.h"
#include "shape.h"
#include "../util/vec2.h"

Shape::Shape() {

}

void Shape::attach(Fixture* f) {
    fixture = f;
}

void Shape::calcArea() {
    return;
}

void Shape::calcCentroid() {
    return;
}

Vec2d Shape::getPos() const {
    return fixture->getBody()->getPos();
}

float Shape::getAngle() const {
    return fixture->getBody()->getAngle();
}

double Shape::getMass() const {
    return fixture->getBody()->getMass();
}

