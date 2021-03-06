/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile$
  
-------------------------------------------------------------------------
  Copyright 2008 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
  the U.S. Government retains certain rights in this software.
-------------------------------------------------------------------------

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef __vtkVariantCast_h
#define __vtkVariantCast_h

#include <vtkUnicodeString.h>

// .SECTION Thanks
// Developed by Timothy M. Shead (tshead@sandia.gov) at Sandia National Laboratories.

// Description:
// Converts a vtkVariant to some other type.  Wherever possible, implicit conversions are
// performed, so this method can be used to convert from nearly any type to a string, or
// from a string to nearly any type.  Note that some conversions may fail at runtime, such
// as a conversion from the string "abc" to a numeric type.
//
// The optional 'valid' flag can be used by callers to verify whether conversion succeeded.
template<typename T>
T vtkVariantCast(const vtkVariant& value, bool* valid = 0)
{
  vtkGenericWarningMacro(<< "cannot cast vtkVariant containing " << value.GetTypeAsString() << " to unsupported type.");

  if(valid)
    *valid = false;
  
  static T dummy;
  return dummy;
}

template<>
inline char vtkVariantCast<char>(const vtkVariant& value, bool* valid)
{
  return value.ToChar(valid);
}

template<>
inline unsigned char vtkVariantCast<unsigned char>(const vtkVariant& value, bool* valid)
{
  return value.ToUnsignedChar(valid);
}

template<>
inline short vtkVariantCast<short>(const vtkVariant& value, bool* valid)
{
  return value.ToShort(valid);
}

template<>
inline unsigned short vtkVariantCast<unsigned short>(const vtkVariant& value, bool* valid)
{
  return value.ToUnsignedShort(valid);
}

template<>
inline int vtkVariantCast<int>(const vtkVariant& value, bool* valid)
{
  return value.ToInt(valid);
}

template<>
inline unsigned int vtkVariantCast<unsigned int>(const vtkVariant& value, bool* valid)
{
  return value.ToUnsignedInt(valid);
}

template<>
inline long vtkVariantCast<long>(const vtkVariant& value, bool* valid)
{
  return value.ToLong(valid);
}

template<>
inline unsigned long vtkVariantCast<unsigned long>(const vtkVariant& value, bool* valid)
{
  return value.ToUnsignedLong(valid);
}

#ifdef VTK_TYPE_USE___INT64

template<>
inline __int64 vtkVariantCast<__int64>(const vtkVariant& value, bool* valid)
{
  return value.To__Int64(valid);
}

template<>
inline unsigned __int64 vtkVariantCast<unsigned __int64>(const vtkVariant& value, bool* valid)
{
  return value.ToUnsigned__Int64(valid);
}

#endif


#ifdef VTK_TYPE_USE_LONG_LONG

template<>
inline long long vtkVariantCast<long long>(const vtkVariant& value, bool* valid)
{
  return value.ToLongLong(valid);
}

template<>
inline unsigned long long vtkVariantCast<unsigned long long>(const vtkVariant& value, bool* valid)
{
  return value.ToUnsignedLongLong(valid);
}

#endif

template<>
inline float vtkVariantCast<float>(const vtkVariant& value, bool* valid)
{
  return value.ToFloat(valid);
}

template<>
inline double vtkVariantCast<double>(const vtkVariant& value, bool* valid)
{
  return value.ToDouble(valid);
}

template<>
inline vtkStdString vtkVariantCast<vtkStdString>(const vtkVariant& value, bool* valid)
{
  if(valid)
    *valid = true;

  return value.ToString();
}

template<>
inline vtkUnicodeString vtkVariantCast<vtkUnicodeString>(const vtkVariant& value, bool* valid)
{
  if(valid)
    *valid = true;

  return value.ToUnicodeString();
}

template<>
inline vtkVariant vtkVariantCast<vtkVariant>(const vtkVariant& value, bool* valid)
{
  if(valid)
    *valid = true;

  return value;
}

#endif

