add_library(my_api coopapi.c itemapi.h)

add_executable(coopapi main.c)

target_link_libraries(coopapi my_api json-c::json-c pdcurses::pdcurses ${libcurl_LIBRARY})