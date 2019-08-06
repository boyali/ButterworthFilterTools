#include <iostream>
#include "ButterTool.h"
#include <cmath>
#include <vector>
#include <complex>
#include <gtest/gtest.h>

class TestSuite : public ::testing::Test
{
public:
    TestSuite()
    {}

    ~TestSuite()
    {}
};

TEST_F(TestSuite, OrderTest)
{

    double Wp, Ws, Ap, As;

    Wp = 2.0; // passband frequency [rad/sec]
    Ws = 3.0; // stopband frequency [rad/sec]
    Ap = 6.0; // passband ripple mag or loss [dB]
    As = 20.0; // stop band rippe attenuation [dB]

    ButterworthFilter bf;

    bf.Buttord(Wp, Ws, Ap, As);
    Order_Cutoff NWn = bf.getOrderCutOff();

    ASSERT_EQ(5, NWn.N);
    ASSERT_NEAR(1.89478, NWn.Wc, 0.1);

    // test transfer functions
    bf.computeContinuousTimeTF();
    bf.computeDiscreteTimeTF();

    std::vector<double> An = bf.getAn();
    std::vector<double> Bn = bf.getBn();

    /*
     * Bd = [0.1913    0.9564    1.9128    1.9128    0.9564    0.1913]
     * Ad = [1.0000    1.8849    1.8881    1.0137    0.2976    0.0365]
     *
     */

    ASSERT_NEAR(1.8849, An[1], 0.01);
    ASSERT_NEAR(1.8881, An[2], 0.01);
    ASSERT_NEAR(1.0137, An[3], 0.01);
    ASSERT_NEAR(0.29762, An[4], 0.01);
    ASSERT_NEAR(0.0365, An[5], 0.01);

    ASSERT_NEAR(0.9564, Bn[1], 0.01);
    ASSERT_NEAR(1.9128, Bn[2], 0.01);
    ASSERT_NEAR(1.9128, Bn[3], 0.01);
    ASSERT_NEAR(0.9564, Bn[4], 0.01);
    ASSERT_NEAR(0.1913, Bn[5], 0.01);

    // Test with defined sampling frequency
    bool use_sampling_frequency = true;
    bf.setOrder(2);
    bf.setCuttoffFrequency(10, 100);
    bf.computeContinuousTimeTF(use_sampling_frequency);
    bf.computeDiscreteTimeTF(use_sampling_frequency);

    An = bf.getAn();
    Bn = bf.getBn();

    ASSERT_NEAR(1.0, An[0], 0.01);
    ASSERT_NEAR(-1.14298, An[1], 0.01);
    ASSERT_NEAR(0.4128, An[2], 0.01);

    ASSERT_NEAR(0.067455, Bn[0], 0.01);
    ASSERT_NEAR(0.134911, Bn[1], 0.01);
    ASSERT_NEAR(0.067455, Bn[2], 0.01);

}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}