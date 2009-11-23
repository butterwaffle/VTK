#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "vtkStdString.h"
#include <vtksys/RegularExpression.hxx>
#include <vtksys/SystemTools.hxx>
#include <vtksys/ios/sstream>
#include <vtkstd/vector>
#include <vtkstd/string>
#include <vtkstd/deque>

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

bool isInString( vtkstd::deque<int>& nests )
{
  if ( nests.empty() )
    return false;
  int c = nests.back();
  if ( c == '\'' || c == '"' )
    return true;
  return false;
}

// Read a single descriptor specification from the final argument of vtkDescriptorMacro.
int readDescriptorArg( FILE* hdr, vtkStdString& argOut )
{
  argOut.clear();
  int reason = -1;
  vtkstd::deque<int> nests;
  bool inString = false; // shouldn't force strings to contain equally-nested parentheses.
  // nesting of '(', '\'', '"', or '[' will result in terminating characters being ignored until the *matching* close operation.
  do
    {
    int c = getc( hdr );
    switch ( c )
      {
    case EOF:
      reason = 0;
      return reason;
    case '(':
      if ( ! inString )
        {
        nests.push_back( ')' );
        }
      argOut.append( (char*) &c );
      break;
    case '\'':
    case '"':
      if ( nests.back() == c )
        {
        nests.pop_back();
        inString = false;
        }
      else if ( ! inString )
        {
        nests.push_back( c );
        inString = true;
        }
      argOut.append( (char*) &c );
      break;
    case ';':
      if ( nests.empty() )
        {
        reason = 1;
        return reason;
        }
      argOut.append( (char*) &c );
      break;
    case ')':
      if ( nests.empty() )
        {
        reason = 2;
        return reason;
        }
      else if ( ! inString && nests.back() == ')' )
        {
        nests.pop_back();
        }
      argOut.append( (char*) &c );
      break;
    case '\\':
      c = getc( hdr );
      if ( c == EOF )
        {
        reason = 0;
        return reason;
        }
      // fall through...
    default:
      argOut.append( (char*) &c );
      break;
      }
    }
  while ( ! feof( hdr ) );
  return reason;
}

// Given an opening mark, return its matching closing mark
int closingMark( int openingMark )
{
  switch ( openingMark )
    {
  case '(':
    return ')';
  case '[':
    return ']';
  case '{':
    return '}';
  case '<':
    return '>';
  case '`':
  case '|':
  case '\'':
  case '"':
    return openingMark;
    }
  return 0; // not a nesting mark.
}

