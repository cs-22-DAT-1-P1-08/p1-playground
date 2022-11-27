set(SOURCES
    ${SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/main.c
    ${CMAKE_CURRENT_LIST_DIR}/tjek_api.c
    ${CMAKE_CURRENT_LIST_DIR}/util/curlext.c
)

add_executable(p1_playground main.c)
target_sources(p1_playground PRIVATE ${SOURCES})
target_link_libraries(p1_playground json-c::json-c pdcurses::pdcurses ${libcurl_LIBRARY})