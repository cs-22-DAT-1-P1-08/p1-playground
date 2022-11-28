add_library(my_api api.c api.h)

add_executable(location main.c)

target_link_libraries(location my_api json-c::json-c pdcurses::pdcurses ${libcurl_LIBRARY})