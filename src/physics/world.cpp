
#include "world.h"
#include "body.h"
#include "../util/blockallocator.h"
#include <iostream>


World::World() {
}

void World::update(float timestep) {

    Body* b = bodyLink;

    while(b->next != nullptr) {
        b->pos.increase(b->vel);
        b->vel.increase(b->acc);
        b->acc.erase();
        b = b->next;
    }
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
