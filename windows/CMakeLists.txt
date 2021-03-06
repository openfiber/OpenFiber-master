PROJECT(OpenFiber)

# Minimum version of CMake required

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

# CMake policies that need to be specified
# Note: see our main CMakeLists.txt file...

CMAKE_POLICY(SET CMP0020 OLD)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_SOURCE_DIR}/../cmake/common.cmake)

# Initialise the project

INITIALISE_PROJECT()

# Files that make up the CLI version of OpenFiber

SET(SOURCES
    src/main.cpp

    ../src/misc/cliapplication.cpp
    ../src/misc/common.cpp
    ../src/misc/utils.cpp
)

SET(HEADERS_MOC
)

SET(RESOURCES
    ../res/common.qrc
)

# Various include directories

INCLUDE_DIRECTORIES(
    ../src/misc
)

# Build the CLI version of OpenFiber

QT5_WRAP_CPP(SOURCES_MOC ${HEADERS_MOC})
QT5_ADD_RESOURCES(SOURCES_RCS ${RESOURCES})

ADD_EXECUTABLE(${PROJECT_NAME}
    ${SOURCES}
    ${SOURCES_MOC}
    ${SOURCES_RCS}
)

QT5_USE_MODULES(${PROJECT_NAME}
    Core
)

# Additional linker settings

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
)

# Copy the CLI version of OpenFiber to the main build folder

COPY_FILE_TO_BUILD_DIR(${CMAKE_BINARY_DIR} bin
                       ${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX} ${PROJECT_NAME}.com)
