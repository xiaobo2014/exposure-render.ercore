# set(lib ${SV_LIB_MODULE_COMMON_NAME})

# set(lib vtkEr)

IF(ER_VTK)

    INCLUDE_DIRECTORIES(
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
    )

    # Make sure classes are exported
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_EXPORTING")

    SET(General
        vtkErObject.h
        vtkErObject.cpp
        vtkErVolume.h
        vtkErVolume.cpp
        vtkErTracer.h
        vtkErTracer.cpp
        vtkErTexture.h
        vtkErTexture.cpp
        vtkErBindable.h
        vtkErBindable.cpp
        vtkErCamera.h
        vtkErCamera.cpp
        vtkErAlignment.h
        vtkErAlignment.cpp
        vtkErBitmap.h
        vtkErBitmap.cpp
        vtkErShape.h
        vtkErShape.cpp
        vtkErTimerCallback.h
        vtkErTimerCallback.cpp
        vtkErVolumeProperty.h
        vtkErVolumeProperty.cpp
        vtkErInteractorStyleTrackballCamera.h
        vtkErInteractorStyleTrackballCamera.cpp
        vtkErStable.h
        vtkErStable.cpp
        vtkErConfigure.h
        vtkErDll.h
        vtkErUtilities.h
        vtkErInterface.h
        vtkErInterface.cpp
    )

    SOURCE_GROUP("General" FILES ${General})

    ADD_LIBRARY(vtkEr ${General})

    # Make use of precompiled headers to speed up compilation
    if (MSVC)
        set_target_properties(vtkEr PROPERTIES COMPILE_FLAGS "/YuvtkErStable.h")
        set_source_files_properties(vtkErStable.cpp PROPERTIES COMPILE_FLAGS "/YcvtkErStable.h")
    endif(MSVC)

    # TARGET_LINK_LIBRARIES(vtkEr vtkCommon vtkRendering vtkGraphics vtkVolumeRendering ErCore OpenGL32.lib)
    # TARGET_LINK_LIBRARIES(vtkEr ${VTK_LIBRARIES} ErCore OpenGL32.lib)
    TARGET_LINK_LIBRARIES(vtkEr ${VTK_LIBRARIES} ErCore)

ENDIF(ER_VTK)



## Set up for exports
#set(export_directive rxCommon)
#set(export_file_name rxCommonExports.h)

#string(TOUPPER ${export_directive} EXPORT_NAME)
#set(_export_macro_names
#    EXPORT_MACRO_NAME ${EXPORT_NAME}_EXPORT
#    NO_EXPORT_MACRO_NAME ${EXPORT_NAME}_NO_EXPORT
#    DEPRECATED_MACRO_NAME ${EXPORT_NAME}_DEPRECATED
#    NO_DEPRECATED_MACRO_NAME ${EXPORT_NAME}_NO_DEPRECATED
#    )

## Generate export headers
#generate_export_header(${lib}
#                       ${_export_macro_names}
#                       EXPORT_FILE_NAME ${export_file_name}
#                       )

#target_include_directories(${lib} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

#install(TARGETS ${lib}
#  RUNTIME DESTINATION bin COMPONENT CoreExecutables
#  LIBRARY DESTINATION lib COMPONENT CoreLibraries
#  ARCHIVE DESTINATION lib COMPONENT CoreLibraries
#)




