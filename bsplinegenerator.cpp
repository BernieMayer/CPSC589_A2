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

void BSplineGenerator::addControlPoint(vector<vec2> aPoint)
{
    controlPoints.push_back(aPoint);
}

int BSplineGenerator::delta(double u, int m, int k)
{
    for (int i = 0; i < (m + k -1); i++)
    {
        if (u >=knotSequence.at(i) u < knotSequence.at(i + 1))
                   return i;
    }
}

vec2 BSplineGenerator::E_delta_1(double u, int m, int k)
{
    int d = delta(u, m,k);  //determines the delta..
    vector<vec2> c;
    int i
    for (i = 0; i < k - 1; i++)
        c.push_back(controlPoints.at(d - i));


    for (int r = k; r < 2; r--)
    {
        for (int s = 0; s <(r -2); s++){
            double omega = (u - controlPoints.at(i))/(controlPoints.at(U + r -1)
                                                  - controlPoints.at(i));
            c.at(s) = omega * c.at(s) + (1 - omega) * c.at(s +1);
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
        vec2 point = E_delta_1(u, controlPoints.size(), k);
        graphData.push_back(point);
        u += 0.1;
   }
}

void BSplineGenerator::generateGraph()
{
    generateGraph(0.1);
}



