
#ifndef WORLD_H
#define WORLD_H

#include "body.h"
#include "../util/blockallocator.h"

class World {
    public:
        World();
    
        BlockAllocator allocator;

        Body* bodyLink = nullptr;
        int bodyCount = 0;

        void update(float timestep);
        void updateBodies(float timestep);

        Body* createBody(Fixture *f);
};

#endif // !WORLD_H
