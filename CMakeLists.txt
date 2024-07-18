cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(FrontClass)
set(MODULE_NAME "FrontClass")
message(STATUS "CMake version is sufficient. Proceeding with configuration.")

# Define an option to manually set the build scenario
option(EMSDK_COMPILE "Compile with Emscripten" ON)

# Check if EMSDK is set in the environment and use it if the manual flag is not set
# Check if EMSDK is defined in the environment
if (DEFINED ENV{EMSDK})
    set(EMSDK_COMPILE ON)
    message(STATUS "EMSDK is set in the environment. EMSDK_COMPILE flag is ON.")
else()
    set(EMSDK_COMPILE OFF)
    message(STATUS "EMSDK is not set in the environment. EMSDK_COMPILE flag is OFF.")
endif()

# Check if the compiler is Emscripten or GCC
if (CMAKE_C_COMPILER_ID STREQUAL "Emscripten")
    set(EMSDK_COMPILE ON)
    set(GCC_COMPILE OFF)
    message(STATUS "Emscripten compiler is active. EMSDK_COMPILE flag is ON.")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -frounding-math -DCGAL_NO_ASSERTIONS")
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(EMSDK_COMPILE OFF)
    set(GCC_COMPILE ON)
    message(STATUS "GCC compiler is active. GCC_COMPILE flag is ON.")
    message(STATUS "EMSDK_COMPILE flag is OFF.")
    # Add GCC-specific flags if needed
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -frounding-math -DCGAL_NO_ASSERTIONS")
else()
    message(STATUS "Unknown compiler.")
endif()


# Conditional block if EMSDK_COMPILE is ON
if (EMSDK_COMPILE)
    message("Set emscripten CMAKE_BUILD_TYPE Release")
    set(CMAKE_BUILD_TYPE Release)
else()
    #use compiler options from IDE
endif()

if (EMSDK_COMPILE)
    message("Set emscripten environment")
    find_package(Python3)
    # Setup some paths
    set(PYTHON ${Python3_EXECUTABLE} CACHE STRING "Python path")
    set(EMSCRIPTEN_ROOT $ENV{EMSDK}/upstream/emscripten CACHE STRING "Emscripten path")
    set(CMAKE_TOOLCHAIN_FILE ${EMSCRIPTEN_ROOT}/cmake/Modules/Platform/Emscripten.cmake)
    set(WEBIDL_BINDER_SCRIPT ${EMSCRIPTEN_ROOT}/tools/webidl_binder.py)

    #set(RECAST_FRONT_MATTER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/front-matter.js)
    set(WEBIDL_INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src/FrontClass.idl)
endif()

set(WEBIDL_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/front_class.hpp)
set(WEBIDL_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/)
set(WEBIDL_SRC_DIR_LIB ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/)

#add_definitions(-DBOOST_NO_CXX98_FUNCTION_BASE)

# Include cgal and boost headers
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../CPP_lib/CGAL-5.4/include)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../CPP_lib/boost_1_79_0)
#set(CGAL_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../../../CPP_lib/CGAL-5.4/include)
#set(BOOST_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../../../CPP_lib/boost_1_79_0)

set(glm_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../CPP_lib/glm)
set(glm_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../../../CPP_lib/glm)
message(STATUS "glm_DIR: ${glm_INCLUDE_DIRS}")
include_directories(${glm_DIR})

set(include_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/lib)
include_directories(${include_DIR})


SET(DrawSupport_INC
    src/lib/DrawSupport/drawSupport.hpp
)
SET(DrawSupport_SRC
    src/lib/DrawSupport/drawHalfEdges.cpp
    src/lib/DrawSupport/triangulateCentralPoint.cpp

)

SET(Operations_INC
    src/lib/Operations/Operations.hpp
    src/lib/CreateOperations/createOperations.hpp
    src/lib/Modificators/Modificators.hpp

)
SET(Operations_SRC
    src/lib/Operations/addFace.cpp
    src/lib/Operations/addVertex.cpp
    src/lib/Operations/addEdge.cpp
    src/lib/Operations/setFromGeometry.cpp

    src/lib/Operations/removeEdge.cpp
    src/lib/Operations/removeFace.cpp
    src/lib/Operations/removeVertex.cpp
    src/lib/Operations/cutFace.cpp
    src/lib/Operations/splitEdge.cpp

    src/lib/Operations/testFunctions.cpp

    src/lib/CreateOperations/cubeCreater.cpp
    src/lib/CreateOperations/planeCreater.cpp

    src/lib/Modificators/catmullClark.cpp
    src/lib/Modificators/quadSubDivide.cpp
)

SET(main_INC
    src/lib/Core/Halfedge.hpp
    src/lib/Core/Vertex.hpp
    src/lib/Core/Face.hpp
    src/lib/Core/Geometry.hpp
    src/lib/Core/Struct.hpp

    src/lib/Core/HalfedgeDS.hpp
    src/front_class.hpp
)
SET(main_SRC_ext

    src/lib/Core/Geometry.cpp
    src/lib/Core/Face.cpp
    src/lib/Core/Vertex.cpp
    src/lib/Core//Halfedge.cpp
    src/lib/Core/HalfedgeDS.cpp

    #src/front_class.cpp
)

SET(main_SRC
   
    src/front_class.cpp
)

add_library(${MODULE_NAME} STATIC
        ${Operations_INC} ${Operations_SRC}
        ${DrawSupport_INC} ${DrawSupport_SRC}
        ${main_INC} ${main_SRC}
        ${main_SRC_ext}
)


