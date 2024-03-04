
#include "world.h"
#include "body.h"
#include "../util/blockallocator.h"
#include "../homarus.h"
#include <iostream>


World::World() {
    gravity = Vec2d(0,0);
}

void World::update(float timestep) {

    Body* b = bodyLink;

    while(b != nullptr) {
        updateBody(b, timestep);
        b = b->next;
    }
}

void World::updateBody(Body*b, float timestep) {

    b->acc += gravity * timestep;

    if(b->getType() != BODY_STATIC) {
        b->pos = b->pos +  (b->vel * timestep);
        b->vel = b->vel + (b->acc * timestep);
        b->acc.erase();
    } else {
        b->vel.erase();
    }

    Body *collider = bodyLink;
    while(collider != nullptr) {
        if(b == collider) {
            collider = b->getNext();
            continue;
        }

        if(b->isColliding(collider)) {
            collide(b, collider);
        }
        
        collider = collider->getNext();
    }
}

void World::collide(Body *a, Body *b) {
    Vec2d overlap = a->fixture.shape->getOverlap(b->fixture.shape);
    std::cout << "Overlap before collision: " << overlap << std::endl;

    // This check can be made more efficient
    Vec2d aVel = a->fixture.getBody()->vel;
    Vec2d bVel = b->fixture.getBody()->vel;

    double aVelMag = aVel.mag();
    double bVelMag = bVel.mag();
 
    if(aVelMag == 0 && bVelMag == 0) {
        if(a->getType() != BODY_STATIC) {
            aVelMag = 1;
        }

        if(b->getType() != BODY_STATIC) {
            bVelMag = 1;
        }
    }

    // Static Collision
    a->pos += overlap*1.0001 * (aVelMag /(aVelMag + bVelMag));
    b->pos -= overlap*1.0001 * (bVelMag /(aVelMag + bVelMag));

    std::cout << "Overlap after collision: " << a->fixture.shape->getOverlap(b->fixture.shape) << std::endl;
    
    // TODO: Apply conservation of linear and angular momentum to alter velocities
    // Need a way to get the intersection point of the two bodies for angular momentum transfer,
    // but this is a later problem
    a->vel.erase();
    b->vel.erase();
}

Body* World::createBody(Fixture *f) {
   void* mem = allocator.allocate(sizeof(Body));
   Body* b = new Body(f);
    std::cout << mem << std::endl;

   b->next = bodyLink;
   if(b->next) {
       b->next->last = b;
   }

   bodyLink = b;
   bodyCount++;
   return b;
}
