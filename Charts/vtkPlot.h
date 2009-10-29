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

// .NAME vtkPlot - Abstract class for plots.
//
// .SECTION Description
//

#ifndef __vtkPlot_h
#define __vtkPlot_h

#include "vtkContextMapper2D.h"

class vtkVariant;

class VTK_CHARTS_EXPORT vtkPlot : public vtkContextMapper2D
{
public:
  vtkTypeRevisionMacro(vtkPlot, vtkObject);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

  // Description:
  // Set the plot color
  virtual void SetColor(unsigned char r, unsigned char g, unsigned char b,
                        unsigned char a);

  // Description:
  // Get/set the width of the line.
  vtkSetMacro(Width, float);
  vtkGetMacro(Width, float);

  // Description:
  // A General setter/getter that should be overridden. It can silently drop
  // options, case is important
  void SetProperty(const char *property, vtkVariant *var);
  vtkVariant GetProperty(const char *property);

//BTX
protected:
  vtkPlot();
  ~vtkPlot();

  // This is not staying, but for now it is convenient. The data references
  // will live in the data series objects which will plot their own data.
  vtkTable *Table;

  // Columns from the table that are being plotted.
  vtkIdType XColumn, YColumn;

  unsigned char r, g, b, a;

  float Width;

private:
  vtkPlot(const vtkPlot &); // Not implemented.
  void operator=(const vtkPlot &); // Not implemented.

//ETX
};

#endif //__vtkPlot_h
