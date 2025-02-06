#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include <time.h>

#define ROWS 10
#define COLUMNS 10
#define MINES 10

//
// TYPES
//
typedef struct{
    const char HORIZONTAL;
    const char VERTICAL;

    const char CORNER_UP_SX;
    const char CORNER_UP_DX;
    const char CORNER_DOWN_SX;
    const char CORNER_DOWN_DX;

    const char CORNER_VERTICAL_SX;
    const char CORNER_VERTICAL_DX;
    const char CORNER_HORIZONTAL_DOWN;
    const char CORNER_HORIZONTAL_UP;
    const char INNER_CORNER;
} charset_grid;

typedef struct{
    const char EMPTY;
    const char MINE;
    const char VISIBLE;
    const char HIDDEN;
    const char FLAGGED;
    const charset_grid GRID;
} charset;

extern const charset ENGINE_CHARS = {
    .HIDDEN = 254,
    .EMPTY = 63,
    .VISIBLE = 176,
    .MINE = 33,
    .FLAGGED = -1,
    .GRID = {
        .HORIZONTAL = 205,
        .VERTICAL = 186,

        .CORNER_UP_SX = 201,
        .CORNER_UP_DX = 187,
        .CORNER_DOWN_SX = 200,
        .CORNER_DOWN_DX = 188,

        .CORNER_VERTICAL_SX = 204,
        .CORNER_VERTICAL_DX = 185,
        .CORNER_HORIZONTAL_DOWN = 203,
        .CORNER_HORIZONTAL_UP = 202,

        .INNER_CORNER = 206
    }
};

typedef struct{
    const char KEY_DOWN;
    const char KEY_LEFT;
    const char KEY_RIGHT;
    const char KEY_UP;
    const char KEY_INTERACT;
} keys;

const keys KEYSTROKES = {
    .KEY_UP = 119,
    .KEY_DOWN = 115,
    .KEY_LEFT = 97,
    .KEY_RIGHT = 100,

    .KEY_INTERACT = 112
};

typedef struct{
    short int is_visible;
    short int is_fagged;
    char content;
} cell_info;

cell_info game_grid[ROWS][COLUMNS];
short int mines_placed = 0;
int position[2] = {0, 0};

//
// SIGNATURES
//
void print_grid();
void game_loop();
short int is_valid_input(char input);
void handle_input(char command);
void place_mines();
void init_grid();
void uncover_cells(const int coords[2]);
int count_surrounding_mines(const int pos[2]);
void take_nearby_row(int*** cells, int* cell_count, int x, int y);
void take_nearby_cells(int*** numbered_cells, int*** empty_cells, int* empty_cell_count, int* numbered_cell_count, const int pos[2]);
void add_cell_to_array(int*** cells, int* length, const int x, const int y);

//
// FUNCTIONS
//
inline void game_loop(){
    char command;

    print_grid();

    do{
        fflush(stdin);
        command = _getch();
    }
    while(is_valid_input(command) == 0);

    system("@cls||clear");
    handle_input(command);
}

inline void place_mines(){
    srand(time(NULL));
    mines_placed = 1;

    for(int i = 0; i < MINES; ++i){
        int x, y;
        do{
            x = rand() % (ROWS + 1);
            y = rand() % (COLUMNS + 1);
        }while(game_grid[x][y].content == ENGINE_CHARS.MINE && x != position[0] && y != position[1]);

        game_grid[x][y].content = ENGINE_CHARS.MINE;
    }
}

inline void init_grid(){
    for(int x = 0; x < ROWS; ++x){
        for(int y = 0; y < COLUMNS; ++y){
            game_grid[x][y].is_visible = 0;
            game_grid[x][y].content = ENGINE_CHARS.EMPTY;
        }
    }
}

inline short int is_valid_input(const char input){
    const char keys[] = {
        KEYSTROKES.KEY_UP,
        KEYSTROKES.KEY_DOWN,
        KEYSTROKES.KEY_LEFT,
        KEYSTROKES.KEY_RIGHT,
        KEYSTROKES.KEY_INTERACT
    };

    int i = 0;
    while(i < sizeof(keys)){
        if(input == keys[i++]){
            return 1;
        }
    }

    return 0;
}

