
#include "../physics/fixture.h"
#include "../physics/collision.h"
#include "shape.h"
#include "circle.h"
#include "polygon.h"
#include "../util/vec2.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

void Polygon::orderVertices() {
    // Subtract first vertex from all verticies to make first vertex (0,0)
    Vec2d first = vertices[0];
    first.mult(-1);
    for(int i = 0; i < vertices.size(); i++) {
        vertices[i].increase(first);
    }

    std::sort(vertices.begin(), vertices.end(), ccwCompare);
}

bool Polygon::ccwCompare(Vec2d a, Vec2d b) {
    double cross = a.crossZ(b);

    if(cross == 0) {
       // TODO: Make vertex closer to center come first in verticies vector
       //       when cross product is zero. Have to get around being unable 
       //       to access centroid vector from static context
       //
       //       For now only allow convex polygons, or at least no concave
       //       polygons with 90 degree angles on the inside
       return false;
    }

    return cross > 0;
}

void Polygon::rotate(float angle) {
    rotateVertices(angle);
}

Polygon::Polygon(std::vector<Vec2d> vertices) {
    this->vertices = vertices;

    orderVertices();

    calcArea();
    calcCentroid();

    for(int i = 0; i < this->vertices.size(); i++) {
        Vec2d v = this->vertices[i];
        this->vertices[i] = Vec2d(v.x - centroid.x, v.y-centroid.y);
        //std::cout << this->vertices[i] << std::endl;
    }
    centroid = Vec2d(0,0);

    float maxDistSquare = 0;
    for(Vec2d v : this->vertices) {
        Vec2d diff = centroid - v;
        float distSquare = diff.x*diff.x + diff.y*diff.y;
        if(distSquare > maxDistSquare) {
            maxDistSquare = distSquare;
        }
    }
    maxRadius = sqrt(maxDistSquare);

    //std::cout << "Area: " << area << std::endl;
    //std::cout << "Centroid: (" << centroid.x << ", " << centroid.y << ")" << std::endl;
}

void Polygon::calcArea() {
    float a = 0;

    for(int i = 0; i < vertices.size(); i++) {
        Vec2d vert = vertices[i];
        Vec2d next = vertices[(i+1) % (vertices.size())];

        a += (vert.x * next.y) - (next.x * vert.y);
    }

    a /= 2;

    area = a;
}

void Polygon::calcCentroid() {
    if(!area) calcArea();

    int vertCount = vertices.size();

    float cx = 0;
    float cy = 0;

    for(int i = 0; i < vertCount; i++) {

        Vec2d vert = vertices[i];
        Vec2d next = vertices[(i+1)%(vertCount)];

        cx += (vert.x + next.x) * (vert.x * next.y - next.x * vert.y);
        cy += (vert.y + next.y) * (vert.x * next.y - next.x * vert.y);
    }

    cx /= (6*area);
    cy /= (6*area);

    centroid = Vec2d(cx, cy);
}

bool Polygon::pointInside(Vec2d p) const {
    // Make position relative to body's position, not world
    Vec2d v1 = vertices[0];
    v1 = v1 + getPos();

    int intersects = 0;
    
    for(int i = 0; i <= vertices.size(); i++) {
        Vec2d v2 = vertices[i%vertices.size()];
        v2 = v2 + getPos();

        //std::cout << "p.y: " << p.y << " | Max: " << std::max(v1.y, v2.y) << " | Min: " << std::min(v1.y, v2.y) << std::endl;
        if(p.y >= std::min(v1.y, v2.y) && p.y <= std::max(v1.y, v2.y)) {
            if(p.x <= std::max(v1.x, v2.x)) {
                float xIntersect = ((p.y-v1.y) * (v2.x - v1.x)) / (v2.y - v1.y) + v1.x;

                if(p.x <= xIntersect) {
                    intersects++;
                }
            }
        }

        v1 = v2;
    }
   
    // Return true if a ray being traced to the right intersects an odd number of edges of the polygon
    return intersects % 2;
}

