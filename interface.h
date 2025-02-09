#ifndef CARATTERI_H_INCLUDED
#define CARATTERI_H_INCLUDED

#define ANSI_COLOR_WHITE "\033[38;5;15m"
#define ANSI_COLOR_BLACK "\033[38;5;0m"
#define ANSI_COLOR_GRAY_DK "\033[38;5;237m"
#define ANSI_COLOR_GRAY_LG "\033[38;5;245m"

#define ANSI_COLOR_RED "\033[38;5;203m"
#define ANSI_COLOR_RED_LG "\033[38;5;196m"
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

#include <string.h>
#include <unistd.h>

//
// SIGNATURES
//
void print_grid();
int flagged_cells_count();
void print_header_string(char** print_string);
void get_header_message(char** header_message, int* visual_length);
void horizontal_separator_line(int line_number, char** cell_string, int *print_length);
void print_cell(const int pos[2], int is_current, char** cell_string, int *print_length);
int count_surrounding_mines(const int pos[2]);
int count_row(int x, int y);
void print_number(short int mine_count, short int is_current, char** cell_string, int *print_length);

//
// FUNCTIONS
//
inline extern void print_grid(){
    // aggrego tutto in una stringa per motivi di velocità
    // chiamare più volte printf/write rallenta molto

    char* string_grid;
    int string_grid_length = 1;

    char* header_string;
    print_header_string(&header_string);

    string_grid_length += strlen(header_string);
    string_grid = malloc(string_grid_length);
    strcpy(string_grid, header_string);

    for(int i = 0; i < row_count; ++i){
        char* string_row;
        int string_row_length = 1;

        horizontal_separator_line(i, &string_row, &string_row_length);

        for(int j = 0; j < col_count; ++j){
            const int pos[] = {i, j};
            int cell_length = 1;

            const int left_sep_length = strlen(ANSI_COLOR_GRAY_DK) + 1 + strlen(ANSI_RESET) + 3;
            char *left_separator = malloc(left_sep_length);
            left_separator[0] = '\0';
            strcat(left_separator, ANSI_COLOR_GRAY_DK);
            strcat(left_separator, (char[]){GRID_CHARSET.VERTICAL, '\0'});
            strcat(left_separator, ANSI_RESET);
            strcat(left_separator, " \0");
            cell_length += left_sep_length;

            char *value_string = malloc(0);
            int value_string_length = 0;
            if(i == position[0] && j == position[1]){
                    print_cell(pos, 1, &value_string, &value_string_length);
            }
            else print_cell(pos, 0, &value_string, &value_string_length);
            cell_length += value_string_length;

            char *cell_string = malloc(cell_length);
            cell_string[0] = '\0';
            strcat(cell_string, left_separator);
            strcat(cell_string, value_string);


            if(j == col_count - 1){
                const int rigth_sep_length = strlen(ANSI_COLOR_GRAY_DK) + 1 + strlen(ANSI_RESET) + 2;
                char *rigth_separator = malloc(rigth_sep_length);

                rigth_separator[0] = '\0';
                strcat(rigth_separator, ANSI_COLOR_GRAY_DK);
                strcat(rigth_separator, (char[]){GRID_CHARSET.VERTICAL, '\0'});
                strcat(rigth_separator, ANSI_RESET);
                strcat(rigth_separator, "\n\0");

                cell_length += rigth_sep_length;

                cell_string = realloc(cell_string, cell_length);
                strcat(cell_string, rigth_separator);
                free(rigth_separator);
            }

            string_row_length += strlen(cell_string);
            string_row = realloc(string_row, string_row_length);
            strcat(string_row, cell_string);

            free(cell_string);
            free(value_string);
            free(left_separator);
        }

        string_grid_length += strlen(string_row);
        string_grid = realloc(string_grid, string_grid_length);
        strcat(string_grid, string_row);

        free(string_row);
    }

    char* last_separator;
    horizontal_separator_line(row_count, &last_separator, &string_grid_length);

    string_grid = realloc(string_grid, string_grid_length);
    strcat(string_grid, last_separator);

    // printf("%s", string_grid);

    // uguale a printf ma manda in output direttamente in byte
    // e skippa la formattazione rendendolo più veloce
    write(STDOUT_FILENO, string_grid, strlen(string_grid));
    free(string_grid);
}

