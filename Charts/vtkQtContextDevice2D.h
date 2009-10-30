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

// .NAME vtkQtContextDevice2D - Class for drawing 2D primitives using OpenGL.
//
// .SECTION Description
// This defines the

#ifndef __vtkQtContextDevice2D_h
#define __vtkQtContextDevice2D_h

#include "vtkContextDevice2D.h"

class vtkWindow;
class vtkViewport;
class vtkRenderer;
//BTX
class QPainter;
//ETX
class vtk2DPainter;

class VTK_CHARTS_EXPORT vtkQtContextDevice2D : public vtkContextDevice2D
{
public:
  vtkTypeRevisionMacro(vtkQtContextDevice2D, vtkObject);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

  // Description:
  // Creates a 2D Painter object.
  static vtkQtContextDevice2D *New();

  // Description:
  // Draw a poly line using the vtkPoints2D - fastest code path due to memory
  // layout of the coordinates.
  virtual void DrawPoly(float *points, int n);

  // Description:
  // Draw a poly line using the vtkPoints2D - fastest code path due to memory
  // layout of the coordinates.
  virtual void DrawPoints(float *points, int n);

//BTX
  // Description:
  // Draw some text to the screen!
  virtual void DrawText(float *point, vtkTextProperty *tprop,
                        const vtkStdString &string);
//ETX

  // Description:
  // Set the color for the device using unsigned char of length 4, RGBA.
  virtual void SetColor(unsigned char *color);

  // Description:
  // Set the color for the device using unsigned char of length 3, RGB.
  virtual void SetColor3(unsigned char *color);

  // Description:
  // Set the point size for glyphs/sprites.
  virtual void SetPointSize(float size);

  // Description:
  // Set the line width for glyphs/sprites.
  virtual void SetLineWidth(float width);

  // Description:
  // Supply a float array of length 4 with x1, y1, x2, y2 specifying the extents
  // of the display
  virtual void SetViewExtents(float *x);

  virtual void PushMatrix();
  virtual void PopMatrix();

  // Description:
  // Supply an int array of length 4 with x1, y1, x2, y2 specifying clipping
  // for the display.
  virtual void SetClipping(int *x);

  // Description:
  // Disable clipping of the display.
  virtual void DisableClipping();

//BTX
  // QPainter can not be wrapped.
  virtual void Begin(QPainter *painter);
  virtual void End();
//ETX

//BTX
protected:
  vtkQtContextDevice2D();
  virtual ~vtkQtContextDevice2D();

  // We need to store a pointer to the renderer for the text rendering
  QPainter *Painter;

private:
  vtkQtContextDevice2D(const vtkQtContextDevice2D &); // Not implemented.
  void operator=(const vtkQtContextDevice2D &);   // Not implemented.

//ETX
};

#endif //__vtkQtContextDevice2D_h
