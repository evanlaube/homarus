
#include "world.h"
#include "body.h"
#include "../util/blockallocator.h"
#include "../homarus.h"
#include "collision.h"
#include <iostream>


World::World() {
    gravity = Vec2d(0,0);
}

void World::update(float timestep) {

    Body* b = bodyLink;
    timestep = 0.01;

    while(b != nullptr) {
        //updateBody(b, timestep);
        b->acc += gravity * timestep;

        if(b->getType() != BODY_STATIC) {
            b->pos = b->pos +  (b->vel * timestep);
            b->vel = b->vel + (b->acc * timestep);
            b->acc.erase();
        } else {
            b->vel.erase();
        }
        b = b->next;
    }

    b = bodyLink;
    while(b != nullptr) {
        Body *collider = bodyLink;
        while(collider != nullptr) {
            if(b == collider) {
                collider = b->getNext();
                continue;
            }

            Collision c = b->fixture.shape->getCollision(collider->fixture.shape);

            if(c.colliding)  {
                collide(b, collider, c);
            }

            collider = collider->getNext();
        }
        b = b->next;
    }
}

void World::updateBody(Body*b, float timestep) {

    b->acc += gravity * timestep;

    if(b->getType() != BODY_STATIC) {
        b->pos += (b->vel * timestep);
        b->vel += (b->acc * timestep);
        b->acc.erase();
    } else {
        b->vel.erase();
    }

    Body *collider = bodyLink;
    int i = 0;
    while(collider != nullptr) {
        if(b == collider) {
            collider = b->getNext();
            continue;
        }

        Collision c = b->fixture.shape->getCollision(collider->fixture.shape);

        if(c.colliding)  {
            collide(b, collider, c);
        }
        
        collider = collider->getNext();
    }
}

void World::collide(Body *a, Body *b, Collision c) {
    
    if(!c.colliding) {
        std::cout << "Collising triggered, but not colliding" << std::endl;
        return;
    }

    std::cout << "KE before collision: " << getTotalKE() << std::endl;
    Vec2d overlap = c.overlap;
    Vec2d tangent = c.tangent;
    Vec2d intersect = c.intersection;

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
    // A small pecrentage gap is needed (1%) in order to stop objects from
    // 'sticking' together (it's an interesting bug). Therefore,
    // TODO: Change this code up to have a smaller margin of error. 
    a->pos += overlap * 1.01 * (aVelMag /(aVelMag + bVelMag));
    b->pos -= overlap * 1.01 * (bVelMag /(aVelMag + bVelMag));
    
    Vec2d normal = Vec2d(tangent.y, -tangent.x);

    double dptanA = a->vel.dot(tangent);
    double dptanB = b->vel.dot(tangent);

    double dpnormA = a->vel.dot(normal);
    double dpnormB = b->vel.dot(normal);

    double p2 = (a->mass * dpnormA + b->mass * dpnormB + a->mass * (dpnormA - dpnormB)) / (a->mass + b->mass);
    double p1 = p2 - (dpnormA - dpnormB);

    a->vel.x = dptanA * tangent.x + p1 * normal.x;
    a->vel.y = dptanA * tangent.y + p1 * normal.y;

    b->vel.x = dptanB * tangent.x + p2 * normal.x;
    b->vel.y = dptanB * tangent.y + p2 * normal.y;

    std::cout << "KE after collision: " << getTotalKE() << std::endl;
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

double World::getTotalKE() const {
    
    double total = 0;

    Body* b = bodyLink;
    while(b != nullptr) {
        double velMagSquared = b->getVel().magSquared();
        total += (0.5) * b->mass * velMagSquared;
        b = b->next;
    }
    return total;
}
