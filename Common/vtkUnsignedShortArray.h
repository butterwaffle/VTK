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
// .NAME vtkUnsignedShortArray - dynamic, self-adjusting unsigned short integer array
// .SECTION Description
// vtkUnsignedShortArray is an array of unsigned short integer numbers. It
// provides methods for insertion and retrieval of integer values, and will
// automatically resize itself to hold new data.

#ifndef __vtkUnsignedShortArray_h
#define __vtkUnsignedShortArray_h

#include "vtkDataArray.h"

class VTK_COMMON_EXPORT vtkUnsignedShortArray : public vtkDataArray 
{
public:
  static vtkUnsignedShortArray *New();

  vtkTypeRevisionMacro(vtkUnsignedShortArray,vtkDataArray);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Allocate memory for this array. Delete old storage only if necessary.
  // Note that ext is no longer used.
  int Allocate(const vtkIdType sz, const vtkIdType ext=1000);

  // Description:
  // Release storage and reset array to initial state.
  void Initialize();

  // Description:
  // Create the same type object as this (virtual constructor).
  vtkDataArray *MakeObject();

  // Description:
  // Get the data type.
  int GetDataType() {return VTK_UNSIGNED_SHORT;};

  // Description:
  // Set the number of n-tuples in the array.
  void SetNumberOfTuples(const vtkIdType number);

  // Description:
  // Get a pointer to a tuple at the ith location. This is a dangerous method
  // (it is not thread safe since a pointer is returned).
  float *GetTuple(const vtkIdType i);

  // Description:
  // Copy the tuple value into a user-provided array.
  void GetTuple(const vtkIdType i, float * tuple);
  void GetTuple(const vtkIdType i, double * tuple);

  // Description:
  // Set the tuple value at the ith location in the array.
  void SetTuple(const vtkIdType i, const float * tuple);
  void SetTuple(const vtkIdType i, const double * tuple);

  // Description:
  // Insert (memory allocation performed) the tuple into the ith location
  // in the array.
  void InsertTuple(const vtkIdType i, const float * tuple);
  void InsertTuple(const vtkIdType i, const double * tuple);
  
  // Description:
  // Insert (memory allocation performed) the tuple onto the end of the array.
  vtkIdType InsertNextTuple(const float * tuple);
  vtkIdType InsertNextTuple(const double * tuple);

  // Description:
  // Return the data component at the ith tuple and jth component location.
  // Note that i is less than NumberOfTuples and j is less than NumberOfComponents.
  float GetComponent(const vtkIdType i, const int j);

  // Description:
  // Set the data component at the ith tuple and jth component location.
  // Note that i is less than NumberOfTuples and j is less than 
  // NumberOfComponents. Make sure enough memory has been allocated (use 
  // SetNumberOfTuples() and SetNumberOfComponents()).
  void SetComponent(const vtkIdType i, const int j, const float c);

  // Description:
  // Insert the data component at ith tuple and jth component location. 
  // Note that memory allocation is performed as necessary to hold the data.
  void InsertComponent(const vtkIdType i, const int j, const float c);

  // Description:
  // Get the data at a particular index.
  unsigned short GetValue(const vtkIdType id) {return this->Array[id];};

  // Description:
  // Set the data at a particular index. Does not do range checking. Make sure
  // you use the method SetNumberOfValues() before inserting data.
  void SetValue(const vtkIdType id, const unsigned short value) {
    this->Array[id] = value;};
  
  // Description:
  // Specify the number of values for this object to hold. Does an
  // allocation as well as setting the MaxId ivar. Used in conjunction with
  // SetValue() method for fast insertion.
  void SetNumberOfValues(const vtkIdType number);

  // Description:
  // Insert data at a specified position in the array.
  void InsertValue(const vtkIdType id, const unsigned short i);

  // Description:
  // Insert data at the end of the array. Return its location in the array.
  vtkIdType InsertNextValue(const unsigned short);

  // Description:
  // Get the address of a particular data index. Performs no checks
  // to verify that the memory has been allocated etc.
  unsigned short *GetPointer(const vtkIdType id) {return this->Array + id;}
  void *GetVoidPointer(const vtkIdType id)
    {return (void *)this->GetPointer(id);};

  // Description:
  // Get the address of a particular data index. Make sure data is allocated
  // for the number of items requested. Set MaxId according to the number of
  // data values requested.
  unsigned short *WritePointer(const vtkIdType id, const vtkIdType number);

  // Description:
  // Deep copy of another unsigned short array.
  void DeepCopy(vtkDataArray *ia);

  // Description:
  // This method lets the user specify data to be held by the array.  The 
  // array argument is a pointer to the data.  size is the size of 
  // the array supplied by the user.  Set save to 1 to keep the class
  // from deleting the array when it cleans up or reallocates memory.
  // The class uses the actual array provided; it does not copy the data 
  // from the suppled array.
  void SetArray(unsigned short* array, vtkIdType size, int save);
  void SetVoidArray(void *array, vtkIdType size, int save) 
    {this->SetArray((unsigned short*)array, size, save);};

  // Description:
  // Resize object to just fit data requirement. Reclaims extra memory.
  void Squeeze() {this->ResizeAndExtend (this->MaxId+1);};

  // Description:
  // Resize the array while conserving the data.
  virtual void Resize(vtkIdType numTuples);

#ifndef VTK_REMOVE_LEGACY_CODE
  // Description:
  // For legacy compatibility. Do not use.
  void DeepCopy(vtkDataArray &ia) 
    {VTK_LEGACY_METHOD(DeepCopy,"3.2"); this->DeepCopy(&ia);}
#endif
  

protected:
  vtkUnsignedShortArray(vtkIdType numComp=1);
  ~vtkUnsignedShortArray();

  unsigned short *Array;   // pointer to data
  unsigned short *ResizeAndExtend(const vtkIdType sz);
    // function to resize data

  int TupleSize; //used for data conversion
  float *Tuple;

  int SaveUserArray;
private:
  vtkUnsignedShortArray(const vtkUnsignedShortArray&);  // Not implemented.
  void operator=(const vtkUnsignedShortArray&);  // Not implemented.
};

inline void vtkUnsignedShortArray::SetNumberOfValues(const vtkIdType number) 
{
  this->Allocate(number);
  this->MaxId = number - 1;
}

inline unsigned short *vtkUnsignedShortArray::WritePointer(const vtkIdType id,
                                                           const vtkIdType number) 
{
  vtkIdType newSize=id+number;
  if ( newSize > this->Size )
    {
    this->ResizeAndExtend(newSize);
    }
  if ( (--newSize) > this->MaxId )
    {
    this->MaxId = newSize;
    }
  return this->Array + id;
}

inline void vtkUnsignedShortArray::InsertValue(const vtkIdType id,
                                               const unsigned short i)
{
  if ( id >= this->Size )
    {
    this->ResizeAndExtend(id+1);
    }
  this->Array[id] = i;
  if ( id > this->MaxId )
    {
    this->MaxId = id;
    }
}

inline vtkIdType vtkUnsignedShortArray::InsertNextValue(const unsigned short i)
{
  this->InsertValue (++this->MaxId,i); 
  return this->MaxId;
}

#endif




