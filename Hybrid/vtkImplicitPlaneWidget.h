/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile$
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 1993-2002 Ken Martin, Will Schroeder, Bill Lorensen 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkImplicitPlaneWidget - 3D widget for manipulating an infinite plane
// .SECTION Description
// This 3D widget defines an infinite plane that can be interactively placed
// in a scene. The widget is represented by a plane with a normal vector; the
// plane is contained by a bounding box, and where the plane intersects the
// bounding box the edges are shown (possibly tubed). The normal can be
// selected and moved to rotate the plane; the plane itself can be selected
// and translated in various directions.
//
// To use this object, just invoke SetInteractor() with the argument of the
// method a vtkRenderWindowInteractor.  You may also wish to invoke
// "PlaceWidget()" to initially position the widget. If the "i" key (for
// "interactor") is pressed, the vtkImplicitPlaneWidget will appear. (See
// superclass documentation for information about changing this behavior.)
// If you select the normal vector, the plane can be arbitrarily rotated. The
// plane can be translated within the bounding box by selecting it with the
// left mouse button. Selecting any part of the widget with the middle mouse
// button enables translation of the plane along its normal. (Once selected
// using middle mouse, moving "up" in the middle moves the plane in the
// direction of the normal; moving "down" moves it in the opposite
// direction.) The right mouse button can be used to uniformly scale the
// bounding box (moving "up" the box scales larger; moving "down" the box
// scales smaller). Events that occur outside of the widget (i.e., no part of
// the widget is picked) are propagated to any other registered obsevers
// (such as the interaction style).  Turn off the widget by pressing the "i"
// key again (or invoke the Off() method).
//
// The vtkImplicitPlaneWidget has several methods that can be used in
// conjunction with other VTK objects.  The GetPolyData() method can be used
// to get a polygonal representation (the single polygon clipped by the
// bounding box).  Typical usage of the widget is to make use of the
// StartInteractionEvent, InteractionEvent, and EndInteractionEvent
// events. The InteractionEvent is called on mouse motion; the other two
// events are called on button down and button up (either left or right
// button). (Note: there is also a PlaceWidgetEvent that is invoked when
// the widget is placed with PlaceWidget().)
//
// Some additional features of this class include the ability to control the
// properties of the widget. You can set the properties of the selected and
// unselected representations of the plane. For example, you can set the
// property for the handles and plane. In addition there are methods to
// constrain the plane so that it is aligned along the x-y-z axes.

// .SECTION Caveats
// Note that handles and the plane can be picked even when they are "behind"
// other actors.  This is an intended feature and not a bug.

// .SECTION See Also
// vtk3DWidget vtkBoxWidget vtkPlaneWidget vtkLineWidget vtkPointWidget
// vtkSphereWidget vtkImagePlaneWidget

#ifndef __vtkImplicitPlaneWidget_h
#define __vtkImplicitPlaneWidget_h

#include "vtkPolyDataSourceWidget.h"

class vtkActor;
class vtkPolyDataMapper;
class vtkCellPicker;
class vtkConeSource;
class vtkLineSource;
class vtkSphereSource;
class vtkTubeFilter;
class vtkPlane;
class vtkCutter;
class vtkProperty;
class vtkImageData;
class vtkOutlineFilter;
class vtkFeatureEdges;
class vtkPolyData;
class vtkTransform;

class VTK_HYBRID_EXPORT vtkImplicitPlaneWidget : public vtkPolyDataSourceWidget
{
public:
  // Description:
  // Instantiate the object.
  static vtkImplicitPlaneWidget *New();

  vtkTypeRevisionMacro(vtkImplicitPlaneWidget,vtkPolyDataSourceWidget);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Methods that satisfy the superclass' API.
  virtual void SetEnabled(int);
  virtual void PlaceWidget(float bounds[6]);
  void PlaceWidget()
    {this->Superclass::PlaceWidget();}

  // Description:
  // Get the center of the plane.
  void SetCenter(float x, float y, float z);
  void SetCenter(float x[3]);
  float* GetCenter();
  void GetCenter(float xyz[3]);

  // Description:
  // Get the normal to the plane.
  void SetNormal(float x, float y, float z);
  void SetNormal(float x[3]);
  float* GetNormal();
  void GetNormal(float xyz[3]);
  
  // Description:
  // Force the plane widget to be aligned with one of the x-y-z axes.
  // Remember that when the state changes, a ModifiedEvent is invoked.
  // This can be used to snap the plane to the axes if it is orginally
  // not aligned.
  vtkSetMacro(NormalToXAxis,int);
  vtkGetMacro(NormalToXAxis,int);
  vtkBooleanMacro(NormalToXAxis,int);
  vtkSetMacro(NormalToYAxis,int);
  vtkGetMacro(NormalToYAxis,int);
  vtkBooleanMacro(NormalToYAxis,int);
  vtkSetMacro(NormalToZAxis,int);
  vtkGetMacro(NormalToZAxis,int);
  vtkBooleanMacro(NormalToZAxis,int);

  // Description:
  // Turn on/off tubing of the wire outline of the plane. The tube thickens
  // the line by wrapping with a vtkTubeFilter.
  vtkSetMacro(Tubing,int);
  vtkGetMacro(Tubing,int);
  vtkBooleanMacro(Tubing,int);

  // Description:
  // Grab the polydata that defines the plane. The polydata contains a single
  // polygon that is clipped by the bounding box.
  void GetPolyData(vtkPolyData *pd);

