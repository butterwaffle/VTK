#ifndef __vtkPointMarkStatBox_h
#define __vtkPointMarkStatBox_h

#include "vtkPointMark.h"

// .NAME vtkPointMarkStatBox - draw a statistical box plot mark at a set of points
// .SECTION Description
// This is a subclass of vtkPointMark that draws a statistical box plot glyph
// which consists of a dot at the mean, a rectangle enclosing some portion of the distribution
// to each side of the mean, and a "T" shape marking the distance beyond the rectangle
// comprising the remainder of the distribution.


class VTK_CHARTS_EXPORT vtkPointMarkStatBox : public vtkPointMark
{
public:
  static vtkPointMarkStatBox* New();
  virtual void PrintSelf( ostream& os, vtkIndent indent );
  vtkTypeRevisionMacro(vtkPointMarkStatBox,vtkPointMark);

protected:
  vtkPointMarkStatBox();
  virtual ~vtkPointMarkStatBox();

  virtual void SetupParameters();
  virtual void DrawMarksInternal( vtkContext2D* ctxt,
    vtkDataArray* xc, int xcomp, vtkDataArray* yc, int ycomp,
    vtkIdType ti, vtkIdType te, vtkIdType ts );

private:
  vtkPointMarkStatBox( const vtkPointMarkStatBox& ); // Not implemented.
  void operator = ( const vtkPointMarkStatBox& ); // Not implemented.
};

#endif // __vtkPointMarkStatBox_h
