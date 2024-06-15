
#ifndef VECTOR_2D_H
#define VECTOR_2D_H


#include <cmath>
#include <ostream>
#include <valarray>

struct Vec2d {

    double x;
    double y;

    Vec2d() : x(0), y(0) {}

    Vec2d(double x, double y) {
        this->x = x;
        this->y = y;
    }

    double mag() const {
        return sqrt(x*x + y*y);
    }

    double magSquared() const {
        // Return magnitude squared - needed in some calculations for efficiency purposes
        return x*x + y*y;
    }

    Vec2d norm() const{

        double m = mag();

        if(m == 0) {
            return Vec2d(0,0);
        }

        double mx = x/m;
        double my = y/m;

        return Vec2d(mx, my);
    }

    double dot(Vec2d b) const {
        return x*b.x + y*b.y;
    }

    // Returns the z component of the cross product
    //  - For 2D vectors, the x and y component is 0 when taking
    //    a cross product
    double crossZ(Vec2d b) const {
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

    Vec2d operator+(const Vec2d& a) const {
        return Vec2d(x + a.x, y + a.y);
    }

    Vec2d operator-(const Vec2d& b) const {
        return Vec2d(x - b.x, y - b.y);
    }

    friend Vec2d operator*(Vec2d a, double k) {
        return Vec2d(k*a.x, k*a.y);
    }

    friend Vec2d operator*(double k, Vec2d a) {
        return Vec2d(k*a.x, k*a.y);
    }

    void operator+=(const Vec2d& b) {
        x = x + b.x;
        y = y + b.y;
    }

    void operator-=(const Vec2d& b) {
        x = x - b.x;
        y = y - b.y;
    }
};

#endif // !VECTOR_2D_H
