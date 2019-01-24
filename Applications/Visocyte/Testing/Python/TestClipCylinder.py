#### import the simple module from the visocyte
from visocyte.simple import *

# Create a wavelet and clip it with a Cylinder.
wavelet = Wavelet()
Show()

clip = Clip()
clip.ClipType = "Cylinder"
clip.Invert = True

cylinder = clip.ClipType
cylinder.Axis = [-1, 1, -1]
cylinder.Center = [8, 4, -3]
cylinder.Radius = 3
Show()

Render()

# compare with baseline image
import os
import sys
try:
  baselineIndex = sys.argv.index('-B')+1
  baselinePath = sys.argv[baselineIndex]
except:
  print ("Could not get baseline directory. Test failed.")
  exit(1)
baseline_file = os.path.join(baselinePath, "TestClipCylinder.png")

from visocyte.vtk.test import Testing
from visocyte.vtk.util.misc import vtkGetTempDir
Testing.VTK_TEMP_DIR = vtkGetTempDir()
Testing.compareImage(GetActiveView().GetRenderWindow(), baseline_file,
                     threshold=25)
Testing.interact()
