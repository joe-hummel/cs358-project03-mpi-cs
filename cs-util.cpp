/* cs-util.cpp */

//
// Utility functions in support of contrast-stretching.
//
// Author:
//   Prof. Joe Hummel
//   Northwestern University
//

#include <iostream>
#include <mpi.h>

#include "app.h"
#include "cs-util.h"
#include "matrix.h"

using namespace std;


//
// Given 9 values (imagine a pixel and its 8 surrounding neighbors), returns the median
// value while also returning min and max values (via last 2 parameters)
//
uchar median(uchar a, uchar b, uchar c, uchar d, uchar e, 
						 uchar f, uchar g, uchar h, uchar i, uchar &min, uchar &max)
{
	uchar A[9];

	A[0] = a;  // capture 9 values:
	A[1] = b;
	A[2] = c;
	A[3] = d;
	A[4] = e;
	A[5] = f;
	A[6] = g;
	A[7] = h;
	A[8] = i;

	// selection sort:
	for(int i=0; i < 8; i++)
	{
		int min = i;

		// select smallest in subset:
		for (int j=i+1; j < 9; j++)
		{
			if (A[j] < A[min])
				min = j;
		}

		// swap:
		uchar temp = A[i];
		A[i] = A[min];
		A[min] = temp;
	}//for

	// return min, max & median of 9 elements:
	min = A[0];
	max = A[8];

	return A[4];
}


//
// Given a pixel value (a single RGB value) P and it's 8 neighbors, returns the new
// pixel value P' based on min & max values of neighbors.
//
uchar NewPixelValue(uchar UL, uchar UP, uchar UR,
                    uchar L,  uchar P,  uchar R,
                    uchar DL, uchar DW, uchar DR, 
                    int stepby)
{
	uchar newp, med, min, max;
	double ratio;

	med = median(UL, UP, UR, L, P, R, DL, DW, DR, min, max);

	if (min == max)  // pixels are all the same:
		newp = min;
	else
	{
		// are neighbors darker than pixel, or lighter?
		ratio = (P - min) / (double) (max - min);

		if (ratio < 0.5)  // darker, so make pixel darker if possible:
		{
			if (P > stepby) // beware of underflow:
				newp = P - stepby;
			else  
				newp = 0;
		}
		else if (ratio > 0.5)  // lighter, so make pixel lighter if possible:
		{
			if (P < 255-stepby)  // beware of overflow:
				newp = P + stepby;
			else  
				newp = 255;
		}
		else  // neither, so leave pixel alone:
			newp = P;
	}

	// done:
	return newp;
}


//
// stretch_one_pixel: stretches one pixel, returning the # of RGB values that
// have changed from the current pixel to the new pixel.
//
int stretch_one_pixel(uchar** image2, uchar** image, int row, int col)
{
	int prevrow = row - 1;  // row above
	int nextrow = row + 1;  // row below
	int changes = 0;

	// columns are a little trickier, since a "column" is really 3 physical cols: RGB
	col = col*3;
	int prevcol = col - 3;  // previous column start:
	int nextcol = col + 3;  // next column start:

	//
	// now update each pixel based on nearest neighbors around that pixel:
	//

	// Blue:
	image2[row][col] = NewPixelValue(image[prevrow][prevcol],
		image[prevrow][col],
		image[prevrow][nextcol],
		image[row][prevcol],
		image[row][col],
		image[row][nextcol],
		image[nextrow][prevcol],
		image[nextrow][col],
		image[nextrow][nextcol],
		1 /*stepby*/);

	// did value change?
	if (image2[row][col] != image[row][col])
	  changes++;

	// Green:
	col++;
	prevcol++;
	nextcol++;

	image2[row][col] = NewPixelValue(image[prevrow][prevcol],
		image[prevrow][col],
		image[prevrow][nextcol],
		image[row][prevcol],
		image[row][col],
		image[row][nextcol],
		image[nextrow][prevcol],
		image[nextrow][col],
		image[nextrow][nextcol],
		1 /*stepby*/);

	// did value change?
	if (image2[row][col] != image[row][col])
	  changes++;

	// Red:
	col++;
	prevcol++;
	nextcol++;

	image2[row][col] = NewPixelValue(image[prevrow][prevcol],
		image[prevrow][col],
		image[prevrow][nextcol],
		image[row][prevcol],
		image[row][col],
		image[row][nextcol],
		image[nextrow][prevcol],
		image[nextrow][col],
		image[nextrow][nextcol],
		1 /*stepby*/);

	// did value change?
	if (image2[row][col] != image[row][col])
	  changes++;

	return changes;
}


//
// Performs contrast stetch over the given image --- i.e. makes lighter colors lighter and
// darker colors darker.
//
// NOTE: the image matrix is a 2D matrix of pixels.  However, each pixel is 3 colors RGB,
// which means a "column" in the matrix is actually 3 columns: Blue, Green, and Red.  So
// as we loop over the matrix, we loop by 3 as we go along the column.
//
uchar** ContrastStretch(uchar** image, int rows, int cols, int steps)
{
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
