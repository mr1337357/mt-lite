#load files used in every build
set(SRCS "main.c")
set(INC ".")

list(APPEND SRCS "util/minheap.c")

list(APPEND INC "boards/")

#load files used by configuration
include(../.config)

idf_component_register(SRCS ${SRCS} INCLUDE_DIRS ${INC})

add_definitions("-DBOARD=${BOARD}")
