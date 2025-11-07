/* main.cpp */

//
// Performs a contrast stretch over a Windows bitmap (.bmp) file, making 
// lighter pixels lighter and darker pixels darker. This is an MPI-based
// version designed to run on distributed memory machines.
//
// Usage: mpiexec -n 4 cs infile.bmp outfile.bmp steps
//
// Author:
//   Prof. Joe Hummel
//   Northwestern University
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <cassert>
#include <cstdint>
#include <math.h>
#include <mpi.h>

#include "app.h"
#include "bitmap.h"
#include "matrix.h"

using namespace std;


//
// function prototypes:
//
uchar** mainp(uchar** image, int rows, int cols, int steps, int numProcs);
void    workerp_mid(int myRank, int numProcs);
void    workerp_end(int myRank, int numProcs);


//
// main:
//
int main(int argc, char* argv[])
{
  int myRank, numProcs;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  
  if (myRank == 0) { 
    //
    // main process:
    //
    char *infile;
    char *outfile;
    int   steps;

    //
    // process command-line args to program:
    //
    if (argc != 4)
    {
      cout << endl;
      cout << "Usage: mpiexec -n 4 cs infile.bmp outfile.bmp steps" << endl;
      cout << endl;
      MPI_Finalize();
      return 0;
    }

    infile = argv[1];
    outfile = argv[2];
    steps = atoi(argv[3]);

    cout << endl;
    cout << "** Starting Contrast Stretch **" << endl;
    cout << "   Input file:  " << infile << endl;
    cout << "   Output file: " << outfile << endl;
    cout << "   Steps:       " << steps << endl;

    //
    // now let's input bitmap file:
    //
    BITMAPFILEHEADER bitmapFileHeader;
    BITMAPINFOHEADER bitmapInfoHeader;
    uchar** image;
    int rows, cols;

    image = ReadBitmapFile(infile, bitmapFileHeader, bitmapInfoHeader, rows, cols);
    if (image == NULL)
    {
      cout << "** Failed to open image file, halting..." << endl;
      MPI_Finalize();
      return 1;
    }
    
    cout << "   Image size:  "
         << rows << " Rows, "
         << cols << " Columns"
         << endl;
    cout << endl;

    //
    // okay, perform contrast stretching:
    //
    cout << "** Processing..." << endl;

    auto start = chrono::high_resolution_clock::now();

    image = mainp(image, rows, cols, steps, numProcs);

    auto stop = chrono::high_resolution_clock::now();
    auto diff = stop - start;
    auto duration = chrono::duration_cast<chrono::milliseconds>(diff);

    //
    // Done, save image and output exec time:
    //
    cout << endl;
    cout << "** Done!  Time: " << duration.count() / 1000.0 << " secs" << endl;

    cout << "** Writing bitmap..." << endl;
    WriteBitmapFile(outfile, bitmapFileHeader, bitmapInfoHeader, image);

    cout << "** Execution complete." << endl;
    cout << endl;
  }
  else if (myRank == (numProcs-1)) { 
    //
    // last worker process:
    //
    workerp_end(myRank, numProcs);
  }
  else {
    //
    // mid worker processes:
    //
    workerp_mid(myRank, numProcs);
  }

  MPI_Finalize();
	return 0;
}
