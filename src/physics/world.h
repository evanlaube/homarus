
#ifndef WORLD_H
#define WORLD_H

#include "body.h"
#include "../util/blockallocator.h"
#include "../util/vec2.h"
#include "../util/gridpartitioner.h"

/**
 * The World class is the container for all entities that are created. Only one world is needed
 * per physics simulation. The World handles updating all entities, as well as managing the memory
 * that they are created in.
 *
 * The World should be created first, before any bodies are created.
 */

class World {
    public:
        World();
    
        /** Memory manager to store all entities in world */
        BlockAllocator allocator;

        /** Partitioning system to make checking collisions more efficient */
        GridPartitioner partitioner;

        /** Pointer to first body in linked list of bodies */
        Body* bodyLink = nullptr;

        /** Number of bodies in world */
        int bodyCount = 0;

        /**
         * This function allocates sufficient memory for a body, as well as attaching 
         * a fixture to it and giving it a position.
         *
         * @param f The fixture to attach to the body (must already have shape)
         * @param pos The position the body will be created at
         * @return A pointer to the created body
         */
        Body* createBody(Fixture *f, Vec2d pos);

        /**
         * This function progresses bodies in world forward in time over multiple physics steps
         *
         * @param timestep the amount of time to progress the world
         * @param updates the number of physics steps that the update will make
         */
        void update(double timestep, int updates);

        /**
         * This function makes a singular physics step forward in the world
         *
         * @param timestep The amount the step moves forward in time
         */
        void step(double timestep);

        /**
         * This function resolves the collision of two bodies and changes their velocities accordingly
         *
         * @param a The first body in the collision
         * @param b The second body in the collision
         * @param c The Collision object involving a and b
         */
        void collide(Body* a, Body* b, Collision c);

        /**
         * This function calculates the total kinetic energy of all bodies contained
         * in the world. This includes both linear and angular kinetic energy
         *
         * @return The calculated amount of kinetic energy in joules.
         */
        double getTotalKE() const;

        void setGravity(float x, float y) { gravity = Vec2d(x,y); }
        Vec2d getGravity() const { return gravity; }

    private:
        /** Vector storing the acceleration on all bodies due to gravity (in m/s^2) */
        Vec2d gravity;
};

#endif // !WORLD_H
