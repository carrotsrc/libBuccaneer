cmake_minimum_required (VERSION 2.6)
project (RackscallionTests)

#config

# Build
set( GCC_CFLAG "-std=c++11 -ggdb -I../../rackoonio -I../" )
set( GCC_LDFLAGS "-L../ -L../../rackoonio" )
set( EXTRA_LIBS ${EXTRA_LIBS} -L../ -L../../rackoonio asound pthread rackio rackscallion )

# Integration
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CFLAG}" )

add_executable( bin/rack src/Rack.cpp)
target_link_libraries ( bin/rack  ${EXTRA_LIBS} )

add_executable( bin/telemetry src/Telemetry.cpp)
target_link_libraries ( bin/telemetry  ${EXTRA_LIBS} )
