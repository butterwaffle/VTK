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

// .NAME vtkContextItem - Factory class for drawing 2D charts
//
// .SECTION Description
// This defines the interface for a chart.

#ifndef __vtkContextItem_h
#define __vtkContextItem_h

#include "vtkObject.h"

class vtkContext2D;
class vtkContextItem;

class VTK_CHARTS_EXPORT vtkContextItem : public vtkObject
{
public:
  vtkTypeRevisionMacro(vtkContextItem, vtkObject);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

  // Description:
  // Paint event for the chart, called whenever the chart needs to be drawn
  virtual bool Paint(vtkContext2D *painter) = 0;

  // Description:
  // Is the supplied x, y coordinate inside this item?
  virtual bool Hit(int x, int y);

  // Description:
  // Mouse enter event
  virtual bool MouseEnterEvent(int x, int y);

  // Description:
  // Mouse enter event
  virtual bool MouseMoveEvent(int x, int y);

  // Description:
  // Mouse enter event
  virtual bool MouseLeaveEvent(int x, int y);

  // Description:
  // Left mouse button down event
  virtual bool MouseButtonPressEvent(int button, int x, int y);

  // Description:
  // Left mouse button up event
  virtual bool MouseButtonReleaseEvent(int button, int x, int y);

//BTX
protected:
  vtkContextItem();
  ~vtkContextItem();

private:
  vtkContextItem(const vtkContextItem &); // Not implemented.
  void operator=(const vtkContextItem &);   // Not implemented.
//ETX
};

#endif //__vtkContextItem_h
