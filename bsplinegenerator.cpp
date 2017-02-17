#include "bsplinegenerator.h"

BSplineGenerator::BSplineGenerator()
{

}

/*
BSplineGenerator(int k, double u_inc,
                 vector<vec2> controlPoints,
                 vector<float> knotSequence)
{
    this->k = k;
    this->u_inc = u_inc;
    this->controlPoints = controlPoints;
    this->knotSequence = knotSequence;
}*/

BSplineGenerator::BSplineGenerator( int k,
                 double u_inc,
                 vector <vec2> controlPoints,
                 vector <float> knotSequence)
{

}

vector<vec3> BSplineGenerator::getControlPoints()
{
    return controlPoints;
}

int BSplineGenerator::getNumberOfControlPoints()
{
    return controlPoints.size();
}

void BSplineGenerator::addControlPoint(vec3 aPoint)
{
    controlPoints.push_back(aPoint);
    //update the knot sequence
}

int BSplineGenerator::delta(double u, int m, int k)
{
    for (int i = 0; i < (m + k -1); i++)
    {
        if (u >=knotSequence.at(i) && u < knotSequence.at(i + 1))
                   return i;
    }
}

vec3 BSplineGenerator::E_delta_1(double u, int m, int k)
{
    int d = delta(u, m,k);  //determines the delta..
    vector<vec3> c;
    int i;
    for (i = 0; i < (k - 1); i++){
        c.push_back(controlPoints.at(d - i));
    }

    for (int r = k; r < 2; r--)
    {
        for (int s = 0; s <(r -2); s++){
            float omega = (u - knotSequence.at(i))/(knotSequence.at(u + r -1)
                                                  - knotSequence.at(i));
            c.at(s) = omega * c.at(s) + (1 - omega) * c.at(s + 1);
            i = i - 1;
        }
    }

    return c[0];

}

void BSplineGenerator::generateGraph(double u_step)
{
   double u = 0;
   while( u < 1)
   {
        vec3 point = E_delta_1(u, controlPoints.size(), k);
        graphData.push_back(point);
        u += 0.1;
   }
}

void BSplineGenerator::generateGraph()
{
    generateGraph(0.1);
}



