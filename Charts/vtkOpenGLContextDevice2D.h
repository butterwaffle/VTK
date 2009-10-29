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

// .NAME vtkOpenGLContextDevice2D - Class for drawing 2D primitives using OpenGL.
//
// .SECTION Description
// This defines the

#ifndef __vtkOpenGLContextDevice2D_h
#define __vtkOpenGLContextDevice2D_h

#include "vtkContextDevice2D.h"

class vtkWindow;
class vtkViewport;
class vtkRenderer;
class vtkLabelRenderStrategy;
class vtk2DPainter;

class VTK_CHARTS_EXPORT vtkOpenGLContextDevice2D : public vtkContextDevice2D
{
public:
  vtkTypeRevisionMacro(vtkOpenGLContextDevice2D, vtkObject);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

  // Description:
  // Creates a 2D Painter object.
  static vtkOpenGLContextDevice2D *New();

  // Description:
  // Draw a poly line using the vtkPoints2D - fastest code path due to memory
  // layout of the coordinates.
  virtual void DrawPoly(float *points, int n);

  // Description:
  // Draw a poly line using the vtkPoints2D - fastest code path due to memory
  // layout of the coordinates.
  virtual void DrawPoints(float *points, int n);

  // Description:
  // Draws a rectangle
  virtual void DrawRect(float *points, int n);

//BTX
  // Description:
  // Draw some text to the screen!
  virtual void DrawText(float *point, vtkTextProperty *tprop,
                        const vtkStdString &string);
//ETX

  // Description:
  // Set the global color for the device.
  virtual void SetColor(int r, int g, int b, int a);
  virtual void SetColor(int r, int g, int b);

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


  virtual void PopMatrix();

  // Description:
  // Supply an int array of length 4 with x1, y1, x2, y2 specifying clipping
  // for the display.
  virtual void SetClipping(int *x);

  // Description:
  // Disable clipping of the display.
  virtual void DisableClipping();

  virtual void Begin(vtkViewport* viewport);
  virtual void End();

//BTX
protected:
  vtkOpenGLContextDevice2D();
  virtual ~vtkOpenGLContextDevice2D();

  // We need to store a pointer to the renderer for the text rendering
  vtkRenderer *Renderer;

  // We also need a label render strategy
  vtkLabelRenderStrategy *TextRenderer;

private:
  vtkOpenGLContextDevice2D(const vtkOpenGLContextDevice2D &); // Not implemented.
  void operator=(const vtkOpenGLContextDevice2D &);   // Not implemented.

//ETX
};

#endif //__vtkOpenGLContextDevice2D_h
