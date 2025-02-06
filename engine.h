#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define ROWS 10
#define COLUMNS 10
#define MINES 10

//
// SIGNATURES
//
void print_grid();
void game_loop();
short int is_valid_input(char input);
void handle_input(key command, int coords[2]);
void place_mines();
void init_grid();
void uncover_cells(const int coords[2]);

// helper
int count_surrounding_mines(const int pos[2]);
short int is_cell_mine(const int coords[2]);
short int is_cell_discoverable(const int coords[2]);
short int is_cell_flag(const int coords[2]);
void take_nearby_row(int*** cells, int* cell_count, short int (*filter_fn)(const int[2]), int x, int y);
void filter_surrounding_cells(int*** dest_array, int* dest_array_length, short int (*filter_fn)(const int[2]), const int pos[2]);
void add_cell_to_array(int*** cells, int* length, int x, int y);
void debug();

//
// GLOBAL VARS
//
cell_info game_grid[ROWS][COLUMNS];
short int mines_placed = 0;
int position[2] = {0, 0};
short int has_lost = 0;


//
// MAIN FUNCTIONS
//
inline void game_loop(){
    char command;
    debug();

    print_grid();

    do{
        fflush(stdin);
        command = _getch();
    }
    while(is_valid_input(command) == 0);

    system("@cls||clear");
    handle_input(command, position);
}

inline void init_grid(){
    for(int x = 0; x < ROWS; ++x){
        for(int y = 0; y < COLUMNS; ++y){
            cell_info *const cell = &game_grid[x][y];
            cell->is_flagged = 0;
            cell->is_visible = 0;
            cell->content = ENGINE_CHARS.EMPTY;
        }
    }
}

inline short int is_valid_input(const char input){
    switch(input) {
        case KEY_UP:
        case KEY_DOWN:
        case KEY_LEFT:
        case KEY_RIGHT:
        case KEY_INTERACT:
        case KEY_FLAG:
            return 1;
        default:
            return 0;

    }
}

inline void handle_input(const key command, int coords[2]){
    cell_info *const current_cell = &game_grid[coords[0]][coords[1]];

    switch(command) {
        // MOVE IN GRID
        case KEY_UP:
            if(position[0] > 0) {
                --coords[0];
            }
            break;
        case KEY_DOWN:
            if(position[0] < ROWS - 1) {
                ++coords[0];
            }
            break;
        case KEY_LEFT:
            if(position[1] > 0) {
                --coords[1];
            }
            break;
        case KEY_RIGHT:
            if(position[1] < COLUMNS - 1) {
                ++coords[1];
            }
            break;

        // GRID INTERACTIONS
        case KEY_FLAG:
            if (current_cell->is_flagged) {
                current_cell->is_flagged = 0;
            }
            else current_cell->is_flagged = 1;
            break;

        case KEY_INTERACT:
            // done as the first move
            // prevents the user from losing immediately
            if(mines_placed == 0){
                place_mines();
            }

            // flagged cells can't be interacted with
            if (current_cell->is_flagged) {
                break;
            }

            if (current_cell->content == ENGINE_CHARS.MINE) {
                has_lost = 1;
                print_grid();
                break;
            }

            if (current_cell->content == ENGINE_CHARS.EMPTY) {
                const int mine_count = count_surrounding_mines(coords);

                // flag chaining
                if (current_cell->is_visible && mine_count != 0) {

                    int** cells = malloc(0);
                    int surrounding_flags_count = 0;

                    filter_surrounding_cells(&cells, &surrounding_flags_count, is_cell_flag, coords);

                    if (surrounding_flags_count == mine_count) {

                        filter_surrounding_cells(&cells, &surrounding_flags_count, is_cell_discoverable, coords);

                        for (int i = 0; i < surrounding_flags_count; ++i) {
                            handle_input(ENGINE_COMMANDS.KEY_INTERACT, cells[i]);
                        }
                    }
                }
                else uncover_cells(coords);
            }

            break;
    }
}

