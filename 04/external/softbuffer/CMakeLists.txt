cmake_minimum_required(VERSION 3.0...3.25)

project(
    Softbuffer
    VERSION 0.1.0
    LANGUAGES CXX C
)

add_library(softbuffer SHARED IMPORTED GLOBAL)

if (NOT ARCH)
    set(ARCH ${CMAKE_SYSTEM_PROCESSOR})
    if (ARCH STREQUAL "AMD64")
        set(ARCH "x86_64")
    endif()
endif()

if(WIN32)
    set(SOFTBUFFER_RUNTIME_LIB ${CMAKE_CURRENT_SOURCE_DIR}/bin/windows-${ARCH}/softbuffer_bindings.dll)

    set_target_properties(
        softbuffer
        PROPERTIES
            IMPORTED_LOCATION "${SOFTBUFFER_RUNTIME_LIB}"
            IMPORTED_IMPLIB "${SOFTBUFFER_RUNTIME_LIB}.lib"
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/bindings"
    )
else()
    set(SOFTBUFFER_RUNTIME_LIB ${CMAKE_CURRENT_SOURCE_DIR}/bin/linux-${ARCH}/libsoftbuffer_bindings.so)

    set_target_properties(
        softbuffer
        PROPERTIES
            IMPORTED_LOCATION "${SOFTBUFFER_RUNTIME_LIB}"
            IMPORTED_IMPLIB "${SOFTBUFFER_RUNTIME_LIB}"
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/bindings"
    )
endif()

set(SOFTBUFFER_RUNTIME_LIB ${SOFTBUFFER_RUNTIME_LIB} PARENT_SCOPE)
set(SOFTBUFFER_RUNTIME_LIB ${SOFTBUFFER_RUNTIME_LIB} CACHE INTERNAL "Path to the Softbuffer library binary")

function(target_copy_softbuffer_binaries Target)
    add_custom_command(
        TARGET ${Target} POST_BUILD
        COMMAND
            ${CMAKE_COMMAND} -E copy_if_different
            ${SOFTBUFFER_RUNTIME_LIB}
            $<TARGET_FILE_DIR:${Target}>
        COMMENT
            "Copying '${SOFTBUFFER_RUNTIME_LIB}' to '$<TARGET_FILE_DIR:${Target}>'..."
    )
endfunction()
