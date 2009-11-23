# Visualization Tool Kit

This is the Visualization Tool Kit (VTK).
See [the VTK website](http://www.vtk.org) for more information.

## Descriptor Branch

This branch is dedicated to adding some form of introspection to `vtkObject`
and its subclasses.
Two new classes, `vtkClassDescriptor`
and `vtkMemberDescriptor` are defined.
Each subclass of `vtkObject` may create an instance of the class descriptor
which can be queried for a list of member descriptors.
These member descriptors enumerate member variables publicly accessible via
`SetXXX()` and `GetXXX()` methods.
Each member descriptor may be used to get the current value of a specific instance
and, when the member descriptor is marked as read-write (as opposed to read-only),
the value of that member may also be set for any instance of the class.

Superclass members are reported by each subclass so there is no need to
manually fetch superclasses and request their class descriptors.

Member descriptors whose member values should be clamped may be queried for
the allowable range of values the member may take on.

As an example of how to add a descriptor to any `vtkObject` subclass,
consider the `vtkParametricDini` subclass.
Adding this:
    vtkDescriptorMacro(vtkParametricDini,vtkParametricFunction,
      A RW VTK_DOUBLE ARCHIVE;
      B RW VTK_DOUBLE ARCHIVE;
    );
to a public section of the header and this:
    #include "vtkParametricDiniDescriptor.cxx"
    vtkCxxDescriptorMacro(vtkParametricDini);
to the C++ implementation file will result in 2 member descriptors
being reported for vtkParametricDini (beyond those reported for its
subclasses).

### Specifying members

Inside the `vtkDescriptorMacro`, the final argument consists of a
semicolon-separated list of class members.
Each declaration has 3 mandatory components that must be in the following order

* the name of the member (e.g., `Xxx`);
* the access type, either `RO` (for read-only where only `GetXxx()` is defined)
  or `RW` (for read-write where `SetXxx()` is also defined);
* the storage type, which must be one of the type macros defined in `vtkType.h`.

The storage type may be followed by an integer in square brackets
indicating the member is a vector of the specified dimension.
Optional components of the member specification include

* The `BOOLEAN` keyword to indicate that only true and false values are accepted.
* The `ENUM` keyword followed by a list of space-separated values
  in curly brackets indicates that the member may only take on those discrete values;
* The `MIN` keyword followed by a single value
  (or a space-separated tuple in square brackets for vector-valued members)
  indicates that the member must take on values at or above the given value.
* The `MAX` keyword followed by a single value
  (or a space-separated tuple in square brackets for vector-valued members)
  indicates that the member must take on values at or below the given value.
* The `DEFAULT` keyword followed by a single value
  (or a space-separated tuple in square brackets for vector-valued members)
  indicating the member's initial value on construction.

Note that for properties with the storage type VTK_VARIANT, any values specified
using the optional keywords above are parsed like so:

* The keyword `INVALID` (or any case-insensitive variation) indicates a
  vtkVariant with no valid value assigned. No quotation marks of any sort
  should be placed around the keyword.
* Any value inside single or double quotation marks is treated as a string value
* Any other value is treated as numeric if conversion is possible or a string otherwise.
  In the future, conventions for different integer number bases may be possible but
  for now, this is not so.

Some examples include:

    Foober RW VTK_DOUBLE[3] MIN [ 0 0 0 ] MAX [ 1 1 1 ] DEFAULT [ 0 0 0 ] ARCHIVE;
    Goober RO VTK_UNICODE_STRING ENUM { 'Deadly Cold' 'Not So Hot' 'LP On Fire' };
    Hoober RW VTK_UNICODE_STRING[2] ENUM { [ 'a' 'b' ] [ 'p' 'q' ] [ 'x' 'y' ] };

## To Do

* Take care of destruction on program exit to make valgrind happy.
* Fix `vtkObject::GetClassDescriptor( char* className )` since it may
  not work with changes required to work around lazy dynamic library loading.