inline void horizontal_separator_line(const int line_number, char** cell_string, int *print_length){
    const int PRINT_LENGTH = col_count * 4 + 1;
    char buffer[PRINT_LENGTH + 1];

    for(int i = 0; i < PRINT_LENGTH; ++i){
        if(i == 0){
            if(line_number == 0){
                buffer[i] = GRID_CHARSET.CORNER_UP_SX;
            }
            else if (line_number == row_count){
                buffer[i] = GRID_CHARSET.CORNER_DOWN_SX;
            }
            else buffer[i] = GRID_CHARSET.CORNER_VERTICAL_SX;
        }
        else if(i == PRINT_LENGTH - 1){
            if(line_number == 0){
                buffer[i] = GRID_CHARSET.CORNER_UP_DX;
            }
            else if (line_number == row_count){
                buffer[i] = GRID_CHARSET.CORNER_DOWN_DX;
            }
            else buffer[i] = GRID_CHARSET.CORNER_VERTICAL_DX;
        }
        else{
            if(i % 4 == 0){
                if(line_number == 0){
                    buffer[i] = GRID_CHARSET.CORNER_HORIZONTAL_DOWN;
                }
                else if(line_number == row_count){
                    buffer[i] = GRID_CHARSET.CORNER_HORIZONTAL_UP;
                }
                else buffer[i] = GRID_CHARSET.INNER_CORNER;
            }
            else buffer[i] = GRID_CHARSET.HORIZONTAL;
        }
    }
    buffer[PRINT_LENGTH] = '\0';

    *print_length += strlen(ANSI_COLOR_GRAY_DK) + strlen(buffer) + strlen(ANSI_RESET) + 2;
    *cell_string = malloc(*print_length);
    (*cell_string)[0] = '\0';

    strcat(*cell_string, ANSI_COLOR_GRAY_DK);
    strcat(*cell_string, buffer);
    strcat(*cell_string, ANSI_RESET);
    strcat(*cell_string, "\n\0");
}

inline void print_cell(const int pos[2], const int is_current, char** cell_string, int *print_length){
    const cell_info info = game_grid[pos[0]][pos[1]];
    const char* highlight = "";
    const char* color = "";
    char displayed_char = ' ';

    if(info.content == ENGINE_CHARS.EMPTY && info.is_visible){
        const int mine_count = count_surrounding_mines(pos);
        print_number(mine_count, is_current, cell_string, print_length);
        return;
    }

    if (info.content == ENGINE_CHARS.MINE && has_lost) {
        displayed_char = ENGINE_CHARS.MINE;
        color = ANSI_COLOR_RED;
    }
    else {
        if(is_current){
            highlight = ANSI_COLOR_SELECTED;
        }

        if(!info.is_visible){
            displayed_char = ENGINE_CHARS.HIDDEN;
            color = ANSI_COLOR_GRAY_LG;
        }

        if (info.is_flagged) {
            displayed_char = ENGINE_CHARS.FLAGGED;
            color = ANSI_COLOR_RED;
        }
    }

    *print_length += strlen(ANSI_FORMAT_BOLD) + strlen(highlight) + strlen(color) + 1 + strlen(ANSI_RESET) + 2;
    *cell_string = (char*) malloc(*print_length);
    (*cell_string)[0] = '\0';

    strcat(*cell_string, ANSI_FORMAT_BOLD);
    strcat(*cell_string, highlight);
    strcat(*cell_string, color);
    strcat(*cell_string, (char[]) {displayed_char, '\0'});
    strcat(*cell_string, ANSI_RESET);
    strcat(*cell_string, " \0");
}

inline void print_number(const short int mine_count, const short int is_current, char** cell_string, int *print_length){
    char displayed_char = '0' + mine_count;
    const char* highlight = "";
    const char* color = NULL;


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
            color = ANSI_COLOR_RED_LG;
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
        default: ;
    }

    *print_length += strlen(ANSI_FORMAT_BOLD) + strlen(highlight) + strlen(color) + 1 + strlen(ANSI_RESET) + 2;
    *cell_string = (char*) malloc(*print_length);
    (*cell_string)[0] = '\0';

    strcat(*cell_string, ANSI_FORMAT_BOLD);
    strcat(*cell_string, highlight);
    strcat(*cell_string, color);
    strcat(*cell_string, (char[]) {displayed_char, '\0'});
    strcat(*cell_string, ANSI_RESET);
    strcat(*cell_string, " \0");
}

