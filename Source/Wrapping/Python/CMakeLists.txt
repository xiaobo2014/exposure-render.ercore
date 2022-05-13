# Version 2.8.1 is the minium requirement for this script.
# this is lower than the general minimum requirement.
#cmake_minimum_required ( VERSION 2.8.1 FATAL_ERROR )

#include(mitkTargetLinkLibrariesWithDynamicLookup)

project( OffScreenFFR_Python )

set(CMAKE_SHARED_LINKER_FLAGS "" CACHE INTERNAL "" FORCE)
set(CMAKE_MODULE_LINKER_FLAGS "" CACHE INTERNAL "" FORCE)

#mitk_check_dynamic_lookup(MODULE
#  SHARED
#  OffScreenFFR_UNDEFINED_SYMBOLS_ALLOWED
#  )

#
# Find the necessary libraries etc..
#

if ( OffScreenFFR_UNDEFINED_SYMBOLS_ALLOWED )
  set( _QUIET_LIBRARY "QUIET" )
else()
  set( _QUIET_LIBRARY "REQUIRED" )
endif()

#find_package ( PythonInterp REQUIRED )
#find_package ( PythonLibs ${_QUIET_LIBRARY} )

#set(Python_ADDITIONAL_VERSIONS 3.6)
#find_package(PythonLibs 3.6 REQUIRED)
#message(STATUS "Found Python Lib: ${PYTHON_LIBRARIES} (found version \"${PYTHONLIBS_VERSION_STRING}\")")
#message(STATUS "Found Python Include: ${PYTHON_INCLUDE_DIR} (found version \"${PYTHONLIBS_VERSION_STRING}\")")

#find_package(PythonInterp 3.6 REQUIRED)
#message(STATUS "Found Python: ${PYTHON_EXECUTABLE} (found version \"${PYTHON_VERSION_STRING}\")")

#unset(PYTHON_LIBRARY)
include_directories(${PYTHON_INCLUDE_PATH})

include_directories(../../vtkEr)
include_directories ( ${CMAKE_CURRENT_SOURCE_DIR} )
#
# Options
#
option ( OffScreenFFR_Python_THREADS "Enable threaded python usage by unlocking the GIL." ON )
mark_as_advanced( OffScreenFFR_Python_THREADS )
option ( OffScreenFFR_Python_EGG "Add building of python eggs to the dist target." OFF )
mark_as_advanced( OffScreenFFR_Python_EGG )
option ( OffScreenFFR_Python_WHEEL "Add building of python wheels to the dist target." ON )
mark_as_advanced( OffScreenFFR_Python_WHEEL )

# Prepare the SWIG-File, i.e. especially add necessary include folders
rxSwigPrepareFiles(OffScreenFFR.i "ErCore;vtkEr")

# Add additional SWIG Parameters
# These parameters depend on the target language
set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_GLOBAL_FLAGS} -features autodoc=1 -keyword )
if( OffScreenFFR_Python_THREADS )
  set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} -threads)
endif()
set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR})

# Create the actual SWIG project
#swig_add_module(OffScreenFFR python OffScreenFFR.i )
swig_add_library(OffScreenFFR
                 LANGUAGE python
                 SOURCES OffScreenFFR.i)

swig_link_libraries(OffScreenFFR
                    ErCore
                    vtkEr
                    ${PYTHON_LIBRARIES}
                    ${VTK_LIBRARIES}
                    )

#if(SV_INSTALL_LIBS)
  # message(WARNING "SKBuild exists")
  # Currently this installation
  install(FILES
      ${CMAKE_CURRENT_BINARY_DIR}/OffScreenFFR.py
      ${CMAKE_CURRENT_SOURCE_DIR}/Packaging/__init__.py
      DESTINATION OffScreenFFR
      COMPONENT Runtime
  )

  #dev_message("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
  #dev_message(${SWIG_MODULE_OffScreenFFR_REAL_NAME})

  install(TARGETS ${SWIG_MODULE_OffScreenFFR_REAL_NAME}
    RUNTIME DESTINATION OffScreenFFR COMPONENT CoreExecutables
    LIBRARY DESTINATION OffScreenFFR COMPONENT CoreLibraries
    ARCHIVE DESTINATION OffScreenFFR COMPONENT CoreLibraries
    )

#endif()

