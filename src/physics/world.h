
#ifndef WORLD_H
#define WORLD_H

#include "body.h"
#include "../util/blockallocator.h"
#include "../util/vec2.h"

class World {
    public:
        World();
    
        BlockAllocator allocator;

        Body* bodyLink = nullptr;
        int bodyCount = 0;

        void update(float timestep);
        void updateBody(Body *b, float timestep);
        void collide(Body* a, Body* b, Collision c);

        void setGravity(float x, float y) { gravity = Vec2d(x,y); }
        Vec2d getGravity() const { return gravity; }

        Body* createBody(Fixture *f);
    private:
        Vec2d gravity;
};

#endif // !WORLD_H
