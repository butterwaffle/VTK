from vtkpython import *
import sys, os, os.path

def vtkGetDataRoot():
    
    dataIndex=-1;
    for i in range(0, len(sys.argv)):
        if sys.argv[i] == '-D' and i < len(sys.argv)-1:
            dataIndex = i+1

    if dataIndex != -1:
        dataRoot = sys.argv[dataIndex]
    else:
        try:
            dataRoot = os.environ['VTK_DATA_ROOT']
        except KeyError:
            dataRoot = '../../../../VTKData'

    return dataRoot

    
def vtkRegressionTestImage( renWin ):
    
    imageIndex=-1;
    for i in range(0, len(sys.argv)):
        if sys.argv[i] == '-V' and i < len(sys.argv)-1:
            imageIndex = i+1

    if imageIndex != -1:
        fname = vtkGetDataRoot() + '/' + sys.argv[imageIndex]

        rt_w2if = vtkWindowToImageFilter()
        rt_w2if.SetInput(renWin)

        if os.path.isfile(fname):
            pass
        else:
            rt_pngw = vtkPNGWriter()
            rt_pngw.SetFileName(fname)
            rt_pngw.SetInput(rt_w2if.GetOutput())
            rt_pngw.Write()
            rt_pngw = None

        rt_png = vtkPNGReader()
        rt_png.SetFileName(fname)

        rt_id = vtkImageDifference()
        rt_id.SetInput(rt_w2if.GetOutput())
        rt_id.SetImage(rt_png.GetOutput())
        rt_id.Update()

        if rt_id.GetThresholdedError() <= 10:
            return 1
        else:
            sys.stderr.write('Failed image test: %f\n'
                             % rt_id.GetThresholdedError())
            return 0
    return 2
            
