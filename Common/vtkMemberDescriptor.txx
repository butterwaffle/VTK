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

#ifndef __vtkMemberDescriptor_txx
#define __vtkMemberDescriptor_txx

#include "vtkVariant.h"
#include "vtkMemberDescriptor.h"

template< class C_, typename V_ >
class VTK_COMMON_EXPORT vtkMemberDescriptorImpl : public vtkMemberDescriptor
{
public:
  virtual vtkVariant GetValue( vtkObject* cls )
    {
    return (static_cast<C_*>( cls )->*GetMethod)();
    }
  virtual void SetValue( vtkObject* cls, vtkVariant val )
    {
    (static_cast<C_*>( cls )->*SetMethod)( vtkVariantConverter( val ) );
    }
protected:
  friend class vtkClassDescriptor;
  typedef void (C_::*SetMemberType)( V_ );
  typedef V_ (C_::*GetMemberType)();

  vtkMemberDescriptorImpl( vtkStdString name, bool serializable, GetMemberType gmeth, SetMemberType smeth )
    : vtkMemberDescriptor( name, serializable )
    {
    // NB: C_::ClassDescriptor hasn't been set inside C_::PrepareDescriptor yet, so the next won't work:
    // this->Class = C_::ClassDescriptor;
    this->Class = 0;
    this->SetMethod = smeth;
    this->GetMethod = gmeth;
    }

  SetMemberType SetMethod;
  GetMemberType GetMethod;
};

template< class C_, typename V_ >
class VTK_COMMON_EXPORT vtkClampedMemberDescriptorImpl : public vtkMemberDescriptorImpl<C_,V_>
{
public:
  virtual bool GetClamped() { return true; }
  virtual vtkVariant GetMin()
    {
    return vtkVariant( this->Min );
    }
  virtual vtkVariant GetMax()
    {
    return vtkVariant( this->Max );
    }
protected:
  friend class vtkClassDescriptor;

  vtkClampedMemberDescriptorImpl( vtkStdString name, bool serializable, typename vtkMemberDescriptorImpl<C_,V_>::GetMemberType gmeth, typename vtkMemberDescriptorImpl<C_,V_>::SetMemberType smeth, V_ min, V_ max )
    : vtkMemberDescriptorImpl<C_,V_>( name, serializable, gmeth, smeth )
    {
    this->Min = min;
    this->Max = max;
    }

  V_ Min;
  V_ Max;
};


#endif // __vtkMemberDescriptor_txx