inline void handle_input(const char command){
    if(command == KEYSTROKES.KEY_UP){
        if(position[0] > 0){
           position[0] -= 1;
        }
        return;
    }

    if(command == KEYSTROKES.KEY_DOWN){
        if(position[0] < ROWS - 1){
           ++position[0];
        }
        return;
    }

    if(command == KEYSTROKES.KEY_LEFT){
        if(position[1] > 0){
           position[1] -= 1;
        }
        return;
    }

    if(command == KEYSTROKES.KEY_RIGHT){
        if(position[1] < COLUMNS - 1){
           ++position[1];
        }
        return;
    }

    if(command == KEYSTROKES.KEY_INTERACT){
        if(mines_placed == 0){
            place_mines();
        }
        uncover_cells(position);
    }
}

inline void uncover_cells(const int coords[2]){
    const int mine_count = count_surrounding_mines(coords);
    cell_info* current_cell = &game_grid[coords[0]][coords[1]];

    current_cell->is_visible = 1;


    if(mine_count != 0){
        return;
    }

    int** empty_cells = malloc(0);
    int** numbered_cells = malloc(0);
    int numbered_cell_count = 0;
    int empty_cell_count = 0;

    take_nearby_cells(&numbered_cells, &empty_cells, &empty_cell_count, &numbered_cell_count, coords);


    for(int i = 0; i < empty_cell_count; ++i){
        cell_info* cell = &game_grid[empty_cells[i][0]][empty_cells[i][1]];
        cell->is_visible = 1;
    }

    for(int i = 0; i < numbered_cell_count; ++i){
        const int x = numbered_cells[i][0],
                  y = numbered_cells[i][1];

        cell_info* cell = &game_grid[x][y];
        cell->is_visible = 1;
    }

    for(int i = 0; i < empty_cell_count; ++i){
        uncover_cells(empty_cells[i]);
    }
}

inline void take_nearby_cells(int*** numbered_cells, int*** empty_cells, int* empty_cell_count, int* numbered_cell_count, const int pos[2]){
    const int x = pos[0],
              y = pos[1];

    int **covered_cells = malloc(0);
    int covered_cell_count = 0;

    // mine sopra
    if(x > 0){
        take_nearby_row(&covered_cells, &covered_cell_count, x - 1, y);
    }
    // mine sotto
    if(x < ROWS - 1){
        take_nearby_row(&covered_cells, &covered_cell_count, x + 1, y);
    }

    // mine sinistra
    if(y > 0 && game_grid[x][y - 1].is_visible == 0){
        add_cell_to_array(&covered_cells, &covered_cell_count, x, y - 1);
    }

    // mine destra
    if(y < COLUMNS - 1 && game_grid[x][y + 1].is_visible == 0){
        add_cell_to_array(&covered_cells, &covered_cell_count, x, y + 1);
    }

    for(int i = 0; i < covered_cell_count; ++i) {
        const int mine_count = count_surrounding_mines(covered_cells[i]);

        if(mine_count != 0) {
            add_cell_to_array(numbered_cells, numbered_cell_count, covered_cells[i][0], covered_cells[i][1]);
        }
        else add_cell_to_array(empty_cells, empty_cell_count, covered_cells[i][0], covered_cells[i][1]);
    }
}

inline void take_nearby_row(int*** cells, int* cell_count, const int x, const int y){
    if(count_surrounding_mines((int[]){x, y}) == 0 && game_grid[x][y].is_visible == 0){
        add_cell_to_array(cells, cell_count, x, y);
    }

    if(y > 0 && game_grid[x][y - 1].is_visible == 0){
        add_cell_to_array(cells, cell_count, x, y - 1);
    }

    if(y < COLUMNS - 1 && game_grid[x][y + 1].is_visible == 0){
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

#endif // ENGINE_H_INCLUDED
