PROJECT(OpenFiber)

# Minimum required version of CMake

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

# Some in-house CMake functions/macros

INCLUDE(${CMAKE_SOURCE_DIR}/cmake/common.cmake)

# Initialise the project

INITIALISE_PROJECT()

# Version/snapshot of OpenFiber

SET(SNAPSHOT ON)

SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 0)
SET(VERSION_PATCH 0)

IF(SNAPSHOT)
    TRY_RUN(DATE_RUN DATE_COMPILE
            ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/cmake/date.c
            RUN_OUTPUT_VARIABLE DATE)

    SET(VERSION ${DATE})
ELSE()
    SET(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
ENDIF()

FILE(WRITE ${CMAKE_BINARY_DIR}/version.txt ${VERSION})


# Files that make up the GUI version of OpenFiber

SET(SOURCES
    src/main.cpp
    src/mainwindow.cpp

    src/misc/cliapplication.cpp
    src/misc/common.cpp
    src/misc/utils.cpp

    src/plugins/coreinterface.cpp
    src/plugins/i18ninterface.cpp
    src/plugins/interface.cpp
    src/plugins/plugin.cpp
    src/plugins/plugininfo.cpp
    src/plugins/pluginmanager.cpp
    src/plugins/solverinterface.cpp
)

SET(HEADERS_MOC
    src/mainwindow.h

    src/plugins/plugin.h
    src/plugins/pluginmanager.h
)

SET(UIS
    src/mainwindow.ui
)

SET(RESOURCES
    res/ui.qrc
    res/common.qrc
)

# Various include directories

INCLUDE_DIRECTORIES(
    src/misc
    src/plugins
)

# Update the translation (.ts) files and generate the language (.qm) files
# which will later on be embedded in the executable as resources

UPDATE_LANGUAGE_FILES(${PROJECT_NAME} TRUE)

# Set the application icon, but only for Windows, since in the case of
# Linux, it's done through res/OpenFiber.qrc

IF(WIN32)
    LIST(APPEND SOURCES ${CMAKE_SOURCE_DIR}/res/${PROJECT_NAME}.rc)
ENDIF()

SET(PLUGINS
    # Build the plugins which may be required by other plugins
    # Note: the build order is very important...
    3rdparty/SUNDIALS
)

FOREACH(PLUGIN ${PLUGINS})
    SET(PLUGIN_DIR src/plugins/${PLUGIN})

    ADD_SUBDIRECTORY(${PLUGIN_DIR})
    # Note: this will process the CMakeLists.txt file of the plugin resulting in
    #       PLUGIN_INCLUDE_DIRS being set, hence we can use it below to update
    #       our include directories

    FOREACH(PLUGIN_INCLUDE_DIR ${PLUGIN_INCLUDE_DIRS})
        INCLUDE_DIRECTORIES(${PLUGIN_DIR}/${PLUGIN_INCLUDE_DIR})
    ENDFOREACH()

    INCLUDE_DIRECTORIES(build/${PLUGIN_DIR})
ENDFOREACH()

# Build the GUI version of OpenFiber

QT5_WRAP_CPP(SOURCES_MOC ${HEADERS_MOC})
QT5_WRAP_UI(SOURCES_UIS ${UIS})
QT5_ADD_RESOURCES(SOURCES_RCS ${RESOURCES})

ADD_EXECUTABLE(${PROJECT_NAME} WIN32
    ${SOURCES}
    ${SOURCES_MOC}
    ${SOURCES_UIS}
    ${SOURCES_RCS}
)

QT5_USE_MODULES(${PROJECT_NAME}
    Concurrent
    Help
    Network
    PrintSupport
    Svg
    WebKitWidgets
    Widgets
    Xml
)

# Additional compiler and linker settings

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "${LINK_FLAGS_PROPERTIES}"
)

# Some post-processing specific to Windows and Linux
# Copy the OpenFiber executable to the build/bin folder

COPY_FILE_TO_BUILD_DIR(${CMAKE_BINARY_DIR} bin
                       ${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX})

IF(WIN32)
    # Copy the OpenFiber executable to the build/Release folder, so that
    # CPack can find it since this is where it expects it to be, except
    # that make.bat asks for the OpenFiber executable to be generated in
    # build to match what does Qt Creator, so...

    COPY_FILE_TO_BUILD_DIR(${CMAKE_BINARY_DIR} Release
                           ${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX})
ENDIF()

# Package OpenFiber (using CPack)

