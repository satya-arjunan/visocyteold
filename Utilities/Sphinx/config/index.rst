.. Visocyte-Python documentation master file, created by
   sphinx-quickstart on Fri Feb  8 01:06:42 2013.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Visocyte's Python documentation!
===========================================

Visocyte offers rich scripting support through Python. This support is available
as part of the Visocyte client (visocyte), an MPI-enabled batch application
(pvbatch), the Visocyte python client (pvpython), or any other Python-enabled
application. Using Python, users and developers can gain access to the Visocyte
visualization engine.

Main modules
==================

.. toctree::
   :maxdepth: 2

   quick-start

   The Visocyte Python Package <visocyte>
   simple <visocyte.simple>
   servermanager <visocyte.servermanager>

   coprocessing <visocyte.coprocessing>
   benchmark <visocyte.benchmark>

   Available readers, sources, writers, filters and animation cues <visocyte.servermanager_proxies>

Python Version Support
======================

Visocyte currently supports Python 2.7, but support for 3.5 is coming. To help
this development, please use Python 3 compatible code constructs. This section
discusses the most common needed changes.

.. toctree::
   :maxdepth: 2

   python-2-vs-3


API Changes
===========

This documents changes to Visocyte's Python APIs between different versions, starting
with Visocyte 4.2.0.

.. toctree::
   :maxdepth: 2

   api-changes
