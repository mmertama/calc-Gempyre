cmake_minimum_required (VERSION 3.14)

set(NAME calc)
project (${NAME})

set(CMAKE_CXX_STANDARD 17)

include_directories(
    include
)

find_package (gempyre REQUIRED)
include(hiillos)
include(gempyre)



add_executable(${PROJECT_NAME}
    src/computor.h
    src/main.cpp
    gui/${NAME}.html
    )

gempyre_add_resources(PROJECT ${PROJECT_NAME} TARGET include/${NAME}_resource.h SOURCES gui/${NAME}.html gui/${NAME}.css gui/${NAME}.png)
hiillos_make_application(${PROJECT_NAME})

add_custom_command(
        OUTPUT include/${NAME}.html.h
        COMMAND python ${GEMPYRE_PATH}/scripts/genStringResource.py gui/${NAME}.html include/${NAME}.html.h WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS gui/${NAME}.html
        )


target_link_libraries (${PROJECT_NAME} gempyre::gempyre)
