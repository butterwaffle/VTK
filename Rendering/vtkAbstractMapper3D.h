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
// .NAME vtkAbstractMapper3D - abstract class specifies interface to map 3D data
// .SECTION Description
// vtkAbstractMapper3D is an abstract class to specify interface between 3D
// data and graphics primitives or software rendering techniques. Subclasses
// of vtkAbstractMapper3D can be used for rendering geometry or rendering
// volumetric data.
//
// This class also defines an API to support hardware clipping planes (at most
// six planes can be defined). It also provides geometric data about the input
// data it maps, such as the bounding box and center.
//
// .SECTION See Also
// vtkAbstractMapper vtkMapper vtkPolyDataMapper vtkVolumeMapper

#ifndef __vtkAbstractMapper3D_h
#define __vtkAbstractMapper3D_h

#include "vtkAbstractMapper.h"

class vtkWindow;
class vtkDataSet;

class VTK_RENDERING_EXPORT vtkAbstractMapper3D : public vtkAbstractMapper
{
public:
  vtkTypeRevisionMacro(vtkAbstractMapper3D,vtkAbstractMapper);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Return bounding box (array of six floats) of data expressed as
  // (xmin,xmax, ymin,ymax, zmin,zmax).
  virtual float *GetBounds()=0;

  // Description:
  // Get the bounds for this mapper as (Xmin,Xmax,Ymin,Ymax,Zmin,Zmax).
  virtual void GetBounds(float bounds[6]);
  
  // Description:
  // Return the Center of this mapper's data.
  float *GetCenter();
  
  // Description:
  // Return the diagonal length of this mappers bounding box.
  float GetLength();

  // Description:
  // Is this a ray cast mapper? A subclass would return 1 if the
  // ray caster is needed to generate an image from this mapper.
  virtual int IsARayCastMapper() {return 0;};

  // Description:
  // Is this a "render into image" mapper? A subclass would return 1 if the
  // mapper produces an image by rendering into a software image buffer.
  virtual int IsARenderIntoImageMapper() {return 0;};

  // Description:
  // Update the network connected to this mapper.
  virtual void Update()=0;

protected:
   vtkAbstractMapper3D();
  ~vtkAbstractMapper3D() {};

  float Bounds[6];
  float Center[3];

private:
  vtkAbstractMapper3D(const vtkAbstractMapper3D&);  // Not implemented.
  void operator=(const vtkAbstractMapper3D&);  // Not implemented.
};

#endif
