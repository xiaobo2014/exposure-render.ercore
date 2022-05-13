
# This function is used to prepare all includes and files
# that are necessary for a general swig project. 
function(rxSwigPrepareFiles swig_file library_names)
    # Ensure that the input file is parsed as a c++ file. This is done via 
    # an additional source file property. 
    set_source_files_properties ( ${swig_file} PROPERTIES CPLUSPLUS ON )
    
    # This variable is used to add additional parameters to SWIG. 
    # Using a list is necessary in order to be able to pass multiple parameters
    # which are given as optional parameters to the input file. 
    set(ADDITIONAL_TMP_SWIG_INCLUDES "")

    foreach(library_name ${library_names})
        # Extracting all include directories from each given project and
        # then including these directories to the newly created swig project. 
        get_property(LIBRARY_INCLUDES 
                     TARGET ${library_name}
                     PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
        # Adding each include path as an additional swig parameter using 
        # the swig-option "-I":
        foreach(INCLUDE_PATH ${LIBRARY_INCLUDES})
            list(APPEND ADDITIONAL_TMP_SWIG_INCLUDES -I${INCLUDE_PATH} )
        endforeach()
    endforeach()
    
    # Add the Common Folder to the include system of SWIG
    list(APPEND ADDITIONAL_TMP_SWIG_INCLUDES -I${RX_WRAPPING_COMMON_DIR} )

    # This is necessary, because SWIG hard-codeds the integer size. See
    # https://github.com/swig/swig/issues/568
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
      list(APPEND ADDITIONAL_TMP_SWIG_INCLUDES -DSWIGWORDSIZE64)
    endif()


    # Set the additional parameters to the input project file:
    set_property(SOURCE ${swig_file} PROPERTY SWIG_FLAGS ${ADDITIONAL_TMP_SWIG_INCLUDES} )
endfunction()
