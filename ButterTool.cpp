//
// Created by ali on 17/07/19.
//
#include <iostream>
#include <cmath>
#include <complex>

#include "ButterTool.h"


ButterworthFilter::ButterworthFilter() {
    mOrder = 0;
    mCutoff_Frequency = 0.0;

}


void ButterworthFilter::Buttord(double Wp, double Ws, double Ap, double As) {
    /*
     *  Inputs are double mWp, mWs, mAp, mAs;
     *  Wp; passband frequency [rad/sc],
     *  Ws: stopband frequeny [rad/sc],
     *  Ap. {As}: pass {stop} band ripple [dB]
     *
     * */


    double alpha, beta;

    alpha = Ws / Wp;
    beta = sqrt((pow(10, As / 10.0) - 1.0) / (pow(10, Ap / 10.0) - 1.0));


    setOrder(std::ceil(log(beta) / log(alpha)));

    // right limit, left limit
    /*
     * The left and right limits of the magnitutes satisfy the specs at the frequencies Ws and Wp
     * Scipy.buttord gives left limit as the cut-off frequency whereas Matlab gives right limit
     *
     * */
    double right_lim = Ws * (pow((pow(10.0, As / 10.0) - 1.0), -1.0 / (2 * mOrder)));
    double left_lim = Wp * (pow((pow(10.0, Ap / 10.0) - 1.0), -1.0 / (2 * mOrder)));

    // mCutoff_Frequency = left_lim;
    setCuttoffFrequency(right_lim);


}

void ButterworthFilter::setOrder(int N) {
    mOrder = N;
}

void ButterworthFilter::setCuttoffFrequency(double val) {
    mCutoff_Frequency = val;
}


std::vector<std::complex<double>> ButterworthFilter::poly(std::vector<std::complex<double >> &rts) {


    std::vector<std::complex<double >> roots = rts;
    std::vector<std::complex<double>> coefficients(roots.size() + 1, {0, 0});

    int n = roots.size();

    coefficients[0] = {1.0, 0.0};


    for (int i = 0; i < n; i++) {

        for (int j = i; j != -1; j--) {
            std::cout << roots[j] << std::endl;
            coefficients[j + 1] = coefficients[j + 1] - (roots[i] * coefficients[j]);
        }

    }

    for (int i = 0; i < coefficients.size(); i++) {
        std::cout << coefficients[i] << " ";
    }

    return coefficients;


}

void ButterworthFilter::PrintFilter_Specs() {
    /*
     * Prints the order and cut-off angular frequency (rad/sec) of the filter
     * */
    std::cout << "\nThe order of the filter : " << this->mOrder << std::endl;
    std::cout << "Cut-off Frequency : " << this->mCutoff_Frequency << " rad/sec\n" << std::endl;
}