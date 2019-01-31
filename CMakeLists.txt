
#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(KaonDecay)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(KaonDecay  KaonDecay.cc ${sources} ${headers})
target_link_libraries(KaonDecay ${Geant4_LIBRARIES})


### add CERN ROOT
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

target_link_libraries(KaonDecay ${ROOT_LIBRARIES})


#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build KaonDecay. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(KaonDecay_SCRIPTS
  visQt.mac
  vis.mac
  run.mac
  py/plotHisto.py
  py/printEvents.py
  )

foreach(_script ${KaonDecay_SCRIPTS})
  get_filename_component(FILE_NAME ${_script} NAME)
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${FILE_NAME}
    COPYONLY
    )
endforeach()




#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS KaonDecay DESTINATION bin)


