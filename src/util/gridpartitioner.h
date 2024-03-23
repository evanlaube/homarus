#ifndef GRID_PARTITIONER_H
#define GRID_PARTITIONER_H

#include <vector>
#include <unordered_set>
#include "../physics/body.h"

class GridPartitioner {
    public:
        GridPartitioner(int sizeX, int sizeY, int width, int height);
        void insertBody(Body* b);
        void removeBody(Body* b);
        void update(Body* bodyLink);
        std::unordered_set<Body*> getNeighbors(Body* b) const;
    private:
        int gridSizeX;
        int gridSizeY;

        int width;
        int height;

        std::vector<std::vector<std::unordered_set<Body*>>> grid;

        std::pair<int, int> getGridIndex(double x, double y) const;

        void clear();
};

#endif // !GRID_PARTITIONER_H
