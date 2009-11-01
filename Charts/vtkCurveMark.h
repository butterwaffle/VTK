#ifndef __vtkCurveMark_h
#define __vtkCurveMark_h

#include "vtkPointMark.h"

// .NAME vtkCurveMark - draw lines between a set of points
// .SECTION Description
// This is a subclass of vtkPointMark that doesn't draw marks.
// Instead it draws lines between the marks.
// The line width, line color, join style, cap style, and dash
// pattern may be varied from segment to segment.
//
// Because  doesn't currently provide a way to draw lines with
// varying width or dash length, those parameters will take on their
// value at the left endpoint of the line segment for the entire segment.

class VTK_CHARTS_EXPORT vtkCurveMark : public vtkPointMark
{
public:
  static vtkCurveMark* New();
  virtual void PrintSelf( ostream& os, vtkIndent indent );
  vtkTypeRevisionMacro(vtkCurveMark,vtkPointMark);

  virtual void DrawMarks( vtkContext2D* ctxt,
    vtkDataArray* xc, int xcomp, vtkDataArray* yc, int ycomp,
    vtkIdType ti, vtkIdType te, vtkIdType ts );

protected:
  vtkCurveMark();
  virtual ~vtkCurveMark();

  virtual void SetupParameters();
  virtual void DrawMarksInternal( vtkContext2D* ctxt,
    vtkDataArray* xc, int xcomp, vtkDataArray* yc, int ycomp,
    vtkIdType ti, vtkIdType te, vtkIdType ts );

private:
  vtkCurveMark( const vtkCurveMark& ); // Not implemented.
  void operator = ( const vtkCurveMark& ); // Not implemented.
};

#endif // __vtkCurveMark_h
