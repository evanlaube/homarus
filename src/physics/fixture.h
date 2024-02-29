

#ifndef FIXTURE_H
#define FIXTURE_H

class Shape;
class Body;

class Fixture {

    public:
        Fixture();
        Fixture(Shape shape);
        Fixture(Shape *shape);

        float mass;

        bool attach(Body* b);

        void setShape(Shape *s);
        Shape* getShape() { return shape; }
        Body* getBody() { return body; }
        Shape* shape;

    private:
        Body* body;
        bool attached = false;

};

#endif // !FIXTURE_H
