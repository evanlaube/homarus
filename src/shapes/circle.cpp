
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
    //if(s->pointInside(getPos())) {
    //    return true;
    //}
    
    std::vector<Vec2d> verts = s->getVertices();
    /*for(Vec2d p : verts) {
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
        //std::cout << "V1: " << v1 << " | v2: " << v2 
        //    << " | c: " << getPos() << " | r: "
        //    << r << " | mL: " << mLine << " | mPerp: " << mPerp 
        //    << " | int: " << intersect<< std::endl;

        if(pointInside(intersect)) {
            return true;
        }

        v1 = v2;
    }
    */
    return false;

}

