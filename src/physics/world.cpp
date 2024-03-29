
#include "world.h"
#include "body.h"
#include "../util/blockallocator.h"
#include "../homarus.h"
#include "collision.h"
#include "../util/gridpartitioner.h"
#include <iostream>
#include <unordered_set>


// TODO: Make grid partitioner support infinite size
//   Achieve this by getting the average size of all bodies and setting grid size to that.
// For now initialize width and height values
// TODO: Make grid partitioner assume best size for the grid
// For now just use 200 subdivisions?
World::World() : partitioner(200, 200, 1080, 720) {
    gravity = Vec2d(0,0);
}

void World::step(double timestep, int updates) {
    for(int i = 0; i < updates; i++) {
        update(timestep/updates);
    }
}

void World::update(double timestep) {
    Body* b = bodyLink;

    while(b != nullptr) {
        if(b->getType() != BODY_STATIC) {
            b->pos += (b->vel * timestep);
            b->vel += (b->acc * timestep);
            b->vel += gravity * timestep;
            b->acc.erase();
        } else {
            b->acc.erase();
        }
        
        b = b->next;
    }

    partitioner.update(bodyLink);

    b = bodyLink;
    while(b != nullptr) {
        std::unordered_set<Body*> neighbors = partitioner.getNeighbors(b);

        for(Body* collider : neighbors) {
            if(b == collider) {
                continue;
            }

            Collision c = b->getShape()->getCollision(collider->getShape());

            if(c.colliding) {
                collide(b, collider, c);
            }
        }

        b = b->getNext();
    }
}

void World::collide(Body *a, Body *b, Collision c) {
    if(a->getType() == BODY_STATIC && b->getType() == BODY_STATIC)
        return;

    if(!c.colliding) {
        std::cout << "Collising triggered, but not colliding" << std::endl;
        return;
    }

    //std::cout << c << std::endl;
    //std::cout << "KE before collision: " << getTotalKE() << std::endl;
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

    if(b->getType() == BODY_STATIC) {
        bVelMag = 0;
    }

    if(a->getType() == BODY_STATIC) {
        aVelMag = 0;
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

    if(a->getType() == BODY_STATIC) {
        p1 = 0;
        p2 = -dpnormB;
    } else if(b->getType() == BODY_STATIC) {
        p2 = 0;
        p1 = -dpnormA;
    }

    a->vel.x = dptanA * tangent.x + p1 * normal.x;
    a->vel.y = dptanA * tangent.y + p1 * normal.y;

    b->vel.x = dptanB * tangent.x + p2 * normal.x;
    b->vel.y = dptanB * tangent.y + p2 * normal.y;

    //std::cout << "KE after collision: " << getTotalKE() << std::endl;
}

Body* World::createBody(Fixture *f, Vec2d pos) {
    void* mem = allocator.allocate(sizeof(Body));
    Body* b = new Body(f);
    b->next = bodyLink;

    if(b->next) {
        b->next->last = b;
    }

    b->pos = pos;

    bodyLink = b;
    bodyCount++;

    partitioner.insertBody(b);

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
