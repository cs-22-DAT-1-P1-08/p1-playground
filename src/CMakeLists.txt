add_executable(p1_playground main.c)
target_link_libraries(p1_playground json-c::json-c pdcurses::pdcurses ${libcurl_LIBRARY})