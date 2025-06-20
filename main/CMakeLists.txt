message("here is ${CMAKE_SOURCE_DIR}")

#load files used in every build
set(SRCS "main.c")
set(INC ".")

list(APPEND INC "boards/")

#load files used by configuration
include(../.config)

idf_component_register(SRCS ${SRCS} INCLUDE_DIRS ${INC})

add_definitions("-DBOARD=${BOARD}")