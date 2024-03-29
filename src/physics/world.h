
#ifndef WORLD_H
#define WORLD_H

#include "body.h"
#include "../util/blockallocator.h"
#include "../util/vec2.h"
#include "../util/gridpartitioner.h"

class World {
    public:
        World();
    
        BlockAllocator allocator;

        GridPartitioner partitioner;

        Body* bodyLink = nullptr;
        int bodyCount = 0;

        void step(double timestep, int updates);
        void update(double timestep);
        void updateBody(Body *b, float timestep);
        void collide(Body* a, Body* b, Collision c);

        double getTotalKE() const;

        void setGravity(float x, float y) { gravity = Vec2d(x,y); }
        Vec2d getGravity() const { return gravity; }

        Body* createBody(Fixture *f, Vec2d pos);
    private:
        Vec2d gravity;
};

#endif // !WORLD_H
