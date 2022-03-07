cmake_minimum_required (VERSION 3.14)

set(NAME calc)
project (${NAME})

set(CMAKE_CXX_STANDARD 17)

include_directories(
    include
)

find_package (gempyre REQUIRED)
include(gempyre)

add_executable(${PROJECT_NAME}
    src/computor.h
    src/main.cpp
    )

target_sources(
    ${PROJECT_NAME} PRIVATE
    gui/calc.html
    gui/calc.css
    )

set(APP_ICON ${CMAKE_SOURCE_DIR}/gui/calc.png)

gempyre_add_resources(PROJECT ${PROJECT_NAME} TARGET include/${NAME}_resource.h SOURCES gui/${NAME}.html gui/${NAME}.css gui/${NAME}.png)

target_link_libraries (${PROJECT_NAME} gempyre::gempyre)
