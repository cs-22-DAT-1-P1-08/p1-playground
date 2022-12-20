#include <stdio.h>
#include "api/coop_api.h"
#include "api/tjek_api.h"
#include "api/location_api.h"
#include "ui/main_view.h"
#include "ui/shopping_list_view.h"
#include "ui/components/table.h"
#include "ui/results_view.h"
#include "sorting_functions.h"
#include "store.h"
#include <stdlib.h>
#include <string.h>

typedef enum view_states_t {
    MAIN_VIEW,
    SHOPPING_LIST_VIEW,
    RESULTS_VIEW
} view_states_t;

int main() {
    /* Retrieve here API key */
    char* here_api_key = getenv("HERE_API_KEY");
    if (here_api_key == NULL) {
        fprintf(stderr, "Failed to retrieve HERE_API_KEY from environment variables.");
        exit(EXIT_FAILURE);
    }

    /* Initialize stores */
    store_t *daglibrugsen = get_coop_store("Dagli'Brugsen", "1290", DB_DEALER_ID);
    store_t *coop365 = get_coop_store("Coop 365", "24165", COOP365_DEALER_ID);
    store_t *stores[] = { daglibrugsen, coop365 };

    /* Initialize curses */
    initscr();
    noecho();

    start_color();
    init_color(1, (short)(77.0/255.0*1000), (short)(128.0/255.0*1000), (short)(247.0/255.0*1000));
    init_pair(1, 1, COLOR_BLACK);

    /* Initialize program state */
    dlist_t *shopping_list = calloc(1, sizeof(dlist_t));
    view_states_t state = MAIN_VIEW;

    location_t *home = malloc(sizeof(location_t));
    initialize_location(home);

    /* Main loop & view switcher */
    bool is_running = true;
    while (is_running) {
        switch (state) {
            case SHOPPING_LIST_VIEW: {
                render_shopping_list(stdscr, shopping_list);
                state = RESULTS_VIEW;
                break;
            }
            case RESULTS_VIEW: {
                results_view_data_t *result_data = calloc(1, sizeof(results_view_data_t));
                result_data->shopping_list = shopping_list;
                result_data->stores = stores;
                result_data->store_count = sizeof(stores) / sizeof(stores[0]),
                        result_data->home = home;
                result_data->here_api_key = here_api_key;

                enum ResultViewMenuOptions result = render_results_view(stdscr, result_data);
                if (result == RVM_SHOPPING_LIST) {
                    state = SHOPPING_LIST_VIEW;
                } else if (result == RVM_MAIN_SCREEN) {
                    state = MAIN_VIEW;
                }
                break;
            }
            default:
            case MAIN_VIEW: {
                enum MainViewMenuOptions selected;

                WINDOW *main_view = create_main_view(stdscr);
                render_main_view(main_view, &selected, home);
                destroy_main_view(main_view);

                if (selected == QUIT) is_running = false;

                if (home && strlen(home->street) > 0) {
                    strcpy(home->place_name, "home");
                    addr_to_geo(home, here_api_key);

                    fill_nearest_store(daglibrugsen, home);
                    fill_nearest_store(coop365, home);
                }

                state = SHOPPING_LIST_VIEW;
                break;
            }
        }
    }

    getch();
    endwin();

    free_store(daglibrugsen);
    free_store(coop365);
    return 0;
}