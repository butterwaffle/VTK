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
// .NAME vtkChartActor - an actor that draws a 2D chart
// .SECTION Description
// vtkChartActor contains a vtkChart object which renders a 2D chart in the
// overlay. This class is quite thin and provides an entry point for the chart
// into the rendering pipeline.

// .SECTION See Also
// vtkProp  vtkMapper2D vtkProperty2D

#ifndef __vtkChartActor_h
#define __vtkChartActor_h

#include "vtkProp.h"
#include "vtkCoordinate.h" // For vtkViewportCoordinateMacro

class vtkContext2D;
class vtkChart;
class vtkProperty2D;

class VTK_CHARTS_EXPORT vtkChartActor : public vtkProp
{
public:
  void PrintSelf(ostream& os, vtkIndent indent);
  vtkTypeRevisionMacro(vtkChartActor,vtkProp);

  // Description:
  // Creates an actor2D with the following defaults:
  // position (0,0) (coordinate system is viewport);
  // at layer 0.
  static vtkChartActor* New();

  // Description:
  // Support the standard render methods.
  virtual int RenderOverlay(vtkViewport *viewport);
  virtual int RenderOpaqueGeometry(vtkViewport *viewport);
  virtual int RenderTranslucentPolygonalGeometry(vtkViewport *viewport);

  // Description:
  // Does this prop have some translucent polygonal geometry?
  virtual int HasTranslucentPolygonalGeometry();

  // Description:
  // Set/Get the vtk2DPainter.
  virtual void SetContext(vtkContext2D *context);
  vtkGetObjectMacro(Context, vtkContext2D);

  // Description:
  // Set/Get the layer number in the overlay planes into which to render.
  vtkSetMacro(LayerNumber, int);
  vtkGetMacro(LayerNumber, int);

  // Description:
  // Get the chart object for the Actor.
  vtkGetObjectMacro(Chart, vtkChart);

  // Description:
  // Returns this actor's vtkProperty2D.  Creates a property if one
  // doesn't already exist.
  vtkProperty2D* GetProperty();

  // Description:
  // Set this vtkProp's vtkProperty2D.
  virtual void SetProperty(vtkProperty2D*);

  // Description:
  // Get the PositionCoordinate instance of vtkCoordinate.
  // This is used for for complicated or relative positioning.
  // The position variable controls the lower left corner of the Actor2D
  vtkViewportCoordinateMacro(Position);

  // Description:
  // Set the Prop2D's position in display coordinates.
  void SetDisplayPosition(int,int);

  // Description:
  // Access the Position2 instance variable. This variable controls
  // the upper right corner of the Actor2D. It is by default
  // relative to Position and in normalized viewport coordinates.
  // Some 2D actor subclasses ignore the position2 variable
  vtkViewportCoordinateMacro(Position2);

  // Description:
  // Set/Get the height and width of the 2DActor. The value is expressed
  // as a fraction of the viewport. This really is just another way of
  // setting the Position2 instance variable.
  void SetWidth(double w);
  double GetWidth();
  void SetHeight(double h);
  double GetHeight();

  // Description:
  // Return this objects MTime.
  virtual unsigned long GetMTime();

  // Description:
  // For some exporters and other operations we must be
  // able to collect all the actors or volumes. These methods
  // are used in that process.
  virtual void GetActors2D(vtkPropCollection *pc);

  // Description:
  // Shallow copy of this vtkChartActor. Overloads the virtual vtkProp method.
  virtual void ShallowCopy(vtkProp *prop);

  // Description:
  // Release any graphics resources that are being consumed by this actor.
  // The parameter window could be used to determine which graphic
  // resources to release.
  virtual void ReleaseGraphicsResources(vtkWindow *);

  // Description:
  // Return the actual vtkCoordinate reference that the mapper should use
  // to position the actor. This is used internally by the mappers and should
  // be overridden in specialized subclasses and otherwise ignored.
  virtual vtkCoordinate *GetActualPositionCoordinate(void)
    { return this->PositionCoordinate; }

  // Description:
  // Return the actual vtkCoordinate reference that the mapper should use
  // to position the actor. This is used internally by the mappers and should
  // be overridden in specialized subclasses and otherwise ignored.
  virtual vtkCoordinate *GetActualPosition2Coordinate(void)
    { return this->Position2Coordinate; }

protected:
  vtkChartActor();
  ~vtkChartActor();

  vtkChart *Chart;
  vtkContext2D *Context;
  int LayerNumber;
  vtkProperty2D *Property;
  vtkCoordinate *PositionCoordinate;
  vtkCoordinate *Position2Coordinate;

private:
  vtkChartActor(const vtkChartActor&);  // Not implemented.
  void operator=(const vtkChartActor&);  // Not implemented.
};

#endif
