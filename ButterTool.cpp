//
// Created by ali on 17/07/19.
//
#include <iostream>
#include <cmath>
#include <complex>
#include <string>
#include <cstdio>

#include "ButterTool.h"




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
    int order = std::ceil(log(beta) / log(alpha));

    setOrder(order);

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
            //std::cout << roots[j] << std::endl;
            coefficients[j + 1] = coefficients[j + 1] - (roots[i] * coefficients[j]);
        }

    }

    return coefficients;


}


void ButterworthFilter::computePhaseAngles() {

    mPhaseAngles.resize(mOrder);
    int i = 1;

    for (auto &&x:mPhaseAngles) {
        x = M_PI_2 + (M_PI * (2.0 * i - 1.0) / (2.0 * mOrder));
        i++;
    }

}

void ButterworthFilter::computeContinuousTimeRoots() {

    //First compute  the phase angles of the roots
    computePhaseAngles();

    mContinuousTimeRoots.resize(mOrder);
    int i = 0;

    for (auto &&x:mContinuousTimeRoots) {
        x = {mCutoff_Frequency * cos(mPhaseAngles[i]), mCutoff_Frequency * sin(mPhaseAngles[i])};
        i++;
    }

}

void ButterworthFilter::computeContinuousTimeTF() {

    computeContinuousTimeRoots();
    mContinuousTimeDenominator.resize(mOrder + 1);

    mContinuousTimeDenominator = poly(mContinuousTimeRoots);
    mContinuousTimeNumerator = pow(mCutoff_Frequency, mOrder);

}

void ButterworthFilter::computeDiscreteTimeTF() {

    // Resizes the roots and zeros to the new order of discrete time then fills the values by Bilinear Transformation
    mDiscreteTimeZeros.resize(mOrder, {-1.0, 0.0}); // Butter puts zeros at -1.0 for causality
    mDiscreteTimeRoots.resize(mOrder, {0.0, 0.0});
    mAn.resize(mOrder, 0.0);
    mBn.resize(mOrder, 0.0);

    mDiscreteTimeGain = {mContinuousTimeNumerator, 0.0};

    // Bilinear Transformation of the Roots
    int i = 0;
    for (auto &&dr: mDiscreteTimeRoots) {
        dr = (1.0 + 2.0 * mContinuousTimeRoots[i] / 2.0) / (1.0 - Td * mContinuousTimeRoots[i] / 2.0);

        mDiscreteTimeGain = mDiscreteTimeGain / (1.0 - mContinuousTimeRoots[i]);
        i++;
    }

    mDiscreteTimeDenominator = poly(mDiscreteTimeRoots);

    // Obtain the coefficients of numerator and denominator
    i = 0;
    mDiscreteTimeNumerator = poly(mDiscreteTimeZeros);
    for (auto &&dn :mDiscreteTimeNumerator) {
        dn = dn * mDiscreteTimeGain;
        mBn[i] = dn.real();
        i++;
    }


    i = 0;
    for (auto &&dd :mDiscreteTimeDenominator) {
        mAn[i] = dd.real();
        i++;
    }



}

Order_Cutoff ButterworthFilter::getOrderCutOff() {

    Order_Cutoff NWc{mOrder, mCutoff_Frequency};

    return NWc;
}

DifferenceAnBn ButterworthFilter::getAnBn() {

//    DifferenceAnBn AnBn;
//    AnBn.An.resize(mAn.size(), 0.0);
//    AnBn.Bn.resize(mBn.size(), 0.0);
//
//    AnBn.An = mAn;
//    AnBn.Bn = mBn;

    DifferenceAnBn AnBn{mAn, mBn};


    return AnBn;
}

std::vector<double> ButterworthFilter::getAn() {

    return mAn;
}

std::vector<double> ButterworthFilter::getBn() {

    return mBn;
}

void ButterworthFilter::PrintFilter_Specs() {
    /*
     * Prints the order and cut-off angular frequency (rad/sec) of the filter
     *
     * */
    std::cout << "\nThe order of the filter : " << this->mOrder << std::endl;
    std::cout << "Cut-off Frequency : " << this->mCutoff_Frequency << " rad/sec\n" << std::endl;

}

void ButterworthFilter::PrintFilter_ContinuousTimeRoots() {
    /*
     * Prints the order and cut-off angular frequency (rad/sec) of the filter
     * */
    std::cout << "\n Roots of Continous Time Filter Transfer Function Denominator are : " << std::endl;

    for (auto &&x:mContinuousTimeRoots) {
        std::cout << std::real(x) << " + j " << std::imag(x) << std::endl;
    }
    std::cout<<"\n" ;

}

void ButterworthFilter::PrintContinuousTimeTF() {

    int n = mOrder;

    std::cout << "\nThe Continuous Time Transfer Function of the Filter is ;\n" << std::endl;
    printf("         %4.3f \n", mContinuousTimeNumerator);

    for (int i = 0; i <= n; i++) {
        std::cout << "--------";
    }

    std::cout << "--------\n";

    for (int i = n; i > 0; i--) {

        printf("%4.3f *", mContinuousTimeDenominator[n - i].real());
        printf("s[%d] + ", i);

    }

    printf("%4.3f \n", mContinuousTimeDenominator[n].real());
}

void ButterworthFilter::PrintDiscreteTimeTF() {

    int n = mOrder;

    std::cout << "\nThe Discrete Time Transfer Function of the Filter is ;\n" << std::endl;

    for (int i = n; i > 0; i--) {

        printf("%4.3f *", mDiscreteTimeNumerator[n - i].real());
        printf("z[-%d] + ", i);

    }
    printf("%4.3f", mDiscreteTimeNumerator[n].real());
    std::cout << "\n";

    for (int i = 0; i <= n; i++) {
        std::cout << "--------";
    }

    std::cout << "--------\n";

    for (int i = n; i > 0; i--) {

        printf("%4.3f *", mDiscreteTimeDenominator[n - i].real());
        printf("z[-%d] + ", i);

    }

    printf("%4.3f", mDiscreteTimeDenominator[n].real());
    std::cout << "\n" << std::endl;

}
