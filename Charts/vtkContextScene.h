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

// .NAME vtkContextScene - Factory class for drawing 2D charts
//
// .SECTION Description
// This defines the interface for a chart.

#ifndef __vtkContextScene_h
#define __vtkContextScene_h

#include "vtkObject.h"

class vtkContext2D;
class vtkContextItem;

class vtkInteractorStyle;
class vtkAnnotationLink;

class vtkRenderWindow;

class VTK_CHARTS_EXPORT vtkContextScene : public vtkObject
{
public:
  vtkTypeRevisionMacro(vtkContextScene, vtkObject);
  virtual void PrintSelf(ostream &os, vtkIndent indent);

  // Description:
  // Creates a 2D Painter object.
  static vtkContextScene * New();

  // Description:
  // Paint event for the chart, called whenever the chart needs to be drawn
  virtual bool Paint(vtkContext2D *painter);

  void AddItem(vtkContextItem *item);
  int NumberOfItems();
  vtkContextItem * GetItem(int index);

  // Description:
  // Set/Get the vtkAnnotationLink for the chart.
  virtual void SetAnnotationLink(vtkAnnotationLink *link);
  vtkGetObjectMacro(AnnotationLink, vtkAnnotationLink);

  // Description:
  // This function allows you to set the overall dimensions of the chart.
  // An int pointer of length 6 is expected with the dimensions in the order of
  // width, height, left border, bottom border, right border, top border in
  // pixels of the device.
  vtkSetVector2Macro(Geometry, int);

  // Description:
  // Add the scene as an observer on the supplied interactor style.
  void AddInteractorStyle(vtkInteractorStyle *interactor);

  virtual void SetWindow(vtkRenderWindow *window);

//BTX
protected:
  vtkContextScene();
  ~vtkContextScene();

  // Description:
  // Called to process events - figure out what child(ren) to propagate events
  // to.
  virtual void ProcessEvents(vtkObject* caller, unsigned long eventId,
                             void* callData);

  // Description:
  // Process a rubber band selection event.
  virtual void ProcessSelectionEvent(vtkObject* caller, void* callData);

  virtual void MouseMoveEvent(int x, int y);
  virtual void LeftButtonPressEvent(int x, int y);
  virtual void LeftButtonReleaseEvent(int x, int y);
  virtual void MiddleButtonPressEvent(int x, int y);
  virtual void MiddleButtonReleaseEvent(int x, int y);
  virtual void RightButtonPressEvent(int x, int y);
  virtual void RightButtonReleaseEvent(int x, int y);

  vtkAnnotationLink *AnnotationLink;

  // Store the chart dimensions - width, height of scene in pixels
  int Geometry[2];

  // Description:
  // The command object for the charts.
  class Command;
  friend class Command;
  Command *Observer;

  // Description:
  // Private storage object - where we hide all of our STL objects...
  class Private;
  Private *Storage;

  vtkRenderWindow *Window;

private:
  vtkContextScene(const vtkContextScene &); // Not implemented.
  void operator=(const vtkContextScene &);   // Not implemented.
//ETX
};

#endif //__vtkContextScene_h
