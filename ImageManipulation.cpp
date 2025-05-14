/*==========================================================================
  File: ImageManipulation.cpp
  Task: Perform various manipulations on images
============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "ReadBMP.h"
#include "ImageManipulation.h"

/*-------------------------------------------------------------------------*/
int Power2( int N)
{
  int    x = 2;

  while (x < N)
    x <<= 1;

  return (x >> 1);
}
/*-------------------------------------------------------------------------*/
int CropImageToPowerOf2( MyImageStruct *Image)
{
  return CropImageToPowerOf2( Image, Power2( Image->Width ), Power2( Image->Height ) );
}
/*-------------------------------------------------------------------------*/
int  CropImageToPowerOf2( MyImageStruct *Image, int NewWidth, int NewHeight )
{
  int             temp;
  unsigned char   *NewPixels, *NewP, *p;
  int             j, k;
  GLint						w;

  /* Verify that the dimensions are a power of two; change them if they are not */
  temp = Power2( NewWidth );
  if (temp != NewWidth)  NewWidth  = temp;
  temp = Power2( NewHeight );
  if (temp != NewHeight) NewHeight = temp;

  /* Test to see if this OpenGL implementation can support a texture map this large */
  do {
    glTexImage2D(GL_PROXY_TEXTURE_2D, 0, GL_RGB, NewWidth, NewHeight, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glGetTexLevelParameteriv( GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
    if (w == 0)
    {
      NewWidth  >>= 1; // divides by 2
      NewHeight >>= 1;
    }
  } while (w != NewWidth);

  /* create a copy of the image at this new size */
  NewPixels = (unsigned char *) malloc( NewWidth * NewHeight * 3 );
  if (NewPixels != NULL)
  {
    NewP = NewPixels;
    for (j=0; j<NewHeight; j++)
    {
      p = Image->Pixels + (j * Image->Width * 3);
      for (k=0; k<NewWidth*3; k++)
      {
        *NewP++ = *p++;
      }
    }

    free( Image->Pixels );

    Image->Width  = NewWidth;
    Image->Height = NewHeight;
    Image->Pixels = NewPixels;
    return 1; /* true; success */
  }
  return 0; /* failure */
}
/*-------------------------------------------------------------------------*/
void FoldImage( MyImageStruct *Image)
{
  int             HalfWidth  = Image->Width >> 1;
  unsigned char   *pLeft, *pRight;
  int             j, k;

  /* fold the left side of the image onto the right side of the image to
     make the left and right sides identical. This makes the image align
     with itself when used as a texture map. */
  for (j=0; j<Image->Height; j++)
  {
    pLeft = Image->Pixels + (j * Image->Width * 3);
    pRight = pLeft + (Image->Width * 3) - 3;
    for (k=0; k<HalfWidth; k++)
    {
      *pLeft++ = *pRight;     /* red */
      *pLeft++ = *(pRight+1); /* green */
      *pLeft++ = *(pRight+2); /* blue */

      pRight -= 3; /* back up to the previous pixel */
    }
  }

}
