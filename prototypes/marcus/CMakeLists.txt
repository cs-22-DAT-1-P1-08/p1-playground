add_executable(marcus main.c)
target_link_libraries(marcus json-c::json-c pdcurses::pdcurses)
