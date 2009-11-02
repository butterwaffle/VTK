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

#include "vtkChart.h"

#include "vtkPoints2D.h"

#include "vtkObjectFactory.h"

//-----------------------------------------------------------------------------
vtkCxxRevisionMacro(vtkChart, "$Revision$");

//-----------------------------------------------------------------------------
vtkChart::vtkChart()
{
}

//-----------------------------------------------------------------------------
vtkChart::~vtkChart()
{
}

//-----------------------------------------------------------------------------
vtkPlot * vtkChart::AddPlot(Type type)
{
  return NULL;
}

//-----------------------------------------------------------------------------
vtkIdType vtkChart::GetNumberPlots()
{
  return 0;
}

//-----------------------------------------------------------------------------
void vtkChart::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  // Print out the chart's geometry if it has been set
  os << indent << "Origin: " << this->Geometry[0] << "\t" << this->Geometry[1]
     << endl;
  os << indent << "Width: " << this->Geometry[2] << endl
     << indent << "Height: " << this->Geometry[3] << endl;
  os << indent << "Right border: " << this->Geometry[4] << endl
     << indent << "Top border: " << this->Geometry[5] << endl;
}
