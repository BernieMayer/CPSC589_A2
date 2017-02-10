#ifndef BSPLINEGENERATOR_H
#define BSPLINEGENERATOR_H


#include <glm/glm.hpp>

using namespace glm;
using namespace std;

class BSplineGenerator
{
public:
    BSplineGenerator();

    BSplineGenerator( int k, double u_inc, vector<vec2> controlPoints
                     vector<float> knotSequence);


    vector<vec3> graphData;
private:
    int k; //order of the curve
    double u_inc; //User control over incrementation

    vector<vec2> controlPoints;
    vector<float> knotSequence;






};

#endif // BSPLINEGENERATOR_H
