/*==========================================================================
  File: ImageManipulation.h
  Task: Perform various manipulations on images
============================================================================*/

#ifndef IMAGEMANIPULATION_H
#define IMAGEMANIPULATION_H

int  Power2(int N);
int  CropImageToPowerOf2( MyImageStruct *Image);
int  CropImageToPowerOf2( MyImageStruct *Image, int width2, int height2 );
void FoldImage( MyImageStruct *Image);

#endif
