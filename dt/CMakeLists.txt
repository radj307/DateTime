# DateTime/dt/
cmake_minimum_required (VERSION 3.20)

file(GLOB HEADERS
	RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
	CONFIGURE_DEPENDS
	"*.h*"
)
file(GLOB SRCS
	RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
	CONFIGURE_DEPENDS
	"*.c*"
)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/rc")

include (VersionTag)
MAKE_VERSION_HEADER("${CMAKE_CURRENT_SOURCE_DIR}/rc/version.h" dt "${DateTime_VERSION}")

add_executable (dt "${CMAKE_CURRENT_SOURCE_DIR}/rc/version.h" "${HEADERS}")

set_property(TARGET dt PROPERTY CXX_STANDARD 20)
set_property(TARGET dt PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET dt PROPERTY POSITION_INDEPENDENT_CODE ON)

if (MSVC)
	target_compile_options(dt PRIVATE "/Zc:__cplusplus" "/Zc:preprocessor")
endif()

target_link_libraries(dt PRIVATE TermAPI optlib filelib)

target_sources(dt PRIVATE "${SRCS}")

include(PackageInstaller)

INSTALL_EXECUTABLE(dt "${CMAKE_INSTALL_PREFIX}/bin/")
