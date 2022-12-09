find_package(json-c)

add_executable(json_example json-example.c)
target_link_libraries(json_example json-c::json-c)

add_executable(pdcurses_example pdcurses-example.c)
target_link_libraries(pdcurses_example pdcurses::pdcurses)

