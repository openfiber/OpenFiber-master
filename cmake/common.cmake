MACRO(INITIALISE_PROJECT)
#   SET(CMAKE_VERBOSE_MAKEFILE ON)
    SET(CMAKE_INCLUDE_CURRENT_DIR ON)

    # Check whether we are building in 32-bit or 64-bit

    IF(CMAKE_SIZEOF_VOID_P EQUAL 4)
        SET(32BIT_MODE ON)
    ELSEIF(CMAKE_SIZEOF_VOID_P EQUAL 8)
        SET(32BIT_MODE OFF)
    ELSE()
        MESSAGE(FATAL_ERROR "Sorry, but OpenFiber can only be built in 32-bit or 64-bit...")
    ENDIF()

    # Required packages

    FIND_PACKAGE(Qt5Widgets REQUIRED)

    # Keep track of some information about Qt

    SET(QT_BINARY_DIR ${_qt5Widgets_install_prefix}/bin)
    SET(QT_LIBRARY_DIR ${_qt5Widgets_install_prefix}/lib)
    SET(QT_PLUGINS_DIR ${_qt5Widgets_install_prefix}/plugins)
    SET(QT_VERSION_MAJOR ${Qt5Widgets_VERSION_MAJOR})
    SET(QT_VERSION_MINOR ${Qt5Widgets_VERSION_MINOR})
    SET(QT_VERSION_PATCH ${Qt5Widgets_VERSION_PATCH})

    IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        MESSAGE("Building a debug version...")

        SET(DEBUG_MODE ON)

        # Default compiler settings

        IF(WIN32)

        ELSE()

        ENDIF()

        # Make sure that debugging is on for Qt

        ADD_DEFINITIONS(-DQT_DEBUG)
    ELSE()
        MESSAGE("Building a release version...")

        SET(DEBUG_MODE OFF)

        # Default compiler and linker settings

        IF(WIN32)

        ELSE()

        ENDIF()

        # Make sure that debugging is off for Qt

        ADD_DEFINITIONS(-DQT_NO_DEBUG)
    ENDIF()

    # Ask for Unicode to be used

    ADD_DEFINITIONS(-DUNICODE)

    IF(WIN32)
        ADD_DEFINITIONS(-D_UNICODE)
    ENDIF()

    # Default location of external dependencies

    IF(WIN32)
        SET(DISTRIB_DIR windows/x86)
    ELSEIF(APPLE)
        SET(DISTRIB_DIR osx)
    ELSE()
        IF(32BIT_MODE)
            SET(DISTRIB_DIR linux/x86)
        ELSE()
            SET(DISTRIB_DIR linux/x64)
        ENDIF()
    ENDIF()

    IF(WIN32)
        IF(DEBUG_MODE)
            SET(DISTRIB_BINARY_DIR ${DISTRIB_DIR}/debug)
        ELSE()
            SET(DISTRIB_BINARY_DIR ${DISTRIB_DIR}/release)
        ENDIF()
    ELSE()
        SET(DISTRIB_BINARY_DIR ${DISTRIB_DIR})
    ENDIF()

    # Set the RPATH information on Linux
    # Note: this prevent us from having to use the uncool LD_LIBRARY_PATH...

    IF(NOT WIN32 AND NOT APPLE)
        SET(CMAKE_INSTALL_RPATH "$ORIGIN/../lib:$ORIGIN/../plugins/${PROJECT_NAME}")
    ENDIF()

ENDMACRO()

