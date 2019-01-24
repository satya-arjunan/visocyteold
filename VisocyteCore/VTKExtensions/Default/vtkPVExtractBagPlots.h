/*=========================================================================

  Program:   Visocyte
  Module:    vtkPVExtractBagPlots.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.visocyte.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkPVExtractBagPlots
 *
 *
 * This filter generates data needed to display bag and functional bag plots.
*/

#ifndef vtkPVExtractBagPlots_h
#define vtkPVExtractBagPlots_h

#include "vtkMultiBlockDataSetAlgorithm.h"
#include "vtkPVVTKExtensionsDefaultModule.h" //needed for exports

class vtkDoubleArray;
class vtkMultiBlockDataSet;

class PVExtractBagPlotsInternal;

class VTKPVVTKEXTENSIONSDEFAULT_EXPORT vtkPVExtractBagPlots : public vtkMultiBlockDataSetAlgorithm
{
public:
  static vtkPVExtractBagPlots* New();
  vtkTypeMacro(vtkPVExtractBagPlots, vtkMultiBlockDataSetAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Interface for preparing selection of arrays in Visocyte.
   */
  void EnableAttributeArray(const char*);
  void ClearAttributeArrays();
  //@}

  //@{
  /**
   * Set/get if the process must be done on the transposed of the input table
   * Default is TRUE.
   */
  vtkGetMacro(TransposeTable, bool);
  vtkSetMacro(TransposeTable, bool);
  vtkBooleanMacro(TransposeTable, bool);
  //@}

  //@{
  /**
   * Set/get if the PCA must be done in robust mode.
   * Default is FALSE.
   */
  vtkGetMacro(RobustPCA, bool);
  vtkSetMacro(RobustPCA, bool);
  //@}

  //@{
  /**
   * Set/get the kernel width for the HDR filter.
   * Default is 1.0
   */
  vtkGetMacro(KernelWidth, double);
  vtkSetMacro(KernelWidth, double);
  //@}

  //@{
  /**
   * Set/get if the kernel width must be automatically
   * computed using Silverman's rules (sigma*n^(-1/(d+4)))
   * Default is FALSE.
   */
  vtkGetMacro(UseSilvermanRule, bool);
  vtkSetMacro(UseSilvermanRule, bool);
  vtkBooleanMacro(UseSilvermanRule, bool);
  //@}

  //@{
  /**
   * Set/get the size of the grid to compute the PCA on.
   * Default is 100.
   */
  vtkGetMacro(GridSize, int);
  vtkSetMacro(GridSize, int);
  //@}

  //@{
  /**
   * Set/get the user quantile (in percent). Beyond this threshold, input
   * curves are considered as outliers.
   * Default is 95.
   */
  vtkGetMacro(UserQuantile, int);
  vtkSetClampMacro(UserQuantile, int, 0, 100);
  //@}

protected:
  vtkPVExtractBagPlots();
  ~vtkPVExtractBagPlots() override;

  int FillInputPortInformation(int port, vtkInformation* info) override;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  void GetEigenvalues(vtkMultiBlockDataSet* outputMetaDS, vtkDoubleArray* eigenvalues);
  void GetEigenvectors(
    vtkMultiBlockDataSet* outputMetaDS, vtkDoubleArray* eigenvectors, vtkDoubleArray* eigenvalues);

  PVExtractBagPlotsInternal* Internal;

  double KernelWidth;
  int GridSize;
  int UserQuantile;
  bool TransposeTable;
  bool RobustPCA;
  bool UseSilvermanRule;

private:
  vtkPVExtractBagPlots(const vtkPVExtractBagPlots&) = delete;
  void operator=(const vtkPVExtractBagPlots&) = delete;
};

#endif // vtkPVExtractBagPlots_h