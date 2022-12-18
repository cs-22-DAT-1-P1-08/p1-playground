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
    ${CMAKE_CURRENT_LIST_DIR}/ui/main_view.c
    ${CMAKE_CURRENT_LIST_DIR}/ui/results_view.c
    ${CMAKE_CURRENT_LIST_DIR}/ui/shopping_list_view.c
    ${CMAKE_CURRENT_LIST_DIR}/ui/components/arrow_menu.c
    ${CMAKE_CURRENT_LIST_DIR}/ui/components/table.c
)

add_executable(p1_playground main.c)
target_sources(p1_playground PRIVATE ${SOURCES})
target_link_libraries(p1_playground json-c::json-c pdcurses::pdcurses ${libcurl_LIBRARY})

#add_library(p1_library)
#target_sources(p1_library PRIVATE ${SOURCES})
#target_link_libraries(p1_library json-c::json-c pdcurses::pdcurses ${libcurl_LIBRARY})
