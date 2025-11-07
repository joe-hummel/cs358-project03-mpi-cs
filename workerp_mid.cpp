/*workerp_mid.cpp*/

//
// MPI worker process in the contrast stretching algorithm. A "mid" worker
// has neighbors above and below.
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
// Denotes a "mid" worker process with a process id ("rank") of myRank.
// The total # of processes launched is denoted by numProcs. Each worker
// processes N/numProcs rows of the image, and sends the results back
// to the main process when finished. A "mid" worker has neighbors above
// and below.
//
void workerp_mid(int myRank, int numProcs)
{

}
