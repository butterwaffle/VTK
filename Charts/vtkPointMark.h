#ifndef __vtkPointMark_h
#define __vtkPointMark_h

#include "vtkObject.h"

#include "vtkVariant.h" // needed for BindParameter()

// .NAME vtkPointMark - draw a mark at a set of points, optionally parameterized on a point-by-point basis
// .SECTION Description
// This class is responsible for drawing a single style of mark (a box, an X, a star, etc.)
// for an array of points.
// Use the class by querying it for adjustable rendering parameters using
// GetNumberOfParameters(), GetParameterName(), and GetParameterInformation().
// Bind a constant or varying value to each parameter (when defaults are not satisfactory)
// using BindParameter().
// Then call DrawMarks() with a context (and an array of coordinates that describe the placement of glyphs?).
//
// Note that this does not draw lines between marks, nor does it handle switching between
// mark subclasses on a per-point basis.
// For that, you should use the yet-to-be-written vtkCurveMark class.
// It also does not handle filling areas.
// For that, you should use the yet-to-be-written vtkPatchMark class.
//
// <h1>Notes on Subclassing</h1>
// If you intend to write a subclass for a new point mark shape, you are responsible for implementing
// SetupParameters() and DrawMarksInternal().
// SetupParameters() must create a vtkInformationObject() for each parameter, set the PARAMETER_TYPE, PARAMETER_NAME, and
// other associated keys on the object, and then call AddParameter().
// It should also call SetParameter() on each parameter to provide a default value.
// DrawMarksInternal() should use the value bound to each parameter to render the specified marks.
//
// You should call SetupParameters() within your subclass' constructor.
//
// <h1>Implementation Notes</h1>
// When AreAllParametersConstant() returns true the same glyph may be pasted at each point coordinate.
// To avoid many repetitive drawing commands for complex glyphs, cairo_push_group() and cairo_pop_group()
// might then be used to create a pattern. The pattern will then be painted at each point.
// Otherwise, DrawMarksInternal() will be called upon to render the glyphs individually at each point coordinate.

class vtkAbstractArray;
class vtkPointMarkInternal;
class vtkContext2D;
class vtkDataArray;
class vtkInformationDoubleVectorKey;
class vtkInformationIntegerKey;
class vtkInformationIntegerVectorKey;
class vtkInformationStringKey;
class vtkInformation;
class vtkVariantArray;

class VTK_CHARTS_EXPORT vtkPointMark : public vtkObject
{
public:
  //static vtkPointMark* Create( const char* markName );
  virtual void PrintSelf( ostream& os, vtkIndent indent );
  vtkTypeRevisionMacro(vtkPointMark,vtkObject);

  static vtkInformationIntegerKey* PARAMETER_TYPE();
  static vtkInformationStringKey* PARAMETER_NAME();
  static vtkInformationIntegerKey* PARAMETER_NUMBER_OF_COMPONENTS();
  static vtkInformationIntegerVectorKey* PARAMETER_INTEGER_BOUNDS();
  static vtkInformationDoubleVectorKey* PARAMETER_REAL_BOUNDS();

  virtual int GetNumberOfParameters();
  virtual const char* GetParameterName( int param );
  virtual int GetParameterHandle( const char* paramName );
  virtual vtkInformation* GetParameterInformation( int param );
  virtual vtkInformation* GetParameterInformation( const char* name )
    { return this->GetParameterInformation( this->GetParameterHandle( name ) ); }
  virtual void ResetParameters();

  virtual void BindParameter( int param, vtkVariant value );
  virtual void BindParameter( const char* paramName, vtkVariant value )
    { this->BindParameter( this->GetParameterHandle( paramName ), value ); }

  virtual void BindParameter( int param, vtkAbstractArray* values, int component );
  virtual void BindParameter( const char* paramName, vtkAbstractArray* values, int component )
    { this->BindParameter( this->GetParameterHandle( paramName ), values, component ); }

  virtual int GetParameterValue( int param, vtkVariant& constVal, vtkAbstractArray*& arrayVal, int& component );
  virtual int GetParameterValue( const char* paramName, vtkVariant& constVal, vtkAbstractArray*& arrayVal, int& component )
    { return this->GetParameterValue( this->GetParameterHandle( paramName ), constVal, arrayVal, component ); }

  virtual void DrawMarks( vtkContext2D* ren,
    vtkDataArray* xc, int xcomp,
    vtkDataArray* yc, int ycomp,
    vtkIdType ti, vtkIdType te, vtkIdType ts );

  // Description:
  // Returns a non-zero value when all parameters are bound to a single value
  // or zero when at least one parameter is bound to a vtkAbstractArray,
  // indicating that the parameter varies with each mark to be drawn.
  virtual int AreAllParametersConstant();

  // Description:
  // Fill the vtkVariantArray with a single row containing a vtkVariant for each component of each parameter value.
  // This is meant for use within DrawMarksInternal.
  void GetParameterValuesForTuple( vtkIdType pt, vtkVariantArray* pvals );

  // Description:
  // Set the coordinates to use for rendering the point marks.
  // The abscissa and ordinate axes may be specified independently or as components of the same array.
  // Note that nominal arrays (i.e., vtkStringArray) cannot be used directly; you must construct
  // a secondary array that maps strings to numeric values first.
  virtual void SetCoordinates(
    vtkDataArray* xCoords, int xComponent,
    vtkDataArray* yCoords, int yComponent,
    vtkIdType tupleStart, vtkIdType tupleEnd, vtkIdType tupleStride );

protected:
  vtkPointMark();
  virtual ~vtkPointMark();

  virtual int AddParameter( vtkInformation* paramInfo );

  virtual void SetupParameters() = 0;
  virtual void DrawMarksInternal( vtkContext2D* ctxt,
    vtkDataArray* xc, int xcomp, vtkDataArray* yc, int ycomp,
    vtkIdType ti, vtkIdType te, vtkIdType ts ) = 0;

  vtkPointMarkInternal* P;

private:
  vtkPointMark( const vtkPointMark& ); // Not implemented.
  void operator = ( const vtkPointMark& ); // Not implemented.
};

#endif // __vtkPointMark_h
