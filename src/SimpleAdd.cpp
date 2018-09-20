//! \file
//!
//! @author Keith Smith
//! @date 20Sep2018
//!
//! Copyright, all right if you insist
//!

#include <iostream>			// cout
#include <iomanip>			// setw()
#include <omp.h>
#include "timestamp.h"

#include "SimpleAdd.h"

using namespace std;

void SimpleAdding()
{
	double wtime;

	timestamp ( );
	cout << "\n";
	cout << "SIMPLEADD_OPENMP\n";
	cout << "  C++/OpenMP version\n";

	cout << "\n";
	cout << "  Number of processors available = " << omp_get_num_procs ( ) << "\n";
	cout << "  Number of threads =              " << omp_get_max_threads ( ) << "\n";

	wtime = omp_get_wtime ( );

	int i;
	int j;
	int n=1000;
	int sum;
	int total = 0;

	double wtime_nomp = omp_get_wtime ( );

	for ( i = 0; i < n; i++ )
	{
		for( j = 0; j< n; j++)
		{
			sum = i + j;
			total += sum;
		}
	}
	wtime_nomp = omp_get_wtime ( ) - wtime_nomp;

	total = 0; // reset total

#pragma omp parallel \
		shared ( n ) \
private ( i, j, sum )

#pragma omp for reduction ( + : total )
	for ( i = 0; i < n; i++ )
	{
		for( j = 0; j< n; j++)
		{
			sum = i  + j;
			total += sum;
		}
	}

	wtime = omp_get_wtime ( ) - wtime;

	cout << "\n";
	cout << "Total = " << setw(8) << total << endl;
	cout << "  Elapsed time1 (slow) = " << wtime_nomp << endl;
	cout << "  Elapsed time = " << wtime << endl;
	cout << "  % increase = " << (wtime_nomp-wtime)/wtime*100 << endl;
	//
	//  Terminate.
	//
	cout << endl;
	cout << "SIMPLEADD_OPENMP:" << endl;
	cout << "  Normal end of execution." << endl;
	cout << endl;
	timestamp ( );
}
