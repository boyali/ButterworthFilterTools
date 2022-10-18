//
// Created by ali on 17/07/19.
//
#include <cmath>
#include <complex>
#include <cstdio>
#include <iostream>
#include <string>

#include "../include/ButterTool.h"

/**
 *  @brief Computes the minimum of an analog Butterworth filter order and cut-off frequency give
 *  the pass and stop-band frequencies and ripple magnitude (tolerances).
 *  @param Wp [in] pass-band frequency [rad/sc],
 *  @param Ws [in] stop-band frequency [rad/sc],
 *  @param Ap [in] pass-band ripple [dB]
 *  @param As [in] stop-band ripple [dB]
 * */

void ButterworthFilter::Buttord(
    const double & Wp, const double & Ws, const double & Ap, const double & As)
{
  // N*ln(alpha) > ln(beta)

  auto alpha = Ws / Wp;
  auto beta = sqrt((pow(10, As / 10.0) - 1.0) / (pow(10, Ap / 10.0) - 1.0));
  auto order = static_cast<int>(std::ceil(log(beta) / log(alpha)));

  setOrder(order);

  // right limit, left limit
  /**
   * The left and right limits of the magnitudes satisfy the specs at the
   * frequencies Ws and Wp Scipy.buttord gives left limit as the cut-off
   * frequency whereas Matlab gives right limit
   * */

  double right_lim = Ws * (pow((pow(10.0, As / 10.0) - 1.0), -1.0 / (2. * order)));
  // double left_lim = Wp * (pow((pow(10.0, Ap / 10.0) - 1.0), -1.0 / (2. * order)));

  setCuttoffFrequency(right_lim);
}

void ButterworthFilter::setOrder(int const & N) { order_ = N; }

void ButterworthFilter::setCuttoffFrequency(const double & Wc) { cutoff_frequency_rad_sec = Wc; }

/**
 * @brief Sets the cut-off and sampling frequencies.
 * @param fc [in] cut-off frequency in Hz.
 * @param fs [in] sampling frequency in Hz.
 * */
void ButterworthFilter::setCuttoffFrequency(const double & fc, const double & fs)
{
  /*
   * fc is the cut-off frequency in [Hz]
   * fs is the sampling frequency in [Hz]
   * */
  if (fc >= fs / 2) {
    print("Invalid argument : Cut-off frequency  fc must be less than fs/2 \n");
    return;
  }

  cutoff_frequency_rad_sec = fc * 2.0 * M_PI;
  sampling_frequency_hz = fs;
}

sOrderCutoff ButterworthFilter::getOrderCutOff() const
{
  return sOrderCutoff{order_, cutoff_frequency_rad_sec};
}

/**
 * @brief Matlab equivalent : [b, a]  = butter(n, Wn, 's')
 * */
void ButterworthFilter::computeContinuousTimeTF(const bool & use_sampling_frequency)
{
  // First compute  the phase angles of the roots
  computePhaseAngles();
  computeContinuousTimeRoots(use_sampling_frequency);

  continuous_time_denominator_ = poly(continuous_time_roots_);
  continuous_time_numerator_ = std::pow(cutoff_frequency_rad_sec, order_);
}

void ButterworthFilter::computePhaseAngles()
{
  phase_angles_.resize(order_, 0.);
  int k{1};

  for (auto & x : phase_angles_) {
    x = M_PI_2 + (M_PI * (2.0 * k - 1.0) / (2.0 * order_));
    k++;
    print("Phase angle x = ", x);
  }
}

void ButterworthFilter::computeContinuousTimeRoots(const bool & use_sampling_freqency)
{
  continuous_time_roots_.resize(order_, {0.0, 0.0});
  int k{};

  if (use_sampling_freqency) {
    print("\n Sampling Frequency is used to compute pre-warped frequency \n");

    double const & Fc = (sampling_frequency_hz / M_PI) *
                       tan(cutoff_frequency_rad_sec / (sampling_frequency_hz * 2.0));

    for (auto & x : continuous_time_roots_) {
      x = {cos(phase_angles_[k]) * Fc * 2.0 * M_PI, sin(phase_angles_[k]) * Fc * 2.0 * M_PI};
      k++;
    }

  } else {
    for (auto & x : continuous_time_roots_) {
      x = {
          cutoff_frequency_rad_sec * cos(phase_angles_[k]),
          cutoff_frequency_rad_sec * sin(phase_angles_[k])};
      k++;
    }
  }
}
std::vector<std::complex<double>> ButterworthFilter::poly(
    std::vector<std::complex<double>> const & roots)
{
  std::vector<std::complex<double>> coefficients(roots.size() + 1, {0, 0});

  int const n{static_cast<int>(roots.size())};

  coefficients[0] = {1.0, 0.0};

  for (int i = 0; i < n; i++) {
    for (int j = i; j != -1; j--) {
      coefficients[j + 1] = coefficients[j + 1] - (roots[i] * coefficients[j]);
    }
  }

  return coefficients;
}

/**
 *  @brief Prints the order and cut-off angular frequency (rad/sec) of the filter
 * */

