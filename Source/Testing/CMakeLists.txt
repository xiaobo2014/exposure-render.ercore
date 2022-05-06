# IF(ER_VTK_EXAMPLE)
project(vtkErExample)

ADD_EXECUTABLE(vtkErExample main.cpp)

TARGET_LINK_LIBRARIES(vtkErExample
                      ${VTK_LIBRARIES}
                      vtkEr
                      )

# ENDIF(ER_VTK_EXAMPLE)



