set(SOURCES
    ${SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/main.c
    ${CMAKE_CURRENT_LIST_DIR}/store.c
    ${CMAKE_CURRENT_LIST_DIR}/sorting_functions.c
    ${CMAKE_CURRENT_LIST_DIR}/api/location_api.c
    ${CMAKE_CURRENT_LIST_DIR}/api/tjek_api.c
    ${CMAKE_CURRENT_LIST_DIR}/api/coop_api.c
    ${CMAKE_CURRENT_LIST_DIR}/util/curlext.c
    ${CMAKE_CURRENT_LIST_DIR}/util/dlist.c
        )

add_executable(p1_playground main.c)
target_sources(p1_playground PRIVATE ${SOURCES})
target_link_libraries(p1_playground json-c::json-c pdcurses::pdcurses ${libcurl_LIBRARY})
