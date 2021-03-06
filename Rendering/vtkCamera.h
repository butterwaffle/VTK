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
// .NAME vtkCamera - a virtual camera for 3D rendering
// .SECTION Description
// vtkCamera is a virtual camera for 3D rendering. It provides methods
// to position and orient the view point and focal point. Convenience 
// methods for moving about the focal point also are provided. More 
// complex methods allow the manipulation of the computer graphics
// model including view up vector, clipping planes, and 
// camera perspective.
// .SECTION See Also
// vtkPerspectiveTransform

#ifndef __vtkCamera_h
#define __vtkCamera_h

#include "vtkObject.h"

class vtkMatrix4x4;
class vtkPerspectiveTransform;
class vtkRenderer;
class vtkTransform;
class vtkHomogeneousTransform;

class VTK_RENDERING_EXPORT vtkCamera : public vtkObject
{
 public:
  void PrintSelf(ostream& os, vtkIndent indent);
  vtkTypeRevisionMacro(vtkCamera,vtkObject);

  // Description:
  // Construct camera instance with its focal point at the origin, 
  // and position=(0,0,1). The view up is along the y-axis, 
  // view angle is 30 degrees, and the clipping range is (.1,1000).
  static vtkCamera *New();

  // Description:
  // Set/Get the position of the camera in world coordinates.
  // The default position is (0,0,1).
  void SetPosition(double x, double y, double z);
  void SetPosition(const double a[3]) {
    this->SetPosition(a[0], a[1], a[2]); };
  vtkGetVector3Macro(Position,double);

  // Description:
  // Set/Get the focal of the camera in world coordinates.
  // The default focal point is the origin.
  void SetFocalPoint(double x, double y, double z);
  void SetFocalPoint(const double a[3]) {
    this->SetFocalPoint(a[0], a[1], a[2]);};
  vtkGetVector3Macro(FocalPoint,double);

  // Description:
  // Set/Get the view up direction for the camera.  The default
  // is (0,1,0).
  void SetViewUp(double vx, double vy, double vz);
  void SetViewUp(const double a[3]) {
    this->SetViewUp(a[0], a[1], a[2]); }
  vtkGetVector3Macro(ViewUp,double);

  // Description:
  // Recompute the ViewUp vector to force it to be perpendicular to
  // camera->focalpoint vector.  Unless you are going to use
  // Yaw or Azimuth on the camera, there is no need to do this.
  void OrthogonalizeViewUp();

  // Description:
  // Move the focal point so that it is the specified distance from
  // the camera position.  This distance must be positive.
  void SetDistance(double);

  // Description:
  // Return the distance from the camera position to the focal point.
  // This distance is positive.
  vtkGetMacro(Distance,double);

  // Description:
  // Get the vector in the direction from the camera position to the
  // focal point.  This is usually the opposite of the ViewPlaneNormal,
  // the vector perpendicular to the screen, unless the view is oblique.
  vtkGetVector3Macro(DirectionOfProjection,double);

  // Description:
  // Move the position of the camera along the direction of projection. Moving
  // towards the focal point (e.g., greater than 1) is a dolly-in, moving away
  // from the focal point (e.g., less than 1) is a dolly-out.
  void Dolly(double distance);

  // Description:
  // Set the roll angle of the camera about the direction of projection.
  void SetRoll(double angle);
  double GetRoll();

  // Description:
  // Rotate the camera about the direction of projection.
  void Roll(double angle);

  // Description:
  // Rotate the camera about the view up vector centered at the focal point.
  // Note that the view up vector is not necessarily perpendicular to the
  // direction of projection.
  void Azimuth(double angle);

  // Description:
  // Rotate the focal point about the view up vector centered at the camera's
  // position. Note that the view up vector is not necessarily perpendicular
  // to the direction of projection.
  void Yaw(double angle);

  // Description:
  // Rotate the camera about the cross product of the direction of projection
  // and the view up vector centered on the focal point.
  void Elevation(double angle);

  // Description:
  // Rotate the focal point about the cross product of the view up vector
  // and the direction of projection, centered at the camera's position.
  void Pitch(double angle);

  // Description:
  // Set/Get the value of the ParallelProjection instance variable. This
  // determines if the camera should do a perspective or parallel projection.
  void SetParallelProjection(int flag);
  vtkGetMacro(ParallelProjection,int);
  vtkBooleanMacro(ParallelProjection,int);

