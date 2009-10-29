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

// .NAME vtkContextMapper2D - Abstract class for plots.
//
// .SECTION Description
//

#ifndef __vtkContextMapper2D_h
#define __vtkContextMapper2D_h

#include "vtkAlgorithm.h"

class vtkContext2D;
class vtkTable;

class VTK_CHARTS_EXPORT vtkContextMapper2D : public vtkAlgorithm
{
public:
  vtkTypeRevisionMacro(vtkContextMapper2D, vtkObject);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

  // Description:
  // Paint event for the XY plot, called whenever the chart needs to be drawn
  virtual bool Paint(vtkContext2D *painter) = 0;

  // This is a temporary class mainly added to do some basic testing.
  virtual void SetTable(vtkTable *table, const char *xColumn, const char *yColumn);
  virtual void SetTable(vtkTable *table, vtkIdType xColumn, vtkIdType yColumn);
  vtkGetObjectMacro(Table, vtkTable);

//BTX
protected:
  vtkContextMapper2D();
  ~vtkContextMapper2D();

  // This is not staying, but for now it is convenient. The data references
  // will live in the data series objects which will plot their own data.
  vtkTable *Table;

  // Columns from the table that are being plotted.
  vtkIdType XColumn, YColumn;

private:
  vtkContextMapper2D(const vtkContextMapper2D &); // Not implemented.
  void operator=(const vtkContextMapper2D &); // Not implemented.
//ETX
};

#endif //__vtkContextMapper2D_h
