add_executable(p1_playground main.c)
target_link_libraries(p1_playground json-c::json-c pdcurses::pdcurses ${libcurl_LIBRARY})

add_library(curlext util/curlext.c util/curlext.h)
target_link_libraries(curlext ${libcurl_LIBRARY})

add_executable(etilbudsavis tjek_api.c)
target_link_libraries(etilbudsavis json-c::json-c pdcurses::pdcurses ${libcurl_LIBRARY} curlext)