# protobuf fork for Visocyte

This branch contains changes required to embed protobuf into Visocyte. This
includes changes made primarily to the build system to allow it to be embedded
into another source tree as well as a header to facilitate mangling of the
symbols to avoid conflicts with other copies of the library within a single
process.

  * Ignore whitespace errors for VTK's commit checks.
  * Integrate the CMake build with VTK's module system.
  * Mangle all exported symbols to have a `vtkprotobuf` namespace.
  * Include a function to handle wrapping `.proto` files.
