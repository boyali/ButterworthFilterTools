#include <iostream>
#include "ButterTool.h"
#include <cmath>
#include <vector>
#include <complex>
#include <gtest/gtest.h>

using namespace std;


TEST(FilterSpecTest, Orderis2) {

    ButterworthFilter bf;

    bf.setOrder(2);
    bf.setCuttoffFrequency(0.95107);

    Order_Cutoff NWn = bf.getOrderCutOff();
    EXPECT_EQ(2, NWn.N);

}

TEST(FilterSpecTest, WcTest) {

    ButterworthFilter bf;

    bf.setOrder(2);
    bf.setCuttoffFrequency(0.95107);

    Order_Cutoff NWn = bf.getOrderCutOff();
    EXPECT_EQ(0.95107, NWn.Wc);

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

    // Compute Continous Time TF
    bf.computeContinuousTimeTF();
    bf.PrintFilter_ContinuousTimeRoots();
    bf.PrintContinuousTimeTF();

    // Compute Discrete Time TF
    bf.computeDiscreteTimeTF();
    bf.PrintDiscreteTimeTF();

    // get An Bn

    DifferenceAnBn AnBn = bf.getAnBn();


    return RUN_ALL_TESTS();
}