SET(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
SET(CPACK_PACKAGE_CONTACT "Alexander Kursanov (alexander.kursanov@gmail.com)")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A cross-platform modelling environment of cardiac fiber")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/distrib/readMe.txt")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
SET(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME}" "${PROJECT_NAME}")

IF(WIN32)
    # Select NSIS and ZIP as the packagers on Windows

    SET(CPACK_GENERATOR NSIS ZIP)

    SET(CPACK_SYSTEM_NAME "Windows")

    SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/distrib/licensing.txt")

    # Install the GUI version of OpenFiber

    INSTALL(FILES ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}
            DESTINATION bin)
    #INSTALL(FILES ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}.com
    #        DESTINATION bin)

    # MinGW files required by OpenFiber

    FOREACH(MINGW_LIBRARY mingwm10 libgcc_s_sjlj-1 libwinpthread-1 libstdc++-6)
        INSTALL(FILES ${CMAKE_SOURCE_DIR}/distrib/windows/x86/${MINGW_LIBRARY}${CMAKE_SHARED_LIBRARY_SUFFIX} DESTINATION bin)
    ENDFOREACH()

    # Qt libraries required by OpenFiber

    WINDOWS_DEPLOY_QT_LIBRARIES(Qt5CLucene Qt5Core Qt5Gui Qt5Help Qt5Multimedia Qt5MultimediaWidgets Qt5Network Qt5OpenGL Qt5PrintSupport Qt5Qml Qt5Quick Qt5Sql Qt5Svg Qt5V8 Qt5WebKit Qt5WebKitWidgets Qt5Widgets Qt5Xml)
    WINDOWS_DEPLOY_QT_LIBRARIES(icudt49 icuin49 icuuc49)
    WINDOWS_DEPLOY_QT_LIBRARIES(libEGL libGLESv2 D3DCompiler_43)

    # Qt plugins required by OpenFiber

    WINDOWS_DEPLOY_QT_PLUGIN(accessible qtaccessiblequick qtaccessiblewidgets)
    WINDOWS_DEPLOY_QT_PLUGIN(imageformats qgif qico qjpeg qmng qsvg qtga qtiff qwbmp)

    FOREACH(QT_PLUGIN_FILE qminimal qwindows)
        INSTALL(FILES ${QT_PLUGINS_DIR}/platforms/${QT_PLUGIN_FILE}${CMAKE_SHARED_LIBRARY_SUFFIX}
                DESTINATION bin/platforms)
        # Note: yes, the platforms plugin must be installed in the bin folder
        #       and not the plugins folder...
    ENDFOREACH()

    WINDOWS_DEPLOY_QT_PLUGIN(printsupport windowsprintersupport)
    WINDOWS_DEPLOY_QT_PLUGIN(sqldrivers qsqlite)


    # Batch and VBScript files to run OpenFiber (useful when downloading a ZIPped
    # version of OpenFiber)

    SET(BatFile ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bat)
    SET(VbsFile ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.vbs)

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/windows/application.bat.in ${BatFile})
    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/windows/application.vbs.in ${VbsFile})

    INSTALL(FILES ${BatFile}
            DESTINATION .)
    INSTALL(FILES ${VbsFile}
            DESTINATION .)
