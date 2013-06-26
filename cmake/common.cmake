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
        ${TARGET_NAME}_fr
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
