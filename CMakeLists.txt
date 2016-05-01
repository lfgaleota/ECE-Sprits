cmake_minimum_required(VERSION 3.0)
project(ECE-Sprits)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(SOURCE_FILES main.c init.c inc/init.h circular_menu.c inc/circular_menu.h inc/structures.h collisions.c inc/collisions.h objects.c inc/objects.h line_tracking.c inc/line_tracking.h physics.c inc/physics.h game.c inc/game.h memory.c inc/memory.h capacities.c inc/capacities.h level.c inc/level.h popup_menu.c inc/popup_menu.h menu.c inc/menu.h save.c inc/save.h config.c inc/config.h)
add_executable(ECE-Sprits ${SOURCE_FILES})
target_link_libraries(ECE-Sprits m z png loadpng jpgalleg alleg44)
