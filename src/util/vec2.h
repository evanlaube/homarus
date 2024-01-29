
#ifndef VECTOR_2D_H
#define VECTOR_2D_H


#include <cmath>
#include <ostream>

struct Vec2d {

    double x;
    double y;

    Vec2d() {
        Vec2d(0, 0);
    }

    Vec2d(double x, double y) {
        this->x = x;
        this->y = y;
    }

    double mag() {
        return sqrt(x*x + y*y);
    }

    Vec2d norm() {

        double m = mag();

        double mx = x/m;
        double my = y/m;

        return Vec2d(mx, my);
    }

    double dot(Vec2d b) {
        return x*b.x + y*b.y;
    }

    // Returns the z component of the cross product
    //  - For 2D vectors, the x and y component is 0 when taking
    //    a cross product
    double crossZ(Vec2d b) {
        return (x * b.y) - (y * b.x);
    }

    void increase(Vec2d b) {
        x += b.x;
        y += b.y;
    }

    void mult(float s) {
        x *= s;
        y *= s;
    }

    void erase() {
        x = 0;
        y = 0;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vec2d& vec) {
        os << "(" << vec.x << ", " << vec.y << ")";
        return os;
    }

};

#endif // !VECTOR_2D_H