MACRO(UPDATE_LANGUAGE_FILES TARGET_NAME)
    # Update the translation (.ts) files (if they exist) and generate the
    # language (.qm) files which will later on be embedded in the project
    # itself
    # Note: this requires SOURCES, HEADERS_MOC and UIS to be defined for the
    #       current CMake project, even if that means that these variables are
    #       to be empty (the case with some plugins for example). Indeed, since
    #       otherwise the value of these variables, as defined in a previous
    #       project, may be used, so...

    SET(LANGUAGE_FILES
        ${TARGET_NAME}_ru
    )

    FOREACH(LANGUAGE_FILE ${LANGUAGE_FILES})
        SET(TS_FILE i18n/${LANGUAGE_FILE}.ts)

        IF(EXISTS ${PROJECT_SOURCE_DIR}/${TS_FILE})
            EXECUTE_PROCESS(COMMAND ${QT_BINARY_DIR}/lupdate -no-obsolete
                                                             ${SOURCES} ${HEADERS_MOC} ${UIS}
                                                         -ts ${TS_FILE}
                            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
            EXECUTE_PROCESS(COMMAND ${QT_BINARY_DIR}/lrelease ${PROJECT_SOURCE_DIR}/${TS_FILE}
                                                          -qm ${CMAKE_BINARY_DIR}/${LANGUAGE_FILE}.qm)
        ENDIF()
    ENDFOREACH()
ENDMACRO()

MACRO(ADD_PLUGIN PLUGIN_NAME)
    # Various initialisations

    SET(PLUGIN_NAME ${PLUGIN_NAME})

    SET(SOURCES)
    SET(HEADERS_MOC)
    SET(UIS)
    SET(INCLUDE_DIRS)
    SET(DEFINITIONS)
    SET(PLUGIN_DEPENDENCIES)
    SET(PLUGIN_BINARY_DEPENDENCIES)
    SET(QT_MODULES)
    SET(QT_DEPENDENCIES)
    SET(EXTERNAL_BINARY_DEPENDENCIES_DIR)
    SET(EXTERNAL_BINARY_DEPENDENCIES)
    SET(EXTERNAL_LIBRARY_DEPENDENCIES)
    SET(TESTS)

    # Analyse the extra parameters

    SET(TYPE_OF_PARAMETER 0)

    FOREACH(PARAMETER ${ARGN})
        IF(${PARAMETER} STREQUAL "THIRD_PARTY")
            # Add a definition in case of compilation from within Qt Creator
            # using MSVC since JOM overrides some of our settings

            IF(WIN32)
                ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
            ENDIF()
        ELSEIF(${PARAMETER} STREQUAL "SOURCES")
            SET(TYPE_OF_PARAMETER 1)
        ELSEIF(${PARAMETER} STREQUAL "HEADERS_MOC")
            SET(TYPE_OF_PARAMETER 2)
        ELSEIF(${PARAMETER} STREQUAL "UIS")
            SET(TYPE_OF_PARAMETER 3)
        ELSEIF(${PARAMETER} STREQUAL "INCLUDE_DIRS")
            SET(TYPE_OF_PARAMETER 4)
        ELSEIF(${PARAMETER} STREQUAL "DEFINITIONS")
            SET(TYPE_OF_PARAMETER 5)
        ELSEIF(${PARAMETER} STREQUAL "PLUGIN_DEPENDENCIES")
            SET(TYPE_OF_PARAMETER 6)
        ELSEIF(${PARAMETER} STREQUAL "PLUGIN_BINARY_DEPENDENCIES")
            SET(TYPE_OF_PARAMETER 7)
        ELSEIF(${PARAMETER} STREQUAL "QT_MODULES")
            SET(TYPE_OF_PARAMETER 8)
        ELSEIF(${PARAMETER} STREQUAL "QT_DEPENDENCIES")
            SET(TYPE_OF_PARAMETER 9)
        ELSEIF(${PARAMETER} STREQUAL "EXTERNAL_BINARY_DEPENDENCIES_DIR")
            SET(TYPE_OF_PARAMETER 10)
        ELSEIF(${PARAMETER} STREQUAL "EXTERNAL_BINARY_DEPENDENCIES")
            SET(TYPE_OF_PARAMETER 11)
        ELSEIF(${PARAMETER} STREQUAL "EXTERNAL_LIBRARY_DEPENDENCIES")
            SET(TYPE_OF_PARAMETER 12)
        ELSEIF(${PARAMETER} STREQUAL "TESTS")
            SET(TYPE_OF_PARAMETER 13)
        ELSE()
            # Not one of the headers, so add the parameter to the corresponding
            # set

            IF(${TYPE_OF_PARAMETER} EQUAL 1)
                LIST(APPEND SOURCES ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 2)
                LIST(APPEND HEADERS_MOC ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 3)
                LIST(APPEND UIS ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 4)
                LIST(APPEND INCLUDE_DIRS ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 5)
                LIST(APPEND DEFINITIONS ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 6)
                LIST(APPEND PLUGIN_DEPENDENCIES ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 7)
                LIST(APPEND PLUGIN_BINARY_DEPENDENCIES ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 8)
                LIST(APPEND QT_MODULES ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 9)
                LIST(APPEND QT_DEPENDENCIES ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 10)
                SET(EXTERNAL_BINARY_DEPENDENCIES_DIR ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 11)
                LIST(APPEND EXTERNAL_BINARY_DEPENDENCIES ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 12)
                LIST(APPEND EXTERNAL_LIBRARY_DEPENDENCIES ${PARAMETER})
            ELSEIF(${TYPE_OF_PARAMETER} EQUAL 13)
                LIST(APPEND TESTS ${PARAMETER})
            ENDIF()
        ENDIF()
    ENDFOREACH()

    # Various include directories

    SET(PLUGIN_INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

    INCLUDE_DIRECTORIES(${INCLUDE_DIRS})

    # Resource file, if any

    SET(QRC_FILE res/${PLUGIN_NAME}.qrc)

    IF(EXISTS ${PROJECT_SOURCE_DIR}/${QRC_FILE})
        SET(RESOURCES ${QRC_FILE})
    ELSE()
        SET(RESOURCES)
    ENDIF()

    # Update the translation (.ts) files and generate the language (.qm) files
    # which will later on be embedded in the plugin itself

    IF(NOT "${RESOURCES}" STREQUAL "")
        UPDATE_LANGUAGE_FILES(${PLUGIN_NAME})
    ENDIF()

    # Definition to make sure that the plugin can be used by other plugins

    ADD_DEFINITIONS(-D${PLUGIN_NAME}_PLUGIN)

    # Some plugin-specific definitions

    FOREACH(DEFINITION ${DEFINITIONS})
        ADD_DEFINITIONS(-D${DEFINITION})
    ENDFOREACH()

    # Generate and add the different files needed by the plugin

    IF("${HEADERS_MOC}" STREQUAL "")
        SET(SOURCES_MOC)
    ELSE()
        QT5_WRAP_CPP(SOURCES_MOC ${HEADERS_MOC})
    ENDIF()

    IF("${UIS}" STREQUAL "")
        SET(SOURCES_UIS)
    ELSE()
        QT5_WRAP_UI(SOURCES_UIS ${UIS})
    ENDIF()

    IF("${RESOURCES}" STREQUAL "")
        SET(SOURCES_RCS)
    ELSE()
        QT5_ADD_RESOURCES(SOURCES_RCS ${RESOURCES})
    ENDIF()

    ADD_LIBRARY(${PROJECT_NAME} SHARED
        ${SOURCES}
        ${SOURCES_MOC}
        ${SOURCES_UIS}
        ${SOURCES_RCS}
    )

    # OpenFiber dependencies

    FOREACH(PLUGIN_DEPENDENCY ${PLUGIN_DEPENDENCIES})
        TARGET_LINK_LIBRARIES(${PROJECT_NAME}
            ${PLUGIN_DEPENDENCY}Plugin
        )
    ENDFOREACH()

    # OpenFiber binary dependencies

    FOREACH(PLUGIN_BINARY_DEPENDENCY ${PLUGIN_BINARY_DEPENDENCIES})
        TARGET_LINK_LIBRARIES(${PROJECT_NAME}
            ${PLUGIN_BINARY_DEPENDENCY}
        )
    ENDFOREACH()

    # Qt modules

    FOREACH(QT_MODULE ${QT_MODULES})
        QT5_USE_MODULES(${PROJECT_NAME}
            ${QT_MODULE}
        )
    ENDFOREACH()

    # Linker settings

    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME ${PLUGIN_NAME}
        LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
    )

    # External binary dependencies

    IF(NOT ${EXTERNAL_BINARY_DEPENDENCIES_DIR} STREQUAL "")
        FOREACH(EXTERNAL_BINARY_DEPENDENCY ${EXTERNAL_BINARY_DEPENDENCIES})
            TARGET_LINK_LIBRARIES(${PROJECT_NAME}
                ${EXTERNAL_BINARY_DEPENDENCIES_DIR}/${EXTERNAL_BINARY_DEPENDENCY}
            )
        ENDFOREACH()
    ENDIF()

    # External library dependencies

    FOREACH(EXTERNAL_LIBRARY_DEPENDENCY ${EXTERNAL_LIBRARY_DEPENDENCIES})
        TARGET_LINK_LIBRARIES(${PROJECT_NAME}
            ${EXTERNAL_LIBRARY_DEPENDENCY}
        )
    ENDFOREACH()

    # Location of our plugins

    IF(WIN32)
        STRING(REPLACE "${${MAIN_PROJECT_NAME}_SOURCE_DIR}" "" PLUGIN_BUILD_DIR ${PROJECT_SOURCE_DIR})
        SET(PLUGIN_BUILD_DIR "${CMAKE_BINARY_DIR}${PLUGIN_BUILD_DIR}")
        # Note: MSVC generate things in a different place to GCC, so...
    ELSE()
        SET(PLUGIN_BUILD_DIR ${LIBRARY_OUTPUT_PATH})
    ENDIF()

    # Copy the plugin to our plugins directory
    # Note: this is done so that we can, on Windows and Linux, test the use of
    #       plugins in OpenFiber without first having to package OpenFiber...

    SET(PLUGIN_FILENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})

    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy ${PLUGIN_BUILD_DIR}/${PLUGIN_FILENAME}
                                                        ${DEST_PLUGINS_DIR}/${PLUGIN_FILENAME})

    # On Windows, make a copy of the plugin to our main build directory, since
    # this is where it will be on Linux and OS X and where any test which
    # requires the plugin will expect it to be, but this is not where MSVC
    # generates the plugin, so...

    IF(WIN32)
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy ${PLUGIN_BUILD_DIR}/${PLUGIN_FILENAME}
                                                            ${CMAKE_BINARY_DIR}/${PLUGIN_FILENAME})
    ENDIF()

    # Package the plugin itself, but only if we are not on OS X since it will
    # have already been copied

    IF(NOT APPLE)
        INSTALL(FILES ${PLUGIN_BUILD_DIR}/${PLUGIN_FILENAME}
                DESTINATION plugins/${MAIN_PROJECT_NAME})
    ENDIF()
