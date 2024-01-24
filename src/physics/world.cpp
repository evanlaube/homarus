
#include "world.h"
#include "body.h"
#include <vector>


World::World() {
    bodies = std::vector<Body>();
}

void World::update(float timestep) {
    for(Body b : bodies) {
        b.pos.increase(b.vel);
        b.vel.increase(b.acc);

        b.acc.erase();
    }
}

void World::addBody(Body b) {
    bodies.push_back(b);
}
