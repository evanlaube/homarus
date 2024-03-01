
#include "../physics/fixture.h"
#include "../physics/body.h"
#include "shape.h"
#include "circle.h"
#include "polygon.h"
#include "../util/vec2.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>


Shape::Shape() {

}

void Shape::attach(Fixture* f) {
    fixture = f;
}

void Shape::calcArea() {
    return;
}

void Shape::calcCentroid() {
    return;
}

Vec2d Shape::getPos() const {
    return fixture->getBody()->getPos();
}

float Shape::getAngle() {
    return fixture->getBody()->getAngle();
}

Circle::Circle(float radius) {
    this->r = radius;
    maxRadius = radius;

    calcCentroid();
    calcArea();
}

void Circle::calcArea() {
    area = 3.14 * r * r;
}

void Circle::calcCentroid() {
    centroid = Vec2d(0,0);
}

bool Circle::pointInside(Vec2d point) const {
    Vec2d diff = getPos() - point;
    // Return true if x^2 + y^2 < r^2
    return pow(diff.x, 2) + pow(diff.y, 2) < pow(r, 2);

}

bool Circle::checkOverlap(Shape *s) const {
    Vec2d a = getPos();
    Vec2d b = s->getPos();

    Vec2d diff = b - a;
    float distSquare = diff.x * diff.x + diff.y * diff.y;
    float radSum = (maxRadius + s->getMaxRadius());

    if(distSquare > (radSum*radSum)) {
        return false;
    }

    return s->checkCircleOverlap((Circle*) this);
}

bool Circle::checkCircleOverlap(Circle *s) const {
    Vec2d a = getPos();
    Vec2d b = s->getPos();

    Vec2d diff = b - a;

    float distSquare = diff.x * diff.x + diff.y * diff.y;
    float radSum = (r + s->getRadius());

    return distSquare < (radSum*radSum);
}

bool Circle::checkPolygonOverlap(Polygon *s) const {
    if(s->pointInside(getPos())) {
        return true;
    }
    
    std::vector<Vec2d> verts = s->getVertices();
    for(Vec2d p : verts) {
        if(pointInside(s->getPos() + p)) {
            return true;
        }
    }

    Vec2d v1 = s->getPos() + verts[0];
    for(int i = 0; i <= verts.size(); i++) {
        Vec2d v2 = s->getPos() + verts[i%verts.size()];

        // TODO: Special conditions for vertical and zero slope
        float mLine = (v2.y - v1.y) / (v2.x - v1.x);
        float mPerp = (v2.x - v1.x) / (v1.y - v2.y);
        
        // x-value of intersection of edge and line perpendicular to edge that 
        // crosses through the center of the circle.
        float xInt = ((mLine * v1.x) - (mPerp * getPos().x) + getPos().y - v1.y) / (mLine - mPerp);
        float yInt = mLine * (xInt - v1.x) + v1.y;

        Vec2d intersect(xInt, yInt);
        //std::cout << "V1: " << v1 << " | v2: " << v2 << " | c: " << getPos() << " | r: "
        //    << r << " | mL: " << mLine << " | mPerp: " << mPerp << " | int: " << intersect<< std::endl;

        if(pointInside(intersect)) {
            return true;
        }

        v1 = v2;
    }

    return false;

}

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
        std::cout << this->vertices[i] << std::endl;
    }
    centroid = Vec2d(0,0);

    float maxDistSquare = 0;
    for(Vec2d v : vertices) {
        Vec2d diff = centroid - v;
        float distSquare = diff.x*diff.x + diff.y*diff.y;
        if(distSquare > maxDistSquare) {
            maxDistSquare = distSquare;
        }
    }
    maxRadius = maxDistSquare;

    std::cout << "Area: " << area << std::endl;
    std::cout << "Centroid: (" << centroid.x << ", " << centroid.y << ")" << std::endl;
}

void Polygon::calcArea() {
    float a = 0;

    for(int i = 0; i < vertices.size(); i++) {
        Vec2d vert = vertices[i];
        Vec2d next = vertices[(i+1) % (vertices.size())];

        a += (vert.x * next.y) - (next.x * vert.y);

        std::cout << vert << " -> " << next << std::endl;
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
    p = p - getPos();

    Vec2d v1 = vertices[0];

    int intersects = 0;
    
    for(int i = 0; i <= vertices.size(); i++) {
        Vec2d v2 = vertices[i%vertices.size()];
        

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
        if(pointInside(v)) {
            return true;
        }
    }

    for(Vec2d v : vertices) {
        if(s->pointInside(v)) {
            return true;
        }
    }

    return false;
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
