
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

Vec2d Shape::getPos() {
    return fixture->getBody()->getPos();
}

float Shape::getAngle() {
    return fixture->getBody()->getAngle();
}

Circle::Circle(float radius) {
    this->r = radius; 

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
    // For now just return false
    return false;
}

bool Circle::checkOverlap(Shape *s) const {
    // For now just return false
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

bool Polygon::pointInside(Vec2d point) const {
    // For now just return false
    return false;
}

bool Polygon::checkOverlap(Shape *s) const {
    // For now just return false
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
