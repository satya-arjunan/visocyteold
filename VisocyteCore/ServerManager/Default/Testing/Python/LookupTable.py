# Tests a few of the lut API provided by visocyte.simple

from visocyte.simple import *

w = Wavelet()
UpdatePipeline()

ainfo = w.PointData["RTData"]

assert AssignLookupTable(ainfo, "Cool to Warm") == True

try:
    AssignLookupTable(ainfo, "--non-existent--")

    # this should never happen!
    assert False
except RuntimeError:
    pass

assert len(GetLookupTableNames()) > 0
