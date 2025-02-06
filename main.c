#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "engine.h"
#include "interface.h"

int main(){
    init_grid();


    while(!has_lost && !has_won){
        game_loop(position);
    }

    // system("@cls||clear");
    //
    // if (has_lost) {
    //     printf("You lost\n");
    // }
    // else printf("You win\n");
    //
    print_grid();
    getchar();
    return 0;
}

// TODO
// semplifica condizioni
// const, short, e unsigned
// count mine col filter
// il flag prima dello scopri