inline void place_mines(){
    srand(time(NULL));
    mines_placed = 1;

    for(int i = 0; i < MINES; ++i){
        int x, y;
        do{
            x = rand() % (ROWS + 1);
            y = rand() % (COLUMNS + 1);
        }while(game_grid[x][y].content == ENGINE_CHARS.MINE || ( x == position[0] && y == position[1] ));

        cell_info *const cell = &game_grid[x][y];
        cell->content = ENGINE_CHARS.MINE;
    }
}

inline void uncover_cells(const int coords[2]){
    const int mine_count = count_surrounding_mines(coords);
    cell_info* current_cell = &game_grid[coords[0]][coords[1]];

    current_cell->is_visible = 1;

    if(mine_count != 0){
        return;
    }

    int** surrounding_cells = malloc(0);
    int surrounding_cell_count = 0;

    filter_surrounding_cells(&surrounding_cells, &surrounding_cell_count, is_cell_discoverable, coords);

    for(int i = 0; i < surrounding_cell_count; ++i){
        const int* coord = surrounding_cells[i];

        cell_info* cell = &game_grid[coord[0]][coord[1]];
        cell->is_visible = 1;
    }

    for(int i = 0; i < surrounding_cell_count; ++i) {
        const int* coord = surrounding_cells[i];

        uncover_cells(coord);
    }
}

//
// HELPER FUNCTIONS
//
inline int count_surrounding_mines(const int pos[2]){
    int mine_count = 0;
    int** cells = malloc(0);

    filter_surrounding_cells(&cells, &mine_count, is_cell_mine, pos);

    return mine_count;
}

inline short int is_cell_discoverable(const int coords[2]) {
    const cell_info* current_cell = &game_grid[coords[0]][coords[1]];
    return !current_cell->is_visible && current_cell->content == ENGINE_CHARS.EMPTY;
}

inline short int is_cell_mine(const int coords[2]) {
    const cell_info* current_cell = &game_grid[coords[0]][coords[1]];
    return current_cell->content == ENGINE_CHARS.MINE;
}

inline short int is_cell_flag(const int coords[2]) {
    const cell_info* current_cell = &game_grid[coords[0]][coords[1]];
    return current_cell->is_flagged;
}

inline void filter_surrounding_cells(int*** dest_array, int* dest_array_length, short int (*filter_fn)(const int[2]), const int pos[2]){
    const int x = pos[0],
              y = pos[1];

    // mine sopra
    if(x > 0){
        take_nearby_row(dest_array, dest_array_length, filter_fn, x - 1, y);
    }
    // mine sotto
    if(x < ROWS - 1){
        take_nearby_row(dest_array, dest_array_length,filter_fn, x + 1, y);
    }

    // mine sinistra
    if(y > 0 && filter_fn((int[]){x, y - 1})){
        add_cell_to_array(dest_array, dest_array_length, x, y - 1);
    }

    // mine destra
    if(y < COLUMNS - 1 && filter_fn((int[]){x, y + 1})){
        add_cell_to_array(dest_array, dest_array_length, x, y + 1);
    }
}

inline void take_nearby_row(int*** cells, int* cell_count, short int (*filter_fn)(const int[2]), const int x, const int y){
    if(filter_fn((int[2]){x, y})){
        add_cell_to_array(cells, cell_count, x, y);
    }

    if(y > 0 && filter_fn((int[]){x, y - 1})){
        add_cell_to_array(cells, cell_count, x, y - 1);
    }

    if(y < COLUMNS - 1 && filter_fn((int[]){x, y + 1})){
        add_cell_to_array(cells, cell_count, x, y + 1);
    }
}

inline void add_cell_to_array(int*** cells, int* length, const int x, const int y){
    *cells = realloc(*cells, (*length + 1) * sizeof(int*));
    (*cells)[*length] = (int*) malloc(2 * sizeof(int));

    (*cells)[*length][0] = x;
    (*cells)[*length][1] = y;

    (*length)++;
}

inline void debug() {
    for(int i = 0; i < ROWS; ++i) {
        for(int j = 0; j < COLUMNS; ++j) {
            printf("%d ", game_grid[i][j].is_visible);
        }
        printf("\n");
    }
}

#endif // ENGINE_H_INCLUDED
