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

#include "vtkContextItem.h"

// Get my new commands
#include "vtkCommand.h"

#include "vtkAnnotationLink.h"
#include "vtkInteractorStyle.h"
#include "vtkInteractorStyleRubberBand2D.h"
#include "vtkObjectFactory.h"

//-----------------------------------------------------------------------------
vtkCxxRevisionMacro(vtkContextItem, "$Revision$");

//-----------------------------------------------------------------------------
vtkContextItem::vtkContextItem()
{
}

//-----------------------------------------------------------------------------
vtkContextItem::~vtkContextItem()
{
}

//-----------------------------------------------------------------------------
bool vtkContextItem::Hit(int x, int y)
{
  return false;
}

//-----------------------------------------------------------------------------
bool vtkContextItem::MouseEnterEvent(int x, int y)
{
  return false;
}

//-----------------------------------------------------------------------------
bool vtkContextItem::MouseMoveEvent(int x, int y)
{
  return false;
}

//-----------------------------------------------------------------------------
bool vtkContextItem::MouseLeaveEvent(int x, int y)
{
  return false;
}

//-----------------------------------------------------------------------------
bool vtkContextItem::MouseButtonPressEvent(int button, int x, int y)
{
  return false;
}

//-----------------------------------------------------------------------------
bool vtkContextItem::MouseButtonReleaseEvent(int button, int x, int y)
{
  return false;
}

//-----------------------------------------------------------------------------
void vtkContextItem::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
