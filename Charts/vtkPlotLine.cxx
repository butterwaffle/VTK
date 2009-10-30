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

#include "vtkPlotLine.h"

#include "vtkContext2D.h"
#include "vtkPen.h"
#include "vtkContextDevice2D.h"
#include "vtkPoints2D.h"
#include "vtkTable.h"
#include "vtkAbstractArray.h"
#include "vtkFloatArray.h"

#include "vtkStdString.h"

#include "vtkObjectFactory.h"

vtkCxxRevisionMacro(vtkPlotLine, "$Revision$");

//-----------------------------------------------------------------------------
vtkStandardNewMacro(vtkPlotLine);

//-----------------------------------------------------------------------------
vtkPlotLine::vtkPlotLine()
{
  this->Table = 0;
  this->XColumn = 0;
  this->YColumn = 0;
  this->Points = 0;
  this->DrawPoints = true;
  this->DrawLines = true;
  this->Label = new vtkStdString;
}

//-----------------------------------------------------------------------------
vtkPlotLine::~vtkPlotLine()
{
  delete this->Label;
}

//-----------------------------------------------------------------------------
bool vtkPlotLine::Paint(vtkContext2D *painter)
{
  // This is where everything should be drawn, or dispatched to other methods.
  vtkDebugMacro(<< "Paint event called in vtkPlotLine.");

  if (this->Points)
    {
    painter->GetPen()->SetColor(this->r, this->g, this->b, this->a);

    if (this->DrawLines)
      {
      painter->GetPen()->SetWidth(this->Width);
      painter->DrawPoly(this->Points);
      }
    if (this->DrawPoints)
      {
      painter->GetPen()->SetWidth(this->Width*3.0);
      painter->DrawPoints(this->Points);
      }

    }
}

//-----------------------------------------------------------------------------
void vtkPlotLine::SetTable(vtkTable *table,
                              const char *xColumn,
                              const char *yColumn)
{
  this->Table = table;
  vtkDebugMacro(<< "SetTable called.\n"
      << table->GetColumn(0)->GetClassName() << "\t"
      << table->GetColumn(0)->GetName() << "\t"
      << table->GetColumn(0)->GetNumberOfTuples() << "\t"
      << table->GetColumn(0)->GetActualMemorySize()
      << endl
      << table->GetColumn(1)->GetClassName() << "\t"
      << table->GetColumn(1)->GetName()
      << endl);

  this->Modified();
}

//-----------------------------------------------------------------------------
void vtkPlotLine::SetTable(vtkTable *table,
                              vtkIdType xColumn,
                              vtkIdType yColumn)
{
  this->Table = table;
  vtkDebugMacro(<<"SetTable called.\n"
      << table->GetColumn(xColumn)->GetClassName() << "\t"
      << table->GetColumn(xColumn)->GetName() << "\t"
      << table->GetColumn(xColumn)->GetNumberOfTuples() << "\t"
      << table->GetColumn(xColumn)->GetActualMemorySize()
      << endl
      << table->GetColumn(yColumn)->GetClassName() << "\t"
      << table->GetColumn(yColumn)->GetName()
      << endl);

  // Set the column IDs for the x and y columns
  this->XColumn = xColumn;
  this->YColumn = yColumn;

  // Set the plot label based on the column header
  *this->Label = table->GetColumnName(yColumn);

  // Now copy the data from the columns into a vtkPoints2D, this should
  // possibly be done in a prepare function as it could take a while for big
  // data sets.
  vtkFloatArray *xArray = vtkFloatArray::SafeDownCast(this->Table->GetColumn(xColumn));
  vtkFloatArray *yArray = vtkFloatArray::SafeDownCast(this->Table->GetColumn(yColumn));
  if (xArray && yArray)
    {
    vtkIdType n = xArray->GetSize();
    if (this->Points)
      {
      this->Points->Delete();
      this->Points = NULL;
      }
    this->Points = vtkPoints2D::New();
    this->Points->SetNumberOfPoints(n);
    float *x = xArray->GetPointer(0);
    float *y = yArray->GetPointer(0);
    for (vtkIdType i = 0; i < n; ++i)
      {
      this->Points->SetPoint(i, x[i], y[i]);
      }
    }
  this->Modified();
}

//-----------------------------------------------------------------------------
void vtkPlotLine::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  // Print some details about this plot
  os << indent << "Label: \"" << *this->Label << "\"" << endl;
  os << indent << "Draw lines: ";
  if (this->DrawLines)
    {
    os << "on" << endl;
    }
  else
    {
    os << "off" << endl;
    }
  os << indent << "Draw points: ";
  if (this->DrawPoints)
    {
    os << "on" << endl;
    }
  else
    {
    os << "off" << endl;
    }
  os << indent << "Width: " << this->Width << endl;
  os << indent << "Color: " << this->r << ", " << this->g
     << ", " << this->b << ", " << this->a << endl;
  os << indent << "Table: ";
  if (this->Table)
    {
    os << "X = column " << this->XColumn
       << ", Y = column " << this->YColumn << endl;
    this->Table->PrintSelf(os, indent.GetNextIndent());
    }
  else
    {
    os << "(none)" << endl;
    }

}
