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
#include "vtkChartActor.h"

#include "vtkContext2D.h"
#include "vtkOpenGLContextDevice2D.h"
#include "vtkChartXY.h"

#include "vtkViewport.h"

#include "vtkProperty2D.h"
#include "vtkMapper2D.h"
#include "vtkPropCollection.h"
#include "vtkObjectFactory.h"

vtkCxxRevisionMacro(vtkChartActor, "$Revision$");
vtkStandardNewMacro(vtkChartActor);

vtkCxxSetObjectMacro(vtkChartActor,Property, vtkProperty2D);
vtkCxxSetObjectMacro(vtkChartActor,Context, vtkContext2D);

//----------------------------------------------------------------------------
// Creates an actor2D with the following defaults:
// position -1, -1 (view coordinates)
// orientation 0, scale (1,1), layer 0, visibility on
vtkChartActor::vtkChartActor()
{
  this->Context = vtkContext2D::New();
  vtkOpenGLContextDevice2D *pd = vtkOpenGLContextDevice2D::New();
  this->Context->Begin(pd);

  this->LayerNumber = 0;
  this->Property = NULL;
  //
  this->PositionCoordinate = vtkCoordinate::New();
  this->PositionCoordinate->SetCoordinateSystem(VTK_VIEWPORT);
  //
  this->Position2Coordinate = vtkCoordinate::New();
  this->Position2Coordinate->SetCoordinateSystemToNormalizedViewport();
  this->Position2Coordinate->SetValue(0.5, 0.5);
  this->Position2Coordinate->SetReferenceCoordinate(this->PositionCoordinate);

  this->Chart = vtkChartXY::New();
}

//----------------------------------------------------------------------------
// Destroy an actor2D.
vtkChartActor::~vtkChartActor()
{
  if (this->Context)
    {
    this->Context->GetDevice()->Delete();
    this->Context->Delete();
    this->Context = NULL;
    }

  if (this->Property)
    {
    this->Property->UnRegister(this);
    this->Property = NULL;
    }
  if (this->PositionCoordinate)
    {
    this->PositionCoordinate->Delete();
    this->PositionCoordinate = NULL;
    }
  if (this->Position2Coordinate)
    {
    this->Position2Coordinate->Delete();
    this->Position2Coordinate = NULL;
    }

  if (this->Chart)
    {
    this->Chart->Delete();
    this->Chart = NULL;
    }
}

//----------------------------------------------------------------------------
void vtkChartActor::ReleaseGraphicsResources(vtkWindow *win)
{
}

//----------------------------------------------------------------------------
// Renders an actor2D's property and then it's mapper.
int vtkChartActor::RenderOverlay(vtkViewport* viewport)
{
  vtkDebugMacro(<< "vtkChartActor::RenderOverlay");

  if (!this->Property)
    {
    vtkDebugMacro(<< "vtkChartActor::Render - Creating Property2D");
    // Force creation of default property
    this->GetProperty();
    }

  this->Property->Render(viewport);

  if (!this->Context)
    {
    vtkErrorMacro(<< "vtkChartActor::Render - No painter set");
    return 0;
    }

  // This is the entry point for all 2D rendering.
  // First initialize the drawing device.
  this->Context->GetDevice()->Begin(viewport);

  int size[2];
  size[0] = viewport->GetSize()[0];
  size[1] = viewport->GetSize()[1];

  int geometry[] = { size[0], size[1],
                     60,      60,
                     25,      25 };
  this->Chart->SetGeometry(&geometry[0]);

  this->Chart->Paint(this->Context);

  this->Context->GetDevice()->End();

  return 1;
}

//----------------------------------------------------------------------------
// Does nothing for 2D charts.
int vtkChartActor::RenderOpaqueGeometry(vtkViewport* viewport)
{
  vtkDebugMacro(<< "vtkChartActor::RenderOpaqueGeometry");

  return 1;
}

//-----------------------------------------------------------------------------
// Does nothing for 2D charts.
int vtkChartActor::RenderTranslucentPolygonalGeometry(vtkViewport* viewport)
{
  vtkDebugMacro(<< "vtkChartActor::RenderTranslucentPolygonalGeometry");

  return 1;
}

