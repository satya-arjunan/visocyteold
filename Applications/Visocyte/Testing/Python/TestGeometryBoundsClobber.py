# Test visocyte/visocyte#17941.
# Ensure that the bounds that the grid axes uses are always up-to-date.

from visocyte.simple import *

r = CreateRenderView()
r.ViewSize = [300, 300]
r.AxesGrid.Visibility = 1

s = Sphere()
Show()
Render()

# change radius
s.Radius = 0.25
Render()

import os
import sys
try:
  baselineIndex = sys.argv.index('-B')+1
  baselinePath = sys.argv[baselineIndex]
except:
  print("Could not get baseline directory. Test failed.")
baseline_file = os.path.join(baselinePath, "TestGeometryBoundsClobber.png")
from visocyte.vtk.test import Testing
from visocyte.vtk.util.misc import vtkGetTempDir
Testing.VTK_TEMP_DIR = vtkGetTempDir()
Testing.compareImage(r.GetRenderWindow(), baseline_file, threshold=40)
Testing.interact()