inline void print_header_string(char** print_string) {
    const int HEADER_LENGTH = row_count * 4 + 1;
    char* mid_padding;
    int visual_length;
    char* header_message;

    get_header_message(&header_message, &visual_length);

    int final_header_length;

    if (HEADER_LENGTH > visual_length) {
        final_header_length = HEADER_LENGTH;
        mid_padding = malloc(HEADER_LENGTH - visual_length + 1);
        memset(mid_padding, ' ', HEADER_LENGTH - visual_length);
        mid_padding[HEADER_LENGTH - visual_length] = '\0';
    }
    else {
        final_header_length = visual_length;
        mid_padding = malloc(1);
        mid_padding[0] = '\0';
    }

    char* header_top = malloc(final_header_length + 1);
    char* header_bottom = malloc(final_header_length + 1);

    memset(header_top, GRID_CHARSET.HORIZONTAL, final_header_length);
    memset(header_bottom, GRID_CHARSET.HORIZONTAL, final_header_length);

    header_top[final_header_length] = '\0';
    header_bottom[final_header_length] = '\0';

    header_top[0] = GRID_CHARSET.CORNER_UP_SX;
    header_top[final_header_length - 1] = GRID_CHARSET.CORNER_UP_DX;

    header_bottom[0] = GRID_CHARSET.CORNER_DOWN_SX;
    header_bottom[final_header_length - 1] = GRID_CHARSET.CORNER_DOWN_DX;

    // il 4 per i \n
    *print_string = malloc( 1 +
        strlen(header_top) * 2 +
        strlen(ANSI_COLOR_GRAY_DK) * 2 +
        strlen(header_message) + 4 +
        strlen(mid_padding)
    );
    *print_string[0] = '\0';


    strcat(*print_string, ANSI_COLOR_GRAY_DK);
    strcat(*print_string, header_top);
    strcat(*print_string, "\n");
    strcat(*print_string, (char[]){ GRID_CHARSET.VERTICAL, ' ', '\0' });
    strcat(*print_string, header_message);
    strcat(*print_string, mid_padding);
    strcat(*print_string, ANSI_COLOR_GRAY_DK);
    strcat(*print_string, (char[]){ GRID_CHARSET.VERTICAL, ' ', '\0' });
    strcat(*print_string, "\n");
    strcat(*print_string, header_bottom);
    strcat(*print_string, "\n");

    free(header_message);
    free(header_top);
    free(header_bottom);
    free(mid_padding);
}

inline void get_header_message(char** header_message, int* visual_length) {
    const char* IN_GAME = ANSI_FORMAT_BOLD ANSI_COLOR_GRAY_LG "Remaining mines: " ANSI_RESET ANSI_COLOR_WHITE;
    const char* WIN = ANSI_COLOR_GREEN ANSI_FORMAT_BOLD "You won!" ANSI_RESET ANSI_COLOR_GRAY_DK " (Press any key to continue)";
    const char* LOSE = ANSI_COLOR_RED  ANSI_FORMAT_BOLD "You lost!" ANSI_COLOR_GRAY_DK " (Press any key to continue)";

    if (has_won) {
        *header_message = malloc(strlen(WIN) + 1);
        strcpy(*header_message, WIN);
        // + 2 per la lunghezza del separatore e spazio iniziale
        *visual_length = 8 + 28 + 2 + 1;
    }
    else if (has_lost) {
        *header_message = malloc(strlen(LOSE) + 1);
        strcpy(*header_message, LOSE);
        *visual_length = 9 + 28 + 2 + 1;
    }
    else {
        const int remaining_mines = mine_count - flagged_cells_count();
        char remaining_mines_string[4];
        itoa(remaining_mines, remaining_mines_string, 10);

        *header_message = malloc(strlen(IN_GAME) + strlen(remaining_mines_string) + 1);
        strcpy(*header_message, IN_GAME);
        strcat(*header_message, remaining_mines_string);
        *visual_length = 17 + strlen(remaining_mines_string) + 1 + 2;
    }

}

inline int flagged_cells_count() {
    int count = 0;
    for (int i = 0; i < row_count; ++i) {
        for (int j = 0; j < col_count; ++j) {
            if (game_grid[i][j].is_flagged) {
                ++count;
            }
        }
    }
    return count;
}

#endif // CARATTERI_H_INCLUDED