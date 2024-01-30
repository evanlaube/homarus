

#ifndef FIXTURE_H
#define FIXTURE_H

class Shape;

class Fixture {

    public:
        Fixture();
        Fixture(Shape *shape);

        Shape *shape;
        float mass;
        // TODO: make this private by some method
        bool attached = false;

        void setShape(Shape *s);
        Shape* getShape() { return shape; }
    private:

};

#endif // !FIXTURE_H
