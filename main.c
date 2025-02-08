#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "engine.h"
#include "interface.h"
#include "menu.h"

playing_state game_state = GAME_STATE.MAIN_MENU;

int main(){

    while (1 == 1) {
        handle_menu();
    }

    // init_grid();
    //
    //
    // while(!has_lost && !has_won){
    //     game_loop(position);
    // }
    //
    // system("@cls||clear");
    //
    // print_grid();
    // if (has_lost) {
    //     printf("You lost");
    // }
    // else printf("You win");
    //
    // getchar();
    return 0;
}

// TODO
// semplifica condizioni
// const, short, e unsigned
// count mine col filter
// il flag prima dello scopri
// selettore mine