//-----------------------------------------------------------------------------
int vtkChartActor::HasTranslucentPolygonalGeometry()
{
  return 0; // No translucent handling in the pipeline.
}

//----------------------------------------------------------------------------
unsigned long int vtkChartActor::GetMTime()
{
  unsigned long mTime = this->Superclass::GetMTime();
  unsigned long time;

  time  = this->PositionCoordinate->GetMTime();
  mTime = ( time > mTime ? time : mTime );
  time  = this->Position2Coordinate->GetMTime();
  mTime = ( time > mTime ? time : mTime );

  if ( this->Property != NULL )
    {
    time = this->Property->GetMTime();
    mTime = ( time > mTime ? time : mTime );
    }

  return mTime;
}

//----------------------------------------------------------------------------
// Set the Prop2D's position in display coordinates.
void vtkChartActor::SetDisplayPosition(int XPos, int YPos)
{
  this->PositionCoordinate->SetCoordinateSystem(VTK_DISPLAY);
  this->PositionCoordinate->SetValue(static_cast<float>(XPos),
                                     static_cast<float>(YPos),0.0);
}

//----------------------------------------------------------------------------
void vtkChartActor::SetWidth(double w)
{
  double *pos;

  pos = this->Position2Coordinate->GetValue();
  this->Position2Coordinate->SetCoordinateSystemToNormalizedViewport();
  this->Position2Coordinate->SetValue(w,pos[1]);
}

//----------------------------------------------------------------------------
void vtkChartActor::SetHeight(double w)
{
  double *pos;

  pos = this->Position2Coordinate->GetValue();
  this->Position2Coordinate->SetCoordinateSystemToNormalizedViewport();
  this->Position2Coordinate->SetValue(pos[0],w);
}

//----------------------------------------------------------------------------
double vtkChartActor::GetWidth()
{
  return this->Position2Coordinate->GetValue()[0];
}

//----------------------------------------------------------------------------
double vtkChartActor::GetHeight()
{
  return this->Position2Coordinate->GetValue()[1];
}

//----------------------------------------------------------------------------
// Returns an Prop2D's property2D.  Creates a property if one
// doesn't already exist.
vtkProperty2D *vtkChartActor::GetProperty()
{
  if (this->Property == NULL)
    {
    this->Property = vtkProperty2D::New();
    this->Property->Register(this);
    this->Property->Delete();
    this->Modified();
    }
  return this->Property;
}

//----------------------------------------------------------------------------
void vtkChartActor::GetActors2D(vtkPropCollection *ac)
{
  ac->AddItem(this);
}

//----------------------------------------------------------------------------
void vtkChartActor::ShallowCopy(vtkProp *prop)
{
  vtkChartActor *a = vtkChartActor::SafeDownCast(prop);
  if ( a != NULL )
    {
    this->SetContext(a->GetContext());
    this->SetLayerNumber(a->GetLayerNumber());
    this->SetProperty(a->GetProperty());
    this->SetPosition(a->GetPosition());
    this->SetPosition2(a->GetPosition2());
    }

  // Now do superclass
  this->vtkProp::ShallowCopy(prop);
}

//----------------------------------------------------------------------------
void vtkChartActor::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Layer Number: " << this->LayerNumber << "\n";
  os << indent << "PositionCoordinate: " << this->PositionCoordinate << "\n";
  this->PositionCoordinate->PrintSelf(os, indent.GetNextIndent());

  os << indent << "Position2 Coordinate: " << this->Position2Coordinate << "\n";
  this->Position2Coordinate->PrintSelf(os, indent.GetNextIndent());

  os << indent << "Property: " << this->Property << "\n";
  if (this->Property)
    {
    this->Property->PrintSelf(os, indent.GetNextIndent());
    }
  os << indent << "Context: " << this->Context << "\n";
  if (this->Context)
    {
    this->Context->PrintSelf(os, indent.GetNextIndent());
    }
}
