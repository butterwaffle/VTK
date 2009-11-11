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

// .NAME vtkBlockItem - Factory class for drawing 2D charts
//
// .SECTION Description
// This defines the interface for a chart.

#ifndef __vtkBlockItem_h
#define __vtkBlockItem_h

#include "vtkContextItem.h"

class vtkContext2D;
class vtkBlockItem;

class VTK_CHARTS_EXPORT vtkBlockItem : public vtkContextItem
{
public:
  vtkTypeRevisionMacro(vtkBlockItem, vtkObject);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

  static vtkBlockItem *New();

  // Description:
  // Paint event for the chart, called whenever the chart needs to be drawn
  virtual bool Paint(vtkContext2D *painter);

  // Description:
  // Is the supplied x, y coordinate inside this item?
  virtual bool Hit(int x, int y);

  // Description:
  // Mouse enter event
  virtual bool MouseEnterEvent(int x, int y);

  // Description:
  // Mouse move event
  virtual bool MouseMoveEvent(int x, int y);

  // Description:
  // Mouse leave event
  virtual bool MouseLeaveEvent(int x, int y);

  // Description:
  // Left mouse button down event
  virtual bool LeftMouseButtonPressEvent(int x, int y);

  // Description:
  // Left mouse button up event
  virtual bool LeftMouseButtonReleaseEvent(int x, int y);

  vtkSetStringMacro(Label);
  vtkGetStringMacro(Label);

  vtkSetVector4Macro(Dimensions, int);
  vtkGetVector4Macro(Dimensions, int);

//BTX
protected:
  vtkBlockItem();
  ~vtkBlockItem();

  int Dimensions[4];

  int LastPosition[2];

  char *Label;

  bool MouseOver;
  bool LeftButtonPressed;

private:
  vtkBlockItem(const vtkBlockItem &); // Not implemented.
  void operator=(const vtkBlockItem &);   // Not implemented.
//ETX
};

#endif //__vtkBlockItem_h
