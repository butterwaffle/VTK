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

#include "vtkContextMapper2D.h"

#include "vtkContext2D.h"
#include "vtkContextDevice2D.h"
#include "vtkPoints2D.h"
#include "vtkTable.h"
#include "vtkAbstractArray.h"
#include "vtkFloatArray.h"

#include "vtkObjectFactory.h"

vtkCxxRevisionMacro(vtkContextMapper2D, "$Revision$");

//-----------------------------------------------------------------------------
vtkContextMapper2D::vtkContextMapper2D()
{
  this->Table = 0;
  this->XColumn = 0;
  this->YColumn = 0;
}

//-----------------------------------------------------------------------------
vtkContextMapper2D::~vtkContextMapper2D()
{
}

//-----------------------------------------------------------------------------
void vtkContextMapper2D::SetTable(vtkTable *table,
                                  const char *xColumn,
                                  const char *yColumn)
{
  this->Table = table;
  // Currently have to iterate through the columns to match on the names
  for (vtkIdType i = 0; i < table->GetNumberOfColumns(); ++i)
    {
    const char *name = table->GetColumnName(i);
    if (strcmp(xColumn, name))
      {
      this->XColumn = i;
      }
    else if (strcmp(yColumn, name))
      {
      this->YColumn = i;
      }
    }
  this->Modified();
}

//-----------------------------------------------------------------------------
void vtkContextMapper2D::SetTable(vtkTable *table,
                                  vtkIdType xColumn,
                                  vtkIdType yColumn)
{
  this->Table = table;

  // Set the column IDs for the x and y columns
  this->XColumn = xColumn;
  this->YColumn = yColumn;
  this->Modified();
}

//-----------------------------------------------------------------------------
void vtkContextMapper2D::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
