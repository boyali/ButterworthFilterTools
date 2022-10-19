
#include "ButterTool.h"
#include <cmath>
#include <vector>
#include <complex>
#include <gtest/gtest.h>

// The fixture for testing class Foo.
class ButterWorthTestFixture : public ::testing::Test
{
protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  ButterWorthTestFixture()
  {
    // You can do set-up work for each test here.
  }

  ~ButterWorthTestFixture() override
  {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override
  {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  void TearDown() override
  {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
};

TEST_F(ButterWorthTestFixture, OrderTest)
{

  double tol = 1e-4;

  // 1st Method
  double Wp{2.};   // pass-band frequency [rad/sec]
  double Ws{3.};   // stop-band frequency [rad/sec]
  double Ap{6.};   // pass-band ripple mag or loss [dB]
  double As{20.};  // stop band ripple attenuation [dB]

  ButterworthFilter bf;
  bf.Buttord(Wp, Ws, Ap, As);

  auto NWc = bf.getOrderCutOff();
  print("The computed order and frequency for the give specification : ");
  print("Minimum order N = ", NWc.N, ", and The cut-off frequency Wc = ", NWc.Wc, "rad/sec \n");
  bf.printFilterSpecs();

  /**
   * Approximate the continuous and discrete time transfer functions.
   * */
  bf.computeContinuousTimeTF();

  // Print continuous time roots.
  bf.printFilterContinuousTimeRoots();
  bf.printContinuousTimeTF();

  // Compute the discrete time transfer function.
  bf.computeDiscreteTimeTF();
  bf.printDiscreteTimeTF();

  ASSERT_EQ(5, NWc.N);
  ASSERT_NEAR(1.89478, NWc.Wc, tol);

  // test transfer functions
  bf.computeContinuousTimeTF();
  bf.computeDiscreteTimeTF();

  std::vector<double> An = bf.getAn();
  std::vector<double> Bn = bf.getBn();

  /**
   * Bd = [0.1913    0.9564    1.9128    1.9128    0.9564    0.1913]
   * Ad = [1.0000    1.8849    1.8881    1.0137    0.2976    0.0365]
   */

  ASSERT_NEAR(1.8849, An[1], tol);
  ASSERT_NEAR(1.8881, An[2], tol);
  ASSERT_NEAR(1.0137, An[3], tol);
  ASSERT_NEAR(0.29762, An[4], tol);
  ASSERT_NEAR(0.0365, An[5], tol);

  ASSERT_NEAR(0.9564, Bn[1], tol);
  ASSERT_NEAR(1.9128, Bn[2], tol);
  ASSERT_NEAR(1.9128, Bn[3], tol);
  ASSERT_NEAR(0.9564, Bn[4], tol);
  ASSERT_NEAR(0.1913, Bn[5], tol);
}


TEST_F(ButterWorthTestFixture, butterDefinedSamplingOrder1)
{
  ButterworthFilter bf;
  double tol{1e-12};

  // Test with defined sampling frequency
  int order{1};
  double cut_off_frq_hz{5.};
  double sampling_frq_hz{40.};
  bool use_sampling_frequency = true;

  // Prepare the filter
  bf.setOrder(order);
  bf.setCuttoffFrequency(cut_off_frq_hz, sampling_frq_hz);
  bf.computeContinuousTimeTF(use_sampling_frequency);
  bf.computeDiscreteTimeTF(use_sampling_frequency);

  auto An = bf.getAn();
  auto Bn = bf.getBn();

  std::vector<double> An_gtruth{1., -0.414213562373095};
  std::vector<double> Bn_gtruth{0.292893218813452, 0.292893218813452};

  for (size_t k = 0; k < An.size(); ++k) {
    ASSERT_NEAR(An[k], An_gtruth[k], tol);
    ASSERT_NEAR(Bn[k], Bn_gtruth[k], tol);
  }
}

TEST_F(ButterWorthTestFixture, butterDefinedSamplingOrder2)
{
  ButterworthFilter bf;
  double tol{1e-12};

  // Test with defined sampling frequency
  int order{2};
  double cut_off_frq_hz{10.};
  double sampling_frq_hz{100};
  bool use_sampling_frequency = true;

  // Prepare the filter
  bf.setOrder(order);
  bf.setCuttoffFrequency(cut_off_frq_hz, sampling_frq_hz);
  bf.computeContinuousTimeTF(use_sampling_frequency);
  bf.computeDiscreteTimeTF(use_sampling_frequency);

  auto An = bf.getAn();
  auto Bn = bf.getBn();

  std::vector<double> An_gtruth{1., -1.142980502539901, 0.412801598096189};
  std::vector<double> Bn_gtruth{0.067455273889072, 0.134910547778144, 0.067455273889072};

  for (size_t k = 0; k < An.size(); ++k) {
    ASSERT_NEAR(An[k], An_gtruth[k], tol);
    ASSERT_NEAR(Bn[k], Bn_gtruth[k], tol);
  }
}

TEST_F(ButterWorthTestFixture, butterDefinedSamplingOrder3)
{
  ButterworthFilter bf;
  double tol{1e-12};

  // Test with defined sampling frequency
  int order{3};
  double cut_off_frq_hz{10.};
  double sampling_frq_hz{100};
  bool use_sampling_frequency = true;

  // Prepare the filter
  bf.setOrder(order);
  bf.setCuttoffFrequency(cut_off_frq_hz, sampling_frq_hz);
  bf.computeContinuousTimeTF(use_sampling_frequency);
  bf.computeDiscreteTimeTF(use_sampling_frequency);

  auto An = bf.getAn();
  auto Bn = bf.getBn();

  std::vector<double> An_gtruth{1., -1.760041880343169, 1.182893262037831, -0.278059917634546};
  std::vector<double> Bn_gtruth{
      0.018098933007514, 0.054296799022543, 0.054296799022543, 0.018098933007514};

  for (size_t k = 0; k < An.size(); ++k) {
    ASSERT_NEAR(An[k], An_gtruth[k], tol);
    ASSERT_NEAR(Bn[k], Bn_gtruth[k], tol);
  }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}