ENDMACRO()

MACRO(COPY_FILE_TO_BUILD_DIR ORIG_DIRNAME DEST_DIRNAME FILENAME)
    IF(EXISTS ${CMAKE_BINARY_DIR}/../cmake)
        # A CMake directory exists at the same level as the binary directory,
        # so we are dealing with the main project

        SET(REAL_DEST_DIRNAME ${CMAKE_BINARY_DIR}/${DEST_DIRNAME})
    ELSE()
        # No CMake directory exists at the same level as the binary directory,
        # so we are dealing with a non-main project

        SET(REAL_DEST_DIRNAME ${CMAKE_BINARY_DIR}/../../build/${DEST_DIRNAME})
    ENDIF()

    IF("${ARGN}" STREQUAL "")
        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy ${ORIG_DIRNAME}/${FILENAME}
                                                            ${REAL_DEST_DIRNAME}/${FILENAME})
    ELSE()
        # An argument was passed so use it to rename the file which is to be
        # copied

        ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy ${ORIG_DIRNAME}/${FILENAME}
                                                            ${REAL_DEST_DIRNAME}/${ARGN})
    ENDIF()
ENDMACRO()

MACRO(WINDOWS_DEPLOY_QT_LIBRARIES)
    FOREACH(LIBRARY ${ARGN})
        # Deploy the Qt library itself

        INSTALL(FILES ${QT_BINARY_DIR}/${LIBRARY}${CMAKE_SHARED_LIBRARY_SUFFIX}
                DESTINATION bin)
    ENDFOREACH()
