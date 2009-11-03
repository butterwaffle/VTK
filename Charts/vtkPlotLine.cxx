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
#include "vtkFloatArray.h"
#include "vtkExecutive.h"
#include "vtkTimeStamp.h"

#include "vtkObjectFactory.h"

vtkCxxRevisionMacro(vtkPlotLine, "$Revision$");

//-----------------------------------------------------------------------------
vtkStandardNewMacro(vtkPlotLine);

//-----------------------------------------------------------------------------
vtkPlotLine::vtkPlotLine()
{
  this->Points = 0;
  this->DrawPoints = true;
  this->DrawLines = true;
  this->Label = 0;
  this->IsCacheDirty = true;
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

  // First check if we have an input
  vtkTable *table = this->GetInput();
  if (!table)
    {
    vtkDebugMacro(<< "Paint event called with no input table set.");
    return false;
    }
  else if(this->GetMTime() > this->BuildTime ||
          table->GetMTime() > this->BuildTime)
    {
    vtkDebugMacro(<< "Paint event called with outdated table cache. Updating.");
    this->UpdateTableCache(table);
    }

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
  return true;
}

//-----------------------------------------------------------------------------
bool vtkPlotLine::UpdateTableCache(vtkTable *table)
{
  // Get the x and y arrays (index 0 and 1 respectively)
  vtkDataArray *x = this->GetInputArrayToProcess(0, table);
  vtkDataArray *y = this->GetInputArrayToProcess(1, table);
  vtkFloatArray *xArray = vtkFloatArray::SafeDownCast(x);
  vtkFloatArray *yArray = vtkFloatArray::SafeDownCast(y);

  // Now arrange the data so that it can be rendered quickly
  if (xArray && yArray)
    {
    if (this->Points)
      {
      this->Points->Delete();
      this->Points = NULL;
      }
    vtkIdType n = xArray->GetSize();
    if (n == 0)
      {
      vtkDebugMacro(<< "No points!");
      return false;
      }
    else if (xArray->GetSize() != yArray->GetSize())
      {
      vtkDebugMacro(<< "Error size of x array does not equal size of y array.");
      return false;
      }

    this->Points = vtkPoints2D::New();
    this->Points->SetNumberOfPoints(n);
    float *x = xArray->GetPointer(0);
    float *y = yArray->GetPointer(0);
    for (vtkIdType i = 0; i < n; ++i)
      {
      this->Points->SetPoint(i, x[i], y[i]);
      }
    this->BuildTime.Modified();
    }
  else
    {
    vtkDebugMacro(<< "Error the x or y array was not a valid type.")
    }
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
}
