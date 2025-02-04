
#ifndef SPRING_H
#define SPRING_H

#include "joint.h"

class Spring : public Joint {
    public:
        Spring(Body* a, Body*b, double constant);
        Spring(Spring* spring);

        void update() override;

        double getEnergy() const override;

        void setConstant(double constant);
        double getConstant() const { return k; }

        void setEquilibriumLength(double length);
        double getEquilibriumLength() const { return equilibriumLength; }

        void setDampingRatio(double ratio) {dampingRatio = ratio; }
        double getDampingRatio() const { return dampingRatio; }
    private:
        /** The spring constant for the spring (N/m) */
        double k = 20;
        /** The equilibrium length of the spring, where 0 force is being applied */
        double equilibriumLength = 10;
        /** The current calculated displacement from equilibrium of the spring. 
         * This value is calculated on every update.
         */
        double displacement = 0;
        
        double dampingRatio = 1;
};

#endif // SPRING_H
