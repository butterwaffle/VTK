SUBDIRS(Cxx)

IF (VTK_WRAP_TCL)
  SUBDIRS(Tcl)
ENDIF (VTK_WRAP_TCL)

IF (VTK_WRAP_PYTHON)
  SUBDIRS(Python)
ENDIF (VTK_WRAP_PYTHON)

IF(PYTHON_EXECUTABLE)
  ADD_TEST(HeaderTesting-Rendering ${PYTHON_EXECUTABLE}
    ${VTK_SOURCE_DIR}/Common/Testing/HeaderTesting.py
    "${VTK_SOURCE_DIR}/Rendering"
    VTK_RENDERING_EXPORT
    vtkCocoaMacOSXSDKCompatibility.h
    vtkCocoaGLView.h
    vtkFreeTypeUtilities.h
    vtkgl.h
    vtkgluPickMatrix.h
    vtkLabelHierarchyPrivate.h
    vtkOpenGL.h
    vtkOpenGLExtensionManagerConfigure.h
    vtkOpenGLState.h
    vtkOpenGLStateCache.h
    vtkPythagoreanQuadruples.h
    vtkRenderState.h
    vtkTDxMotionEventInfo.h
    vtkTkImageViewerWidget.h
    vtkTkInternals.h
    vtkTkRenderWidget.h
    )
ENDIF(PYTHON_EXECUTABLE)
