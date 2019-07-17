#include <iostream>
#include "ButterTool.h"
#include <cmath>
#include <vector>
#include <complex>

using namespace std;

int main() {


    double Wp, Ws, Ap, As;


    Wp = 2.0;
    Ws = 3.0;
    Ap = 6.0;
    As = 20.0;


    ButterworthFilter bf;


    /*
     * After creating a filter object, you can explicitly set the order N and cutOff frequency to get the filter
     * transfer functions;  filter.setOrder(int N), filter.setCutoffFrequency(double Wc).
     *
     * Or can compute these quantities by calling the buttord method;  filter.Buttord(Wp, Ws, Ap, As)      *
     *
     * */


    // Setting filter order N and cut-off frequency explicitly (x-r1)(x-r2)

    bf.setOrder(2);
    bf.setCuttoffFrequency(2.0);
    bf.PrintFilter_Specs();

    // Computing filter coefficients from the specs
    bf.Buttord(Wp, Ws, Ap, As);
    bf.PrintFilter_Specs();


    // Polynomial test
    vector<complex<double>> roots{{-1, 2},
                                  {-1, -2}};


    std::vector<std::complex<double>> coeff = bf.poly(roots);

    return 0;
}