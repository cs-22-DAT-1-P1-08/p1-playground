add_library(my_api api.c api.h)
target_link_libraries(my_api json-c::json-c pdcurses::pdcurses ${libcurl_LIBRARY})
add_executable(location main.c)

target_link_libraries(location my_api json-c::json-c pdcurses::pdcurses ${libcurl_LIBRARY})