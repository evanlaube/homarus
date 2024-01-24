
#include "shape.h"
#include "circle.h"
#include "polygon.h"
#include "../util/vec2.h"
#include <cstdlib>
#include <iostream>
#include <vector>


Shape::Shape() {

}

void Shape::calcProperties() {
    calcArea();
    calcCentroid();
}

void Shape::calcArea() {
    return;
}

void Shape::calcCentroid() {
    return;
}

Circle::Circle(float radius) {
   this->r = radius; 

   calcProperties();
}

void Circle::calcArea() {
    area = 3.14 * r * r;
}


Polygon::Polygon(std::vector<Vec2d> vertices) {
    // TODO: Order verticies in counter clockwise order rather than expecting user to
    this->vertices = vertices;

    calcProperties();

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

    // NOTE: Area can be negative, this is due to the possibility that the 
    //       points are ordered in clockwise order
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
