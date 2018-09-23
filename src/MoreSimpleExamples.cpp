//! \file
//!
//! @author Keith Smith
//! @date Sep 20, 2018
//!
//! Copyright, all right if you insist.
//!
//! Examples from here. https://bisqwit.iki.fi/story/howto/openmp/#IntroductionToOpenmpInC

#include <cmath>
#include <iostream>			// cout
#include <iomanip>			// setw
#include <omp.h>
#include "timestamp.h"
#include "MoreSimpleExamples.h"

using namespace std;

static const int size = 2048;
static double sinTable[size];

void MoreSimpleExamples1()
{
	timestamp ( );
	cout << endl;
	cout << "MoreSimpleExample1_OPENMP" << endl;
	cout << "  C++/OpenMP version" << endl;

	cout << endl;
	cout << "  Number of processors available = " << omp_get_num_procs ( ) << endl;
	cout << "  Number of threads =              " << omp_get_max_threads ( ) << endl;

    double wtime_nomp = omp_get_wtime ( );

    for(int n=0; n<size; ++n)
      sinTable[n] = std::sin(2 * M_PI * n / size);

    wtime_nomp = omp_get_wtime ( ) - wtime_nomp;

	double wtime = omp_get_wtime ( );

	#pragma omp parallel for
	for(int n=0; n<size; ++n)
		sinTable[n] = std::sin(2 * M_PI * n / size);

	wtime = omp_get_wtime ( ) - wtime;

	cout << endl;
	cout << " sinTable[1] = " << sinTable[1] << endl;
	cout << "  Elapsed time1 (slow) = " << wtime_nomp << endl;
	cout << "  Elapsed time = " << wtime << endl;
	double increase = (wtime_nomp-wtime)/wtime_nomp*100;
	cout << "  % increase = " << increase << endl;
	if(increase < 0.0)
	{
		cout << "  parallelism don't work with such short loops" << endl;
	}
	//
	//  Terminate.
	//
	cout << endl;
	cout << "MoreSimpleExample1_OPENMP" << endl;
	cout << "  Normal end of execution." << endl;
	cout << endl;
	timestamp ( );
}

void MoreSimpleExamples2()
{
	timestamp ( );
	cout << endl;
	cout << "MoreSimpleExample2_OPENMP" << endl;
	cout << "  C++/OpenMP version" << endl;

	cout << endl;
	cout << "  Number of processors available = " << omp_get_num_procs ( ) << endl;
	cout << "  Number of threads =              " << omp_get_max_threads ( ) << endl;

    double wtime_nomp = omp_get_wtime ( );

    for(int n=0; n<size; ++n)
      sinTable[n] = std::sin(2 * M_PI * n / size);

    wtime_nomp = omp_get_wtime ( ) - wtime_nomp;

	double wtime = omp_get_wtime ( );

	#pragma omp parallel for
	for(int n=0; n<size; ++n)
		sinTable[n] = std::sin(2 * M_PI * n / size);

	wtime = omp_get_wtime ( ) - wtime;

	cout << endl;
	cout << " sinTable[1] = " << sinTable[1] << endl;
	cout << "  Elapsed time1 (slow) = " << wtime_nomp << endl;
	cout << "  Elapsed time = " << wtime << endl;
	double increase = (wtime_nomp-wtime)/wtime_nomp*100;
	cout << "  % increase = " << increase << endl;
	if(increase < 0.0)
	{
		cout << "  parallelism don't work with such short loops" << endl;
	}
	//
	//  Terminate.
	//
	cout << endl;
	cout << "MoreSimpleExample2_OPENMP" << endl;
	cout << "  Normal end of execution." << endl;
	cout << endl;
	timestamp ( );
}

#include <complex>
#include <cstdio>

typedef std::complex<double> complex_t;

static int MandelbrotCalculate(complex_t c, int maxiter)
{
     // iterates z = z + c until |z| >= 2 or maxiter is reached,
     // returns the number of iterations.
	complex_t z = c;
     int n=0;
     for(; n<maxiter; ++n)
     {
         if( std::abs(z) >= 2.0) break;
         z = z*z + c;
     }
     return n;
}


