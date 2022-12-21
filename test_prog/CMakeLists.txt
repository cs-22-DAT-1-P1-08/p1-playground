add_executable(test_item test_items.c)
add_executable(test_location test_location.c)

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
        ${PROJECT_SOURCE_DIR}/src/ui/components/table.c
        )
target_sources(test_item PRIVATE ${SOURCES_TEST})
target_sources(test_location PRIVATE ${SOURCES_TEST})


target_link_libraries(test_item json-c::json-c pdcurses::pdcurses ${libcurl_LIBRARY})
target_link_libraries(test_location json-c::json-c pdcurses::pdcurses ${libcurl_LIBRARY})


add_test(NAME test_item
       COMMAND test_item)
add_test(NAME test_location
        COMMAND test_location)