#### import the simple module from the visocyte
from visocyte.simple import *
from visocyte import smtesting
from visocyte.vtk.util.misc import vtkGetTempDir
import os, os.path

# Create a sphere and save it as a Houdini file.
sphere = Sphere()
Show()

testDir = vtkGetTempDir()

geoFileName = os.path.join(testDir, "HoudiniWriterData.geo")
writer = CreateWriter(geoFileName, sphere)
writer.UpdatePipeline()

os.remove(geoFileName)
