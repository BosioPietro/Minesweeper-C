#ifndef CARATTERI_H_INCLUDED
#define CARATTERI_H_INCLUDED

#include "utils.h"

#define ANSI_COLOR_WHITE "\033[38;5;15m"
#define ANSI_COLOR_BLACK "\033[38;5;0m"
#define ANSI_COLOR_GRAY_DK "\033[38;5;237m"
#define ANSI_COLOR_GRAY_LG "\033[38;5;246m"

#define ANSI_COLOR_RED "\033[38;5;9m"
#define ANSI_COLOR_BLUE "\033[38;5;33m"
#define ANSI_COLOR_GREEN "\033[38;5;46m"
#define ANSI_COLOR_PURPLE "\033[38;5;13m"
#define ANSI_COLOR_YELLOW "\033[38;5;11m"
#define ANSI_COLOR_CYAN "\033[38;5;14m"
#define ANSI_COLOR_ORANGE "\033[38;5;208m"
#define ANSI_COLOR_PINK "\033[38;5;213m"
#define ANSI_COLOR_SELECTED "\033[48;5;246m"

#define ANSI_FORMAT_BOLD "\033[1m"

#define ANSI_RESET "\033[0m"



//
// SIGNATURES
//
void print_grid();
void horizontal_separator_line(const int line_number);
void print_cell(const int pos[2], const int is_current);
int count_surrouding_mines(const int pos[2]);
int count_row(const int x, const int y);
void print_number(const short int mine_count, const short int is_current);

//
// FUNCTIONS
//
extern void print_grid(){
    for(int i = 0; i < ROWS; ++i){

        horizontal_separator_line(i);

        for(int j = 0; j < COLUMNS; ++j){
            printf("%s%c%s ", ANSI_COLOR_GRAY_DK, ENGINE_CHARS.GRID.VERTICAL, ANSI_RESET);

            const int pos[] = {i, j};
            if(i == position[0] && j == position[1]){
                    print_cell(pos, 1);
            }
            else print_cell(pos, 0);

            printf("%s", ANSI_RESET);

            if(j == COLUMNS - 1){
                printf("%s%c%s\n", ANSI_COLOR_GRAY_DK, ENGINE_CHARS.GRID.VERTICAL, ANSI_RESET);
            }
        }
    }

    horizontal_separator_line(ROWS);
}

void horizontal_separator_line(const int line_number){
    const int PRINT_LENGTH = COLUMNS * 4 + 1;
    printf("%s", ANSI_COLOR_GRAY_DK);

    for(int i = 0; i < PRINT_LENGTH; ++i){
        if(i == 0){
            if(line_number == 0){
                printf("%c", ENGINE_CHARS.GRID.CORNER_UP_SX);
            }
            else if (line_number == ROWS){
                printf("%c", ENGINE_CHARS.GRID.CORNER_DOWN_SX);
            }
            else printf("%c", ENGINE_CHARS.GRID.CORNER_VERTICAL_SX);
        }
        else if(i == PRINT_LENGTH - 1){
            if(line_number == 0){
                printf("%c", ENGINE_CHARS.GRID.CORNER_UP_DX);
            }
            else if (line_number == ROWS){
                printf("%c", ENGINE_CHARS.GRID.CORNER_DOWN_DX);
            }
            else printf("%c", ENGINE_CHARS.GRID.CORNER_VERTICAL_DX);;
        }
        else{
            if(i % 4 == 0){
                if(line_number == 0){
                    printf("%c", ENGINE_CHARS.GRID.CORNER_HORIZONTAL_DOWN);
                }
                else if(line_number == ROWS){
                    printf("%c", ENGINE_CHARS.GRID.CORNER_HORIZONTAL_UP);
                }
                else printf("%c", ENGINE_CHARS.GRID.INNER_CORNER);;
            }
            else printf("%c", ENGINE_CHARS.GRID.HORIZONTAL);
        }
    }
    printf("%s\n", ANSI_RESET);
}

void print_cell(const int pos[2], const int is_current){
    const cell_info info = game_grid[pos[0]][pos[1]];

    if(info.is_visible == 0){
        if(is_current == 0){
            printf("%s%s%c ", ANSI_FORMAT_BOLD, ANSI_COLOR_GRAY_LG, ENGINE_CHARS.HIDDEN);
        }
        else printf("%s%s%c ", ANSI_FORMAT_BOLD, ANSI_COLOR_WHITE, ENGINE_CHARS.HIDDEN);
        return;
    }

    if(info.content == ENGINE_CHARS.MINE){
        if(is_current == 0){
            printf("%s%s%c ", ANSI_FORMAT_BOLD, ANSI_COLOR_RED, ENGINE_CHARS.MINE);
        }
        else printf("%s%s%c ", ANSI_FORMAT_BOLD, ANSI_COLOR_RED, ENGINE_CHARS.MINE);
        return;
    }

    if(info.content == ENGINE_CHARS.EMPTY){
        int mine_count = count_surrouding_mines(pos);
        print_number(mine_count, is_current);
        return;
    }
}

void print_number(const short int mine_count, const short int is_current){
    char displayed_char = '0' + mine_count;
    char* highlight = "";
    char* color;


    if(mine_count == 0){
        displayed_char = ENGINE_CHARS.VISIBLE;
    }

    if(is_current == 1){
        highlight = ANSI_COLOR_SELECTED;
    }

    switch(mine_count){
        case 0:
            color = ANSI_COLOR_BLACK;
            break;
        case 1:
            color = ANSI_COLOR_BLUE;
            break;
        case 2:
            color = ANSI_COLOR_GREEN;
            break;
        case 3:
            color = ANSI_COLOR_RED;
            break;
        case 4:
            color = ANSI_COLOR_PURPLE;
            break;
        case 5:
            color = ANSI_COLOR_YELLOW;
            break;
        case 6:
            color = ANSI_COLOR_CYAN;
            break;
        case 7:
            color = ANSI_COLOR_ORANGE;
            break;
        case 8:
            color = ANSI_COLOR_PINK;
            break;
    }

    printf("%s%s%s%c", ANSI_FORMAT_BOLD, highlight, color, displayed_char);
    printf("%s ", ANSI_RESET);

}

int count_surrouding_mines(const int pos[2]){
    int mine_count = 0,
        x = pos[0],
        y = pos[1];

    // mine sopra
    if(x > 0){
        mine_count += count_row(x - 1, y);
    }

    // mine sotto
    if(x < ROWS - 1){
        mine_count += count_row(x + 1, y);
    }

    // mine sinistra
    if(y > 0 && game_grid[x][y - 1].content == ENGINE_CHARS.MINE){
        ++mine_count;
    }

    // mine destra
    if(y < COLUMNS - 1 && game_grid[x][y + 1].content == ENGINE_CHARS.MINE){
        ++mine_count;
    }

    return mine_count;
}

int count_row(const int x, const int y){
    int mine_count = 0;

    if(game_grid[x][y].content == ENGINE_CHARS.MINE){
        ++mine_count;
    }

    if(y > 0 && game_grid[x][y - 1].content == ENGINE_CHARS.MINE){
        ++mine_count;
    }

    if(y < COLUMNS - 1 && game_grid[x][y + 1].content == ENGINE_CHARS.MINE){
        ++mine_count;
    }

    return mine_count;
}

#endif // CARATTERI_H_INCLUDED
