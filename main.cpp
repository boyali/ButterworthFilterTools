#include "include/ButterTool.h"
#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

using namespace std;

int main()
{


    /*
     * The filter tool can be used in the following ways.
     *   1- Defining specs Wp, Ws, Ap, As and obtaining order and cut-off frequency (rad/sec)
     *      ButterworthFilter bf;
     *      bf.Buttord(Wp, Ws, Ap, As);
     *      bf.computeContinuousTimeTF();
     *      bf.computeDiscreteTimeTF();
     *
     *   2- Defining the order and cut-off frequency (rad/sec) directly and computing the filter TFs
     *      bf.setOrder(2);
     *      bf.setCuttoffFrequency(2.0);
     *      bf.computeContinuousTimeTF();
     *      bf.computeDiscreteTimeTF();
     *
     *   3- Defining the order N, cut-off and sampling frequencies (Hz)
     *
     *
     *
     *
     *
     * */
    double Wp, Ws, Ap, As;

    Wp = 2.0;  // passband frequency [rad/sec]
    Ws = 3.0;  // stopband frequency [rad/sec]
    Ap = 6.0;  // passband ripple mag or loss [dB]
    As = 20.0; // stop band rippe attenuation [dB]

    ButterworthFilter bf;

    /*
     * After creating a filter object, you can explicitly set the order N and cutOff frequency to get the filter
     * transfer functions;  filter.setOrder(int N), filter.setCutoffFrequency(double Wc).
     *
     * Or can compute these quantities by calling the buttord method; filter.Buttord(Wp, Ws, Ap, As)
     *
     * */

    // Setting filter order N and cut-off frequency explicitly (x-r1)(x-r2)
    bf.setOrder(2);
    bf.setCuttoffFrequency(2.0);
    bf.PrintFilter_Specs();

    // Computing filter coefficients from the specs
    bf.Buttord(Wp, Ws, Ap, As);

    // Get the computed order and Cut-off frequency
    /*
     * In the previous command line, given the filter specisifications in the continous time are used to compute the
     * filter order N and cut-off frequency. These variables are stored in the object's private vars. To get it, the
     * following getter method is used.
     *
     * */
    Order_Cutoff NWc = bf.getOrderCutOff();

    cout << " The computed order is ;" << NWc.N << endl;
    cout << " The computed cut-off frequency is ;" << NWc.Wc << endl;

    // Prints filter specs
    bf.PrintFilter_Specs();

    /*
     * Once the filter order N and the cut-off frequency computed and stored in
     * the object,
     *
     * */
    // Compute Continous Time TF
    bf.computeContinuousTimeTF();
    bf.PrintFilter_ContinuousTimeRoots();
    bf.PrintContinuousTimeTF();

    // Compute Discrete Time TF
    bf.computeDiscreteTimeTF();
    bf.PrintDiscreteTimeTF();

    // get An and Bn usign the getters
    DifferenceAnBn AnBn = bf.getAnBn();
    std::vector<double> An = bf.getAn();
    std::vector<double> Bn = bf.getBn();

    /*
     * This is the last method by which the discrete time numerator and Bd and denominator Ad of discrete-time filter
     * transfer function can be obtained.
     *
     * */

    return 0;
}