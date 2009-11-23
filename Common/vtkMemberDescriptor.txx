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
  virtual vtkVariant GetValue( vtkObject* cls, int vtkNotUsed(component) )
    {
    return (static_cast<C_*>( cls )->*GetMethod)();
    }
  virtual void SetValue( vtkObject* cls, vtkVariant val )
    {
    (static_cast<C_*>( cls )->*SetMethod)( vtkVariantConverter( val ) );
    }
  virtual void SetValue( vtkObject* cls, int vtkNotUsed(component), vtkVariant val )
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

template< class C_, typename V_, int d_ >
class VTK_COMMON_EXPORT vtkMemberDescriptorVectorImpl : public vtkMemberDescriptor
{
public:
  virtual int GetNumberOfComponents()
    { return d_; }
  virtual vtkVariant GetValue( vtkObject* cls )
    {
    return this->GetValue( cls, 0 );
    }
  virtual vtkVariant GetValue( vtkObject* cls, int component )
    {
    V_ tmp[d_];
    (static_cast<C_*>( cls )->*GetMethod)( tmp );
    return tmp[component];
    }
  virtual void SetValue( vtkObject* cls, vtkVariant val )
    {
    this->SetValue( cls, 0, val );
    }
  virtual void SetValue( vtkObject* cls, int component, vtkVariant val )
    {
    V_ tmp[d_];
    (static_cast<C_*>( cls )->*GetMethod)( tmp );
    tmp[component] = vtkVariantConverter( val );
    (static_cast<C_*>( cls )->*SetMethod)( tmp );
    }
protected:
  friend class vtkClassDescriptor;
  typedef void (C_::*SetMemberType)( V_[d_] );
  typedef void (C_::*GetMemberType)( V_[d_] );

  vtkMemberDescriptorVectorImpl( vtkStdString name, bool serializable, GetMemberType gmeth, SetMemberType smeth )
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


#endif // __vtkMemberDescriptor_txx
