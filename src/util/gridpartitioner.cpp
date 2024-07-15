
#include "gridpartitioner.h"
#include <algorithm>
#include <cmath>
#include <ostream>
#include <unordered_set>

GridPartitioner::GridPartitioner(int sizeX, int sizeY, int width, int height) {
   
    gridSizeX = sizeX;
    gridSizeY = sizeY;

    this->width = width;
    this->height = height;

    grid.resize(gridSizeX);

    for(int i = 0; i < gridSizeX; i++) {
        grid[i].resize(gridSizeY);
    }
}

void GridPartitioner::insertBody(Body* b) {
    Vec2d pos = b->getPos();
    std::pair<int, int> cell = getGridIndex(pos.x, pos.y);

    if(cell.first < 0 || cell.first >= gridSizeX || cell.second < 0 || cell.second >= gridSizeY) {
        // TODO: Make grid adaptive size!
        // For now just ignore these bodies :(
        return;
    }

    if(b->getShape()->getMaxRadius() > std::max(width/gridSizeX, height/gridSizeY)) {
        std::pair<Vec2d, Vec2d> boundingBox = b->getShape()->getBoundingBox();

        Vec2d minPos = boundingBox.first;
        Vec2d maxPos = boundingBox.second;
        std::pair<int, int> minGridCell = getGridIndex(minPos.x, minPos.y);
        std::pair<int, int> maxGridCell = getGridIndex(maxPos.x, maxPos.y);

        int minX = minGridCell.first;
        int minY = minGridCell.second;
        int maxX = maxGridCell.first;
        int maxY = maxGridCell.second;

        for(int i = minX; i <= maxX; i++) {
            for(int j = minY; j <= maxY; j++) {
                if(i < 0 || j < 0) {
                    continue;
                }
                grid[i][j].insert(b);
            }
        }
    } else {
        grid[cell.first][cell.second].insert(b);
    }
}

void GridPartitioner::removeBody(Body* b) {
    // TODO: implement this.
}

void GridPartitioner::update(Body* bodyLink) {
    clear();

    Body* b = bodyLink;

    int i = 0;
    while(b != nullptr) {
        insertBody(b);
        b = b->getNext();
    }
}

void GridPartitioner::clear() {
    int gridWidth = grid.size();
    int gridHeight = grid[0].size();

    for(int i = 0; i < gridWidth; i++) {
        for(int j = 0; j < gridHeight; j++) {
            grid[i][j].clear();
        }
    }
}

// TODO: improve this method
std::pair<int, int> GridPartitioner::getGridIndex(double x, double y) const {
    
    int cellX = floor(x / ((float)width/gridSizeX));
    int cellY = floor(y / ((float)height/gridSizeY)); 

    return {cellX, cellY};
}

std::unordered_set<Body*> GridPartitioner::getNeighbors(Body* b) const {
    Vec2d pos = b->getPos();

    std::pair<int, int> cell = getGridIndex(pos.x, pos.y);
    
    std::unordered_set<Body*> neighbors;

    int minX = cell.first;
    int minY = cell.second;
    int maxX = cell.first;
    int maxY = cell.second;

    if(b->getShape()->getMaxRadius() > std::max(width/gridSizeX, height/gridSizeY)) {
        std::pair<Vec2d, Vec2d> boundingBox = b->getShape()->getBoundingBox();

        Vec2d minPos = boundingBox.first;
        Vec2d maxPos = boundingBox.second;
        std::pair<int, int> minGridCell = getGridIndex(minPos.x, minPos.y);
        std::pair<int, int> maxGridCell = getGridIndex(maxPos.x, maxPos.y);

        minX = minGridCell.first;
        minY = minGridCell.second;
        maxX = maxGridCell.first;
        maxY = maxGridCell.second;
    }

    for(int i = minX-1; i <= maxX+1; i++) {
        for(int j = minY-1; j <= maxY+1; j++) {

            if(i < 0 || i >= gridSizeX || j < 0 || j >= gridSizeX)
                continue;

            for(Body* neighborBody : grid[i][j]) {
                if(neighborBody == b) {
                    continue;
                }
                neighbors.insert(neighborBody);
            }
        }
    }

    return neighbors;
}
