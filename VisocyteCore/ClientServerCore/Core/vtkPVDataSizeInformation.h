/*=========================================================================

  Program:   Visocyte
  Module:    vtkPVDataSizeInformation.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.visocyte.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkPVDataSizeInformation
 * @brief   PV information object for getting
 * information about data size.
 *
 * vtkPVDataSizeInformation is an information object for getting information
 * about data size. This is a light weight sibling of vtkPVDataInformation which
 * only returns the data size and nothing more. The data size can also be
 * obtained from vtkPVDataInformation.
*/

#ifndef vtkPVDataSizeInformation_h
#define vtkPVDataSizeInformation_h

#include "vtkPVClientServerCoreCoreModule.h" //needed for exports
#include "vtkPVInformation.h"

class VTKPVCLIENTSERVERCORECORE_EXPORT vtkPVDataSizeInformation : public vtkPVInformation
{
public:
  static vtkPVDataSizeInformation* New();
  vtkTypeMacro(vtkPVDataSizeInformation, vtkPVInformation);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Transfer information about a single object into this object.
   */
  void CopyFromObject(vtkObject*) override;

  /**
   * Merge another information object. Calls AddInformation(info, 0).
   */
  void AddInformation(vtkPVInformation* info) override;

  //@{
  /**
   * Manage a serialized version of the information.
   */
  void CopyToStream(vtkClientServerStream*) override;
  void CopyFromStream(const vtkClientServerStream*) override;
  //@}

  /**
   * Remove all information.  The next add will be like a copy.
   */
  void Initialize();

  //@{
  /**
   * Access to memory size information.
   */
  vtkGetMacro(MemorySize, int);
  //@}

protected:
  vtkPVDataSizeInformation();
  ~vtkPVDataSizeInformation() override;

  int MemorySize;

private:
  vtkPVDataSizeInformation(const vtkPVDataSizeInformation&) = delete;
  void operator=(const vtkPVDataSizeInformation&) = delete;
};

#endif
