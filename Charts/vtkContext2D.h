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

// .NAME vtkContext2D - Class for drawing 2D primitives to a graphical context.
//
// .SECTION Description
// This defines the interface for drawing onto a 2D context. The context must
// be set up with a vtkContextDevice2D derived class that provides the functions
// to facilitate the low level calls to the context. Currently an OpenGL and a
// Qt based device implementation are provided (Qt implementation is far from
// finished).

#ifndef __vtkContext2D_h
#define __vtkContext2D_h

#include "vtkObject.h"

class vtkAbstractArray;
class vtkActor;

class vtkWindow;

class vtkStdString;
class vtkTextProperty;

class vtkPoints2D;
class vtkContextDevice2D;

class VTK_CHARTS_EXPORT vtkContext2D : public vtkObject
{
public:
  vtkTypeRevisionMacro(vtkContext2D, vtkObject);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

  // Description:
  // Creates a 2D Painter object.
  static vtkContext2D *New();

  // Description:
  // Begin painting on a vtkContextDevice2D, no painting can occur before this call
  // has been made. Only one painter is allowed at a time on any given paint
  // device. Returns true if successful, otherwise false.
  bool Begin(vtkContextDevice2D *device);

  vtkGetObjectMacro(Device, vtkContextDevice2D);

  // Description:
  // Ends painting on the device, you would not usually need to call this as it
  // should be called by the destructor. Returns true if the painter is no
  // longer active, otherwise false.
  bool End();

  // Description:
  // Draw a line between the specified points.
  void DrawLine(float x1, float y1, float x2, float y2);

  // Description:
  // Draw a line between the specified points.
  void DrawLine(float p[4]);

  // Description:
  // Draw a line between the specified points.
  // Note: Fastest path - points packed in x and y.
  void DrawLine(vtkPoints2D *points);

  // Description:
  // Draw a poly line between the specified points.
  void DrawPoly(float *x, float *y, int n);

  // Description:
  // Draw a poly line between the specified points - fastest code path due to
  // memory layout of the coordinates.
  void DrawPoly(vtkPoints2D *points);

  // Description:
  // Draw a poly line between the specified points, where the float array is of
  // size 2*n and the points are packed x1, y1, x2, y2 etc.
  // Note: Fastest code path - points packed in x and y.
  void DrawPoly(float *points, int n);

  // Description:
  // Draw a point at the supplied x and y coordinate
  void DrawPoint(float x, float y);

  // Description:
  // Draw the specified number of points using the x and y arrays supplied
  void DrawPoints(float *x, float *y, int n);

  // Description:
  // Draw a poly line between the specified points - fastest code path due to
  // memory layout of the coordinates.
  void DrawPoints(vtkPoints2D *points);

  // Description:
  // Draw a poly line between the specified points, where the float array is of
  // size 2*n and the points are packed x1, y1, x2, y2 etc.
  // Note: Fastest code path - points packed in x and y.
  void DrawPoints(float *points, int n);

  // Description:
  // Draw a rectangle with origin at x, y and width w, height h
  void DrawRect(float x, float y, float w, float h);

  // Description:
  // Draw a quadrilateral at the specified points (4 points, 8 floats in x, y).
  void DrawQuad(float x1, float y1, float x2, float y2,
                float x3, float y3, float x4, float y4);
  void DrawQuad(float *p);

//BTX
  // Description:
  // Draw some text to the screen.
  void DrawText(vtkPoints2D *point, vtkTextProperty *tprop,
                const vtkStdString &string);
  void DrawText(int x, int y, vtkTextProperty *tprop,
                const vtkStdString &string);
//ETX
  void DrawText(vtkPoints2D *point, vtkTextProperty *tprop,
                const char *string);
  void DrawText(int x, int y, vtkTextProperty *tprop,
                const char *string);

  // Description:
  // Set the color for the context with range 0-255.
  void SetColor(unsigned char r, unsigned char g, unsigned char b,
                unsigned char a);
  void SetColor(unsigned char r, unsigned char g, unsigned char b);

  // Description:
  // Set the color for the context with range 0.0 - 1.0
  void SetColor(double r, double g, double b, double a);
  void SetColor(double r, double g, double b);

  // Description:
  // Sets the point size when drawing points.
  void SetPointSize(float size);

  // Description:
  // Set the line width for any 2D line construct.
  void SetLineWidth(float width);

//BTX
protected:
  vtkContext2D();
  ~vtkContext2D();

  vtkContextDevice2D *Device;

private:
  vtkContext2D(const vtkContext2D &); // Not implemented.
  void operator=(const vtkContext2D &);   // Not implemented.

//ETX
};

#endif //__vtkContext2D_h
