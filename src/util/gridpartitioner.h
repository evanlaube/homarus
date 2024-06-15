#ifndef GRID_PARTITIONER_H
#define GRID_PARTITIONER_H

#include <vector>
#include <unordered_set>
#include "../physics/body.h"

/**
 * The GridPartitioner is a method of partitioning bodies in order to make collision detection 
 * faster by preventing having to check every body against every other body. Using the grid partitioner,
 * bodies only have to check collisions agains bodies in neighboring cells, which is much faster. 
 */
class GridPartitioner {
    public:
        GridPartitioner(int sizeX, int sizeY, int width, int height);

        /**
         * Updates the grid partitioner by looping through all bodies and adding a pointer
         * to them to every cell in which their bouding box occupies.
         *
         * @param bodyLink The pointer to the first Body in the linkedList of bodies
         */
        void update(Body* bodyLink);

        /**
         * Gets the neighbors of a body in the partitioner. This returns a list of all bodies
         * that could be colliding with the passed body, speeding up collision detection.
         *
         * @param b The body to get the neighbors of.
         * @return An unordered set containing pointers to all of the bodies that the passed body could be colliding with
         */
        std::unordered_set<Body*> getNeighbors(Body* b) const;
    private:
        /** The number of cells along the x-axis of the grid */
        int gridSizeX;
        /** The number of cells along the y-axis of the grid */
        int gridSizeY;

        /** The overall width of the grid */
        int width;
        /** The overall height of the grid */
        int height;

        std::vector<std::vector<std::unordered_set<Body*>>> grid;

        std::pair<int, int> getGridIndex(double x, double y) const;

        void insertBody(Body* b);
        void removeBody(Body* b);

        void clear();
};

#endif // !GRID_PARTITIONER_H
