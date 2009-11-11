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

#include "vtkBlockItem.h"

// Get my new commands
#include "vtkCommand.h"

#include "vtkContext2D.h"
#include "vtkPen.h"
#include "vtkBrush.h"
#include "vtkTextProperty.h"

#include "vtkObjectFactory.h"

//-----------------------------------------------------------------------------
vtkCxxRevisionMacro(vtkBlockItem, "$Revision$");
vtkStandardNewMacro(vtkBlockItem);

//-----------------------------------------------------------------------------
vtkBlockItem::vtkBlockItem()
{
  this->Label = NULL;
  this->MouseOver = false;
  this->MouseButtonPressed = -1;
  this->scalarFunction = NULL;
}

//-----------------------------------------------------------------------------
vtkBlockItem::~vtkBlockItem()
{
  this->SetLabel(NULL);
}

//-----------------------------------------------------------------------------
bool vtkBlockItem::Paint(vtkContext2D *painter)
{
  // Drawing a hard wired diagram 800x600 as a demonstration of the 2D API
  painter->GetTextProp()->SetVerticalJustificationToCentered();
  painter->GetTextProp()->SetJustificationToCentered();
  painter->GetTextProp()->SetColor(0.0, 0.0, 0.0);
  painter->GetTextProp()->SetFontSize(24);
  painter->GetPen()->SetColor(0, 0, 0);

  if (this->MouseOver)
    {
    painter->GetBrush()->SetColor(255, 0, 0);
    }
  else
    {
    painter->GetBrush()->SetColor(0, 255, 0);
    }
  painter->DrawRect(this->Dimensions[0], this->Dimensions[1],
                    this->Dimensions[2]-this->Dimensions[0],
                    this->Dimensions[3]-this->Dimensions[1]);

  int x = this->Dimensions[0] + 0.5 * (this->Dimensions[2]-this->Dimensions[0]);
  int y = this->Dimensions[1] + 0.5 * (this->Dimensions[3]-this->Dimensions[1]);

  if (this->scalarFunction)
    {
    // We have a function pointer - do something...
    ;
    }

  painter->DrawText(x, y, this->Label);
}

//-----------------------------------------------------------------------------
bool vtkBlockItem::Hit(int x, int y)
{
    if (x > this->Dimensions[0] && x < this->Dimensions[2]
     && y > this->Dimensions[1] && y < this->Dimensions[3])
    {
    return true;
    }
  else
    {
    return false;
    }
  return false;
}

//-----------------------------------------------------------------------------
bool vtkBlockItem::MouseEnterEvent(int x, int y)
{
  this->MouseOver = true;
  return true;
}

//-----------------------------------------------------------------------------
bool vtkBlockItem::MouseMoveEvent(int x, int y)
{
  if (this->MouseButtonPressed == 0)
    {
    int deltaX = x - this->LastPosition[0];
    int deltaY = y - this->LastPosition[1];
    this->LastPosition[0] = x;
    this->LastPosition[1] = y;

    // Move the block by this amount
    this->Dimensions[0] += deltaX;
    this->Dimensions[1] += deltaY;
    this->Dimensions[2] += deltaX;
    this->Dimensions[3] += deltaY;

    return true;
    }
  else if (this->MouseButtonPressed == 1)
    {
    int deltaX = x - this->LastPosition[0];
    int deltaY = y - this->LastPosition[1];
    this->LastPosition[0] = x;
    this->LastPosition[1] = y;

    // Move the block by this amount
    this->Dimensions[0] += deltaX;
    this->Dimensions[1] += deltaY;

    return true;
    }
  else if (this->MouseButtonPressed == 2)
    {
    int deltaX = x - this->LastPosition[0];
    int deltaY = y - this->LastPosition[1];
    this->LastPosition[0] = x;
    this->LastPosition[1] = y;

    // Move the block by this amount
    this->Dimensions[2] += deltaX;
    this->Dimensions[3] += deltaY;

    return true;
    }
  return false;
}

//-----------------------------------------------------------------------------
bool vtkBlockItem::MouseLeaveEvent(int x, int y)
{
  this->MouseOver = false;
  return true;
}

//-----------------------------------------------------------------------------
bool vtkBlockItem::MouseButtonPressEvent(int button, int x, int y)
{
  this->MouseButtonPressed = button;
  this->LastPosition[0] = x;
  this->LastPosition[1] = y;
  return true;
}

//-----------------------------------------------------------------------------
bool vtkBlockItem::MouseButtonReleaseEvent(int button, int x, int y)
{
  this->MouseButtonPressed = -1;
  return true;
}

void vtkBlockItem::SetScalarFunctor(double (*scalarFunction)(double, double))
{
  this->scalarFunction = scalarFunction;
}

//-----------------------------------------------------------------------------
void vtkBlockItem::PrintSelf(ostream &os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
