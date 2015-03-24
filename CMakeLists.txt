cmake_minimum_required (VERSION 2.6)
project (libbuccaneer)

#config

set(LIBRESAMPLE "0" CACHE CONDITIONALS "Build code that replies on libResample")

# Build
set( GCC_CFLAG "-std=c++11 -fPIC -shared -ggdb -I../rackoonio" )
set( GCC_LDFLAGS "-wl,-soname,libbuccaneer.so" )

configure_file(
	"${PROJECT_SOURCE_DIR}/pconfig.h.in"
	"${PROJECT_SOURCE_DIR}/pconfig.h"
)
# Libs
include_directories("${PROJECT_SOURCE_DIR}/")
set( EXTRA_LIBS ${EXTRA_LIBS} asound pthread sndfile )

# Files
set( SRC_FACTORIES 
	factories/EventMessageFactory.cpp
	factories/RackUnitFactory.cpp
)


set( SRC_UNITS
	units/RuAlsa.cpp
	units/RuChannelMixer.cpp
	units/RuEcho.cpp
	units/RuFlacLoad.cpp
	units/RuLevels.cpp
	units/RuLoop.cpp
)

if(RESAMPLE)
	set( EXTRA_LIBS ${EXTRA_LIBS} resample )
	set( SRC_UNITS 
		${SRC_UNITS}
		units/RuPitchBender.cpp
	)
endif(RESAMPLE)

set( SRC_ALL
	${SRC_FACTORIES}
	${SRC_UNITS}
)

# Integration
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CFLAG}" )
set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CFLAG}" )

add_library( buccaneer SHARED ${SRC_ALL})
target_link_libraries ( buccaneer  ${EXTRA_LIBS} )
