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
#ifndef __vtkMarchingCubesCases_h
#define __vtkMarchingCubesCases_h
//
// marching cubes case table for generating isosurfaces
//
typedef int EDGE_LIST;
struct VTK_COMMON_EXPORT VTK_TRIANGLE_CASES 
{
  EDGE_LIST edges[16];
  static VTK_TRIANGLE_CASES* GetCases();
};
 
//
// Edges to intersect.  Three at a time form a triangle. Comments at 
// end of line indicate case number (0->255) and base case number (0->15).
//

#endif
