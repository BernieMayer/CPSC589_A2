#include "bsplinegenerator.h"

BSplineGenerator::BSplineGenerator()
{

}

BSplineGenerator::BSplineGenerator( int k, double u_inc, vector<vec2> controlPoints
                                    vector<float> knotSequence)
{
    this->k = k;
    this->u_inc = u_inc;
    this->controlPoints = controlPoints;
    this->knotSequence = knotSequence;
}
