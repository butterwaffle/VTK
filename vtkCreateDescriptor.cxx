#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "vtkStdString.h"
#include <vtksys/RegularExpression.hxx>
#include <vtksys/SystemTools.hxx>
#include <vtkstd/vector>
#include <vtkstd/string>

void usage( char* prog, const char* msg )
{
  printf(
    "Usage:\n  %s"
    " header concrete descriptor\n"
    "where\n"
    "  header     is the path to a header file for a vtkObject subclass\n"
    "  concrete   is 0 or 1, depending on whether the subclass is concrete\n"
    "  descriptor is the destination path of the code to create the descriptor\n"
    "\n%s\n",
    prog, msg );
  exit( 1 );
}

bool advanceToDescriptor( FILE* hdr )
{
  const char match[] = "vtkDescriptorMacro(";
  int mlen = sizeof(match) / sizeof(match[0]) - 1; // subtract NULL term from length
  int posn = 0;
  while ( ! feof( hdr ) && posn < mlen )
    {
    int c = getc( hdr );
    if ( c == match[posn] )
      {
      ++ posn;
      }
    else
      {
      posn = 0;
      }
    }
  return posn == mlen;
}

// Read any argument passed to vtkDescriptorMacro.
// Note that arguments may not contain commas or parentheses.
int readNameArg( FILE* hdr, vtkStdString& argOut )
{
  argOut.clear();
  int reason = -1;
  do
    {
    int c = getc( hdr );
    switch ( c )
      {
    case EOF:
      reason = 0;
      return reason;
    case ',':
      reason = 1;
      return reason;
    case ')':
      reason = 2;
      return reason;
    default:
      argOut.append( (char*) &c );
      break;
      }
    }
  while ( ! feof( hdr ) );
  return reason;
}

// Read a single descriptor specification from the final argument of vtkDescriptorMacro.
int readDescriptorArg( FILE* hdr, vtkStdString& argOut )
{
  argOut.clear();
  int reason = -1;
  do
    {
    int c = getc( hdr );
    switch ( c )
      {
    case EOF:
      reason = 0;
      return reason;
    case ';':
      reason = 1;
      return reason;
    case ')':
      reason = 2;
      return reason;
    default:
      argOut.append( (char*) &c );
      break;
      }
    }
  while ( ! feof( hdr ) );
  return reason;
}

vtkstd::string trimWhitespace( vtkstd::string& orig )
{
  vtksys::RegularExpression trimRe( "[ \n]*([^ \n]+)[ \n]*" );
  if ( trimRe.find( orig ) )
    {
    return trimRe.match( 1 );
    }
  return vtkstd::string();
}

// Remove empty arguments as well as trim whitespace from start and end of non-empty strings.
vtkstd::vector<vtkstd::string> cleanArgs( vtkstd::vector<vtkstd::string>& argsIn )
{
  vtkstd::vector<vtkstd::string> argsOut;
  vtksys::RegularExpression trimRe( "[ \n]*([^ \n]+)[ \n]*" );
  vtkstd::vector<vtkstd::string>::iterator it;
  for ( it = argsIn.begin(); it != argsIn.end(); ++ it )
    {
    if ( trimRe.find( *it ) )
      {
      argsOut.push_back( trimRe.match( 1 ) );
      }
    }
  return argsOut;
}

