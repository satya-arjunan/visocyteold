from visocyte.simple import *
import visocyte

# This test tests backwards compatibility for ColorArrayName and ColorAttributeType
# properties.

assert (visocyte.compatibility.GetVersion().GetVersion() == None),\
    "ParaView modules should never force backwards compatibility to any version"
assert ((visocyte.compatibility.GetVersion() < 4.1) == False),\
    "less-than test should always fail when version is not specified."
assert ((visocyte.compatibility.GetVersion() <= 4.1) == False),\
    "less-equal test should always fail when version is not specified."
assert ((visocyte.compatibility.GetVersion() > 4.1) == True),\
    "greater-than test should always pass when version is not specified."
assert ((visocyte.compatibility.GetVersion() >= 4.1) == True),\
    "greater-equal test should always pass when version is not specified."

Sphere()
r = Show()
v = GetActiveView()

assert (len(r.ColorArrayName) == 2),\
    "'ColorArrayName' must be a 2-tuple"

try:
    a = r.ColorAttributeType
except visocyte.NotSupportedException:
    pass
else:
    raise RuntimeError("Accessing 'ColorAttributeType' must have raised an exception.")

try:
    v = v.CameraClippingRange
except visocyte.NotSupportedException:
    pass
else:
    raise RuntimeError("Accessing 'CameraClippingRange' must have raised an exception.")

# Now switch backwards compatibility to 4.1
visocyte.compatibility.major = 4
visocyte.compatibility.minor = 1
assert ((visocyte.compatibility.GetVersion() < 4.1) == False), "version comparison failed"
assert ((visocyte.compatibility.GetVersion() <= 4.1) == True), "version comparison failed"
assert ((visocyte.compatibility.GetVersion() > 4.1) == False), "version comparison failed"
assert ((visocyte.compatibility.GetVersion() >= 4.1) == True), "version comparison failed"

a = r.ColorAttributeType
assert (type(a) == str), "'ColorAttributeType' must return a string"

a = r.ColorArrayName
assert (type(a) == str), "'ColorArrayName' must return a string"

r.ColorAttributeType = "CELL_DATA"
r.ColorArrayName = "Alpha"

visocyte.compatibility.major = None
visocyte.compatibility.minor = None
assert (r.ColorArrayName[:] == ["CELLS", "Alpha"]), "'ColorArrayName' value is not as expected."


visocyte.compatibility.major = 5
visocyte.compatibility.minor = 0
try:
    a = v.CameraClippingRange
    v.CameraClippingRange = [0, 0, 0]
except visocyte.NotSupportedException:
    raise RuntimeError("Accessing 'CameraClippingRange' must *not* have raised an exception.")