ELSE()
    # Select TGZ as the packager on Linux

    SET(CPACK_GENERATOR TGZ)

    SET(CPACK_SYSTEM_NAME "Linux")

    IF(32BIT_MODE)
        SET(CPACK_SYSTEM_NAME "${CPACK_SYSTEM_NAME}32")
    ELSE()
        SET(CPACK_SYSTEM_NAME "${CPACK_SYSTEM_NAME}64")
    ENDIF()

    # OpenFiber itself

    INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

    # Qt libraries required by OpenFiber
    # Note: unlike for Windows, we don't have a macro for deploying our
    #       Qt libraries. Indeed, on Linux, the version of a file is appended to
    #       its suffix, so unless making the macro unnecessarily cumbersome to
    #       use, it's better to do things ourselves...

    FOREACH(QT_LIBRARY CLucene Core DBus Gui Help Network OpenGL PrintSupport Qml Quick Sql Svg V8 WebKit WebKitWidgets Widgets Xml)
        INSTALL(FILES ${QT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}${QT_LIBRARY}${CMAKE_SHARED_LIBRARY_SUFFIX}.${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}
                DESTINATION lib
                RENAME ${CMAKE_SHARED_LIBRARY_PREFIX}Qt${QT_VERSION_MAJOR}${QT_LIBRARY}${CMAKE_SHARED_LIBRARY_SUFFIX}.${QT_VERSION_MAJOR})
    ENDFOREACH()

    FOREACH(QT_FILE icudata icui18n icuuc)
        INSTALL(FILES ${QT_LIBRARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${QT_FILE}${CMAKE_SHARED_LIBRARY_SUFFIX}.49.1.2
                DESTINATION lib
                RENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${QT_FILE}${CMAKE_SHARED_LIBRARY_SUFFIX}.49)
    ENDFOREACH()

    # Additional libraries required by Qt, but which are not available on a
    # 'blank' version of Ubuntu or their version differs from one version of
    # Ubuntu to another
    # Note: starting with Ubuntu 11.04, some system libraries are located in
    #       /usr/lib/i386-linux-gnu or /usr/lib/x86_64-linux-gnu rather than
    #       /usr/lib, so we need to check those folders first...

    SET(DEFAULT_LIBRARY_FOLDER /usr/lib)
    SET(X86_LIBRARY_FOLDER ${DEFAULT_LIBRARY_FOLDER}/i386-linux-gnu)
    SET(X64_LIBRARY_FOLDER ${DEFAULT_LIBRARY_FOLDER}/x86_64-linux-gnu)

    IF(EXISTS "${X86_LIBRARY_FOLDER}" AND IS_DIRECTORY "${X86_LIBRARY_FOLDER}")
        SET(LIBRARY_FOLDER ${X86_LIBRARY_FOLDER})
    ELSEIF(EXISTS "${X64_LIBRARY_FOLDER}" AND IS_DIRECTORY "${X64_LIBRARY_FOLDER}")
        SET(LIBRARY_FOLDER ${X64_LIBRARY_FOLDER})
    ELSE()
        SET(LIBRARY_FOLDER ${DEFAULT_LIBRARY_FOLDER})
    ENDIF()

    FOREACH(LIBRARY stdc++.so.6)
        # Note: stdc++.so.6 is required by a few Qt libraries and may not be
        #       present or different on the target machine, so...

        IF(EXISTS "${LIBRARY_FOLDER}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIBRARY}")
            SET(CORRECT_LIBRARY ${LIBRARY_FOLDER}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIBRARY})
        ELSEIF(EXISTS "${DEFAULT_LIBRARY_FOLDER}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIBRARY}")
            SET(CORRECT_LIBRARY ${DEFAULT_LIBRARY_FOLDER}/${CMAKE_SHARED_LIBRARY_PREFIX}${LIBRARY})
        ELSE()
            SET(CORRECT_LIBRARY)
        ENDIF()

        IF("${CORRECT_LIBRARY}" STREQUAL "")
            MESSAGE(FATAL_ERROR "The '${LIBRARY}' could not be found...")
        ELSE()
            GET_FILENAME_COMPONENT(REAL_CORRECT_LIBRARY ${CORRECT_LIBRARY} REALPATH)

            INSTALL(FILES ${REAL_CORRECT_LIBRARY} RENAME ${CMAKE_SHARED_LIBRARY_PREFIX}${LIBRARY}
                    DESTINATION lib)
        ENDIF()
    ENDFOREACH()

    # Qt configuration file to tell OpenFiber where to look for plugins

    INSTALL(FILES ${CMAKE_SOURCE_DIR}/distrib/linux/qt.conf
            DESTINATION bin)

    # Qt plugins required by OpenFiber

    LINUX_DEPLOY_QT_PLUGIN(accessible qtaccessiblequick qtaccessiblewidgets)
    LINUX_DEPLOY_QT_PLUGIN(imageformats qgif qico qjpeg qmng qsvg qtga qtiff qwbmp)
    LINUX_DEPLOY_QT_PLUGIN(platforms qlinuxfb qminimal qxcb)
    LINUX_DEPLOY_QT_PLUGIN(printsupport cupsprintersupport)
    LINUX_DEPLOY_QT_PLUGIN(sqldrivers qsqlite)

    # Shell script to run OpenFiber

    SET(ShellFile ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.sh)

    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/distrib/linux/application.sh.in ${ShellFile})

    INSTALL(FILES ${ShellFile} RENAME ${PROJECT_NAME}
            DESTINATION .
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
ENDIF()

# Specify the package file name, depending on whether we are generating a
# snapshot version of OpenFiber

IF(SNAPSHOT)
    SET(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${DATE}-${CPACK_SYSTEM_NAME})
ELSE()
    SET(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${VERSION_MAJOR}-${VERSION_MINOR}-${VERSION_PATCH}-${CPACK_SYSTEM_NAME})
ENDIF()

INCLUDE(CPack)
