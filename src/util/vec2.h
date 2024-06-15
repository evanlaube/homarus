
#ifndef VECTOR_2D_H
#define VECTOR_2D_H


#include <cmath>
#include <ostream>

/**
 * The Vec2d struct is a 2d Vector struct that allows for all needed 2D Vector math to
 * be calculated easily.
 */
struct Vec2d {
    /** x-component of the vector */
    double x;
    /** y-component of the vector */
    double y;

    Vec2d() : x(0), y(0) {}

    Vec2d(double x, double y) {
        this->x = x;
        this->y = y;
    }

    /**
     * Calculate the magnitude of the vector.
     * @return Magnitude of vector.
     */
    double mag() const {
        return sqrt(x*x + y*y);
    }

    /**
     * Calculate squared magnitude of the vector, omitting the costly square root function.
     * This is useful for comparing magnitudes efficiently, as the square root is not needed to
     * determine which magnitude is greater.
     *
     * @return The squared magnitude of the vector.
     */
    double magSquared() const {
        return x*x + y*y;
    }

    /**
     * Calculates a vector in the same direction but with a magnitude of 1;
     *
     * @return Normalized vector.
     */
    Vec2d norm() const{
        double m = mag();

        if(m == 0) {
            return Vec2d(0,0);
        }

        double mx = x/m;
        double my = y/m;

        return Vec2d(mx, my);
    }

    /**
     * Caclculates the dot product between vector and other vector.
     *
     * @param b Other vector in dot product calculation
     * @return Dot product between self and b
     */
    double dot(Vec2d b) const {
        return x*b.x + y*b.y;
    }

    /**
     * Calculates the cross product of the vector and another, only returning the z-component,
     * as that is the only component with magnitude in a 2D cross product.
     *
     * @param b Other vector in cross product calculation
     * @return Z-component of cross product calculation.
     */
    double crossZ(Vec2d b) const {
        return (x * b.y) - (y * b.x);
    }

    /**
     * Sets the vector to the zero vector.
     */
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
