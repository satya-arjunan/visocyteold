/*=========================================================================

  Program:   Visocyte
  Module:    vtkSMDoubleMapPropertyIteratorIterator.cxx

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.visocyte.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "vtkSMDoubleMapPropertyIterator.h"

#include <map>
#include <vector>

#include "vtkObjectFactory.h"
#include "vtkSMDoubleMapProperty.h"

class vtkSMDoubleMapPropertyIteratorInternals
{
public:
  std::map<vtkIdType, std::vector<double> >* Map;
  std::map<vtkIdType, std::vector<double> >::iterator MapIterator;
};

//---------------------------------------------------------------------------
vtkStandardNewMacro(vtkSMDoubleMapPropertyIterator);

//---------------------------------------------------------------------------
vtkSMDoubleMapPropertyIterator::vtkSMDoubleMapPropertyIterator()
{
  this->Property = 0;
  this->Internals = new vtkSMDoubleMapPropertyIteratorInternals;
}

//---------------------------------------------------------------------------
vtkSMDoubleMapPropertyIterator::~vtkSMDoubleMapPropertyIterator()
{
  this->SetProperty(0);
  delete this->Internals;
}

//---------------------------------------------------------------------------
void vtkSMDoubleMapPropertyIterator::SetProperty(vtkSMDoubleMapProperty* property)
{
  if (this->Property != property)
  {
    if (this->Property != NULL)
    {
      this->Property->UnRegister(this);
    }
    this->Property = property;
    if (this->Property != NULL)
    {
      this->Property->Register(this);
      this->Begin();
    }
    this->Modified();
  }
}

//---------------------------------------------------------------------------
void vtkSMDoubleMapPropertyIterator::Begin()
{
  if (!this->Property)
  {
    return;
  }

  this->Internals->Map =
    static_cast<std::map<vtkIdType, std::vector<double> >*>(this->Property->GetMapPointer());
  this->Internals->MapIterator = this->Internals->Map->begin();
}

//---------------------------------------------------------------------------
int vtkSMDoubleMapPropertyIterator::IsAtEnd()
{
  return this->Internals->MapIterator == this->Internals->Map->end();
}

//---------------------------------------------------------------------------
void vtkSMDoubleMapPropertyIterator::Next()
{
  this->Internals->MapIterator++;
}

//---------------------------------------------------------------------------
vtkIdType vtkSMDoubleMapPropertyIterator::GetKey()
{
  return this->Internals->MapIterator->first;
}

//---------------------------------------------------------------------------
void vtkSMDoubleMapPropertyIterator::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
double vtkSMDoubleMapPropertyIterator::GetElementComponent(unsigned int component)
{
  return this->Internals->MapIterator->second[component];
}
