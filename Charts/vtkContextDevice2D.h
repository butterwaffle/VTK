/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile$

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

// .NAME vtkContextDevice2D - Abstract class for drawing 2D primitives.
//
// .SECTION Description
// This defines the interface for a 2DPaintDevice. In this sense a PaintDevice
// is a class used to paint 2D primitives onto a device, such as an OpenGL
// context or a QGraphicsView.

#ifndef __vtkContextDevice2D_h
#define __vtkContextDevice2D_h

#include "vtkObject.h"
#include "vtkWeakPointer.h" // needed for vtkWeakPointer.

class vtkWindow;
class vtkViewport;
class vtkStdString;
class vtkTextProperty;
class vtk2DPainter;
class vtkPoints2D;
class vtkImageData;

class VTK_CHARTS_EXPORT vtkContextDevice2D : public vtkObject
{
public:
  vtkTypeRevisionMacro(vtkContextDevice2D, vtkObject);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

  // Description:
  // Draw a poly line using the vtkPoints2D - fastest code path due to memory
  // layout of the coordinates.
  virtual void DrawPoly(float *points, int n) = 0;

  // Description:
  // Draw a poly line using the vtkPoints2D - fastest code path due to memory
  // layout of the coordinates.
  virtual void DrawPoints(float *points, int n) = 0;

  // Description:
  // Draw a quad using the specified number of points.
  virtual void DrawQuad(float *points, int n) { ; }

//BTX
  // Description:
  // Draw some text to the screen.
  virtual void DrawText(float *point, vtkTextProperty *tprop,
                        const vtkStdString &string) = 0;
//ETX

  // Description:
  // Experimentation with point sprites
  virtual unsigned int AddPointSprite(vtkImageData *image) {;}

  // Description:
  // Set the color for the device using unsigned char of length 4, RGBA.
  virtual void SetColor(unsigned char *color) = 0;

  // Description:
  // Set the color for the device using unsigned char of length 3, RGB.
  virtual void SetColor3(unsigned char *color) = 0;

  // Description:
  // Set the point size for glyphs/sprites.
  virtual void SetPointSize(float size) = 0;

  // Description:
  // Set the line width for glyphs/sprites.
  virtual void SetLineWidth(float width) = 0;

  // Description:
  // Supply a float array of length 4 with x1, y1, x2, y2 specifying the extents.
  // of the display
  virtual void SetViewExtents(float *x) = 0;

  // Description:
  // Pop the matrix onto the stack.
  virtual void PushMatrix() = 0;

  // Description:
  // Pop the matrix off the stack.
  virtual void PopMatrix() = 0;

  // Description:
  // Supply a float array of length 4 with x1, y1, x2, y2 specifying clipping
  // for the display.
  virtual void SetClipping(int *x) = 0;

  // Description:
  // Disable clipping of the display.
  virtual void DisableClipping() = 0;

  virtual void Begin(vtkViewport* viewport) { }
  virtual void End() { }

//BTX
protected:
  vtkContextDevice2D();
  virtual ~vtkContextDevice2D();

private:
  vtkContextDevice2D(const vtkContextDevice2D &); // Not implemented.
  void operator=(const vtkContextDevice2D &);   // Not implemented.

//ETX
};

#endif //__vtkContextDevice2D_h
