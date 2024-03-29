![Visocyte](Documentation/img/visocyte.png)

Introduction
============
[Visocyte][] is an open-source, multi-platform data analysis and
visualization application based on
[Visualization Toolkit (VTK)][VTK].

The first public release was announced in October 2002. Since then, the project
has grown through collaborative efforts between [Kitware Inc.][Kitware],
[Sandia National Laboratories][Sandia],
[Los Alamos National Laboratory][LANL],
[Army Research Laboratory][ARL], and various other
government and commercial institutions, and academic partners.

[Visocyte]: http://www.visocyte.org
[VTK]: http://www.vtk.org
[Kitware]: http://www.kitware.com
[Sandia]: http://www.sandia.gov/
[LANL]: http://www.lanl.gov/
[ARL]: http://www.arl.army.mil/

Learning Resources
==================

* General information is available at the [Visocyte Homepage][].

* [The Visocyte Guide][Guide] can be downloaded (as PDF) or purchased (in print).

* Community discussion takes place on the [Visocyte Discourse][] forum.

* Commercial [support][Kitware Support] and [training][Kitware Training]
  are available from [Kitware][].

* Additional documentation, including Doxygen-generated nightly
  reference documentation, is available [online][Documentation].

[Visocyte Homepage]: http://www.visocyte.org
[Documentation]: http://www.visocyte.org/documentation/
[Visocyte Discourse]: https://discourse.visocyte.org/
[Kitware]: http://www.kitware.com/
[Kitware Support]: http://www.kitware.com/products/support.html
[Kitware Training]: http://www.kitware.com/products/protraining.php
[Guide]: http://www.visocyte.org/visocyte-guide/


Building
========

There are two ways to build Visocyte:

* Perhaps the easiest method for beginners to build Visocyte from source is
using the [Visocyte Superbuild][sbrepo]. The superbuild downloads and builds all
of Visocyte's dependencies as well as Visocyte itself.

* It is also possible to [build Visocyte][build] without using the superbuild.
Visocyte's dependencies must be available on the system.

[sbrepo]: https://gitlab.kitware.com/visocyte/visocyte-superbuild
[build]: Documentation/dev/build.md

Reporting Bugs
==============

If you have found a bug:

1. If you have a source-code fix, please read the [CONTRIBUTING.md][] document.

2. Otherwise, please join the [Visocyte Discourse][] forum and ask about
   the expected and observed behaviors to determine if it is really a bug.

3. Finally, if the issue is not resolved by the above steps, open
   an entry in the [Visocyte Issue Tracker][].

[Visocyte Issue Tracker]: https://gitlab.kitware.com/visocyte/visocyte/issues

Contributing
============

See [CONTRIBUTING.md][] for instructions to contribute.

For Github users
----------------

[Github][] is a mirror of the [official repository][repo]. We do not actively monitor issues or
pull requests on Github. Please use the [official repository][repo] to report issues or contribute
fixes.

[Github]: https://github.com/Kitware/Visocyte
[repo]: https://gitlab.kitware.com/visocyte/visocyte
[CONTRIBUTING.md]: CONTRIBUTING.md

License
=======

Visocyte is distributed under the OSI-approved BSD 3-clause License.
See [Copyright.txt][] for details. For additional licenses, refer to
[Visocyte Licenses][].

[Copyright.txt]: Copyright.txt
[Visocyte Licenses]: http://www.visocyte.org/visocyte-license/