  // Description:
  // Satisfies superclass API.  This returns a pointer to the underlying
  // PolyData (which represents the plane).
  vtkPolyDataSource* GetPolyDataSource();
   
  // Description:
  // Get the implicit function for the plane. The user must provide the
  // instance of the class vtkPlane. Note that vtkPlane is a subclass of
  // vtkImplicitFunction, meaning that it can be used by a variety of filters
  // to perform clipping, cutting, and selection of data.
  void GetPlane(vtkPlane *plane);

  // Description:
  // Satisfies the superclass API.  This will change the state of the widget
  // to match changes that have been made to the underlying PolyDataSource
  void UpdatePlacement(void);

  // Description:
  // Get the properties on the normal (line and cone).
  vtkGetObjectMacro(NormalProperty,vtkProperty);
  vtkGetObjectMacro(SelectedNormalProperty,vtkProperty);
  
  // Description:
  // Get the plane properties. The properties of the plane when selected 
  // and unselected can be manipulated.
  vtkGetObjectMacro(PlaneProperty,vtkProperty);
  vtkGetObjectMacro(SelectedPlaneProperty,vtkProperty);

  // Description:
  // Get the property of the outline.
  vtkGetObjectMacro(OutlineProperty,vtkProperty);
  vtkGetObjectMacro(SelectedOutlineProperty,vtkProperty);

  // Description:
  // Get the property of the intersection edges. (This property also
  // applies to the edges when tubed.)
  vtkGetObjectMacro(EdgesProperty,vtkProperty);

protected:
  vtkImplicitPlaneWidget();
  ~vtkImplicitPlaneWidget();

//BTX - manage the state of the widget
  int State;
  enum WidgetState
  {
    Start=0,
    MovingPlane,
    MovingOutline,
    MovingOrigin,
    Scaling,
    Pushing,
    Rotating,
    Outside
  };
//ETX
    
  //handles the events
  static void ProcessEvents(vtkObject* object, unsigned long event,
                            void* clientdata, void* calldata);

  // ProcessEvents() dispatches to these methods.
  void OnLeftButtonDown();
  void OnLeftButtonUp();
  void OnMiddleButtonDown();
  void OnMiddleButtonUp();
  void OnRightButtonDown();
  void OnRightButtonUp();
  void OnMouseMove();

  // Controlling ivars
  int NormalToXAxis;
  int NormalToYAxis;
  int NormalToZAxis;
  void UpdateRepresentation();

  // The actual plane which is being manipulated
  vtkPlane *Plane;

  // The bounding box is represented by a single voxel image data
  vtkImageData      *Box;
  vtkOutlineFilter  *Outline;
  vtkPolyDataMapper *OutlineMapper;
  vtkActor          *OutlineActor;
  void HighlightOutline(int highlight);
  
  // The cut plane is produced with a vtkCutter
  vtkCutter         *Cutter;
  vtkPolyDataMapper *CutMapper;
  vtkActor          *CutActor;
  void HighlightPlane(int highlight);
  
  // Optional tubes are represented by extracting boundary edges and tubing
  vtkFeatureEdges   *Edges;
  vtkTubeFilter     *EdgesTuber;
  vtkPolyDataMapper *EdgesMapper;
  vtkActor          *EdgesActor;
  int               Tubing; //control whether tubing is on

  // The + normal cone
  vtkConeSource     *ConeSource;
  vtkPolyDataMapper *ConeMapper;
  vtkActor          *ConeActor;
  void HighlightNormal(int highlight);

  // The + normal line
  vtkLineSource     *LineSource;
  vtkPolyDataMapper *LineMapper;
  vtkActor          *LineActor;

  // The - normal cone
  vtkConeSource     *ConeSource2;
  vtkPolyDataMapper *ConeMapper2;
  vtkActor          *ConeActor2;

  // The - normal line
  vtkLineSource     *LineSource2;
  vtkPolyDataMapper *LineMapper2;
  vtkActor          *LineActor2;

  // The center positioning handle
  vtkSphereSource   *Sphere;
  vtkPolyDataMapper *SphereMapper;
  vtkActor          *SphereActor;

  // Do the picking
  vtkCellPicker *Picker;
  
  // Transform the normal (used for rotation)
  vtkTransform *Transform;
  
  // Methods to manipulate the plane
  void ConstrainOrigin(float x[3]);
  void Rotate(int X, int Y, double *p1, double *p2, double *vpn);
  void TranslatePlane(double *p1, double *p2);
  void TranslateOutline(double *p1, double *p2);
  void TranslateOrigin(double *p1, double *p2);
  void Push(double *p1, double *p2);
  void Scale(double *p1, double *p2, int X, int Y);
  
  // Initial bounds
  float InitialBounds[6];
  float InitialLength;

  // Properties used to control the appearance of selected objects and
  // the manipulator in general.
  vtkProperty *NormalProperty;
  vtkProperty *SelectedNormalProperty;
  vtkProperty *PlaneProperty;
  vtkProperty *SelectedPlaneProperty;
  vtkProperty *OutlineProperty;
  vtkProperty *SelectedOutlineProperty;
  vtkProperty *EdgesProperty;
  void CreateDefaultProperties();
  
  void GeneratePlane();
  
private:
  vtkImplicitPlaneWidget(const vtkImplicitPlaneWidget&);  //Not implemented
  void operator=(const vtkImplicitPlaneWidget&);  //Not implemented
};

#endif
