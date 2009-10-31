#ifndef __vtkPointMarkTick_h
#define __vtkPointMarkTick_h

#include "vtkPointMark.h"

// .NAME vtkPointMarkTick - draw tick marks at points (useful for drawing chart axes)
// .SECTION Description
// This is a subclass of vtkPointMark that draws a tick mark glyph at each point.
// Ticks are a single line. The angle of the line to the horizontal may be specified, as well
// as the line width and color. Finally, the justification of the line (is it centered on
// the point, or to one side?) may also be specified.
//
// By default, the Angle of 0.0 is a horizontal line extending to the right of the point
// with increasing angles (up to vtkMath::Pi()) rotating the line counterclockwise and negative
// values (down to -vtkMath::Pi()) rotating the line clockwise.
// Note that when the Justification parameter is set to 0 (centered), values outside of
// [-vtkMath::Pi()/2,vtkMath::Pi()/2] are not unique.

class VTK_CHARTS_EXPORT vtkPointMarkTick : public vtkPointMark
{
public:
  static vtkPointMarkTick* New();
  virtual void PrintSelf( ostream& os, vtkIndent indent );
  vtkTypeRevisionMacro(vtkPointMarkTick,vtkPointMark);

protected:
  vtkPointMarkTick();
  virtual ~vtkPointMarkTick();

  virtual void SetupParameters();
  virtual void DrawMarksInternal( vtkContext2D* ctxt,
    vtkDataArray* xc, int xcomp, vtkDataArray* yc, int ycomp,
    vtkIdType ti, vtkIdType te, vtkIdType ts );

private:
  vtkPointMarkTick( const vtkPointMarkTick& ); // Not implemented.
  void operator = ( const vtkPointMarkTick& ); // Not implemented.
};

#endif // __vtkPointMarkTick_h
