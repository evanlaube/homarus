
#include "world.h"
#include "body.h"
#include "joints/spring.h"
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
World::World() : partitioner(200, 200, 1080/36, 720/36) {
    gravity = Vec2d(0,0);
}

void World::update(double timestep, int updates) {
    for(int i = 0; i < updates; i++) {
        step(timestep/updates);
    }
}

void World::step(double timestep) {
    Body* b = bodyLink;

    // Move objects kinematically
    while(b != nullptr) {
        if(b->getType() != BODY_STATIC) {
            b->vel += gravity * timestep;
            b->vel += (b->acc * timestep);
            b->pos += (b->vel * timestep);
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

    // Resolve collisions
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

    // Update joints
    Joint* j = jointLink;
    while(j != nullptr) {
        j->update();
        j = j->getNext();
    }
}

void World::collide(Body *a, Body *b, Collision c) {
    // Ignore if both bodies are static
    if(a->getType() == BODY_STATIC && b->getType() == BODY_STATIC)
        return;

    // Ensure that passed collision actually has overlap
    if(!c.colliding) {
        std::cout << "Collision triggered, but not colliding" << std::endl;
        return;
    }

    Vec2d overlap = c.overlap;
    Vec2d tangent = c.tangent;
    Vec2d normal = c.normal;
    Vec2d intersect = c.intersection;

    Vec2d aVel = a->fixture.getBody()->getVel();
    Vec2d bVel = b->fixture.getBody()->getVel();

    // Get magnitudes of velocities of both bodies
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
    Vec2d rap = Vec2d(ra.y, -1*ra.x);
    Vec2d rb = intersect - b->getPos();
    Vec2d rbp = Vec2d(rb.y, -1*rb.x);

    // Velocity of a with respect to b
    Vec2d vab = (aVel + rap*a->getOmega()) - (bVel + rbp*b->getOmega());

    // Calculate impulse of collison
    // TODO: Add in coefficinet of restitution
    double j = ((-2 * vab).dot(normal)) / ((normal.dot(normal) * (1.0/a->getMass() + 1.0/b->getMass())) + pow(rap.dot(normal), 2)/a->getMoment() + pow(rbp.dot(normal), 2)/b->getMoment());
    
    // Update linear velocities according to calculated impulse
    a->vel = aVel + normal * (j * (1.0/a->getMass()));
    b->vel = bVel - normal * (j * (1.0/b->getMass()));

    // Update angular velocities according to impulse
    a->omega = a->omega + (normal*j).dot(rap) / a->getMoment(); 
    b->omega = b->omega - (rbp.dot(normal * j)) / b->getMoment(); 
}

Body* World::createBody(Fixture *f, Vec2d pos) {
    void* mem = allocator.allocate(sizeof(Body));
    Body* b = new (mem) Body(f);
    b->next = bodyLink;

    if(b->next) {
        b->next->last = b;
    }

    b->pos = pos;

    bodyLink = b;
    bodyCount++;

    return b;
}

Spring* World::createSpring(Body* a, Body* b, double constant) {
    void* mem = allocator.allocate(sizeof(Spring)); 
    Spring *spring = new (mem) Spring(a, b, constant);
    spring->next = jointLink;
    jointLink = spring;

    return spring;
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
        // Add linear kinetic energy to total
        total += (0.5) * b->mass * velMagSquared;
        // Add rotational kinetic energy to total
        total += 0.5 * b->getMoment() * b->getOmega() * b->getOmega();
        b = b->next;
    }
    return total;
}

double World::getTotalEnergy() const {
    double kineticEnergy = 0;
    double potentialEnergy = 0;

    Body* b = bodyLink;
    while(b != nullptr) {
        if(b->getMass() == INFINITY) {
            b = b->next;
            continue;
        }

        double velMagSquared = b->getVel().magSquared();
        kineticEnergy += (0.5) * b->mass * velMagSquared;

        // TODO: Calculate potential energy from gravity
        
        b = b->getNext();
    }

    double jointEnergy = 0;
    Joint* j = jointLink;
    while(j != nullptr) {
        jointEnergy += j->getEnergy();
        j = j->getNext();
    }

    return potentialEnergy + kineticEnergy + jointEnergy;
}