  // Description:
  // Set/Get the value of the UseHorizontalViewAngle instance variable. If
  // set, the camera's view angle represents a horizontal view angle, rather
  // than the default vertical view angle. This is useful if the application
  // uses a display device which whose specs indicate a particular horizontal
  // view angle, or if the application varies the window height but wants to
  // keep the perspective transform unchanges.
  void SetUseHorizontalViewAngle(int flag);
  vtkGetMacro(UseHorizontalViewAngle, int);
  vtkBooleanMacro(UseHorizontalViewAngle, int);

  // Description:
  // Set/Get the camera view angle, which is the angular height of the
  // camera view measured in degrees.  The default angle is 30 degrees.
  // This method has no effect in parallel projection mode.
  // The formula for setting the angle up for perfect perspective viewing
  // is: angle = 2*atan((h/2)/d) where h is the height of the RenderWindow
  // (measured by holding a ruler up to your screen) and d is the
  // distance from your eyes to the screen.
  void SetViewAngle(double angle);
  vtkGetMacro(ViewAngle,double);

  // Description:
  // Set/Get the scaling used for a parallel projection, i.e. the height
  // of the viewport in world-coordinate distances. The default is 1.
  // Note that the "scale" parameter works as an "inverse scale" ---
  // larger numbers produce smaller images.
  // This method has no effect in perspective projection mode.
  void SetParallelScale(double scale);
  vtkGetMacro(ParallelScale,double);

  // Description:
  // In perspective mode, decrease the view angle by the specified factor.
  // In parallel mode, decrease the parallel scale by the specified factor.
  // A value greater than 1 is a zoom-in, a value less than 1 is a zoom-out.
  void Zoom(double factor);

  // Description:
  // Set/Get the location of the near and far clipping planes along the
  // direction of projection.  Both of these values must be positive.
  // How the clipping planes are set can have a large impact on how 
  // well z-buffering works.  In particular the front clipping
  // plane can make a very big difference. Setting it to 0.01 when it
  // really could be 1.0 can have a big impact on your z-buffer resolution
  // farther away.  The default clipping range is (0.1,1000).
  void SetClippingRange(double dNear, double dFar);
  void SetClippingRange(const double a[2]) {
    this->SetClippingRange(a[0], a[1]); };
  vtkGetVector2Macro(ClippingRange,double);

  // Description:
  // Set the distance between clipping planes.  This method adjusts the 
  // far clipping plane to be set a distance 'thickness' beyond the
  // near clipping plane.
  void SetThickness(double);
  vtkGetMacro(Thickness,double);

  // Description:
  // Set/Get the center of the window in viewport coordinates.
  // The viewport coordinate range is ([-1,+1],[-1,+1]).  This method
  // is for if you have one window which consists of several viewports,
  // or if you have several screens which you want to act together as
  // one large screen.
  void SetWindowCenter(double x, double y);
  vtkGetVector2Macro(WindowCenter,double);

  // Description:
  // Get/Set the oblique viewing angles.  The first angle, alpha, is the
  // angle (measured from the horizontal) that rays along the direction
  // of projection will follow once projected onto the 2D screen.  
  // The second angle, beta, is the angle between the view plane and
  // the direction of projection.  This creates a shear transform
  // x' = x + dz*cos(alpha)/tan(beta), y' = dz*sin(alpha)/tan(beta)
  // where dz is the distance of the point from the focal plane.
  // The angles are (45,90) by default.  Oblique projections 
  // commonly use (30,63.435).
  void SetObliqueAngles(double alpha, double beta);

  // Description:
  // Apply a transform to the camera.  The camera position, focal-point,
  // and view-up are re-calculated using the transform's matrix to
  // multiply the old points by the new transform.
  void ApplyTransform(vtkTransform *t);

  // Description:
  // Get the ViewPlaneNormal.  This vector will point opposite to
  // the direction of projection, unless you have created an sheared output
  // view using SetViewShear/SetObliqueAngles.
  vtkGetVector3Macro(ViewPlaneNormal,double);

  // Description:
  // Set/get the shear transform of the viewing frustum.  Parameters are
  // dx/dz, dy/dz, and center.  center is a factor that describes where
  // to shear around. The distance dshear from the camera where
  // no shear occurs is given by (dshear = center * FocalDistance).
  void SetViewShear(double dxdz, double dydz, double center);
  void SetViewShear(double d[3]);
  vtkGetVector3Macro(ViewShear, double);

  // Description:
  // Set/Get the separation between eyes (in degrees). This is used
  // when generating stereo images.
  vtkSetMacro(EyeAngle,double);
  vtkGetMacro(EyeAngle,double);

  // Description:
  // Set the size of the cameras lens in world coordinates. This is only
  // used when the renderer is doing focal depth rendering. When that is
  // being done the size of the focal disk will effect how significant the
  // depth effects will be.
  vtkSetMacro(FocalDisk,double);
  vtkGetMacro(FocalDisk,double);

