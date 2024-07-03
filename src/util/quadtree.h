
#ifndef QUADTREE_H
#define QUADTREE_H

// Set node max really high to fully divide for now
#include <queue>
#include <unordered_set>
#define MAX_NODE_LEVEL 9999

#include <iostream>
#include <vector>
#include "../physics/body.h"

struct Node {
    public:
        static std::queue<Node*> pool;

        int x;
        int y;
        int w;
        int h;

        int level;

        std::vector<Node*> children;
        std::vector<Body*> bodies;

        Node(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {
            std::cout << this->x << std::endl;
        }

        void add(Body* body) {
            if(bodies.size() < 4 || level == MAX_NODE_LEVEL) {
                bodies.push_back(body);
            } else {
                if(children.size() == 0) {
                    Node* nw = pool.front();
                    pool.pop();
                    Node* ne = pool.front();
                    pool.pop();
                    Node* sw = pool.front();
                    pool.pop();
                    Node* se = pool.front();
                    pool.pop();

                    nw->set(x, y, w/2, h/2, level+1);
                    children.push_back(nw);
                    ne->set(x+w/2, y, w/2, h/2, level+1);
                    children.push_back(ne);
                    sw->set(x, y + w/2, w/2, h/2, level+1);
                    children.push_back(sw);
                    se->set(x+w/2, y + w/2, w/2, h/2, level+1);
                    children.push_back(se);

                    for(Body* b : bodies) {
                        for(Node* node : children) {
                            if(node->contains(b)) {
                                node->add(b);
                            }
                        }
                    }
                }
            }
        }

    private:
        void set(int x, int y, int w, int h, int level) {
            for(Node* c : children) {
                pool.push(c);
            }

            children.clear();
            bodies.clear();

            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
            this->level = level;
        }

        bool contains(Body*b) {
            std::pair<Vec2d, Vec2d> box = b->getBoundingBox();

            if( (box.first.x > x && box.first.x <= x + w) || (box.second.x > x && box.second.x <= x+w) ) {
                if((box.first.y > y && box.first.y <= y + h) || (box.second.y > y && box.second.y <= y+h)) {
                    return true;
                }
            }

            return false;
        }
        
};

class Quadtree {
    public:
        Quadtree();

        void construct(Body* bodyLink);
        std::unordered_set<Body*> getNeighbors(Body*);
    private:

};

#endif // !QUADTREE_H