// Read a single descriptor specification from the final argument of vtkDescriptorMacro.
int splitDescriptorArg( vtkStdString& specIn, vtkstd::vector<vtkStdString>& tokensOut )
{
  tokensOut.clear();
  vtkStdString curToken;
  int reason = 0;
  vtkstd::deque<int> nests;
  vtkStdString::iterator it = specIn.begin();
  bool inString = false; // shouldn't force strings to contain equally-nested parentheses.
  // nesting of '(', '\'', '"', or '[' will result in terminating characters being ignored until the *matching* close operation.
  for ( it = specIn.begin(); it != specIn.end(); ++ it )
    {
    int c = *it;
    switch ( c )
      {
    case ' ':
    case '\n':
    case '\r':
    case '\t':
      if ( ! inString )
        { // Start a new token
        if ( curToken.size() )
          {
          tokensOut.push_back( curToken );
          curToken.clear();
          }
        }
      else
        {
        curToken.append( (char*) &c );
        }
      break;
    case '(':
    case '[':
    case '{':
      if ( ! inString )
        { // Start a new nest
        if ( curToken.size() )
          {
          tokensOut.push_back( curToken );
          curToken.clear();
          }
        tokensOut.push_back( (char*) &c );
        nests.push_back( closingMark( c ) );
        }
      else
        {
        curToken.append( (char*) &c );
        }
      break;
    case '\'':
    case '"':
      if ( nests.back() == c )
        { // This quote closes out a string
        nests.pop_back();
        inString = false;
        curToken.append( (char*) &c );
        tokensOut.push_back( curToken );
        curToken.clear();
        }
      else if ( ! inString )
        { // This quote opens a string
        if ( ! curToken.empty() )
          {
          tokensOut.push_back( curToken );
          curToken.clear();
          }
        nests.push_back( c );
        inString = true;
        curToken.append( (char*) &c );
        }
      else
        { // We're in a string with a different delimiter, so this quote is a literal.
        curToken.append( (char*) &c );
        }
      break;
    case ';':
      if ( nests.empty() )
        {
        reason = 1;
        return reason;
        }
      else if ( ! inString )
        {
        if ( ! curToken.empty() )
          {
          tokensOut.push_back( curToken );
          curToken.clear();
          }
        // tokensOut.push_back( ";" ); // Semicolon is a separator... do not tokenize it.
        }
      else
        {
        curToken.append( (char*) &c );
        }
      break;
    case ')':
    case ']':
    case '}':
      if ( nests.empty() )
        { // A syntax error.. closing mark without an opening mark.
        reason = 2;
        return reason;
        }
      else if ( ! inString && nests.back() == c )
        {
        if ( ! curToken.empty() )
          {
          tokensOut.push_back( curToken );
          curToken.clear();
          }
        tokensOut.push_back( vtkStdString( (char*) &c ) );
        nests.pop_back();
        }
      else if ( inString )
        {
        curToken.append( (char*) &c );
        }
      else
        { // syntax error... mismatched open/close marks.
        reason = 2;
        return reason;
        }
      break;
    case '\\':
      ++ it;
      if ( it == specIn.end() )
        {
        reason = 0;
        return reason;
        }
      c = *it;
      // fall through...
    default:
      curToken.append( (char*) &c );
      break;
      }
    }
  if ( ! curToken.empty() )
    {
    tokensOut.push_back( curToken );
    }
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
vtkstd::vector<vtkStdString> cleanArgs( vtkstd::vector<vtkStdString>& argsIn )
{
  vtkstd::vector<vtkStdString> argsOut;
  vtksys::RegularExpression trimRe( "[ \n]*([^ \n]+)[ \n]*" );
  vtkstd::vector<vtkStdString>::iterator it;
  for ( it = argsIn.begin(); it != argsIn.end(); ++ it )
    {
    if ( trimRe.find( *it ) )
      {
      argsOut.push_back( trimRe.match( 1 ) );
      }
    }
  return argsOut;
}

// A utility to convert strings to numbers.
// It is stolen from vtkVariant.cxx (which we can't use -- the
// descriptor parser cannot rely on vtkCommon since it is used
// to construct vtkCommon).
template <typename T_>
T_ toNumeric( vtkStdString str, T_* ignored = 0 )
{
  (void) ignored;
  vtksys_ios::istringstream vstr( str );
  T_ data;
  vstr >> data;
  return data;
}

void writeClassDescriptor(
  FILE* dsc, const char* classname, const char* superclassname, vtkstd::vector<vtkstd::vector<vtkStdString> >& members )
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
  vtkstd::vector<vtkstd::vector<vtkStdString> >::iterator it;
  vtkstd::vector<vtkStdString>::iterator desc;
  for ( it = members.begin(); it != members.end(); ++ it )
    {
    // II.a. Parse the fixed member specification tokens
    desc = it->begin();
    vtkstd::string memberName = *desc; ++ desc;
    vtkstd::string memberAccess = *desc; ++ desc;
    vtkstd::string memberType = *desc; ++ desc;
    vtkstd::string memberTypeName;
    vtkstd::string addMethod = "AddMember";
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
    else if ( memberType == "VTK_OBJECT" ) { memberTypeName = "vtkObject"; addMethod = "AddObjectMember"; }
    else memberTypeName = "Undefined";
    int memberComponents = 1;
    if ( desc != it->end() && *desc == "[" )
      {
      int dim;
      if ( ++ desc != it->end() && ( dim = toNumeric<int>( *desc ) ) > 0 && ++ desc != it->end() && *desc == "]" )
        {
        memberComponents = dim;
        }
      else
        {
        printf( "Bad member %s %s %s. Skipping\n", memberName.c_str(), memberAccess.c_str(), memberType.c_str() ); 
        }
      }

    bool memberSerial = ( memberAccess == "RO" ? false : true );
    bool memberBoolean = false;
    // II.b. Parse optional member specification tokens
    for ( ; desc != it->end(); ++ desc )
      {
      if ( *desc == "EPHEMERAL" )
        { // The member is not serializable. This only applies to members with RW access.
        if ( memberAccess == "RW" )
          {
          memberSerial = false;
          }
        }
      else if ( *desc == "BOOLEAN" )
        {
        memberBoolean = true;
        }
      else if ( *desc == "ENUM" )
        {
        }
      else if ( *desc == "MIN" )
        {
        }
      else if ( *desc == "MAX" )
        {
        }
      else if ( *desc == "DEFAULT" )
        {
        }
      else if ( *desc == "SUBCLASS" )
        {
        ++ desc;
        if ( desc != it->end() )
          {
          memberTypeName = *desc;
          }
        else
          {
          printf( "SUBCLASS must be followed by a class name\n" );
          return ;
          }
        }
      else if ( *desc == "INVALID" )
        {
        }
      }

    /*
    cout
      << "  " << memberName.c_str() << " (" << memberAccess.c_str() << ") "
      << memberTypeName.c_str() << "[" << memberComponents << "]";
    if ( memberAccess == "RW" && ! memberSerial )
      cout << " EPHEMERAL";
    if ( memberBoolean )
      cout << " BOOLEAN";
    cout << "\n";
    */

    // II.c. Create a new member descriptor from our parse results
    if ( memberAccess == "RW" )
      { // Both Set and Get methods exist
      if ( memberComponents <= 1 )
        {
        fprintf( dsc,
          "  mdesc = cdesc->%s<%s,%s>( \"%s\", %s, &%s::Get%s, &%s::Set%s );\n",
          addMethod.c_str(),
          classname, memberTypeName.c_str(),
          memberName.c_str(),
          memberSerial ? "true" : "false",
          classname, memberName.c_str(),
          classname, memberName.c_str()
        );
        }
      else
        {
        fprintf( dsc,
          "  mdesc = cdesc->%s<%s,%s,%d>( \"%s\", %s, &%s::Get%s, &%s::Set%s );\n",
          addMethod.c_str(),
          classname, memberTypeName.c_str(), memberComponents,
          memberName.c_str(),
          memberSerial ? "true" : "false",
          classname, memberName.c_str(),
          classname, memberName.c_str()
        );
        }
      }
    else
      { // Only a Get method exists
      if ( memberComponents <= 1 )
        {
        fprintf( dsc,
          "  mdesc = cdesc->%s<%s,%s>( \"%s\", %s, &%s::Get%s, 0 );\n",
          addMethod.c_str(),
          classname, memberTypeName.c_str(),
          memberName.c_str(),
          memberSerial ? "true" : "false",
          classname, memberName.c_str()
        );
        }
      else
        {
        fprintf( dsc,
          "  mdesc = cdesc->AddMember<%s,%s,%d>( \"%s\", %s, &%s::Get%s, 0 );\n",
          classname, memberTypeName.c_str(), memberComponents,
          memberName.c_str(),
          memberSerial ? "true" : "false",
          classname, memberName.c_str()
        );
        }
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
      /*
      printf(
        // For the vtk${KIT}Descriptors.cxx file:
        "#include \"%s\"\n"
        "  %s::CollectClassDescriptor();\n",
        vtksys::SystemTools::GetFilenameName( argv[1] ).c_str(),
        classname.c_str() );
        */
      vtkStdString arg;
      // read as long as the descriptor terminator is ';'... we might have more descriptors
      vtkstd::vector< vtkstd::vector<vtkStdString> > descriptors;
      while ( readDescriptorArg( hdr, arg ) == 1 )
        {
        vtkstd::vector<vtkStdString> args;
        //vtksys::SystemTools::Split( arg.c_str(), args, ' ' );
        if ( ! splitDescriptorArg( arg, args ) )
          {
          args = cleanArgs( args );
          if ( args.size() >= 3 )
            {
            descriptors.push_back( args );
            }
          }
        }
      fclose( hdr );
      writeClassDescriptor( dsc, classname, superclassname, descriptors );
      fclose( dsc );
      }
    }
  return 0;
}