void writeClassDescriptor(
  FILE* dsc, const char* classname, const char* superclassname, vtkstd::vector<vtkstd::vector<vtkstd::string> >& members )
{
  // I. Write preamble
  fprintf( dsc,
    "#include \"vtkClassDescriptor.txx\"\n"
    "#include \"vtkMemberDescriptor.txx\"\n"
    "\n"
    "static vtkClassDescriptor* %s_PrepareClassDescriptor()\n"
    "{\n"
    "  vtkClassDescriptor* cdesc = vtkClassDescriptor::RegisterClass( \"%s\", ",
    classname, classname );
  if ( superclassname && strlen( superclassname ) )
    {
    fprintf( dsc,
      "%s::CollectClassDescriptor() );\n"
      "  vtkMemberDescriptor* mdesc;\n",
      superclassname );
    }
  else
    {
    fprintf( dsc,
      "0 );\n"
      "  vtkMemberDescriptor* mdesc;\n" );
    }

  // II. Add member descriptors
  vtkstd::vector<vtkstd::vector<vtkstd::string> >::iterator it;
  vtkstd::vector<vtkstd::string>::iterator desc;
  for ( it = members.begin(); it != members.end(); ++ it )
    {
    desc = it->begin();
    vtkstd::string memberName = *desc; ++ desc;
    vtkstd::string memberAccess = *desc; ++ desc;
    vtkstd::string memberType = *desc; ++ desc;
    vtkstd::string memberTypeName;
    if ( memberType == "VTK_VOID" ) memberTypeName = "void";
    else if ( memberType == "VTK_BIT" ) memberTypeName = "bit";
    else if ( memberType == "VTK_CHAR" ) memberTypeName = "char";
    else if ( memberType == "VTK_SIGNED_CHAR" ) memberTypeName = "signed char";
    else if ( memberType == "VTK_UNSIGNED_CHAR" ) memberTypeName = "unsigned char";
    else if ( memberType == "VTK_SHORT" ) memberTypeName = "short";
    else if ( memberType == "VTK_UNSIGNED_SHORT" ) memberTypeName = "unsigned short";
    else if ( memberType == "VTK_INT" ) memberTypeName = "int";
    else if ( memberType == "VTK_UNSIGNED_INT" ) memberTypeName = "unsigned int";
    else if ( memberType == "VTK_LONG" ) memberTypeName = "long";
    else if ( memberType == "VTK_UNSIGNED_LONG" ) memberTypeName = "unsigned long";
    else if ( memberType == "VTK_LONG_LONG" ) memberTypeName = "long long";
    else if ( memberType == "VTK_UNSIGNED_LONG_LONG" ) memberTypeName = "unsigned long long";
    else if ( memberType == "VTK___INT64" ) memberTypeName = "__int64";
    else if ( memberType == "VTK_UNSIGNED___INT64" ) memberTypeName = "unsigned __int64";
    else if ( memberType == "VTK_FLOAT" ) memberTypeName = "float";
    else if ( memberType == "VTK_DOUBLE" ) memberTypeName = "double";
    else if ( memberType == "VTK_ID_TYPE" ) memberTypeName = "vtkIdType";
    else if ( memberType == "VTK_STRING" ) memberTypeName = "vtkStdString";
    else if ( memberType == "VTK_UNICODE_STRING" ) memberTypeName = "vtkUnicodeString";
    else if ( memberType == "VTK_VARIANT" ) memberTypeName = "vtkVariant";
    else if ( memberType == "VTK_OBJECT" ) memberTypeName = "vtkObject*";
    else memberTypeName = "Undefined";

    bool memberSerial = false;
    bool memberBoolean = false;
    // Parse optional member properties
    for ( ; desc != it->end(); ++ desc )
      {
      if ( *desc == "ARCHIVE" )
        { // The member is serializable. This only applies to members with RW access.
        if ( memberAccess == "RW" )
          {
          memberSerial = true;
          }
        }
      else if ( *desc == "BOOLEAN" )
        {
        memberBoolean = true;
        }
      }
    if ( memberAccess == "RW" )
      { // Both Set and Get methods exist
      fprintf( dsc,
        "  mdesc = cdesc->AddMember<%s,%s>( \"%s\", %s, &%s::Get%s, &%s::Set%s );\n",
        classname, memberTypeName.c_str(),
        memberName.c_str(),
        memberSerial ? "true" : "false",
        classname, memberName.c_str(),
        classname, memberName.c_str()
        );
      }
    else
      { // Only a Get method exists
      fprintf( dsc,
        "  mdesc = cdesc->AddMember<%s,%s>( \"%s\", %s, &%s::Get%s, 0 );\n",
        classname, memberTypeName.c_str(),
        memberName.c_str(),
        memberSerial ? "true" : "false",
        classname, memberName.c_str()
        );
      }
    }

  // III. Write postscript
  fprintf( dsc,
    "  (void)mdesc; /* prevent warnings when no members are declared... */\n"
    "  return cdesc;\n"
    "}\n"
  );
}

int main( int argc, char* argv[] )
{
  if ( argc != 4 )
    {
    usage( argv[0], "Wrong number of arguments." );
    }
  FILE* hdr = fopen( argv[1], "r" );
  FILE* dsc = fopen( argv[3], "w" );
  if ( ! hdr || ! dsc )
    {
    usage( argv[0], "Unable to open input or output." );
    }
  if ( advanceToDescriptor( hdr ) )
    {
    vtkStdString classname;
    vtkStdString superclassname;
    if ( readNameArg( hdr, classname ) && readNameArg( hdr, superclassname ) )
      {
      superclassname = trimWhitespace( superclassname );
      printf( "%s (%s)\n", classname.c_str(), superclassname.c_str() );
      vtkStdString arg;
      // read as long as the descriptor terminator is ';'... we might have more descriptors
      vtkstd::vector< vtkstd::vector<vtkstd::string> > descriptors;
      while ( readDescriptorArg( hdr, arg ) == 1 )
        {
        vtkstd::vector<vtkstd::string> args;
        vtksys::SystemTools::Split( arg.c_str(), args, ' ' );
        args = cleanArgs( args );
        if ( args.size() >= 3 )
          {
          printf( "Property <%s> Access <%s> Type <%s>\n", args[0].c_str(), args[1].c_str(), args[2].c_str() );
          descriptors.push_back( args );
          }
        }
      fclose( hdr );
      writeClassDescriptor( dsc, classname, superclassname, descriptors );
      fclose( dsc );
      }
    }
  return 0;
}
