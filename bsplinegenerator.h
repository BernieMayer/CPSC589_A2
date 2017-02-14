#ifndef BSPLINEGENERATOR_H
#define BSPLINEGENERATOR_H


#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

class BSplineGenerator
{
public:
    BSplineGenerator();
    BSplineGenerator( int k,
                     double u_inc,
                     vector <vec2> controlPoints,
                     vector <float> knotSequence);

    void addControlPoint(vector<vec2> aPoint);

    int delta(double u, int m, int k);

    vec2 E_delta_1(double u, int m, int k);

    void generateGraph(double u_step);
    void generateGraph();

    vector<vec3> graphData;
private:
    int k; //order of the curve
    double u_inc; //User control over incrementation

    vector<vec2> controlPoints;
    vector<float> knotSequence;






};

#endif // BSPLINEGENERATOR_H
