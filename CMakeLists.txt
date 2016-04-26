cmake_minimum_required(VERSION 3.5)
project(ECE-Sprits)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(SOURCE_FILES main.c init.c inc/init.h circular_menu.c inc/circular_menu.h inc/structures.h collisions.c inc/collisions.h objects.c inc/objects.h line_tracking.c inc/line_tracking.h physics.c inc/physics.h game.c inc/game.h memory.c inc/memory.h)
add_executable(ECE-Sprits ${SOURCE_FILES})
target_link_libraries(ECE-Sprits m alleg44 loadpng jpgalleg)
