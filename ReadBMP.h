/*==========================================================================
  File: ReadBMP.h
  Task: Read BMP files and manipulate them for inclusion in OpenGL
        programs as texture maps.
============================================================================*/

#ifndef READBMP_H
#define READBMP_H

typedef struct MyImageStruct {
  unsigned long         Width;
  unsigned long         Height;
  unsigned char         *Pixels; /* Always RGB, row major order */
} MyImageStruct;

void ReadBMP(char *FileName, MyImageStruct *Image);

#endif
