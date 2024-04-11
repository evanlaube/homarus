

#ifndef FIXTURE_H
#define FIXTURE_H

class Shape;
class Body;

class Fixture {

    public:
        Fixture();
        Fixture(Shape shape);
        Fixture(Shape *shape);

        bool attach(Body* b);

        void setShape(Shape *s);
        Shape* getShape() const { return shape; }
        Body* getBody() const { return body; }
        Shape* shape;

    private:
        Body* body;
        bool attached = false;

};

#endif // !FIXTURE_H
