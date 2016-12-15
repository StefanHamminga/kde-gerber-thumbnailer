project(GerberThumbCreator)

# Set minimum required version of cmake.
cmake_minimum_required(VERSION 2.8.12)

set(QT_MIN_VERSION "5.6.0")

# Locate extra-cmake-modules version 0.0.11 and make it a required package
find_package(ECM 1.0.0 REQUIRED NO_MODULE)

# Set value of CMAKE_MODULE_PATH variable where cmake will search for modules
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


find_package(PkgConfig REQUIRED)
pkg_search_module(GERBV REQUIRED libgerbv)
include_directories(${SOME_INCLUDE_DIRS};${GERBV_INCLUDE_DIRS})

find_package(Qt5Gui REQUIRED)
find_package(KF5 REQUIRED COMPONENTS KIO)

include(FeatureSummary)
include(WriteBasicConfigVersionFile)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

set(GerberThumbCreator_SRCS src/gerberthumbcreator.cpp)

add_library(gerberthumbcreator MODULE ${GerberThumbCreator_SRCS})

target_compile_definitions(gerberthumbcreator PRIVATE -DQT_NO_KEYWORDS)

target_link_libraries(gerberthumbcreator gerbv Qt5::Gui KF5::KIOWidgets)

# TODO: Figure out why (on my system) the documentation suggested paths don't resolve to the proper qt5 plugin path, silently breaking the plugin.
#       The manual TARGET specification does install in the proper location.
# This should work:
# install(TARGETS gerberthumbcreator             DESTINATION ${PLUGIN_INSTALL_DIR})
# install(TARGETS gerberthumbcreator             DESTINATION ${QT_PLUGIN_INSTALL_DIR})
# This used to work on Arch Linux:
# install(TARGETS gerberthumbcreator             DESTINATION ${LIB_INSTALL_DIR}/qt5/plugins)
# This works for Arch Linux:
install(TARGETS gerberthumbcreator             DESTINATION ${LIB_INSTALL_DIR}/qt/plugins)
install(FILES   src/gerberthumbcreator.desktop DESTINATION ${SERVICES_INSTALL_DIR})
