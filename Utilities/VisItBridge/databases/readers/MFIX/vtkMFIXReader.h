/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkMFIXReader.h,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkMFIXReader - reads a dataset in MFIX file format
// .SECTION Description
// vtkMFIXReader creates an unstructured grid dataset. It reads a restart
// file and a set of sp files.  The restart file contains the mesh 
// information.  MFIX meshes are either cylindrical or rectilinear, but 
// this reader will convert them to an unstructured grid.  The sp files 
// contain transient data for the cells.  Each sp file has one or more 
// variables stored inside it.  

// .SECTION Thanks
// Thanks to Phil Nicoletti and Brian Dotson at the National Energy 
// Technology Laboratory who developed this class.
// Please address all comments to Terry Jordan (terry.jordan@netl.doe.gov)

// .SECTION See Also
// vtkGAMBITReader

#ifndef __vtkMFIXReader_h
#define __vtkMFIXReader_h

#include "vtkUnstructuredGridAlgorithm.h"

class vtkDataArraySelection;
class vtkDoubleArray;
class vtkStringArray;
class vtkIntArray;
class vtkLongLongArray;
class vtkFloatArray;
class vtkXMLUnstructuredGridWriter;
class vtkCell;
class vtkWedge;
class vtkQuad;
class vtkHexahedron;
class vtkPoints;
class vtkStdString;

class vtkMFIXReader : public vtkUnstructuredGridAlgorithm
{
public:
  static vtkMFIXReader *New();
  vtkTypeMacro(vtkMFIXReader,vtkUnstructuredGridAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  // Description:
  // Specify the file name of the MFIX Restart data file to read.
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);

  // Description:
  // Get the total number of cells. The number of cells is only valid after a
  // successful read of the data file is performed.
  vtkGetMacro(NumberOfCells,int);

  // Description:
  // Get the total number of nodes. The number of nodes is only valid after a
  // successful read of the data file is performed.
  vtkGetMacro(NumberOfPoints,int);

  // Description:
  // Get the number of data components at the nodes and cells.
  vtkGetMacro(NumberOfCellFields,int);
  
  // Description:
  // Which TimeStep to read.
  vtkSetMacro(TimeStep, int);
  vtkGetMacro(TimeStep, int);
 
  // Description:
  // Returns the number of timesteps.
  vtkGetMacro(NumberOfTimeSteps, int);

  // Description:
  // Which TimeStepRange to read
  vtkGetVector2Macro(TimeStepRange, int);
  vtkSetVector2Macro(TimeStepRange, int);


  // Description:
  // Which TimeStepRange to read
  vtkSetMacro(SwapByteOrder, bool);
  vtkGetMacro(SwapByteOrder, bool);
  vtkBooleanMacro(SwapByteOrder, bool);

  // Description
  // Get the number of cell arrays available in the input.
  int GetNumberOfCellArrays(void);

  // Description:
  // Get the name of the  cell array with the given index in
  // the input.
  const char* GetCellArrayName(int index);

  // Description:
  // Get the name of the  cell array with the given index in
  // the input.
  const int GetCellArrayNumComponents(int index);

  // Description:
  // Get/Set whether the cell array with the given name is to
  // be read.
  int GetCellArrayStatus(const char* name);
  void SetCellArrayStatus(const char* name, int status);

  // Description:
  // Turn on/off all cell arrays.
  void DisableAllCellArrays();
  void EnableAllCellArrays();

  // Description:
  // Get the range of cell data.
  void GetCellDataRange(int cellComp, int index, float *min, float *max);

protected:
  vtkMFIXReader();
  ~vtkMFIXReader();
  int RequestInformation(vtkInformation *,
    vtkInformationVector **, vtkInformationVector *) override;
  int RequestData(vtkInformation *,
    vtkInformationVector **, vtkInformationVector *) override;
  int FillOutputPortInformation(int, vtkInformation *) override;

  bool SwapByteOrder;

  //
  // Visocyte Variables
  //

  char *FileName;
  int RequestInformationFlag;
  int MakeMeshFlag;
  int NumberOfPoints;
  int NumberOfCells;
  int NumberOfCellFields;
  vtkIntArray   *VectorLength;
  vtkFloatArray *Minimum;
  vtkFloatArray *Maximum;
  vtkDataArraySelection *CellDataArraySelection;
  int TimeStep;
  int ActualTimeStep;
  int CurrentTimeStep;
  int NumberOfTimeSteps;
  int *TimeSteps; 
  int TimeStepRange[2];
  int TimeStepWasReadOnce;
  
  //
  //  MFIX Variables
  //

