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

    vector<vec3> getControlPoints();
    int getNumberOfControlPoints();

    void addControlPoint(vec3 aPoint);

    int delta(double u, int m, int k);

    vec3 E_delta_1(double u, int m, int k);

    void generateGraph(double u_step);
    void generateGraph();

    vector<vec3> graphData;
    int selected = -1;
private:
    int k; //order of the curve
    double u_inc; //User control over incrementation

    vector<vec3> controlPoints;
    vector<float> knotSequence;






};

#endif // BSPLINEGENERATOR_H
