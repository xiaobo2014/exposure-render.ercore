# set(lib ${SV_LIB_MODULE_COMMON_NAME})

# set(lib ErCore)

SET(General
    General/utilities.h
    General/geometry.h
    General/raymarching.h
    General/rng.h
    General/montecarlo.h
    General/ray.h
    General/textures.h
    General/log.h
    General/defines.h
    General/camera.h
    General/procedural.h
    General/matrix.h
    General/boundingbox.h
    General/exception.h
    General/timestamp.h
    General/enums.h
    General/exposurerender.h
    General/exposurerender.cpp
    General/range.h
    General/tracer.h
    General/statistic.h
    General/statistics.h
    General/volume.h
    General/intersection.h
    General/object.h
    General/texture.h
    General/bitmap.h
    General/alignment.h
    General/transform.h
    General/octree.h
    General/transport.h
    General/volumeproperty.h
)

SOURCE_GROUP("General" FILES ${General})

SET(Buffer
    Buffer/buffer.h
    Buffer/buffer1d.h
    Buffer/buffer2d.h
    Buffer/buffer3d.h
    Buffer/randomseedbuffer.h
    Buffer/randomseedbuffer1d.h
    Buffer/randomseedbuffer2d.h
    Buffer/randomseedbuffer3d.h
    Buffer/randomseedbuffers.h
    Buffer/buffers.h
    Buffer/cudatexture.h
    Buffer/cudatexture1d.h
    Buffer/cudatexture2d.h
    Buffer/cudatexture3d.h
    Buffer/cudatextures.h
    Buffer/framebuffer.h
)

SOURCE_GROUP("Buffer" FILES ${Buffer})

SET(TransferFunction
    TransferFunction/piecewisefunctionnode.h
    TransferFunction/piecewisefunction.h
    TransferFunction/piecewiselinearfunction.h
    TransferFunction/transferfunction.h
    TransferFunction/transferfunction1d.h
    TransferFunction/transferfunction2d.h
    TransferFunction/transferfunctions.h
)

SOURCE_GROUP("Transfer Function" FILES ${TransferFunction})

SET(Shapes
    Shapes/shape.h
    Shapes/shapes.h
    Shapes/plane.h
    Shapes/box.h
    Shapes/disk.h
    Shapes/ring.h
    Shapes/sphere.h
    Shapes/cylinder.h
)

SOURCE_GROUP("Shapes" FILES ${Shapes})

SET(Sample
    Sample/mutate.h
    Sample/rendersample.h
    Sample/surfacesample.h
    Sample/lightsample.h
    Sample/lightingsample.h
    Sample/camerasample.h
    Sample/metrosample.h
    Sample/shadersample.h
    Sample/samples.h
)

SOURCE_GROUP("Sample" FILES ${Sample})

SET(Shading
    Shading/shader.h
    Shading/phasefunction.h
    Shading/brdf.h
    Shading/fresnel.h
    Shading/lambert.h
    Shading/microfacet.h
    Shading/blinn.h
)

SOURCE_GROUP("Shading" FILES ${Shading})

SET(API
    API/hostbase.h
    API/hosttracer.h
    API/hostvolume.h
    API/hostobject.h
    API/hosttexture.h
    API/hostbitmap.h
)

SOURCE_GROUP("API" FILES ${API})

SET(Filter
    Filter/filter.h
    Filter/mitchell.h
    Filter/gaussian.h
    Filter/sinc.h
    Filter/triangle.h
)

SOURCE_GROUP("Filter" FILES ${Filter})

SET(Color
    Color/color.h
    Color/colorxyzf.h
    Color/colorxyzaf.h
    Color/colorrgbf.h
    Color/colorrgbaf.h
    Color/colorrgbauc.h
    Color/kelvin.h
)

SOURCE_GROUP("Color" FILES ${Color})

SET(Vector
    Vector/vec.h
    Vector/vec2i.h
    Vector/vec2f.h
    Vector/vec3i.h
    Vector/vec3f.h
    Vector/vec4i.h
    Vector/vec4f.h
    Vector/indices.h
    Vector/hysteresis.h
    Vector/vector.h
)

SOURCE_GROUP("Vector" FILES ${Vector})

SET(Cuda
    Cuda/estimate.cuh
    Cuda/filtering.cuh
    Cuda/tonemap.cuh
    Cuda/autofocus.cuh
    Cuda/dvr.cuh
    Cuda/render.cuh
    Cuda/samplecamera.cuh
    Cuda/samplelight.cuh
    Cuda/sampleshader.cuh
    Cuda/intersect.cuh
    Cuda/gaussianfilterrgbauc.cuh
    Cuda/gaussianfilterxyzaf.cuh
    Cuda/blendrgbauc.cuh
    Cuda/list.cuh
    Cuda/wrapper.cuh
    Cuda/macros.cuh
    Cuda/core.cu
)

SOURCE_GROUP("Cuda" FILES ${Cuda})

# Create the ErCore project as a shared library (DLL)
CUDA_ADD_LIBRARY(ErCore ${General} ${Buffer} ${TransferFunction} ${Shading} ${Shapes} ${Sample} ${API} ${Filter} ${Color} ${Vector} ${Cuda} SHARED)



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




