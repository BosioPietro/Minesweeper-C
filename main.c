#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "engine.h"
#include "interface.h"

int main(){
    init_grid();

    while(!has_lost){
        game_loop(position);
    }

    printf("Press any key to continue...");
    return 0;
}

// TODO
// semplifica condizioni
// selezionata che lampeggia??
// const, short, e unsigned
// count mine col filter
// il flag prima dello scopri
