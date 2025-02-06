#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "engine.h"
#include "interface.h"

int main(){
    init_grid();

    while(1){
        game_loop(position);
    }

    return 0;
}

// TODO
// const, short, e unsigned
