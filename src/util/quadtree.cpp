
#include "quadtree.h"
#include <climits>
#include <iostream>
#include <queue>
#include <unordered_set>


void Quadtree::update(Body* bodylink) {

    Body* body = bodylink;

    double minx = INT_MAX;
    double miny = INT_MAX;
    double maxx = INT_MIN;
    double maxy = INT_MIN;

    // TODO: Find some way to get rid of this loop through all bodies
    while(body != nullptr) {
        std::pair<Vec2d, Vec2d> box = body->getBoundingBox();
        if(box.first.x < minx) {
            minx = box.first.x;
        }
        if(box.second.x > maxx) {
            maxx = box.second.x;
        }

        if(box.first.y < miny) {
            miny = box.first.y;
        }
        if(box.second.y > maxy) {
            maxy = box.second.y;
        }
        body = body->getNext();
    }

    root = Node(minx, miny, maxx - minx, maxy - miny, 0);
    body = bodylink;

    while(body != nullptr) {
        root.add(body);
        body = body->getNext();
    }
}

std::unordered_set<Body*> Quadtree::getNeighbors(Body* body) {
    std::unordered_set<Body*> result;
    std::pair<Vec2d, Vec2d> box = body->getBoundingBox();
    root.getBodiesInRegion(result, box);
    return result;
}
