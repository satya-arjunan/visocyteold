# Macro for BUG #11065. This makes it possible to show the grid for a dataset in
# the background.
try: visocyte.simple
except: from visocyte.simple import *
visocyte.simple._DisableFirstRenderCameraReset()

spcth_0 = GetActiveSource()
ExtractSurface2 = ExtractSurface()
DataRepresentation5 = Show()
DataRepresentation5.Representation = 'Wireframe'
DataRepresentation5.BackfaceRepresentation = 'Cull Frontface'
