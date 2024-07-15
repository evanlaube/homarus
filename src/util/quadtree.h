
#ifndef QUADTREE_H
#define QUADTREE_H

#include <cmath>
#include <ostream>
#include <queue>
#include <unordered_set>
#include <iostream>
#include <vector>
#include "../physics/body.h"

#define MAX_NODE_DEPTH 12 
#define MAX_NODE_BODIES 4

struct Node {
    public:
        float x;
        float y;
        float w;
        float h;
        bool isLeaf;
        int depth;

        std::vector<Body*> bodies;
        Node* children[4];

        Node(float x, float y, float w, float h, int depth) 
            : x(x), y(y), w(w), h(h), isLeaf(true), depth(depth) {
            std::fill(std::begin(children), std::end(children), nullptr);
        }

        bool contains(Body*b) {
            std::pair<Vec2d, Vec2d> box = b->getBoundingBox();
            return intersects(box);
        }

        void add(Body* body) {

            if(isLeaf) {
                bodies.push_back(body);

                if(bodies.size() > MAX_NODE_BODIES && depth < MAX_NODE_DEPTH) {
                    subdivide();
                    for(Body* b : bodies) {
                        for(Node* n : children) {
                            if(n->contains(b)) {
                                n->add(b);
                                break;
                            }
                        }
                    }
                    bodies.clear();
                }
            } else {
                for(Node* n : children) {
                    if(n->contains(body)) {
                        n->add(body);
                        break;
                    }
                }
            }
        }

        void getBodiesInRegion(std::unordered_set<Body*>& result, std::pair<Vec2d, Vec2d> region) {
            if(!isLeaf) {
                for(Node* n : children) {
                    if(n->intersects(region)) {
                        n->getBodiesInRegion(result, region);
                    }
                }
            } else {
                for(Body* b : bodies) {
                    result.insert(b);
                }
            }
        }

        bool intersects(std::pair<Vec2d, Vec2d> box) {
            return !(box.second.x < x || box.first.x > x + w || box.second.y < y || box.first.y > y + h);
            //if( (box.first.x > x && box.first.x <= x + w) || (box.second.x > x && box.second.x <= x+w) ) {
            //    if((box.first.y > y && box.first.y <= y + h) || (box.second.y > y && box.second.y <= y+h)) {
            //        return true;
            //    }
            //}
            //return false;
        }

    private:
        void subdivide() {
            float halfw = w / 2.0;
            float halfh = h / 2.0;

            children[0] = new Node(x, y, halfw, halfh, depth+1);
            children[1] = new Node(x, y + halfh, halfw, halfh, depth+1);
            children[2] = new Node(x + halfw, y, halfw, halfh, depth+1);
            children[3] = new Node(x + halfw, y + halfh, halfw, halfh, depth+1);
            isLeaf = false;
        }

        
};

class Quadtree {
    public:
        Quadtree(float x, float y, float w, float h)
            : root(x, y, w, h, 0) {};

        void update(Body* bodyLink);
        std::unordered_set<Body*> getNeighbors(Body*);

        Node* getRoot() { return &root; }
    private:
        Node root;

};

#endif // !QUADTREE_H
