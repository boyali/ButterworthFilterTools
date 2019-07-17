//
// Created by ali on 17/07/19.
//

#pragma once

#include <vector>
#include <cmath>
#include <complex>

class ButterworthFilter {

public:

    ButterworthFilter();

    //Prints the filter order and cutoff frequency
    void PrintFilter_Specs();

    void Buttord(double Wp, double Ws, double Ap, double As);

    void setCuttoffFrequency(double val);

    void setOrder(int N);

    // polynomial function returns the coefficients given the roots of a polynomial
    std::vector<std::complex<double>> poly(std::vector<std::complex<double >> &roots);

    //


private:

    // member variables
    /*
     * Frequencies are all in [rad/sec] angular frequincy and Magnitudes are in dB
     *
     * */

//    double mWp, mWs, mAp, mAs;   // WP; passband frequency [rad/sc], Ws: stopband frequeny, Ap. {As}: pass {stop}band ripple [dB],
    int mOrder;                  // filter order
    double mCutoff_Frequency;    // filter cut-off frequency [rad/sec]


};

