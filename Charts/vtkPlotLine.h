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

  // Description:
  // This is a temporary class mainly added to do some basic testing.
  void SetTable(vtkTable *table, const char *xColumn, const char *yColumn);
  void SetTable(vtkTable *table, vtkIdType xColumn, vtkIdType yColumn);
  vtkGetObjectMacro(Table, vtkTable);

//BTX
protected:
  vtkPlotLine();
  ~vtkPlotLine();

  // This is not staying, but for now it is convenient. The data references
  // will live in the data series objects which will plot their own data.
  vtkTable *Table;

  // Columns from the table that are being plotted.
  vtkIdType XColumn, YColumn;

  // Store a well packed set of XY coordinates for this data series.
  vtkPoints2D *Points;

  // A few test options - this should be unified into a nicer data structure
  bool DrawLines, DrawPoints;

  // Plot label, used by legend
  vtkStdString *Label;

private:
  vtkPlotLine(const vtkPlotLine &); // Not implemented.
  void operator=(const vtkPlotLine &); // Not implemented.

//ETX
};

#endif //__vtkPlotLine_h
