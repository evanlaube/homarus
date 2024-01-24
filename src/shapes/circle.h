
#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"
class Circle : public Shape {

    public:
        Circle(float radius);

        void calcArea() override;

    private:
        float r;


};

#endif // !CIRCLE_H
