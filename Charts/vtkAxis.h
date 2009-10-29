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

// .NAME vtkAxis - takes care of drawing axes
//
// .SECTION Description
// The vtkAxis is drawn in screen coordinates. It is usually one of the last
// elements of a chart to be drawn, and is drawn in screen coordinates. It
// renders the axis label, tick marks and tick labels.

#ifndef __vtkAxis_h
#define __vtkAxis_h

#include "vtkObject.h"

class vtkContext2D;

class VTK_CHARTS_EXPORT vtkAxis : public vtkObject
{
public:
  vtkTypeRevisionMacro(vtkAxis, vtkObject);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

  // Description:
  // Creates a 2D Chart object.
  static vtkAxis *New();

  // Description:
  // Set point 1 of the axis (in pixels), this is usually the origin.
  vtkSetVector2Macro(Point1, float);

  // Description:
  // Get point 1 of the axis (in pixels), this is usually the origin.
  vtkGetVector2Macro(Point1, float);

  // Description:
  // Set point 2 of the axis (in pixels), this is usually the terminus.
  vtkSetVector2Macro(Point2, float);

  // Description:
  // Get point 2 of the axis (in pixels), this is usually the terminus.
  vtkGetVector2Macro(Point2, float);

  // Get/Set the number of ticks for this axis.
  vtkSetMacro(NumberOfTicks, int);
  vtkGetMacro(NumberOfTicks, int);

  // Description:
  // Set the logical minimum value of the axis (of the plot)
  vtkSetMacro(Minimum, float);
  vtkGetMacro(Minimum, float);

  // Description:
  // Set the logical maximum value of the axis (of the plot)
  vtkSetMacro(Maximum, float);
  vtkGetMacro(Maximum, float);

  // Description:
  // Get/set the label text for the axis.
  vtkSetStringMacro(Label);
  vtkGetStringMacro(Label);

  // Description:
  // Paint event for the axis, called whenever the axis needs to be drawn
  virtual bool Paint(vtkContext2D *painter);

//BTX
protected:
  vtkAxis();
  ~vtkAxis();

  float Point1[2];       // The position of point 1 (usually the origin)
  float Point2[2];       // The position of point 2 (usually the terminus)
  int NumberOfTicks;   // The number of tick marks to draw
  float Minimum;       // Minimum value of the axis
  float Maximum;       // Maximum values of the axis
  char *Label; // The text label drawn on the axis

private:
  vtkAxis(const vtkAxis &); // Not implemented.
  void operator=(const vtkAxis &);   // Not implemented.
//ETX
};

#endif //__vtkAxis_h
