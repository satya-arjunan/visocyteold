from visocyte.simple import *
import visocyte

Sphere()
r = Show()
v = GetActiveView()

lut = GetColorTransferFunction('Normals')
cb = GetScalarBar(lut, v)

# Check expected properties
assert (type(cb.ScalarBarLength) == float),\
    "'ScalarBarLength' must be a float"
assert (type(cb.ScalarBarThickness) == int),\
    "'ScalarBarThickness' must be an int"

# check that removed properties are not available
try:
    a = cb.AspectRatio
except visocyte.NotSupportedException:
    pass
else:
    raise RuntimeError("Accessing 'AspectRatio' must raise an exception.")

try:
    p = cb.Position2
except visocyte.NotSupportedException:
    pass
else:
    raise RuntimeError("Accessing 'Position2' must raise an exception.")

# Now switch backwards compatibility to 5.3
visocyte.compatibility.major = 5
visocyte.compatibility.minor = 3

a = cb.AspectRatio
assert (type(a) == float), "'AspectRatio' must return a float"

p = cb.Position2
assert (len(cb.Position2) == 2), "'Position2' must contain two elements"

try:
    cb.AspectRatio = 20.0
except visocyte.NotSupportedException:
    raise RuntimeError("Setting 'AspectRatio' must *not* have raised an exception.")

assert (cb.AspectRatio == 20.0), "Value for 'AspectRatio' is not 20.0."

try:
    cb.Position2 = [0.1, 0.2]
except visocyte.NotSupportedException:
    raise RuntimeError("Setting 'Position2' must *not* have raised an exception.")

assert (cb.Position2 == [0.1, 0.2]), "Value for 'Position2' is not [0.1, 0.2]."