#set_target_properties(${MODULE_NAME} PROPERTIES COMPILE_FLAGS "-O3")

if (EMSDK_COMPILE)

    message("Start complile glue files and emscripten wasm")
    set(EMCC_ARGS
      #-flto
      -frounding-math  
      #--extern-pre-js ${RECAST_FRONT_MATTER_FILE}
      --post-js glue.js
      -s INITIAL_MEMORY=47185920  #2684354560 # 640 * 1024 * 1024 = 640MB
      #-s TOTAL_MEMORY=47185920 # 1,288,523,776 bytes (multiple of 64KiB)
      -s ALLOW_MEMORY_GROWTH=1
      -s ALLOW_TABLE_GROWTH=1
      -s EXPORTED_FUNCTIONS=["_malloc","_free"]
      -s EXPORTED_RUNTIME_METHODS=["UTF8ToString","addFunction"]
      -s EXPORT_NAME="FrontClass"
      -s MODULARIZE=1
      -s ENVIRONMENT='web'
      -s NO_EXIT_RUNTIME=1
      -s NO_FILESYSTEM=1
      -s FILESYSTEM=0
      -s EXPORT_ES6=1
      -s BINARYEN_IGNORE_IMPLICIT_TRAPS=1
      #-s ASSERTIONS=1
      -s NO_DISABLE_EXCEPTION_CATCHING
      -DCGAL_NDEBUG
      -DCGAL_DISABLE_ROUNDING_MATH_CHECK
      -DCGAL_DISABLE_GMP
      -DCGAL_NO_STATIC_FILTERS
      #-DCGAL_USE_CORE
      -DBOOST_NO_CXX98_FUNCTION_BASE
      -O3
      #-msimd128
    )

    #if ("${CMAKE_BUILD_TYPE}" STREQUAL "debug")
    #        set(EMCC_ARGS
    #                ${EMCC_ARGS}
    #                #-g
    #                #-s ASSERTIONS
    #                )
    #else()
    #        set(EMCC_ARGS
    #                ${EMCC_ARGS}
    #                -O3)
    #endif()

    set(EMCC_WASM_ESM_ARGS ${EMCC_ARGS}
      -s WASM=1)

    set(EMCC_WASM_COMPAT_ESM_ARGS ${EMCC_ARGS}
      -s SINGLE_FILE=1
      -s WASM=1)

    set(OPENMESH_LIB_PATH "/home/geo/my_projects/Centaurus/CPP_lib/OpenMesh/Build/lib")


    set(EMCC_GLUE_ARGS
      -c
      -I ${main_SRC}
      #-I ${CGAL_INCLUDE}
      #-I ${BOOST_INCLUDE}
      -I ${glm_DIR}
      -include ${WEBIDL_HEADER} 
    )

    # GLUE
    add_custom_command(
      OUTPUT glue.cpp glue.js
      BYPRODUCTS parser.out WebIDLGrammar.pkl
      COMMAND ${PYTHON} ${WEBIDL_BINDER_SCRIPT} ${WEBIDL_INTERFACE} glue
      DEPENDS ${WEBIDL_INTERFACE}
      COMMENT "Generating ${MODULE_NAME} bindings"
      VERBATIM)

    add_custom_command(
      OUTPUT glue.o
      COMMAND emcc glue.cpp ${EMCC_GLUE_ARGS}  -o glue.o
      DEPENDS glue.cpp ${WEBIDL_HEADER}
      COMMENT "Building ${MODULE_NAME} bindings"
      VERBATIM)

    add_custom_target(${MODULE_NAME}-bindings ALL DEPENDS glue.js glue.o )

    # ES6 WASM
    add_custom_command(
      OUTPUT ${MODULE_NAME}.wasm.js ${MODULE_NAME}.wasm.wasm
      COMMAND emcc glue.o lib${MODULE_NAME}.a  ${EMCC_WASM_ESM_ARGS} -o ${MODULE_NAME}.wasm.js
      DEPENDS ${MODULE_NAME}-bindings ${MODULE_NAME} 
      COMMENT "Building ${MODULE_NAME} webassembly"
      VERBATIM)
    add_custom_target(${MODULE_NAME}-wasm ALL DEPENDS ${MODULE_NAME}.wasm.js ${MODULE_NAME}.wasm.wasm)

    # ES6 INLINED BASE64 WASM
    add_custom_command(
      OUTPUT ${MODULE_NAME}.wasm-compat.js
      COMMAND emcc glue.o lib${MODULE_NAME}.a  ${EMCC_WASM_COMPAT_ESM_ARGS} -o ${MODULE_NAME}.wasm-compat.js
      DEPENDS ${MODULE_NAME}-bindings ${MODULE_NAME} 
      COMMENT "Building ${MODULE_NAME} inlined base64 webassembly"
      VERBATIM)
    add_custom_target(${MODULE_NAME}-wasm-compat ALL DEPENDS ${MODULE_NAME}.wasm-compat.js)

else()
    message("Start complile gcc compiler")
    set_target_properties(${MODULE_NAME} PROPERTIES COMPILE_FLAGS "-g -O0")
    add_definitions(-DCGAL_NO_ASSERTIONS -DCGAL_NO_PRECONDITIONS)
    target_compile_definitions(${MODULE_NAME} PRIVATE CGAL_NO_ASSERTIONS CGAL_NO_PRECONDITIONS CGAL_DISABLE_GMP CGAL_NO_GMP CGAL_USE_CORE)
endif()
