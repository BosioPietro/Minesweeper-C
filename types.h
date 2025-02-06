#ifndef TYPES_H
#define TYPES_H

//
// GRID CHARS
//
typedef enum {
    HORIZONTAL = 205,
    VERTICAL = 186,

    CORNER_UP_SX = 201,
    CORNER_UP_DX = 187,
    CORNER_DOWN_SX = 200,
    CORNER_DOWN_DX = 188,

    CORNER_VERTICAL_SX = 204,
    CORNER_VERTICAL_DX = 185,
    CORNER_HORIZONTAL_DOWN = 203,
    CORNER_HORIZONTAL_UP = 202,

    INNER_CORNER = 206
} grid_char;

typedef struct{
    const grid_char HORIZONTAL;
    const grid_char VERTICAL;

    const grid_char CORNER_UP_SX;
    const grid_char CORNER_UP_DX;
    const grid_char CORNER_DOWN_SX;
    const grid_char CORNER_DOWN_DX;

    const grid_char CORNER_VERTICAL_SX;
    const grid_char CORNER_VERTICAL_DX;
    const grid_char CORNER_HORIZONTAL_DOWN;
    const grid_char CORNER_HORIZONTAL_UP;
    const grid_char INNER_CORNER;
} grid_chars;

const grid_chars GRID_CHARSET = {
    .HORIZONTAL = HORIZONTAL,
    .VERTICAL = VERTICAL,

    .CORNER_UP_SX = CORNER_UP_SX,
    .CORNER_UP_DX = CORNER_UP_DX,
    .CORNER_DOWN_SX = CORNER_DOWN_SX,
    .CORNER_DOWN_DX = CORNER_DOWN_DX,

    .CORNER_VERTICAL_SX = CORNER_VERTICAL_SX,
    .CORNER_VERTICAL_DX = CORNER_VERTICAL_DX,
    .CORNER_HORIZONTAL_DOWN = CORNER_HORIZONTAL_DOWN,
    .CORNER_HORIZONTAL_UP = CORNER_HORIZONTAL_UP,
    .INNER_CORNER = INNER_CORNER
};


//
// ENGINE CHARS
//
typedef enum {
    EMPTY = 63,
    MINE = 33,
    VISIBLE = 176,
    HIDDEN = 254,
    FLAGGED = 232
} engine_char;

typedef struct{
    const engine_char EMPTY;
    const engine_char MINE;
    const engine_char VISIBLE;
    const engine_char HIDDEN;
    const engine_char FLAGGED;
} charset;

const charset ENGINE_CHARS = {
    .HIDDEN = HIDDEN,
    .EMPTY = EMPTY,
    .VISIBLE = VISIBLE,
    .MINE = MINE,
    .FLAGGED = FLAGGED
};


//
// CELL
//
typedef struct{
    short int is_visible;
    short int is_flagged;
    engine_char content;
} cell_info;


//
// KEYSTROKES CHARS
//
typedef enum {
    KEY_UP = 119,
    KEY_DOWN = 115,
    KEY_LEFT = 97,
    KEY_RIGHT = 100,
    KEY_INTERACT = 113,
    KEY_FLAG = 101
} key;

typedef struct{
    const key KEY_DOWN;
    const key KEY_LEFT;
    const key KEY_RIGHT;
    const key KEY_UP;
    const key KEY_INTERACT;
    const key KEY_FLAG;
} commands;

commands ENGINE_COMMANDS = {
    .KEY_DOWN = KEY_DOWN,
    .KEY_LEFT = KEY_LEFT,
    .KEY_RIGHT = KEY_RIGHT,
    .KEY_UP = KEY_UP,
    .KEY_INTERACT = KEY_INTERACT,
    .KEY_FLAG = KEY_FLAG
};

#endif //TYPES_H
