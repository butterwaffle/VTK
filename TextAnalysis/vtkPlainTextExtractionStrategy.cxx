/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile$

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/*-------------------------------------------------------------------------
  Copyright 2008 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
  the U.S. Government retains certain rights in this software.
-------------------------------------------------------------------------*/

#include <vtkIdTypeArray.h>
#include <vtkObjectFactory.h>
#include <vtkPlainTextExtractionStrategy.h>
#include <vtkStringArray.h>
#include <vtkUnicodeStringArray.h>

vtkCxxRevisionMacro(vtkPlainTextExtractionStrategy, "$Revision$");
vtkStandardNewMacro(vtkPlainTextExtractionStrategy);

vtkPlainTextExtractionStrategy::vtkPlainTextExtractionStrategy()
{
}

vtkPlainTextExtractionStrategy::~vtkPlainTextExtractionStrategy()
{
}

void vtkPlainTextExtractionStrategy::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

bool vtkPlainTextExtractionStrategy::Extract(
  const vtkIdType document,
  const vtkStdString& mime_type,
  const vtkTypeUInt8* content_begin,
  const vtkTypeUInt8* content_end,
  vtkUnicodeString& text,
  vtkIdTypeArray* tag_document,
  vtkIdTypeArray* tag_begin,
  vtkIdTypeArray* tag_end,
  vtkStringArray* tag_type,
  vtkUnicodeStringArray* tag_text)
{
  // Determine whether we can handle this content or not ...
  if(0 != mime_type.find("text/"))
    return false;

  // Extract text from the content ...
  text = vtkUnicodeString::from_utf8(reinterpret_cast<const char*>(content_begin), reinterpret_cast<const char*>(content_end));

  // Generate a tag for the content ...
  tag_document->InsertNextValue(document);
  tag_begin->InsertNextValue(0);
  tag_end->InsertNextValue(text.character_count());
  tag_type->InsertNextValue("TEXT");
  tag_text->InsertNextValue(text);

  return true;
}