  // Description:
  // Return the matrix of the view transform.
  // The ViewTransform depends on only three ivars:  the Position, the
  // FocalPoint, and the ViewUp vector.  All the other methods are there
  // simply for the sake of the users' convenience.
  virtual vtkMatrix4x4 *GetViewTransformMatrix();

  // Description:
  // Return the view transform.
  // The ViewTransform depends on only three ivars:  the Position, the
  // FocalPoint, and the ViewUp vector.  All the other methods are there
  // simply for the sake of the users' convenience.
  virtual vtkTransform *GetViewTransformObject()
    {
      return this->ViewTransform;
    }
  
  // Description:
  // Return the projection transform matrix, which converts from camera
  // coordinates to viewport coordinates.  The 'aspect' is the
  // width/height for the viewport, and the nearz and farz are the
  // Z-buffer values that map to the near and far clipping planes.
  // The viewport coordinates of a point located inside the frustum are in the
  // range ([-1,+1],[-1,+1],[nearz,farz]).
  // WARNING: the name of the method is wrong, it should be
  // GetProjectionTransformMatrix() (it is used also in parallel projection)
  // @deprecated Replaced by GetProjectionTransformMatrix() as of VTK 5.4.
  VTK_LEGACY(virtual vtkMatrix4x4 *GetPerspectiveTransformMatrix(double aspect,
                                                                 double nearz,
                                                                 double farz));
  
  // Description:
  // Return the projection transform matrix, which converts from camera
  // coordinates to viewport coordinates.  The 'aspect' is the
  // width/height for the viewport, and the nearz and farz are the
  // Z-buffer values that map to the near and far clipping planes.
  // The viewport coordinates of a point located inside the frustum are in the
  // range ([-1,+1],[-1,+1],[nearz,farz]).
   virtual vtkMatrix4x4 *GetProjectionTransformMatrix(double aspect,
                                                      double nearz,
                                                      double farz);
  
  // Description:
  // Return the projection transform matrix, which converts from camera
  // coordinates to viewport coordinates.  The 'aspect' is the
  // width/height for the viewport, and the nearz and farz are the
  // Z-buffer values that map to the near and far clipping planes.
  // The viewport coordinates of a point located inside the frustum are in the
  // range ([-1,+1],[-1,+1],[nearz,farz]).
  virtual vtkPerspectiveTransform *GetProjectionTransformObject(double aspect,
                                                                double nearz,
                                                                double farz);
  

  // Description:
  // Return the concatenation of the ViewTransform and the
  // ProjectionTransform.  This transform will convert world
  // coordinates to viewport coordinates.  The 'aspect' is the
  // width/height for the viewport, and the nearz and farz are the
  // Z-buffer values that map to the near and far clipping planes.
  // The viewport coordinates of a point located inside the frustum are in the
  // range ([-1,+1],[-1,+1],[nearz,farz]).
  // WARNING: the name of the method is wrong, it should be
  // GetCompositeProjectionTransformMatrix() (it is used also in parallel
  // projection)
  // @deprecated Replaced by GetCompositeProjectionTransformMatrix() as of
  // VTK 5.4.
  VTK_LEGACY(virtual vtkMatrix4x4 *GetCompositePerspectiveTransformMatrix(
               double aspect,
               double nearz,
               double farz));
  
  // Description:
  // Return the concatenation of the ViewTransform and the
  // ProjectionTransform.  This transform will convert world
  // coordinates to viewport coordinates.  The 'aspect' is the
  // width/height for the viewport, and the nearz and farz are the
  // Z-buffer values that map to the near and far clipping planes.
  // The viewport coordinates of a point located inside the frustum are in the
  // range ([-1,+1],[-1,+1],[nearz,farz]).
  virtual vtkMatrix4x4 *GetCompositeProjectionTransformMatrix(double aspect,
                                                              double nearz,
                                                              double farz);

  
  // Description:
  // In addition to the instance variables such as position and orientation,
  // you can add an additional transformation for your own use.  This
  // transformation is concatenated to the camera's PerspectiveTransform
  void SetUserTransform(vtkHomogeneousTransform *transform);
  vtkGetObjectMacro(UserTransform,vtkHomogeneousTransform);

  // Description:
  // This method causes the camera to set up whatever is required for
  // viewing the scene. This is actually handled by an subclass of
  // vtkCamera, which is created through New()
  virtual void Render(vtkRenderer *) {}

  // Description:
  // Return the MTime that concerns recomputing the view rays of the camera.
  unsigned long GetViewingRaysMTime();

