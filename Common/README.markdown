== Visualization Tool Kit

This is the Visualization Tool Kit (VTK).
See [http://www.vtk.org|the VTK website][] for more information.

=== Descriptor Branch

This branch is dedicated to adding some form of introspection to {{{vtkObject}}}
and its subclasses.
Two new classes, {{{vtkClassDescriptor}}}
and {{{vtkMemberDescriptor}}} are defined.
Each subclass of {{{vtkObject}}} creates an instance of the class descriptor
which can be queried for a list of member descriptors.
These member descriptors enumerate member variables publicly accessible via
SetXXX() and GetXXX() methods.
Each member descriptor may be used to get the current value of a specific instance
and, when the member descriptor is marked as read-write (as opposed to read-only),
the value of that member may also be set for any instance of the class.

Superclass members are reported by each subclass so there is no need to
manually fetch superclasses and request their class descriptors.

Member descriptors whose member values should be clamped may be queried for
the allowable range of values the member may take on.

=== To Do

* Take care of destruction on program exit to make valgrind happy.
* Fix {{{vtkObject::GetClassDescriptor( char* className )}}} since it may
  not work with changes required to work around lazy dynamic library loading.
