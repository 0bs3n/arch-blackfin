cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(arch_blackfin)

if((NOT BN_API_PATH) AND (NOT BN_INTERNAL_BUILD))
	set(BN_API_PATH $ENV{BN_API_PATH})
	if(NOT BN_API_PATH)
		message(FATAL_ERROR "Provide path to Binary Ninja API source in BN_API_PATH")
	endif()
endif()
if(NOT BN_INTERNAL_BUILD)
	add_subdirectory(${BN_API_PATH} ${PROJECT_BINARY_DIR}/api)
endif()

file(GLOB SOURCES
    disassembler/*.cpp
    disassembler/instructions/*.cpp
	*.cpp)

if(DEMO)
	add_library(arch_blackfin STATIC ${SOURCES})
else()
	add_library(arch_blackfin SHARED ${SOURCES})
endif()

target_compile_options(arch_blackfin PUBLIC -Wno-write-strings)

target_include_directories(arch_blackfin
	PRIVATE ${PROJECT_SOURCE_DIR}
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/disassembler/include)

target_link_libraries(arch_blackfin binaryninjaapi)

set_target_properties(arch_blackfin PROPERTIES
    CXX_STANDARD 17
	CXX_VISIBILITY_PRESET hidden
	CXX_STANDARD_REQUIRED ON
	C_STANDARD 99
	C_STANDARD_REQUIRED ON
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
	POSITION_INDEPENDENT_CODE ON)

if(BN_INTERNAL_BUILD)
	plugin_rpath(arch_blackfin)
	set_target_properties(arch_blackfin PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY ${BN_CORE_PLUGIN_DIR}
		RUNTIME_OUTPUT_DIRECTORY ${BN_CORE_PLUGIN_DIR})
endif()
