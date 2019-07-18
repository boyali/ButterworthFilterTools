//
// Created by ali on 17/07/19.
//

#pragma once

#include <vector>
#include <cmath>
#include <complex>

struct Order_Cutoff {
    int N;
    double Wc;
};

class ButterworthFilter {

public:

//    ButterworthFilter();

    //Prints the filter order and cutoff frequency
    void PrintFilter_Specs();
    void PrintFilter_ContinuousTimeRoots();

    void PrintContinuousTimeTF();

    void Buttord(double Wp, double Ws, double Ap, double As);

    // Setters and Getters
    void setCuttoffFrequency(double val);
    void setOrder(int N);


    // Get the order, cut-off frequency and other filter properties
    Order_Cutoff getOrderCutOff();


    // computes continous time transfer function
    void computeContinuousTimeTF();

private:

    // member variables
    /*
     * Frequencies are all in [rad/sec] angular frequincy and Magnitudes are in dB
     *
     * */

//    double mWp, mWs, mAp, mAs;   // WP; passband frequency [rad/sc], Ws: stopband frequeny, Ap. {As}: pass {stop}band ripple [dB],
    int mOrder = 0;                  // filter order
    double mCutoff_Frequency = 0.0;    // filter cut-off frequency [rad/sec]

    // Continuous time transfer function roots
    std::vector<double> mPhaseAngles{0.0};
    std::vector<std::complex<double >> mContinuousTimeRoots{{0.0, 0.0}};

    std::vector<std::complex<double>> mContinuousTimeDenominator{{0.0, 0.0}};
    double mContinuousTimeNumerator = 0.0;


    // METHODS
    // polynomial function returns the coefficients given the roots of a polynomial
    std::vector<std::complex<double>> poly(std::vector<std::complex<double >> &roots);


    /*
     * Implementation starts by computing the pole locations of the filter in the polar coordinate system .
     * The algorithm first locates the poles  computing the phase angle and then poles as a complex number
     * From the poles, the coefficients of denominator polynomial is calculated.
     *
     * Therefore, without phase, the roots cannot be calculated. The following three methods should be called
     * successively.
     *
     * */

    // computes the filter root locations in the polar coordinate system
    void computePhaseAngles();

    // Computes continuous time roots from the phase angles
    void computeContinuousTimeRoots();

};

