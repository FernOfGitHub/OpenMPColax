//! \file
//!
//! @author Keith Smith
//! @date 20Sep2018
//!
//! Copyright, all right if you insist
//!

//! More examples from here. https://bisqwit.iki.fi/story/howto/openmp/#IntroductionToOpenmpInC

#include <iostream>			// cout
#include <iomanip>			// setw()
#include <omp.h>
#include "timestamp.h"

#include "SimpleAdd.h"

using namespace std;

void SimpleAdding()
{
	double wtime;

	timestamp( );
	cout << endl;
	cout << "SIMPLEADD_OPENMP\n";
	cout << "  C++/OpenMP version\n";

	cout << endl;
	cout << "  Number of processors available = " << omp_get_num_procs ( ) << endl;
	cout << "  Number of threads =              " << omp_get_max_threads ( ) << endl;

	int i, j;
	int n=10000;
	int sum;
	long long total_nomp = 0;

	double wtime_nomp = omp_get_wtime ( );

	for ( i = 1; i <= n; i++ )
	{
		for( j = 1; j<= n; j++)
		{
			sum = i + j;
			total_nomp += sum;
		}
	}
	wtime_nomp = omp_get_wtime ( ) - wtime_nomp;

	long long total = 0; // reset total

	wtime = omp_get_wtime ( );

#pragma omp parallel \
		shared ( n ) \
private ( i, j, sum )

#pragma omp for reduction ( + : total )
	for ( i = 1; i <= n; i++ )
	{
		for( j = 1; j<= n; j++)
		{
			sum = i  + j;
			total += sum;
		}
	}

	wtime = omp_get_wtime ( ) - wtime;

	cout << endl;
	if(total != total_nomp)
	{
		cout << "PARALLEL NOT THE SAME!!!" << endl;
		cout << "total_nomp = " << setw(8) << total_nomp << endl;
		cout << "total = " << setw(8) << total << endl;
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
	cout << "SIMPLEADD_OPENMP:" << endl;
	cout << "  Normal end of execution." << endl;
	cout << endl;
	timestamp ( );
}
