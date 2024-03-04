
#include "../physics/fixture.h"
#include "shape.h"
#include "circle.h"
#include "polygon.h"
#include "../util/vec2.h"
#include <cmath>
#include <iostream>
#include <vector>

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
    for(int i = 1; i <= verts.size(); i++) {
        Vec2d v2 = s->getPos() + verts[i%verts.size()];

        // TODO: Special conditions for vertical and zero slope
        float mLine = (v2.y - v1.y) / (v2.x - v1.x);
        float mPerp = (v2.x - v1.x) / (v1.y - v2.y);
        
        // x-value of intersection of edge and line perpendicular to edge that 
        // crosses through the center of the circle.
        float xInt = ((mLine * v1.x) - (mPerp * getPos().x) + getPos().y - v1.y) / (mLine - mPerp);
        float yInt = mLine * (xInt - v1.x) + v1.y;
        Vec2d intersect(xInt, yInt);

        //std::cout << "V1: " << v1 << " | v2: " << v2 
        //    << " | c: " << getPos() << " | r: "
        //    << r << " | mL: " << mLine << " | mPerp: " << mPerp 
        //    << " | int: " << intersect<< std::endl;

        if(pointInside(intersect) && s->pointInside(intersect)) {
            return true;
        }

        v1 = v2;
    }
    
    return false;
}

Vec2d Circle::getOverlap(Shape* s) const {
    return s->getCircleOverlap((Circle*) this);
}

Vec2d Circle::getCircleOverlap(Circle* s) const {
    Vec2d a = getPos();
    Vec2d b = s->getPos();

    Vec2d diff = a - b;
    float dist = diff.mag();
    Vec2d diffNorm = diff *  (1/dist);

    float overlap = (r + s->r) - dist;
    return diffNorm * overlap;
}

Vec2d Circle::getPolygonOverlap(Polygon* s) const {
    std::vector<Vec2d> verts = s->getVertices();

    for(Vec2d v : verts) { 
        v = v + s->getPos();

        if(pointInside(v)) {
            Vec2d diff = getPos() - v;
            double dist = diff.mag();

            double overlapAmount = r-dist;
            return diff.norm() * overlapAmount;
        }
    }

    // TODO: Overlap calculation using edges still is not entirely accurate,
    // but it works well enough to prevent bodies with restitution from 
    // phasing through eachother.
    Vec2d v1 = s->getPos() + verts[0];
    for(int i = 1; i <= verts.size(); i++) {
        Vec2d v2 = s->getPos() + verts[i%verts.size()];
        
        // TODO: Special conditions for vertical and zero slope
        double mLine = (v2.y - v1.y) / (v2.x - v1.x);
        double mPerp = (v2.x - v1.x) / (v1.y - v2.y);
        
        // x-value of intersection of edge and line perpendicular to edge that 
        // crosses through the center of the circle.
        double xInt = ((mLine * v1.x) - (mPerp * getPos().x) + getPos().y - v1.y) / (mLine - mPerp);
        double yInt = mLine * (xInt - v1.x) + v1.y;
        Vec2d intersect(xInt, yInt);
        //std::cout << "Intersect at: " << intersect << std::endl;
       
        if(pointInside(intersect)) {
            double a = 1 + mPerp * mPerp;
            double b = -2*getPos().x - 2 * mPerp * mPerp * getPos().x;
            double c = getPos().x * getPos().x + mPerp * mPerp * getPos().x * getPos().x - r*r;
           
            double x1 = (-b + sqrt(b*b - 4*a*c) ) / (2*a);
            double y1 = mPerp * (x1 - getPos().x) + getPos().y;
            Vec2d overlapPoint;

            if(s->pointInside(Vec2d(x1, y1))) {
                overlapPoint = Vec2d(x1, y1);
                //std::cout << "I = " << intersect << "\nO_1 = " << Vec2d(x1,y1) <<  "\nA = "
                //    << v1 << "\nB = " << v2 << "\nC = " << getPos() 
                //    << "\na = " << a << "\nb = " << b << "\nc = " << c << std::endl;
                return (intersect - overlapPoint);
            } else {
                double x2 = (-b - sqrt(b*b - 4*a*c) ) / (2*a);
                double y2 = mPerp * (x2 - getPos().x) + getPos().y;

                //std::cout << "I = " << intersect << "\nO1 = " << Vec2d(x1,y1) << "\nO2 = " << Vec2d(x2,y2) << "\nA = "
                //    << v1 << "\nB = " << v2 << "\nC = " << getPos() << std::endl;

                if(s->pointInside(Vec2d(x2, y2))) {
                    overlapPoint = Vec2d(x2, y2);
                    return (overlapPoint - intersect);
                }
            }
        }


        v1 = v2;
    }

    return Vec2d(0,0);
}
