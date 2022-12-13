add_executable(test_prog test.c)

set(SOURCES_TEST
        ${SOURCES_TEST}
        ${PROJECT_SOURCE_DIR}/src/store.c
        ${PROJECT_SOURCE_DIR}/src/sorting_functions.c
        ${PROJECT_SOURCE_DIR}/src/api/location_api.c
        ${PROJECT_SOURCE_DIR}/src/api/tjek_api.c
        ${PROJECT_SOURCE_DIR}/src/api/coop_api.c
        ${PROJECT_SOURCE_DIR}/src/util/curlext.c
        ${PROJECT_SOURCE_DIR}/src/util/dlist.c
        ${PROJECT_SOURCE_DIR}/src/ui/main_view.c
        ${PROJECT_SOURCE_DIR}/src/ui/results_view.c
        ${PROJECT_SOURCE_DIR}/src/ui/shopping_list_view.c
        ${PROJECT_SOURCE_DIR}/src/ui/components/arrow_menu.c
        )
target_sources(test_prog PRIVATE ${SOURCES_TEST})

target_link_libraries(test_prog json-c::json-c pdcurses::pdcurses ${libcurl_LIBRARY})

add_test(NAME test_prog
        COMMAND test_prog)
