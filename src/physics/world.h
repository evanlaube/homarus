
#ifndef WORLD_H
#define WORLD_H

#include "body.h"
#include <vector>

class World {
    public:
        World();

        std::vector<Body> bodies;

        void update(float timestep);
        void updateBodies(float timestep);

        void addBody(Body b);
};

#endif // !WORLD_H
