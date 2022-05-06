#	Copyright (c) 2011, T. Kroes <t.kroes@tudelft.nl>
#	All rights reserved.
#
#	Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
#
#	- Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
#	- Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
#	- Neither the name of the TU Delft nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
#	
#	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 3.0)

OPTION(ER_VTK "Exposure Render connection with VTK" OFF)
OPTION(ER_VTK_PYTHON "Python wrapping for Exposure Render VTK" OFF)
OPTION(ER_VTK_EXAMPLE "Example project which shows how to use Exposure Render in VTK" OFF)

PROJECT(ExposureRender)

# Use CUDA
FIND_PACKAGE(CUDA)

# Supported streaming architectures, uncomment the lines that pertain to the hardware your compiling for
# More on CUDA gpu's: http://developer.nvidia.com/cuda-gpus

# Tesla architecture: sm 1.0 and above
# SET(CUDA_NVCC_FLAGS "-gencode=arch=compute_10,code=sm_10;${CUDA_NVCC_FLAGS}")
# SET(CUDA_NVCC_FLAGS "-gencode=arch=compute_10,code=compute_10;${CUDA_NVCC_FLAGS}")
# SET(CUDA_NVCC_FLAGS "-gencode=arch=compute_11,code=sm_11;${CUDA_NVCC_FLAGS}")
# SET(CUDA_NVCC_FLAGS "-gencode=arch=compute_11,code=compute_11;${CUDA_NVCC_FLAGS}")
# SET(CUDA_NVCC_FLAGS "-gencode=arch=compute_12,code=sm_12;${CUDA_NVCC_FLAGS}")
# SET(CUDA_NVCC_FLAGS "-gencode=arch=compute_12,code=compute_12;${CUDA_NVCC_FLAGS}")
# SET(CUDA_NVCC_FLAGS "-gencode=arch=compute_13,code=sm_13;${CUDA_NVCC_FLAGS}")
# SET(CUDA_NVCC_FLAGS "-gencode=arch=compute_13,code=compute_13;${CUDA_NVCC_FLAGS}")

# Fermi architecture: sm 2.0 and above
SET(CUDA_NVCC_FLAGS "-gencode=arch=compute_50,code=sm_50;${CUDA_NVCC_FLAGS}")
SET(CUDA_NVCC_FLAGS "-gencode=arch=compute_50,code=compute_50;${CUDA_NVCC_FLAGS}")
# SET(CUDA_NVCC_FLAGS "-gencode=arch=compute_21,code=sm_21;${CUDA_NVCC_FLAGS}")
# SET(CUDA_NVCC_FLAGS "-gencode=arch=compute_21,code=compute_21;${CUDA_NVCC_FLAGS}")

# Kepler architecture: sm 3.0 and above
# SET(CUDA_NVCC_FLAGS "-gencode=arch=compute_30,code=sm_30;${CUDA_NVCC_FLAGS}")
# SET(CUDA_NVCC_FLAGS "-gencode=arch=compute_30,code=compute_30;${CUDA_NVCC_FLAGS}")

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CUDA_SDK_ROOT_DIR}/common/inc
    ${CUDA_TOOLKIT_INCLUDE}
)

# Find VTK.
find_package(VTK)
include(${VTK_USE_FILE})

# Make sure classes are exported
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_EXPORTING")


# SOURCE_GROUP("Core" FILES ${vtkErCoreSources})


set(ER_MODULE_DIRS "")
set(ER_MODULE_DIRS ${ER_MODULE_DIRS}
    "Modules/ErCore/API"
    "Modules/ErCore/Buffer"
    "Modules/ErCore/Color"
    "Modules/ErCore/Cuda"
    "Modules/ErCore/Filter"
    "Modules/ErCore/General"
    "Modules/ErCore/Sample"
    "Modules/ErCore/Shading"
    "Modules/ErCore/Shapes"
    "Modules/ErCore/TransferFunction"
    "Modules/ErCore/Vector"
    "Modules/vtkEr"
    )

foreach(dir ${ER_MODULE_DIRS})
    include_directories(${CMAKE_SOURCE_DIR}/${dir} ${CMAKE_BINARY_DIR}/${dir})
endforeach()

add_subdirectory(Modules)
add_subdirectory(Testing)
# add_subdirectory(Wrapping)




