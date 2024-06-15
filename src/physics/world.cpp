
#include "world.h"
#include "body.h"
#include "../util/blockallocator.h"
#include "../homarus.h"
#include "collision.h"
#include "../util/gridpartitioner.h"
#include <cmath>
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

            b->rotate(b->omega * timestep);
            b->omega += b->alpha * timestep;
            b->alpha = 0;

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
        std::cout << "Collision triggered, but not colliding" << std::endl;
        return;
    }

    //std::cout << "KE before collision: " << getTotalKE() << std::endl;
    Vec2d overlap = c.overlap;
    Vec2d tangent = c.tangent;
    Vec2d normal = c.normal;
    Vec2d intersect = c.intersection;

    // This check can be made more efficient
    Vec2d aVel = a->fixture.getBody()->getVel();
    Vec2d bVel = b->fixture.getBody()->getVel();

    double aVelMag = aVel.mag();
    double bVelMag = bVel.mag();

    // Static Collision
    // A small pecrentage gap is needed (1%) in order to stop objects from
    // 'sticking' together (it's an interesting bug). Therefore,
    // TODO: Change this code up to have a smaller margin of error. 
    a->pos += overlap * 1.01 * (aVelMag /(aVelMag + bVelMag));
    b->pos -= overlap * 1.01 * (bVelMag /(aVelMag + bVelMag));

    // Perpendicular 'radius' from COM of body to point of intersect
    Vec2d ra = intersect - a->getPos();
    ra = Vec2d(ra.y, -1*ra.x);
    Vec2d rb = intersect - b->getPos();
    rb = Vec2d(rb.y, -1*rb.x);

    // Velocity of a with respect to b
    Vec2d vab = aVel - bVel;

    // Calculate impulse of collison
    double j = ((-2 * vab).dot(normal)) / ((normal.dot(normal) * (1.0/a->getMass() + 1.0/b->getMass())) + pow(ra.dot(normal), 2)/a->getMoment() + pow(rb.dot(normal), 2)/b->getMoment());
    
    // Update linear velocities according to calculated impulse
    a->vel = aVel + normal * (j * (1.0/a->getMass()));
    b->vel = bVel - normal * (j * (1.0/b->getMass()));

    // Update angular velocities according to impulse
    a->omega = a->omega + (normal*j).dot(ra) / a->getMoment(); 
    b->omega = b->omega - (rb.dot(normal * j)) / b->getMoment(); 

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
        if(b->getMass() == INFINITY) {
            b = b->next;
            continue;
        }

        double velMagSquared = b->getVel().magSquared();
        total += (0.5) * b->mass * velMagSquared;
        total += 0.5 * b->getMoment() * b->getOmega() * b->getOmega();
        b = b->next;
    }
    return total;
}
