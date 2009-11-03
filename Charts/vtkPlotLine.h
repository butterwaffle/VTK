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

// .NAME vtkPlotLine - Class for drawing an XY plot given two columns from a
// vtkTable.
//
// .SECTION Description
//

#ifndef __vtkPlotLine_h
#define __vtkPlotLine_h

#include "vtkPlot.h"

class vtkContext2D;
class vtkTable;
class vtkPoints2D;
class vtkStdString;

class VTK_CHARTS_EXPORT vtkPlotLine : public vtkPlot
{
public:
  vtkTypeRevisionMacro(vtkPlotLine, vtkObject);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

  // Description:
  // Creates a 2D Chart object.
  static vtkPlotLine *New();

  // Description:
  // Set/Get whether the plot should draw lines.
  vtkSetMacro(DrawLines, bool);
  vtkGetMacro(DrawLines, bool);

  // Description:
  // Set/Get whether the plot should draw points.
  vtkSetMacro(DrawPoints, bool);
  vtkGetMacro(DrawPoints, bool);

  // Description:
  // Paint event for the XY plot, called whenever the chart needs to be drawn
  bool Paint(vtkContext2D *painter);

//BTX
protected:
  vtkPlotLine();
  ~vtkPlotLine();

  // Description:
  // Update the table cache.
  bool UpdateTableCache(vtkTable *table);

  // Description:
  // Store a well packed set of XY coordinates for this data series.
  vtkPoints2D *Points;

  // Description:
  // The point cache is marked dirty until it has been initialized.
  bool IsCacheDirty;
  vtkTimeStamp BuildTime;

  // A few test options - this should be unified into a nicer data structure
  bool DrawLines, DrawPoints;

private:
  vtkPlotLine(const vtkPlotLine &); // Not implemented.
  void operator=(const vtkPlotLine &); // Not implemented.

//ETX
};

#endif //__vtkPlotLine_h