  // Description:
  // Mark that something has changed which requires the view rays
  // to be recomputed.
  void ViewingRaysModified();

  // Description:
  // Get the plane equations that bound the view frustum.
  // The plane normals point inward. The planes array contains six
  // plane equations of the form (Ax+By+Cz+D=0), the first four
  // values are (A,B,C,D) which repeats for each of the planes.
  // The planes are given in the following order: -x,+x,-y,+y,-z,+z.
  // Warning: it means left,right,bottom,top,far,near (NOT near,far)
  // The aspect of the viewport is needed to correctly compute the planes
  virtual void GetFrustumPlanes(double aspect, double planes[24]);

  // Description:
  // Get the orientation of the camera.
  double *GetOrientation();
  double *GetOrientationWXYZ();

  // Description:
  // @deprecated The view plane normal is automatically set from the
  // DirectionOfProjection according to the ViewShear.
  VTK_LEGACY(void SetViewPlaneNormal(double x, double y, double z));
  VTK_LEGACY(void SetViewPlaneNormal(const double a[3]));

  // Description:
  // This method is called automatically whenever necessary, it
  // should never be used outside of vtkCamera.cxx.
  void ComputeViewPlaneNormal();

  // Description:
  // Returns a transformation matrix for a coordinate frame attached to
  // the camera, where the camera is located at (0, 0, 1) looking at the
  // focal point at (0, 0, 0), with up being (0, 1, 0).
  vtkMatrix4x4 *GetCameraLightTransformMatrix();

  // Description:
  // Update the viewport
  virtual void UpdateViewport(vtkRenderer *vtkNotUsed(ren)) {}

  // Description:
  // Set the Left Eye setting
  vtkSetMacro(LeftEye,int);
  vtkGetMacro(LeftEye,int);

  // Description:
  // Copy the properties of `source' into `this'.
  // Copy pointers of matrices.
  // \pre source_exists!=0
  // \pre not_this: source!=this
  void ShallowCopy(vtkCamera *source);
  
  // Description:
  // Copy the properties of `source' into `this'.
  // Copy the contents of the matrices.
  // \pre source_exists!=0
  // \pre not_this: source!=this
  void DeepCopy(vtkCamera *source);
  
protected:
  vtkCamera();
  ~vtkCamera();

  // Description:
  // These methods should only be used within vtkCamera.cxx.
  void ComputeDistance();
  void ComputeViewTransform();
  
#ifndef VTK_LEGACY_REMOVE
  // Description:
  // @deprecated Replaced by ComputeProjectionTransform() as of VTK 5.4.
  void ComputePerspectiveTransform(double aspect,
                                   double nearz,
                                   double farz);
  
  // Description:
  // @deprecated Replaced by ComputeCompositeProjectionTransform() as of
  // VTK 5.4.
  void ComputeCompositePerspectiveTransform(double aspect,
                                            double nearz,
                                            double farz);
#endif
  
  // Description:
  // These methods should only be used within vtkCamera.cxx.
  void ComputeProjectionTransform(double aspect,
                                  double nearz,
                                  double farz);
  
  // Description:
  // These methods should only be used within vtkCamera.cxx.
  void ComputeCompositeProjectionTransform(double aspect,
                                           double nearz,
                                           double farz);
  
  
  void ComputeCameraLightTransform();

  // Description:
  // Copy the ivars. Do nothing for the matrices.
  // Called by ShallowCopy() and DeepCopy()
  // \pre source_exists!=0
  // \pre not_this: source!=this
  void PartialCopy(vtkCamera *source);
  
  double WindowCenter[2];
  double ObliqueAngles[2];
  double FocalPoint[3];
  double Position[3];
  double ViewUp[3];
  double ViewAngle;
  double ClippingRange[2];
  double EyeAngle;
  int    ParallelProjection;
  double ParallelScale;
  int    Stereo;
  int    LeftEye;
  double Thickness;
  double Distance;
  double DirectionOfProjection[3];
  double ViewPlaneNormal[3];
  double ViewShear[3];
  int    UseHorizontalViewAngle;
  vtkHomogeneousTransform *UserTransform;

  vtkTransform *ViewTransform;
  vtkPerspectiveTransform *ProjectionTransform;
  vtkPerspectiveTransform *Transform;
  vtkTransform *CameraLightTransform;

  double FocalDisk;

  // ViewingRaysMtime keeps track of camera modifications which will 
  // change the calculation of viewing rays for the camera before it is 
  // transformed to the camera's location and orientation. 
  vtkTimeStamp ViewingRaysMTime;
private:
  vtkCamera(const vtkCamera&);  // Not implemented.
  void operator=(const vtkCamera&);  // Not implemented.
};

#endif

