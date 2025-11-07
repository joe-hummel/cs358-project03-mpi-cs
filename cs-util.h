/*cs-util.h*/

//
// Utility functions in support of contrast-stretching.
//
// Author:
//   Prof. Joe Hummel
//   Northwestern University
//

#pragma once

#include "app.h"


//
// Function prototypes:
//
uchar median(uchar a, uchar b, uchar c, uchar d, uchar e, 
						 uchar f, uchar g, uchar h, uchar i, uchar &min, uchar &max);
             
uchar NewPixelValue(uchar UL, uchar UP, uchar UR,
                    uchar L,  uchar P,  uchar R,
                    uchar DL, uchar DW, uchar DR, 
                    int stepby);
                    
int stretch_one_pixel(uchar** image2, uchar** image, int row, int col);
