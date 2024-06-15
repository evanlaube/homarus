

#ifndef FIXTURE_H
#define FIXTURE_H

class Shape;
class Body;

/** 
 * The Fixture class serves as an attachment between bodies and shapes. This allows
 * bodies to easily be attached to a shape.
 *
 * PLANNED FEATURE: The fixture will allow bodies to be attached to more than one shape, allowing
 * for complex shapes to be easily calculated.
 */
class Fixture {

    public:
        Fixture();
        Fixture(Shape shape);
        Fixture(Shape *shape);

        /**
         * Sets the shape of the fixture, only if it is not already set.
         * @param s The pointer to the shape to set to
         * @return True on success, false if fixture already has a shape set.
         */
        bool setShape(Shape *s);

        /**
         * Attaches the fixture to a body, giving the body the fixtures shape. Cannot
         * be called if fixture has already been attached to a body.
         * @param b The body to attach to
         * @return False if fixture is already attached, and true if attachment is successful
         */
        bool attach(Body* b);

        Shape* getShape() const { return shape; }
        Body* getBody() const { return body; }

    private:
        /** Pointer to the shape of the fixture */
        Shape *shape;
        /** Pointer to the body the fixture is attached to */
        Body* body;
        /** Stores whether or not the fixture has been attached to a body */
        bool attached = false;

};

#endif // !FIXTURE_H
