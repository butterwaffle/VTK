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

// .NAME vtkBrush - provides a brush that fills shapes drawn by vtkContext2D.
//
// .SECTION Description
// The vtkBrush defines the fill (or pattern) of shapes that are drawn by
// vtkContext2D. The color is stored as four unsigned chars (RGBA), where the
// opacity defaults to 255, but can be modified separately to the other
// components. Ideally we would use a lightweight color class to store and pass
// around colors.

#ifndef __vtkBrush_h
#define __vtkBrush_h

#include "vtkObject.h"

class VTK_CHARTS_EXPORT vtkBrush : public vtkObject
{
public:
  vtkTypeRevisionMacro(vtkBrush, vtkObject);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

  // Description:
  // Creates a 2D Chart object.
  static vtkBrush *New();

  // Description:
  // Set the color of the brush.
  void SetColorF(double color[3]);
  void SetColorF(double r, double g, double b);
  void SetColorF(double r, double g, double b, double a);
  void SetOpacityF(double a);
  void SetColor(unsigned char color[3]);
  void SetColor(unsigned char r, unsigned char g, unsigned char b);
  void SetColor(unsigned char r, unsigned char g, unsigned char b,
                unsigned char a);
  void SetOpacity(unsigned char a);

  // Description:
  // Get the color of the brush - expects a double of length 4.
  void GetColorF(double color[4]);

  // Description:
  // Get the color of the brush - expects an unsigned char of length 4.
  void GetColor(unsigned char color[4]);

  // Description:
  // Get the color of the brush - gives a pointer to the underlying data.
  unsigned char * GetColor() { return &this->Color[0]; }

//BTX
protected:
  vtkBrush();
  ~vtkBrush();

  // Storage of the color in RGBA format (0-255 per channel).
  unsigned char Color[4];

private:
  vtkBrush(const vtkBrush &); // Not implemented.
  void operator=(const vtkBrush &);   // Not implemented.
//ETX
};

#endif //__vtkBrush_h
