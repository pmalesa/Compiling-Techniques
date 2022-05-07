cmake_minimum_required(VERSION 3.7.2)

project(macrogenerator)
set(CMAKE_CXX_STANDARD 14)

# Platform specific variables
if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(TARGET_SYSTEM "win64")
    endif()
elseif(UNIX)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(TARGET_SYSTEM "linux64")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(TARGET_SYSTEM "linux32")
    endif()
endif()

message(STATUS "Target system: ${TARGET_SYSTEM}")

# Create sources variables
set(MACROGENERATOR_MAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/src/main.cpp")
set(MACROGENERATOR_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/MacroGenerator.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/MacroGenerator.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/MacroLibrary.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/MacroLibrary.cpp"

    # Add files here
)

set(LINKER_FLAGS)
if(UNIX)
    set(LINKER_FLAGS ${LINKER_FLAGS} "-pthread")
endif()

# Create main target
add_executable(macrogenerator ${GUI_FLAG}) 
target_sources(macrogenerator
    PRIVATE
        ${MACROGENERATOR_MAIN_FILE}
        ${MACROGENERATOR_SOURCES}

)

target_include_directories(macrogenerator PUBLIC "src/")
target_link_libraries(macrogenerator ${LINKER_FLAGS})
