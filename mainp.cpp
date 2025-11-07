/*mainp.cpp*/

//
// MPI main process that coordinates the contrast stretching algorithm.
//
// YOUR NAME
// Northwestern University
//

#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <mpi.h>

#include "app.h"
#include "cs-util.h"
#include "matrix.h"

using namespace std;


// 
// mainp:
//
// Denotes the main process, with process id ("rank") of 0. The total # 
// of processes launches is given by numProcs. The main process is passed
// the image and image-related values, which it distributes to the worker 
// processes and eventually collects the results into a final image that
// is written to the specified output file. The main process also processes
// the first block of N/numProcs rows of the image, and ANY left-over rows
// if N/numProcs does not divide evenly. This way each worker gets exactly
// N/numProcs rows.
//
// Example: if there are 1003 rows in the image and 4 processes, then 
// every process gets 250 rows to work on. However, there are 3 extra rows,
// so the main process will keep those. The distribution is as follows:
//    main:     rows 0..252
//    worker 1: rows 253..502
//    worker 2: rows 503..752
//    worker 3: rows 753..1002
//
uchar** mainp(uchar** image, int rows, int cols, int steps, int numProcs)
{
  //
  // Run the contrast-stretching algorithm:
  //
  
	//
	// First, we need a temporary 2D matrix of the same size:
	//
	uchar** image2 = New2dMatrix<uchar>(rows, cols*3);

	//
	// Now perform contrast stretching, one step at a time:
	//
	int step = 1;

	while (step <= steps)
	{
		cout << "** Step " << step << "..." << endl;

		//
		// For each row (except boundary rows), lighten/darken pixel:
		//
    int diffs = 0;
    
		for (int row = 1; row < rows-1; row++)
		{
			//
			// And for each column (except boundary columns), lighten/darken pixel:
			//
			for (int col = 1; col < cols-1; col++)
			{
				diffs += stretch_one_pixel(image2, image, row, col);
			}
		}
    
    cout << "   Diff " << diffs << endl;

		//
		// The results are in image2, so copy the results back over to image
		// so if we loop again or return, we have the results in the original
		// image:
		//
		// NOTE: we don't copy the boundary rows and columns since those
		// don't change.
		//
		for (int row = 1; row < rows-1; row++) {
			for (int col = 1; col < cols-1; col++) {
				image[row][col*3]     = image2[row][col*3];
				image[row][col*3 + 1] = image2[row][col*3 + 1];
				image[row][col*3 + 2] = image2[row][col*3 + 2];
			}
		}

		// 
		// and repeat:
		//
		step++;

	}//while-each-step

	//
	// done!
	//
	Delete2dMatrix(image2);

	return image;
}