ENDMACRO()

MACRO(WINDOWS_DEPLOY_QT_PLUGIN PLUGIN_CATEGORY)
    FOREACH(PLUGIN_NAME ${ARGN})
        # Deploy the Qt plugin itself

        INSTALL(FILES ${QT_PLUGINS_DIR}/${PLUGIN_CATEGORY}/${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}
                DESTINATION plugins/${PLUGIN_CATEGORY})
    ENDFOREACH()
ENDMACRO()

MACRO(WINDOWS_DEPLOY_LIBRARY DIRNAME FILENAME)
    # Copy the library file to both the build and build/bin folders, so we can
    # test things without first having to deploy OpenFiber

    COPY_FILE_TO_BUILD_DIR(${DIRNAME} . ${FILENAME})
    COPY_FILE_TO_BUILD_DIR(${DIRNAME} bin ${FILENAME})

    # Install the library file

    INSTALL(FILES ${DIRNAME}/${FILENAME}
            DESTINATION bin)
ENDMACRO()

MACRO(LINUX_DEPLOY_QT_PLUGIN PLUGIN_CATEGORY)
    FOREACH(PLUGIN_NAME ${ARGN})
        # Deploy the Qt plugin itself

        INSTALL(FILES ${QT_PLUGINS_DIR}/${PLUGIN_CATEGORY}/${CMAKE_SHARED_LIBRARY_PREFIX}${PLUGIN_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}
                DESTINATION plugins/${PLUGIN_CATEGORY})
    ENDFOREACH()
ENDMACRO()

MACRO(LINUX_DEPLOY_LIBRARY DIRNAME FILENAME)
    # Copy the library file to the build folder, so we can test things without
    # first having to deploy OpenFiber

    COPY_FILE_TO_BUILD_DIR(${DIRNAME} . ${FILENAME})

    # Install the library file

    INSTALL(FILES ${DIRNAME}/${FILENAME} DESTINATION lib)
ENDMACRO()
