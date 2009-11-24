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
// .NAME vtkClassDescriptor - meta-class for describing descendants of vtkObject
// .SECTION Description
// Each instance of this class keeps an inventory of member variables
// (along with functions to get and set their values) for vtkObject or
// one of its subclasses.

// .SECTION See also
// vtkMemberDescriptor

#ifndef __vtkClassDescriptor_h
#define __vtkClassDescriptor_h

#include "vtkStdString.h"
#include <vtkstd/vector>

class vtkMemberDescriptor;
template< class C_, typename V_ > class vtkMemberDescriptorObjImpl;
template< class C_, typename V_ > class vtkMemberDescriptorImpl;
template< class C_, typename V_, int d_ > class vtkMemberDescriptorVectorImpl;
template< class C_ > class vtkMemberDescriptorVoidPImpl;
class vtkClassDescriptorP;

class VTK_COMMON_EXPORT vtkClassDescriptor
{
public:
  static vtkClassDescriptor* RegisterClass( vtkStdString name, vtkClassDescriptor* superclassDescriptor );

  vtkClassDescriptor();
  virtual ~vtkClassDescriptor();

  /// Add a member that points to an opaque type (implemented as a void pointer).
  template< class C_ >
  vtkMemberDescriptor* AddOpaquePointerMember(
    vtkStdString name,
    bool serializable,
    typename vtkMemberDescriptorVoidPImpl<C_>::GetMemberType gmeth,
    typename vtkMemberDescriptorVoidPImpl<C_>::SetMemberType smeth );

  /// Add a member that is a primitive type (and singular, not array-valued).
  template< class C_, typename V_ >
  vtkMemberDescriptor* AddPrimitiveMember(
    vtkStdString name,
    bool serializable,
    typename vtkMemberDescriptorImpl<C_,V_>::GetMemberType gmeth,
    typename vtkMemberDescriptorImpl<C_,V_>::SetMemberType smeth );

  /// Add a member that is a primitive type (and singular, not array-valued) with a minimum and maximum specified.
  template< class C_, typename V_ >
  vtkMemberDescriptor* AddPrimitiveMember(
    vtkStdString name,
    bool serializable,
    typename vtkMemberDescriptorImpl<C_,V_>::GetMemberType gmeth,
    typename vtkMemberDescriptorImpl<C_,V_>::SetMemberType smeth,
    V_ min,
    V_ max );

  /// Add a member that is a vector of primitive types.
  template< class C_, typename V_, int d_ >
  vtkMemberDescriptor* AddPrimitiveVectorMember(
    vtkStdString name,
    bool serializable,
    typename vtkMemberDescriptorVectorImpl<C_,V_,d_>::GetMemberType gmeth,
    typename vtkMemberDescriptorVectorImpl<C_,V_,d_>::SetMemberType smeth );

  /// Add a member that is a vector of primitive types with a minimum and maximum specified.
  template< class C_, typename V_, int d_ >
  vtkMemberDescriptor* AddPrimitiveVectorMember(
    vtkStdString name,
    bool serializable,
    typename vtkMemberDescriptorVectorImpl<C_,V_,d_>::GetMemberType gmeth,
    typename vtkMemberDescriptorVectorImpl<C_,V_,d_>::SetMemberType smeth,
    V_ min[d_],
    V_ max[d_] );

  /// Add a member that points to a subclass of vtkObject.
  template< class C_, typename V_ >
  vtkMemberDescriptor* AddObjectMember(
    vtkStdString name,
    bool serializable,
    typename vtkMemberDescriptorObjImpl<C_,V_>::GetMemberType gmeth,
    typename vtkMemberDescriptorObjImpl<C_,V_>::SetMemberType smeth );

  int GetNumberOfMembers();
  vtkMemberDescriptor* GetMemberDescriptor( int i );
  vtkMemberDescriptor* GetMemberDescriptor( vtkStdString memberName );

  /// Return the class name.
  vtkStdString GetName() { return this->Name; }

  /// Return an object describing attributes of a class.
  static vtkClassDescriptor* GetClassDescriptor( const char* className );

  /// Destroy class descriptors to make memory allocation debuggers happy. Normally called for you by an atexit() callback.
  static void CleanupClassDescriptors();

protected:
  vtkStdString Name;
  vtkClassDescriptor* Superclass;
  vtkstd::vector<vtkMemberDescriptor*> Descriptors;
  static vtkClassDescriptorP* ClassInternals; // Map of vtkClassDescriptors for vtkObject and all its descendants
};

#endif // __vtkClassDescriptor_h
