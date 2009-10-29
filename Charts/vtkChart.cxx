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
/*
//-----------------------------------------------------------------------------
void vtkChart::SetGeometry(int *p)
{
  if (!this->Geometry)
    {
    this->Geometry = vtkPoints2D::New();
    this->Geometry->SetDataTypeToInt();
    this->Geometry->SetNumberOfPoints(3);
    }
  this->Geometry->SetPoint(0, p[0], p[1]);
  this->Geometry->SetPoint(1, p[2], p[3]);
  this->Geometry->SetPoint(2, p[4], p[5]);

  this->Modified();
}

//-----------------------------------------------------------------------------
void vtkChart::SetGeometry(int p0, int p1, int p2, int p3, int p4, int p5)
{
  int p[] = { p0, p1, p2, p3, p4, p5 };
  this->SetGeometry(&p[0]);
}

//-----------------------------------------------------------------------------
vtkPoints2D * vtkChart::GetGeometry()
{
  return this->Geometry;
}
*/

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
