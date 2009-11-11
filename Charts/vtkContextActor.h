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
// .NAME vtkContextActor

#ifndef __vtkContextActor_h
#define __vtkContextActor_h

#include "vtkProp.h"

class vtkContext2D;
class vtkContextScene;
class vtkInteractorStyle;

class VTK_CHARTS_EXPORT vtkContextActor : public vtkProp
{
public:
  void PrintSelf(ostream& os, vtkIndent indent);
  vtkTypeRevisionMacro(vtkContextActor,vtkProp);

  // Description:
  // Creates an actor2D with the following defaults:
  // position (0,0) (coordinate system is viewport);
  // at layer 0.
  static vtkContextActor* New();

  // Description:
  // We only render in the overlay for the context scene.
  virtual int RenderOverlay(vtkViewport *viewport);

  // Description:
  // Set/Get the vtk2DPainter.
  virtual void SetContext(vtkContext2D *context);
  vtkGetObjectMacro(Context, vtkContext2D);

  // Description:
  // Get the chart object for the Actor.
  vtkGetObjectMacro(Scene, vtkContextScene);
  virtual void SetScene(vtkContextScene *scene);

  // Description:
  // Return this objects MTime.
  virtual unsigned long GetMTime();

  // Description:
  // Release any graphics resources that are being consumed by this actor.
  // The parameter window could be used to determine which graphic
  // resources to release.
  virtual void ReleaseGraphicsResources(vtkWindow *);

protected:
  vtkContextActor();
  ~vtkContextActor();

  vtkContextScene *Scene;
  vtkContext2D *Context;

private:
  vtkContextActor(const vtkContextActor&);  // Not implemented.
  void operator=(const vtkContextActor&);  // Not implemented.
};

#endif
