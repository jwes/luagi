project( luagi )

# Required cmake version
cmake_minimum_required( VERSION 2.8.7 )
enable_testing()

set( LIB_NAME ${PROJECT_NAME} )

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules" )
set( ROOT "${CMAKE_SOURCE_DIR}/../.." )
set( INC_DIR "${ROOT}/inc" )
set( SRC_DIR "${ROOT}/src" )
set( TST_DIR "${ROOT}/tst")

# add definitions
include( CustomFunctions )
include( DefineCMakeDefaults )
include( DefineCompilerFlags )

# file list
include( ${CMAKE_SOURCE_DIR}/FileLists.cmake )
file( GLOB TST_FILES
   "${TST_DIR}/*.lua"
)

find_package( Lua52 REQUIRED ) # LUA52_LIBRARIES / LUA52_INCLUDE_DIR
find_package( GIT2 "0.24.0" EXACT REQUIRED ) # GIT2_LIBRARIES / GIT2_INCLUDE_DIRS

########################################################################################################################
# build - make
########################################################################################################################
include_directories( ${INC_DIR} ${GIT2_INCLUDE_DIRS} )

add_library( ${LIB_NAME} SHARED ${SRC_FILES} )
set_target_properties( ${LIB_NAME} PROPERTIES PREFIX "" )
target_link_libraries( ${LIB_NAME} m ${LUA52_LIBRARIES} ${GIT2_LIBRARIES} )

foreach( test_file ${TST_FILES} )

   add_test( NAME ${test_file} 
           COMMAND busted --cpath=${CMAKE_BINARY_DIR}/${LIB_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX} ${test_file}
      WORKING_DIRECTORY ${TST_DIR})

endforeach( test_file )

########################################################################################################################
# install - make install
########################################################################################################################
# install( TARGETS ${LIB_NAME} DESTINATION lib )
