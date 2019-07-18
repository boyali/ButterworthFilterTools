### Butterworth Low-pass Filter Design Tool Class 

This Butterworth low-pass filter design tool can be used to design a Butterworth filter in continuous and discrete time from the given specifications of the filter performance. 

The Butterworth filter is a class implementation. The object is created by a default constructor without any argument. 

The filter can be prepared in two ways. If the filter specifications are known such as passband, stopband frequencies (Wp and Ws) together with the passband and stopband ripple magnitudes (Ap and As), one can call the filter's buttord method with these arguments to obtain the recommended filter order (N) and cut-off frequency (Wc [rad/s]). An example call is demonstrated below;


    ButterworthFilter bf();

    Wp = 2.0; // passband frequency [rad/sec]
    Ws = 3.0; // stopband frequency [rad/sec]
    Ap = 6.0; // passband ripple mag or loss [dB]
    As = 20.0; // stop band rippe attenuation [dB]
    
    // Computing filter coefficients from the specs
    bf.Buttord(Wp, Ws, Ap, As);

    // Get the computed order and Cut-off frequency
    Order_Cutoff NWc = bf.getOrderCutOff();]
    
    cout << " The computed order is ;" << NWc.N << endl;
    cout << " The computed cut-off frequency is ;" << NWc.Wc << endl;
    
The filter order and cut-off frequency can be obtain in a struct using bf.getOrderCutoff() method. These specs can be printed on the screen by calling PrintFilterSpecs() method.

If the user would like to define the order and cut-off frequency directly, the setter methods for these variables can  be called to set the filter order and the desired cut-off frequency for the filter.

#### Obtaining Filter Transfer Functions
The discrete transfer function of the filter requires the roots and gain of the continuous time transfer function. Therefore it is a must to call create the continuous time transfer of the method; 
    
    bf.computeContinuousTimeTF();
    
The computed continuous time transfer funtion roots can be printed on the screen using 

    bf.PrintFilter_ContinuousTimeRoots();
    bf.PrintContinuousTimeTF();
    
methods. The resulting screen output for a 5th order filter is demonstrated below. 

     Roots of Continous Time Filter Transfer Function Denominator are : 
    -0.585518 + j 1.80204
    -1.53291 + j 1.11372
    -1.89478 + j 2.32043e-16
    -1.53291 + j -1.11372
    -0.585518 + j -1.80204
    
    
    The Continuous Time Transfer Function of the Filter is ;
    
             24.422 
    --------------------------------------------------------
    1.000 *s[5] + 6.132 *s[4] + 18.798 *s[3] + 35.619 *s[2] + 41.711 *s[1] + 24.422 
    

#### Discrete Time Transfer Function (Difference Equations)

The digital filter equivalent from the continuous time definitions are produced by using the bilinear transformation. When creating the discrete time function of the ButterworthFilter object, its numerator (Bn) and Denominator (An) coeffients are stored in a vector of filter order size N.

The discrete transfer function method is called using ;

    bf.computeDiscreteTimeTF();
    bf.PrintDiscreteTimeTF();
    
The results are printed on the screen like;
The Discrete Time Transfer Function of the Filter is ;

    0.191 *z[-5] + 0.956 *z[-4] + 1.913 *z[-3] + 1.913 *z[-2] + 0.956 *z[-1] + 0.191
    --------------------------------------------------------
    1.000 *z[-5] + 1.885 *z[-4] + 1.888 *z[-3] + 1.014 *z[-2] + 0.298 *z[-1] + 0.037
    
and the associated difference coefficients An and Bn by withing a struct ; 

    DifferenceAnBn AnBn = bf.getAnBn();
    
 The difference coefficients appear in the filtering equation in the form of.  
 
    An *Yfiltered = Bn * Yunfiltered  
    
 
 To filter a signal given in a vector form ; 
 
 
 Reference: 
 1 . Manolakis, Dimitris G., and Vinay K. Ingle. Applied digital signal processing: theory and practice. Cambridge University Press, 2011. 
 
 
    
