cmake_minimum_required(VERSION 3.1)


project(JCudaCommonJNI)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../jcuda-common/JCudaCommon_CMake.txt)

set (BUILD_SHARED_LIBS OFF)

include_directories (
    src/
    ${JNI_INCLUDE_DIRS}
)
  
add_library(${PROJECT_NAME}
    src/JNIUtils.cpp
    src/Logger.cpp
    src/PointerUtils.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -fPIC)
