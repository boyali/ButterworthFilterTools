#include <iostream>
#include "ButterTool.h"
#include <cmath>
#include <vector>
#include <complex>
#include <gtest/gtest.h>

using namespace std;


TEST(FilterSpecTest, OrderTest) {


    double Wp, Ws, Ap, As;


    Wp = 2.0; // passband frequency [rad/sec]
    Ws = 3.0; // stopband frequency [rad/sec]
    Ap = 6.0; // passband ripple mag or loss [dB]
    As = 20.0; // stop band rippe attenuation [dB]

    ButterworthFilter bf;
    bf.Buttord(Wp, Ws, Ap, As);

    Order_Cutoff NWn = bf.getOrderCutOff();

    ASSERT_EQ(5, NWn.N);

}

TEST(FilterSpecTest, Cutoff_Test) {


    double Wp, Ws, Ap, As;


    Wp = 2.0; // passband frequency [rad/sec]
    Ws = 3.0; // stopband frequency [rad/sec]
    Ap = 6.0; // passband ripple mag or loss [dB]
    As = 20.0; // stop band rippe attenuation [dB]

    ButterworthFilter bf;
    bf.Buttord(Wp, Ws, Ap, As);

    Order_Cutoff NWn = bf.getOrderCutOff();

    ASSERT_NEAR(1.89478, NWn.Wc, 0.1);

}

TEST(TransferFunction, Discrete_TFTest) {


    double Wp, Ws, Ap, As;


    Wp = 2.0; // passband frequency [rad/sec]
    Ws = 3.0; // stopband frequency [rad/sec]
    Ap = 6.0; // passband ripple mag or loss [dB]
    As = 20.0; // stop band rippe attenuation [dB]

    ButterworthFilter bf;
    bf.Buttord(Wp, Ws, Ap, As);

    bf.computeContinuousTimeTF();
    bf.computeDiscreteTimeTF();

//    DifferenceAnBn AnBn = bf.getAnBn();

    std::vector<double> An = bf.getAn();
    std::vector<double> Bn = bf.getBn();

    ASSERT_NEAR(0.29762, An[4], 0.01);
    EXPECT_NEAR(0.9564, Bn[1], 0.01);


}


int main() {
    testing::InitGoogleTest();

    double Wp, Ws, Ap, As;


    Wp = 2.0; // passband frequency [rad/sec]
    Ws = 3.0; // stopband frequency [rad/sec]
    Ap = 6.0; // passband ripple mag or loss [dB]
    As = 20.0; // stop band rippe attenuation [dB]


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

    // Get the computed order and Cut-off frequency

    Order_Cutoff NWc = bf.getOrderCutOff();

    cout << " The computed order is ;" << NWc.N << endl;
    cout << " The computed cut-off frequency is ;" << NWc.Wc << endl;

    bf.PrintFilter_Specs();

    // TEST
    // Setting filter order N and cut-off frequency explicitly (x-r1)(x-r2)
//    bf.setOrder(2);
//    bf.setCuttoffFrequency(10.0);

    // Compute Continous Time TF
    bf.computeContinuousTimeTF();
    bf.PrintFilter_ContinuousTimeRoots();
    bf.PrintContinuousTimeTF();

    // Compute Discrete Time TF
    bf.computeDiscreteTimeTF();
    bf.PrintDiscreteTimeTF();

    // get An Bn

    DifferenceAnBn AnBn = bf.getAnBn();
    int a = 1;


    return RUN_ALL_TESTS();
}