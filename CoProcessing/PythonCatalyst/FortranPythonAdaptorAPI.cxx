/*=========================================================================

  Program:   Visocyte
  Module:    FortranPythonAdaptorAPI.cxx

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.visocyte.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// CATALYST_FORTRAN_USING_MANGLING is defined when Fortran names are mangled. If
// not, then we don't add another implementation for the API routes thus avoid
// duplicate implementations.
#ifdef CATALYST_FORTRAN_USING_MANGLING

#include "FortranPythonAdaptorAPI.h"
#include "CPythonAdaptorAPI.cxx"

#endif
