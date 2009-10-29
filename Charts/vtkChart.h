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

// .NAME vtkChart - Factory class for drawing 2D charts
//
// .SECTION Description
// This defines the interface for a chart.

#ifndef __vtkChart_h
#define __vtkChart_h

#include "vtkObject.h"

class vtkContext2D;
class vtkPlot;

class vtkTable;

class VTK_CHARTS_EXPORT vtkChart : public vtkObject
{
public:
  vtkTypeRevisionMacro(vtkChart, vtkObject);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

  //BTX
  // Description:
  // Enum of the available chart types
  enum Type {
    LINE,
    STACKED};
//ETX

  // Description:
  // Paint event for the chart, called whenever the chart needs to be drawn
  virtual bool Paint(vtkContext2D *painter) = 0;

  // Description:
  // Add a plot to the chart, defaults to using the name of the y column
  virtual vtkPlot * AddPlot(Type type) = 0;

  // Description:
  // Get the number of plots the chart contains.
  virtual vtkIdType GetNumberPlots();

  // Description:
  // This function allows you to set the overall dimensions of the chart.
  // An int pointer of length 6 is expected with the dimensions in the order of
  // width, height, left border, bottom border, right border, top border in
  // pixels of the device.
  vtkSetVector6Macro(Geometry, int);

//BTX
protected:
  vtkChart();
  ~vtkChart();

  // Store the chart dimensions packed into a vtkPoints2D
  // [0] = width, height of chart in screen coordinates
  // [1] = left border, bottom border (roughly - origin of the chart
  // [2] = right border, top border (offset from top right most point)
  int Geometry[6];

private:
  vtkChart(const vtkChart &); // Not implemented.
  void operator=(const vtkChart &);   // Not implemented.
//ETX
};

#endif //__vtkChart_h
