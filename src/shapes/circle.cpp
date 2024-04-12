
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
    return pow(diff.x, 2) + pow(diff.y, 2) <= pow(r, 2);

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

std::pair<Vec2d, Vec2d> Circle::getBoundingBox() const {
    Vec2d pos = getPos();

    Vec2d min = pos - Vec2d(r, r); 
    Vec2d max = pos + Vec2d(r, r);

    return {min, max};
}

Collision Circle::getCollision(Shape* s) const {
    return s->getCircleCollision((Circle*) this);
}

Collision Circle::getCircleCollision(Circle* s) const {
    Vec2d a = getPos();
    Vec2d b = s->getPos();

    Vec2d diff = b - a;
    double dist = diff.mag();

    if(dist > (r+s->getRadius())) {
        return Collision();
    }

    Vec2d diffNorm = diff * (1/dist);

    double overlapAmount = (r + s->r) - dist;
    Vec2d overlap = diffNorm * overlapAmount;

    double mTangent = (a.x - b.x) / (b.y - a.y);

    Vec2d tangent;
    
    if(b.y == a.y) {
        tangent = Vec2d(0, 1);
    } else {
        tangent = Vec2d(1, mTangent).norm();
    }

    Vec2d intersection = b + (diffNorm * s->r); 

    return Collision(overlap, tangent, intersection);
}

Collision Circle::getPolygonCollision(Polygon* s) const {
    std::vector<Vec2d> verts = s->getVertices();

    Vec2d overlap;
    Vec2d tangent;
    Vec2d intersect;

    for(Vec2d v : verts) { 
        v += s->getPos();

        if(pointInside(v)) {
            Vec2d diff = v - getPos();
            double dist = diff.mag();

            double overlapAmount = r-dist;
            overlap = diff.norm() * overlapAmount;

            v += overlap;

            double mTang = (v.x - getPos().x) / (getPos().y - v.y);
            tangent = Vec2d(1, mTang).norm(); 

            intersect = v;
            
            //std::cout << 
            //    "C = " << getPos() << std::endl <<
            //    "V = " << v << std::endl <<
            //    "d = " << getPos() - v << std::endl <<
            //    "m_T = " << mTang << std::endl <<
            //    "t = " << tangent << std::endl << 
            //    "R = " << r << std::endl;

            return Collision(overlap, tangent, intersect);
        }
    }

    Vec2d v1 = s->getPos() + verts[0];
    for(int i = 1; i <= verts.size(); i++) {
        Vec2d v2 = s->getPos() + verts[i%verts.size()];
        
        double mLine = (v2.y - v1.y) / (v2.x - v1.x);
        double mPerp = (v2.x - v1.x) / (v1.y - v2.y);

        if(v1.x == v2.x) {
            double xInt = v1.x; // because v1.x == v2.x
            double yInt = getPos().y; 
            intersect = Vec2d(xInt, yInt);
            
            if(yInt > std::max(v1.y, v2.y) || yInt < std::min(v2.y, v1.y)) {
                v1 = v2;
                continue;
            }


            if(pointInside(intersect)) {
                double x = getPos().x - r;
                double y = getPos().y;
                Vec2d overlapPoint = Vec2d(x, y);
            

                if(s->pointInside(overlapPoint)) {
                    overlap = (overlapPoint - intersect);
                } else {
                    x = getPos().x + r;
                    overlapPoint.x = x;
                    overlap = (overlapPoint - intersect);
                }

                tangent = Vec2d(0, 1);
                return Collision(overlap, tangent, intersect);
            }

        } else if (v1.y == v2.y) {
            double yInt = v1.y; // Because v1.y == v2.y
            double xInt = getPos().x; 

            intersect = Vec2d(xInt, yInt);

            if(xInt > std::max(v1.x, v2.x) || yInt < std::min(v2.x, v1.x)) {
        v1 = v2;
                continue;
            } else {
            }

            if(pointInside(intersect)) {
                double x = getPos().x;
                double y = getPos().y - r;
                Vec2d overlapPoint = Vec2d(x, y);

                if(s->pointInside(overlapPoint)) {
                    overlap = (overlapPoint - intersect);
                } else {
                    y = getPos().y + r;
                    overlapPoint.y = y;
                    overlap = (overlapPoint - intersect);
                }

                tangent = Vec2d(1, 0);
                return Collision(overlap, tangent, intersect);
            }

        } else {
            // x-value of intersection of edge and line perpendicular to edge that 
            // crosses through the center of the circle.
            double xInt = ((mLine * v1.x) - (mPerp * getPos().x) + getPos().y - v1.y) / (mLine - mPerp);
            double yInt = mLine * (xInt - v1.x) + v1.y;
            intersect = Vec2d(xInt, yInt);

            if(pointInside(intersect)) {
                double a = 1 + mPerp * mPerp;
                double b = -2*getPos().x - 2 * mPerp * mPerp * getPos().x;
                double c = getPos().x * getPos().x + mPerp * mPerp * getPos().x * getPos().x - r*r;

                double x1 = (-b + sqrt(b*b - 4*a*c) ) / (2*a);
                double y1 = mPerp * (x1 - getPos().x) + getPos().y;
                Vec2d overlapPoint;

                if(s->pointInside(Vec2d(x1, y1))) {
                    overlapPoint = Vec2d(x1, y1);
                    overlap = (overlapPoint - intersect);
                    if(overlap.x == 0 && overlap.y == 0)
                        return Collision();
                    tangent = Vec2d(1, mLine).norm();
                    return Collision(overlap, tangent, intersect);
                } else {
                    double x2 = (-b - sqrt(b*b - 4*a*c)) / (2*a);
                    double y2 = mPerp * (x2 - getPos().x) + getPos().y;
                    if(s->pointInside(Vec2d(x2, y2))) {
                        overlapPoint = Vec2d(x2, y2);
                        overlap = (overlapPoint - intersect);
                        if(overlap.x == 0 && overlap.y == 0)
                            return Collision();
                        tangent = Vec2d(1, mLine).norm();
                        return Collision(overlap, tangent, intersect);
                    }
                }

            }
        }
        
        //std::cout << "Intersect at: " << intersect << std::endl;
        //


        v1 = v2;
    }
    return Collision();
}

void Circle::calcMoment() {
    moment = (0.5) * getMass() * r * r;
}