bool Polygon::checkOverlap(Shape *s) const {
    Vec2d a = getPos();
    Vec2d b = s->getPos();

    Vec2d diff = b - a;
    float distSquare = diff.x * diff.x + diff.y * diff.y;
    float radSum = (maxRadius + s->getMaxRadius());

    if(distSquare > (radSum*radSum)) {
        return false;
    }

    return s->checkPolygonOverlap((Polygon*)this);
}

bool Polygon::checkCircleOverlap(Circle *s) const {
    return s->checkPolygonOverlap((Polygon*)this);
}

bool Polygon::checkPolygonOverlap(Polygon *s) const {
    
    for(Vec2d v : s->getVertices()) {
        if(pointInside(v + s->getPos())) {
            return true;
        }
    }

    for(Vec2d v : vertices) {
        if(s->pointInside(v + s->getPos())) {
            return true;
        }
    }

    return false;
}

std::pair<Vec2d, Vec2d> Polygon::getBoundingBox() const {

    double minX = vertices[0].x;
    double minY = vertices[0].y;
    double maxX = vertices[0].x;
    double maxY = vertices[0].y;

    for(Vec2d v : vertices) {
        if(v.x < minX) {
            minX = v.x;
        } else if (v.x > maxX) {
            maxX = v.x;
        }

        if(v.y < minY) {
            minY = v.y;
        } else if(v.y > maxY) {
            maxY = v.y;
        }
    }

    return {getPos() + Vec2d(minX, minY), getPos() + Vec2d(maxX, maxY)};
}

void Polygon::rotateVertices(float theta) {
    double cosTheta = cos(theta);
    double sinTheta = sin(theta);

    for(int i = 0; i < vertices.size(); i++) {
        Vec2d v = vertices[i];

        float x = v.x - centroid.x;
        float y = v.y - centroid.y;

        float rx = cosTheta * x + sinTheta * y;
        float ry = -sinTheta * x + cosTheta * y;

        vertices[i] = Vec2d(rx + centroid.x, ry + centroid.y);
    }
}

Collision Polygon::getCollision(Shape *s) const {
    return s->getPolygonCollision((Polygon*)this);
}

Collision Polygon::getCircleCollision(Circle* c) const {
    return c->getPolygonCollision((Polygon*) this);
}

Collision Polygon::getPolygonCollision(Polygon* s) const {
    Vec2d point;
    Polygon* container = nullptr;

    for(Vec2d v : vertices) {
        v = v + getPos();
        if(s->pointInside(v)) {
            point = v;
            container = s;
            break;
        }
    }

    if(!(point.x == 0 && point.y == 0)) {
        for(Vec2d v : s->vertices) {
            v = v + s->getPos();
            if(pointInside(v)) {
                container = (Polygon*) this; 
                point = v;
                break;
            }
        }
    }

    if(container == nullptr) {
        return Collision();
    }

    Vec2d closestEdgePoint;
    double leastDistSquare = -1;

    double intersectTangent;

    Vec2d v1 = container->getVertices()[0] + container->getPos();
    for(int i = 1; i <= container->getVertices().size(); i++) {
       Vec2d v2 = container->getVertices()[i%container->getVertices().size()] + container->getPos(); 

       double mLine = (v2.y - v1.y) / (v2.x - v1.x);
       double mPerp = (v1.x - v2.x) / (v2.y - v1.y);
    
       double xInt = (v1.y - point.y - mLine*v1.x + mPerp*point.x) / (mPerp - mLine);
       double yInt = mPerp * (xInt - point.x) + point.y;

       Vec2d intersect = Vec2d(xInt, yInt);
       double distSquare = (point - intersect).magSquared();

       if(distSquare < leastDistSquare || leastDistSquare == -1) {
           leastDistSquare = distSquare;
           closestEdgePoint = intersect;
            intersectTangent = mLine;
       }

       v1 = v2;
    }

    Vec2d overlap = closestEdgePoint - point;
    Vec2d tangent = Vec2d(1, intersectTangent).norm();
    return Collision(overlap, tangent, closestEdgePoint);
}
