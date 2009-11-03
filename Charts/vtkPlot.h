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
  // Set/Get the plot label.
  vtkSetStringMacro(Label);
  vtkGetStringMacro(Label);

  // Description:
  // This is a convenience function to set the input table and the x, y column
  // for the plot.
  virtual void SetInput(vtkTable *table);
  virtual void SetInput(vtkTable *table, const char *xColumn,
                        const char *yColumn);
  void SetInput(vtkTable *table, vtkIdType xColumn, vtkIdType yColumn);

  // Description:
  // Convenience function to set the input arrays. For most mappers index 0
  // is the x axis, and index 1 is the y axis. The name is the name of the
  // column in the vtkTable.
  virtual void SetInputArray(int index, const char *name);

//BTX
  // Description:
  // A General setter/getter that should be overridden. It can silently drop
  // options, case is important
  void SetProperty(const char *property, vtkVariant *var);
  vtkVariant GetProperty(const char *property);
//ETX

//BTX
protected:
  vtkPlot();
  ~vtkPlot();

  unsigned char r, g, b, a;

  float Width;

  // Plot label, used by legend
  char *Label;

private:
  vtkPlot(const vtkPlot &); // Not implemented.
  void operator=(const vtkPlot &); // Not implemented.

//ETX
};

#endif //__vtkPlot_h