void ButterworthFilter::printFilterContinuousTimeRoots() const
{
  print("\n The roots of the continuous-time filter Transfer Function's Denominator are : \n");

  for (auto const & x : continuous_time_roots_) {
    print(std::real(x), std::imag(x) < 0 ? " - " : " + ", " j", std::abs(std::imag(x)));
  }
}
void ButterworthFilter::printContinuousTimeTF() const
{
  auto const & n = order_;

  std::cout << "\nThe Continuous Time Transfer Function of the Filter is ;\n" << std::endl;

  print(continuous_time_numerator_, " / ");

  for (int i = n; i > 0; i--) {
    printf("%4.3f *", continuous_time_denominator_[n - i].real());
    printf("s[%d] + ", i);
  }

  printf("%4.3f \n", continuous_time_denominator_[n].real());
}

/**
 * @brief This method assumes the continuous time transfer function of filter has already been
 * computed and stored in the object and uses the bilinear transformation to obtain the discrete
 * time transfer function.
 *
 * Matlab equivalent :
 * Td = 2.
 * [numd, dend]=bilinear(sys_filt.Numerator{1}, sys_filt.Denominator{1}, 1/Td)
 * where sys_filt is the continuous time transfer function.
 * */
void ButterworthFilter::computeDiscreteTimeTF(const bool & use_sampling_frequency)
{
  discrete_time_zeros_.resize(order_, {-1.0, 0.0});  // Butter puts zeros at -1.0 for causality
  discrete_time_roots_.resize(order_, {0.0, 0.0});

  An_.resize(order_ + 1, 0.0);
  Bn_.resize(order_ + 1, 0.0);

  discrete_time_gain_ = {continuous_time_numerator_, 0.0};

  // Bi-linear Transformation of the Roots
  int k{};

  if (use_sampling_frequency) {
    for (auto & dr : discrete_time_roots_) {
      dr = (1.0 + continuous_time_roots_[k] / (sampling_frequency_hz * 2.0)) /
           (1.0 - continuous_time_roots_[k] / (sampling_frequency_hz * 2.0));
      k++;
    }

    discrete_time_denominator_ = poly(discrete_time_roots_);

    // Obtain the coefficients of numerator and denominator
    k = 0;
    discrete_time_numerator_ = poly(discrete_time_zeros_);

    // Compute Discrete Time Gain
    std::complex<double> sum_num{0.0, 0.0};
    std::complex<double> sum_den{0.0, 0.0};

    for (auto const & n : discrete_time_numerator_) {
      sum_num += n;
    }

    for (auto const & n : discrete_time_denominator_) {
      sum_den += n;
    }

    discrete_time_gain_ = (sum_den / sum_num);

    for (auto && dn : discrete_time_numerator_) {
      dn = dn * discrete_time_gain_;
      Bn_[k] = dn.real();
      k++;
    }

    k = 0;
    for (auto const & dd : discrete_time_denominator_) {
      An_[k] = dd.real();
      k++;
    }
  } else {
    for (auto && dr : discrete_time_roots_) {
      dr = (1.0 + Td_ * continuous_time_roots_[k] / 2.0) /
           (1.0 - Td_ * continuous_time_roots_[k] / 2.0);

      discrete_time_gain_ = discrete_time_gain_ / (1.0 - continuous_time_roots_[k]);
      k++;
    }

    discrete_time_denominator_ = poly(discrete_time_roots_);

    // Obtain the coefficients of numerator and denominator
    k = 0;
    discrete_time_numerator_ = poly(discrete_time_zeros_);

    for (auto && dn : discrete_time_numerator_) {
      dn = dn * discrete_time_gain_;
      Bn_[k] = dn.real();
      k++;
    }

    k = 0;
    for (auto const & dd : discrete_time_denominator_) {
      An_[k] = dd.real();
      k++;
    }
  }
}
void ButterworthFilter::printDiscreteTimeTF() const
{
  int const & n = order_;
  print("\nThe Discrete Time Transfer Function of the Filter is ;\n");

  for (int i = n; i > 0; i--) {
    printf("%4.3f *", discrete_time_numerator_[n - i].real());
    printf("z[-%d] + ", i);
  }

  printf("%4.3f", discrete_time_numerator_[n].real());
  print(" / ");

  for (int i = n; i > 0; i--) {
    printf("%4.3f *", discrete_time_denominator_[n - i].real());
    printf("z[-%d] + ", i);
  }

  printf("%4.3f", discrete_time_denominator_[n].real());
  std::cout << "\n" << std::endl;
}
std::vector<double> ButterworthFilter::getAn() const { return An_; }
std::vector<double> ButterworthFilter::getBn() const { return Bn_; }

sDifferenceAnBn ButterworthFilter::getAnBn() const { return {An_, Bn_}; }

void ButterworthFilter::printFilterSpecs() const
{
  /**
   * @brief Prints the order and cut-off angular frequency (rad/sec) of the filter
   *
   * */
  print("The order of the filter : ", this->order_);
  print("Cut-off Frequency : ", this->cutoff_frequency_rad_sec, " rad/sec\n");
}
