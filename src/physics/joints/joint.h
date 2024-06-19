
#ifndef JOINT_H
#define JOINT_H

class World;
class Body;

/**
 * A Joint is any kind of physical connection between two bodies. Joints are added to
 * a world and get updated with each step.
 */
class Joint {
    public:
        /**
         * Updates the joint according to its type. Note that this does not take
         * in timestep information, as joints updates apply forces or position constraints.
         */
        virtual void update() = 0;

        /**
         * Calculates the stored potential energy in the joint. Joints do not have
         * mass so they do not store any kinetic energy.
         */
        virtual double getEnergy() const = 0;

        /**
         * Gets the next joint in the likned list of joints.
         */
        Joint* getNext() { return next; }
        
        /**
         * Gets the previous joint in the liked list of joints.
         */
        Joint* getLast() { return last; }
    protected:
        /** The first body the joint is connected to */
        Body* a;
        /** The second body the joint is connected to */
        Body* b;
    private:
        friend class World;
        Joint* next = nullptr;
        Joint* last = nullptr;
};

#endif // !JOINT_H