void MoreSimpleExamples3()
{
	timestamp ( );
	cout << endl;
	cout << "MoreSimpleExample3_OPENMP" << endl;
	cout << "  C++/OpenMP version" << endl;

	cout << endl;
	cout << "  Number of processors available = " << omp_get_num_procs ( ) << endl;
	cout << "  Number of threads =              " << omp_get_max_threads ( ) << endl;

	const int width = 78, height = 44, num_pixels = width*height;

	const complex_t center(-.7, 0), span(2.7, -(4/3.0)*2.7*height/width);
	const complex_t begin = center-span/2.0;//, end = center+span/2.0;
	const int maxiter = 100000;

	double wtime_nomp = omp_get_wtime ( );

	for(int pix=0; pix<num_pixels; ++pix)
	{
		const int x = pix%width, y = pix/width;

		complex_t c = begin + complex_t(x * span.real() / (width +1.0),
				y * span.imag() / (height+1.0));

		int n = MandelbrotCalculate(c, maxiter);
		if(n == maxiter) n = 0;

		{
			char c = ' ';
			if(n > 0)
			{
				static const char charset[] = ".,c8M@jawrpogOQEPGJ";
				c = charset[n % (sizeof(charset)-1)];
			}
			std::putchar(c);
			if(x+1 == width) std::puts("|");
		}
	}
	wtime_nomp = omp_get_wtime ( ) - wtime_nomp;

	double wtime = omp_get_wtime ( );

#pragma omp parallel for ordered schedule(dynamic)
	for(int pix=0; pix<num_pixels; ++pix)
	{
		const int x = pix%width, y = pix/width;

		complex_t c = begin + complex_t(x * span.real() / (width +1.0),
				y * span.imag() / (height+1.0));

		int n = MandelbrotCalculate(c, maxiter);
		if(n == maxiter) n = 0;

		#pragma omp ordered
		{
			char c = ' ';
			if(n > 0)
			{
				static const char charset[] = ".,c8M@jawrpogOQEPGJ";
				c = charset[n % (sizeof(charset)-1)];
			}
			std::putchar(c);
			if(x+1 == width) std::puts("|");
		}
	}
	wtime = omp_get_wtime ( ) - wtime;

	cout << endl;
	cout << "  Elapsed time1 (slow) = " << wtime_nomp << endl;
	cout << "  Elapsed time = " << wtime << endl;
	double increase = (wtime_nomp-wtime)/wtime_nomp*100;
	cout << "  % increase = " << increase << endl;
	if(increase < 0.0)
	{
		cout << "  parallelism don't work with such short loops" << endl;
	}
	//
	//  Terminate.
	//
	cout << endl;
	cout << "MoreSimpleExample3_OPENMP" << endl;
	cout << "  Normal end of execution." << endl;
	cout << endl;
	timestamp ( );
}

long long factorial(long long number)
{
  long long fac = 1;
  #pragma omp parallel for reduction(*:fac)
  for(long long n=2; n<=number; ++n)
    fac *= n;
  return fac;
}

void MoreSimpleExamples4()
{
	timestamp ( );
	cout << endl;
	cout << "MoreSimpleExample4_OPENMP" << endl;
	cout << "  C++/OpenMP version" << endl;

	cout << endl;
	cout << "  Number of processors available = " << omp_get_num_procs ( ) << endl;
	cout << "  Number of threads =              " << omp_get_max_threads ( ) << endl;

	double wtime_nomp = omp_get_wtime ( );

	factorial(5);
	factorial(6);
	long long fac10_nmp = factorial(16);

	wtime_nomp = omp_get_wtime ( ) - wtime_nomp;

	double wtime = omp_get_wtime ( );

	factorial(10);
	factorial(11);
	long long fac10 = factorial(16);

	wtime = omp_get_wtime ( ) - wtime;

	cout << endl;
	if(fac10 != fac10_nmp)
	{
		cout << "PARALLEL NOT THE SAME!!!" << endl;
		cout << "fac10_nmp = " << setw(8) << fac10_nmp << endl;
		cout << "fac10 = " << setw(8) << fac10 << endl;
	}
	cout << "  Elapsed time1 (slow) = " << wtime_nomp << endl;
	cout << "  Elapsed time = " << wtime << endl;
	double increase = (wtime_nomp-wtime)/wtime_nomp*100;
	cout << "  % increase = " << increase << endl;
	if(increase < 0.0)
	{
		cout << "  parallelism don't work with such short loops" << endl;
	}
	//
	//  Terminate.
	//
	cout << endl;
	cout << "MoreSimpleExample4_OPENMP" << endl;
	cout << "  Normal end of execution." << endl;
	cout << endl;
	timestamp ( );
}
