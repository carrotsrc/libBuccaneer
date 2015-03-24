cmake_minimum_required (VERSION 2.6)
project (RackscallionTests)

#config
set(RACKOONIO_LIB "../../rackoonio" CACHE RACKOONIO "Location of the library librackio")
set(RACKOONIO_INC "../../rackoonio" CACHE RACKOONIO "Location of the includes from RackoonIO framework")

# Build
set( GCC_CFLAG "-std=c++11 -ggdb" )
set( GCC_LDFLAG "-L../ -L${RACKOONIO_LIB}" )

set( EXTRA_LIBS
	${EXTRA_LIBS}
	asound
	pthread
	rackio
	rackscallion
)

include_directories(
		${PROJECT_SOURCE_DIR}/../
		${RACKOONIO_INC}
)

# Integration
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CFLAG}" )
set( CMAKE_EXE_LINKER_FLAGS "${GCC_LDFLAG}" )

add_executable( bin/rack src/Rack.cpp)
target_link_libraries ( bin/rack  ${EXTRA_LIBS} )

add_executable( bin/telemetry src/Telemetry.cpp)
target_link_libraries ( bin/telemetry  ${EXTRA_LIBS} )

add_executable( bin/events src/Events.cpp)
target_link_libraries ( bin/events  ${EXTRA_LIBS} )