  vtkFloatArray **CellDataArray; // Arrays for variables that will 
                                 //attach to mesh
  vtkPoints *Points;            // Points array for building grid
  vtkUnstructuredGrid *FluidMesh;    // Unstructured Grid
  vtkUnstructuredGrid *InletMesh;    // Unstructured Grid
  vtkUnstructuredGrid *OutletMesh;    // Unstructured Grid
  vtkUnstructuredGrid *ObstructionMesh;    // Unstructured Grid
  vtkHexahedron *AHexahedron;   // Hexahedron type cell
  vtkWedge *AWedge;             // Wedge type cell
  vtkQuad *AQuad;               // Quad type cell
  vtkIntArray *Flag;            // Cell Flag array
  vtkDoubleArray *Dx;           // Cell widths in x axis
  vtkDoubleArray *Dy;           // Cell widths in y axis
  vtkDoubleArray *Dz;           // Cell widths in z axis
  vtkIntArray *NMax;            // Array to hold number of species per phase
  vtkDoubleArray *C;            // Array used to parse restart file
  vtkIntArray *TempI;           // Array used to parse restart file
  vtkDoubleArray *TempD;        // Array used to parse restart file
  vtkIntArray *SpxFileExists;   // Array for keeping track of 
                                // what spx files exist.

  char FileExtension[15];
  char DataBuffer[513];
  char Version[120];
  float VersionNumber;
  int    DimensionIc;
  int    DimensionBc;
  int    DimensionC;
  int    DimensionIs;
  double Ce;
  double Cf;
  double Phi;
  double PhiW;
  double DeltaTime;
  double XMinimum;
  char RunName[256];
  vtkStringArray *VariableNames;
  vtkIntArray *VariableComponents;
  int IMinimum1;
  int JMinimum1;
  int KMinimum1;
  int IMaximum;
  int JMaximum;
  int KMaximum;
  int IMaximum1;
  int JMaximum1;
  int KMaximum1;
  int IMaximum2;
  int JMaximum2;
  int KMaximum2;
  int IJMaximum2;
  int IJKMaximum2;
  int MMAX;
  int NumberOfSPXFilesUsed;
  double XLength;
  double YLength;
  double ZLength;
  int  NumberOfScalars;
  int NumberOfReactionRates;
  bool BkEpsilon;
  char CoordinateSystem[17];
  char Units[17];

  //
  //  SPX Variables
  //

  int MaximumTimestep;              // maximum timesteps amongst the variables
  int SPXRecordsPerTimestep;        // number of records in a single 
                                    // timestep for a variable
  vtkIntArray *SPXToNVarTable;      // number of variables in each spx file
  vtkIntArray *VariableToSkipTable; // skip value for each variable, this 
                                    // is needed in spx files 
                                    // with more than one variable.
  vtkIntArray *VariableTimesteps;   // number of timesteps for each variable
  vtkIntArray *VariableTimestepTable;  // Since the number of timesteps 
                                       // vary between variables
                                       //  this is a table that looks 
                                       //  up the appropriate timestep 
                                       // for the particular variable.
  vtkIntArray *VariableIndexToSPX;  //  This gives the spx file number for the 
                                    //  particular variable.
  vtkLongLongArray *SPXTimestepIndexTable; //  This a table look up for the index 
                              //  into a file for a certain variable.

private:
  vtkMFIXReader(const vtkMFIXReader&);  // Not implemented.
  void operator=(const vtkMFIXReader&);  // Not implemented.

  void MakeMesh(vtkUnstructuredGrid *, vtkUnstructuredGrid *,
                vtkUnstructuredGrid *, vtkUnstructuredGrid *);
  void SwapDouble(double &value);
  void SwapFloat(float &value);
  void SwapInt(int &value);
  vtkStdString ConvertIntToString(int in);
  int ConvertCharToInt(char in);
  int ConvertStringToInt(const vtkStdString & in);
  void GetInt(istream& in, int &val);
  void GetDouble(istream& in, double& val);
  void GetFloat(istream& in, float& val);
  void SkipBytes(istream& in, int n);
  void RestartVersionNumber(const char* buffer);
  void GetBlockOfDoubles(istream& in, vtkDoubleArray *v, int n);
  void GetBlockOfFloats(istream& in, vtkFloatArray *v, int n);
  void GetBlockOfFloats(FILE* in, vtkFloatArray *v, int n);
  void GetBlockOfInts(istream& in, vtkIntArray *v, int n);
  void ReadRestartFile();
  void GetVariableAtTimestep(int vari , int tstep, vtkFloatArray *v);
  void CreateVariableNames();
  void GetTimeSteps();
  void MakeTimeStepTable(int nvars);
  void SetProjectName (const char *infile);
  void MakeSPXTimeStepIndexTable(int nvars);
  void CalculateMaxTimeStep();
  void GetNumberOfVariablesInSPXFiles();
  void FillVectorVariable( int xindex, int yindex, int zindex, 
         vtkFloatArray *v);
  void ConvertVectorFromCylindricalToCartesian( int xindex, int zindex);
  void GetAllTimes(vtkInformationVector *outputVector);
  void AddCellToMesh(vtkCell *aCell, const int val);

};

#endif

