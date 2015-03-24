cmake_minimum_required (VERSION 2.6)
project (librackscallion)

#config

set(LIBRESAMPLE "0" CACHE CONDITIONALS "Build code that relies on libResample")
set(RACKOONIO_LIB "../rackoonio" CACHE RACKOONIO "Location of the library librackio")
set(RACKOONIO_INC "../rackoonio" CACHE RACKOONIO "Location of the includes from RackoonIO framework")
# Build
set( GCC_CXXFLAG "-std=c++11 -fPIC -shared -ggdb" )
set( GCC_LDFLAG " -L${RACKOONIO_LIB} -Wl,-soname,librackscallion.so" )

configure_file(
	"${PROJECT_SOURCE_DIR}/pconfig.h.in"
	"${PROJECT_SOURCE_DIR}/pconfig.h"
)
# Libs
include_directories(
		${PROJECT_SOURCE_DIR}/ 
		${RACKOONIO_INC}
)
set( EXTRA_LIBS 
	${EXTRA_LIBS}
	rackio
	asound
	pthread
	sndfile )

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
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CXXFLAG}" )
set( CMAKE_SHARED_LINKER_FLAGS "${GCC_LDFLAG}" )

add_library( rackscallion SHARED ${SRC_ALL})
target_link_libraries ( rackscallion  ${EXTRA_LIBS} )
