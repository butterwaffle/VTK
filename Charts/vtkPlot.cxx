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

#include "vtkPlot.h"

#include "vtkContext2D.h"
#include "vtkContextDevice2D.h"
#include "vtkPoints2D.h"
#include "vtkTable.h"
#include "vtkAbstractArray.h"
#include "vtkFloatArray.h"

#include "vtkObjectFactory.h"

vtkCxxRevisionMacro(vtkPlot, "$Revision$");

//-----------------------------------------------------------------------------
vtkPlot::vtkPlot()
{
  this->r = 0;
  this->g = 0;
  this->b = 0;
  this->a = 0;
  this->Width = 1.0;
}

//-----------------------------------------------------------------------------
vtkPlot::~vtkPlot()
{
}

//-----------------------------------------------------------------------------
void vtkPlot::SetColor(unsigned char r, unsigned char g, unsigned char b,
                       unsigned char a)
{
  this->r = r;
  this->g = g;
  this->b = b;
  this->a = a;
}

//-----------------------------------------------------------------------------
void vtkPlot::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  // Print out our color and width
  os << indent << "Color: " << this->r << ", " << this->g
     << ", " << this->b << ", " << this->a << endl;
  os << indent << "Width: " << this->Width << endl;

}
