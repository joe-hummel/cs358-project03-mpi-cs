/*workerp_end.cpp*/

//
// MPI worker process in the contrast stretching algorithm. An "end" worker
// processes the last block of rows in the image, and has only one neighbor
// above; there is only one "end" worker process.
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
// workerp:
// 
// Denotes an "end" worker process with a process id ("rank") of myRank.
// The total # of processes launched is denoted by numProcs. Each worker
// processes N/numProcs rows of the image, and sends the results back
// to the main process when finished. An "end" worker processes the last 
// block of rows in the image, and has only one neighbor above; there is 
// only one "end" worker process.
//
void workerp_end(int myRank, int numProcs)
{

}
