# using gcc 5.4

# centos version
#export CC=/usr/local/bin/gcc
#export CXX=/usr/local/bin/g++

#
# build rxcore using externals already in computer
#
RX_BUILDING_LIB_DIR=/home/liuxiaobo/OffScreenFFR_Building
RX_SOURCE_DIR=/home/liuxiaobo/OffScreenFFR/Source
RX_DISTRIBUTE_DIR=/home/liuxiaobo/OffScreenFFR/Distribute
RX_BUILD_DIR=$RX_DISTRIBUTE_DIR/Build
RX_INSTALL_DIR=$RX_DISTRIBUTE_DIR/InstallPy37
RX_PACKAGES_DIR=$RX_DISTRIBUTE_DIR/Packages
RX_PACKAGE_SCRIPT_DIR=$RX_DISTRIBUTE_DIR/PackageScript/python37

## set python dir
PYTHON_DIR=$RX_BUILDING_LIB_DIR/py37
PYTHON_EXE=$PYTHON_DIR/bin/python
PYTHON_INCLUDE=$PYTHON_DIR/include/python3.7m
PYTHON_LIB=$PYTHON_DIR/lib/libpython3.7m.so

## set VTK dir
# using shared lib
LIBRARY_TYPE="SHARED"
RX_VTK_DIR=$RX_BUILDING_LIB_DIR/VTK8.2.0/EGLInstall/lib/cmake/vtk-8.2

# using static lib
# LIBRARY_TYPE="STATIC"
# RX_VTK_DIR=$RX_BUILDING_LIB_DIR/VTK6.2.0/InstallStatic/lib/cmake/vtk-6.2


rm -Rf $RX_BUILD_DIR/*
mkdir -p $RX_BUILD_DIR
rm -Rf $RX_INSTALL_DIR/*
cd $RX_BUILD_DIR

# run cmake
cmake \
-DCMAKE_BUILD_TYPE:STRING="Release" \
-DCMAKE_INSTALL_PREFIX:PATH="$RX_INSTALL_DIR" \
-DCMAKE_CXX_FLAGS:STRING="-fpermissive" \
-DBUILD_DIR:PATH="$RX_BUILD_DIR" \
-DVTK_DIR:PATH="$RX_VTK_DIR" \
-DPYTHON_EXECUTABLE:FILEPATH="$PYTHON_EXE" \
-DPYTHON_INCLUDE_DIR:PATH="$PYTHON_INCLUDE" \
-DPYTHON_INCLUDE_PATH:PATH="$PYTHON_INCLUDE" \
-DPYTHON_LIBRARY:FILEPATH="$PYTHON_LIB" \
-DRX_LIBRARY_TYPE:STRING=$LIBRARY_TYPE \
$RX_SOURCE_DIR

# install library
make install -j 4

# cp package scripts and files
cp -r $RX_PACKAGE_SCRIPT_DIR/* $RX_INSTALL_DIR

# run package library
cd $RX_INSTALL_DIR
$PYTHON_EXE $RX_INSTALL_DIR/setup.py sdist bdist_wheel

# copy package file
cp $RX_INSTALL_DIR/dist/* $RX_PACKAGES_DIR/cpy37


