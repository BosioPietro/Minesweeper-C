#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "engine.h"
#include "interface.h"
#include "menu.h"

int main(){
    while (game_state != GAME_STATE.QUIT) {
        switch (game_state) {
            case MAIN_MENU:
                handle_menu();
                break;
            case OPTIONS:
                handle_settings();
                break;
            case PLAYING:
                init_grid();
                game_loop();
                break;
            case QUIT:
                break;
        }
    }
    return 0;
}

// TODO
// semplifica condizioni
// const, short, e unsigned
// selettore mine

