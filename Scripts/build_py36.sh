# using gcc 5.4

# centos version
#export CC=/usr/local/bin/gcc
#export CXX=/usr/local/bin/g++

#
# build rxcore using externals already in computer
#
RX_BUILDING_LIB_DIR=/home/liuxiaobo/OffScreenFFR_Building
RX_SOURCE_DIR=/home/liuxiaobo/ProjectCode/exposure/exposure-render.ercore/Source
RX_DISTRIBUTE_DIR=/home/liuxiaobo/ProjectCode/exposure/exposure-render.ercore/Distribute

RX_BUILD_DIR=$RX_DISTRIBUTE_DIR/Build
RX_INSTALL_DIR=$RX_DISTRIBUTE_DIR/InstallPy36
RX_PACKAGES_DIR=$RX_DISTRIBUTE_DIR/Packages
RX_PACKAGE_SCRIPT_DIR=$RX_DISTRIBUTE_DIR/PackageScript/python36

## set python dir
PYTHON_DIR=$RX_BUILDING_LIB_DIR/pyPackage
PYTHON_EXE=$PYTHON_DIR/bin/python
PYTHON_INCLUDE=$PYTHON_DIR/include/python3.6m
PYTHON_LIB=$PYTHON_DIR/lib/libpython3.6m.so

## set VTK dir
# using shared lib
LIBRARY_TYPE="SHARED"
RX_VTK_DIR=/home/liuxiaobo/Library/VTK/VTK8.2.0/VTKBuild

# using static lib
# LIBRARY_TYPE="STATIC"
# RX_VTK_DIR=$RX_BUILDING_LIB_DIR/VTK6.2.0/InstallStatic/lib/cmake/vtk-6.2


# rm -Rf $RX_BUILD_DIR/*
# mkdir -p $RX_BUILD_DIR
# rm -Rf $RX_INSTALL_DIR/*
cd $RX_BUILD_DIR

# run cmake
cmake \
-DCMAKE_BUILD_TYPE:STRING="Debug" \
-DCMAKE_INSTALL_PREFIX:PATH="$RX_INSTALL_DIR" \
-DCMAKE_CXX_FLAGS:STRING="-fpermissive" \
-DBUILD_DIR:PATH="$RX_BUILD_DIR" \
-DCUDA_HOST_COMPILER:FILEPATH="/usr/bin/cc" \
-DCUDA_SDK_ROOT_DIR:PATH="/usr/local/cuda-10.1" \
-DCUDA_TOOLKIT_ROOT_DIR:PATH="/usr/local/cuda" \
-DCUDA_USE_STATIC_CUDA_RUNTIME:BOOL=ON \
-DCUDA_rt_LIBRARY:FILEPATH="/usr/lib/x86_64-linux-gnu/librt.so" \
-DVTK_DIR:PATH="$RX_VTK_DIR" \
-DPYTHON_EXECUTABLE:FILEPATH="$PYTHON_EXE" \
-DPYTHON_INCLUDE_DIR:PATH="$PYTHON_INCLUDE" \
-DPYTHON_INCLUDE_PATH:PATH="$PYTHON_INCLUDE" \
-DPYTHON_LIBRARY:FILEPATH="$PYTHON_LIB" \
-DRX_LIBRARY_TYPE:STRING=$LIBRARY_TYPE \
-DER_VTK:BOOL=ON \
-DER_VTK_EXAMPLE:BOOL=ON \
$RX_SOURCE_DIR

# install library
#make install -j 4

# cp package scripts and files
#cp -r $RX_PACKAGE_SCRIPT_DIR/* $RX_INSTALL_DIR

# run package library
#cd $RX_INSTALL_DIR
#$PYTHON_EXE $RX_INSTALL_DIR/setup.py sdist bdist_wheel

# copy package file
#cp $RX_INSTALL_DIR/dist/* $RX_PACKAGES_DIR/cpy36


