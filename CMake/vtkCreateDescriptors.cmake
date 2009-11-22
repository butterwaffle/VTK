#
# Add custom targets to populate class descriptors with members.
#
MACRO(VTK_CREATE_DESCRIPTOR TARGET SRC_LIST_NAME SOURCES)
  IF(NOT VTK_CREATE_DESCRIPTOR_EXE)
    MESSAGE(SEND_ERROR "VTK_CREATE_DESCRIPTOR_EXE not specified when calling VTK_CREATE_DESCRIPTOR")
  ENDIF(NOT VTK_CREATE_DESCRIPTOR_EXE)

  # The shell into which nmake.exe executes the custom command has some issues
  # with mixing quoted and unquoted arguments :( Let's help.

  IF(CMAKE_GENERATOR MATCHES "NMake Makefiles")
    SET(verbatim "")
    SET(quote "\"")
  ELSE(CMAKE_GENERATOR MATCHES "NMake Makefiles")
    SET(verbatim "VERBATIM")
    SET(quote "")
  ENDIF(CMAKE_GENERATOR MATCHES "NMake Makefiles")

  # For each class
  FOREACH(FILE ${SOURCES})
    # should we wrap the file?
    GET_SOURCE_FILE_PROPERTY(TMP_WRAP_EXCLUDE ${FILE} WRAP_EXCLUDE)
    
    # if we should wrap it
    IF (NOT TMP_WRAP_EXCLUDE)
      
      # what is the filename without the extension
      GET_FILENAME_COMPONENT(TMP_FILENAME ${FILE} NAME_WE)
      
      # the input file might be full path so handle that
      GET_FILENAME_COMPONENT(TMP_FILEPATH ${FILE} PATH)
      
      # compute the input filename
      IF (TMP_FILEPATH)
        SET(TMP_INPUT ${TMP_FILEPATH}/${TMP_FILENAME}.h) 
      ELSE (TMP_FILEPATH)
        SET(TMP_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/${TMP_FILENAME}.h)
      ENDIF (TMP_FILEPATH)
      
      # is it abstract?
      GET_SOURCE_FILE_PROPERTY(TMP_ABSTRACT ${FILE} ABSTRACT)
      IF (TMP_ABSTRACT)
        SET(TMP_CONCRETE 0)
      ELSE (TMP_ABSTRACT)
        SET(TMP_CONCRETE 1)
      ENDIF (TMP_ABSTRACT)
      
      # new source file is nameDescriptor.cxx, add to resulting list
      SET(${SRC_LIST_NAME} ${${SRC_LIST_NAME}} 
        ${TMP_FILENAME}Descriptor.cxx)
      
      # add custom command to output
      ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${TMP_FILENAME}Descriptor.cxx
        DEPENDS ${VTK_CREATE_DESCRIPTOR_EXE} ${VTK_WRAP_HINTS} ${TMP_INPUT}
        COMMAND ${VTK_CREATE_DESCRIPTOR_EXE}
        ARGS 
        "${quote}${TMP_INPUT}${quote}" 
        ${TMP_CONCRETE} 
        "${quote}${CMAKE_CURRENT_BINARY_DIR}/${TMP_FILENAME}Descriptor.cxx${quote}"
        COMMENT "Descriptor - generating ${TMP_FILENAME}Descriptor.cxx"
        ${verbatim}
        )
      
    ENDIF (NOT TMP_WRAP_EXCLUDE)
  ENDFOREACH(FILE)

ENDMACRO(VTK_CREATE_DESCRIPTOR)
