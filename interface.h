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

#define MAX_COL_ROW_COUNT 20

#include <string.h>
#include <unistd.h>

//
// SIGNATURES
//

// game
void show_grid();
void get_header_string(char** print_string);
void get_header_message(char** header_message, int* visual_length);
int flagged_cells_count();
void horizontal_separator_line(int line_number, char** cell_string, int *print_length);
void get_cell_string(const int pos[2], int is_current, char** cell_string, int *print_length);
int count_surrounding_mines(const int pos[2]);
void get_number_string(short int mine_count, short int is_current, char** cell_string, int *print_length);

// menu
void show_menu(int OPTIONS_NUMBER, const char **MENU_OPTIONS, int selected_option);;
void show_controls();
void show_settings(int SETTING_NUMBER, int selected_setting);
void show_change_setting(short int setting_index);

/*
 * Dato che l'interfaccia viene stampata frequentemente
 * e specialmente in game c'è tanto da stampare
 * l'approccio normale coi printf è troppo lento.
 * Per questo mi costruisco una grande stringa da
 * stampare in un solo colpo e la stampo con
 * `write` che funziona come printf ma stampando
 * direttamente i byte che gli passi senze altre elaborazioni
*/


//
// FUNCTIONS
//
inline extern void show_grid(){
    // Prende l'header
    char* header_string = NULL;
    get_header_string(&header_string);


    // Inizializza la stringa della griglia
    // e vi ci appende l'header
    int string_grid_length = 1 + strlen(header_string);
    char *string_grid = malloc(string_grid_length);
    strcpy(string_grid, header_string);

    // Genera la stringa per la griglia
    for(int i = 0; i < row_count; ++i){
        // Inizializza la riga
        char* string_row;
        int string_row_length = 1;

        horizontal_separator_line(i, &string_row, &string_row_length);

        // Genera la stringa per ogni singola cella
        for(int j = 0; j < col_count; ++j){
            const int pos[] = {i, j};
            int cell_length = 1;

            // Separatore di sinistra
            const int left_sep_length = strlen(ANSI_COLOR_GRAY_DK) + 1 + strlen(ANSI_RESET) + 3;
            char *left_separator = malloc(left_sep_length);
            left_separator[0] = '\0';
            strcat(left_separator, ANSI_COLOR_GRAY_DK);
            strcat(left_separator, (char[]){GRID_CHARSET.VERTICAL, '\0'});
            strcat(left_separator, ANSI_RESET);
            strcat(left_separator, " \0");
            cell_length += left_sep_length;

            // Contenuto effettivo della cella
            char *value_string = malloc(0);
            int value_string_length = 0;
            if(i == position[0] && j == position[1]){
                    get_cell_string(pos, 1, &value_string, &value_string_length);
            }
            else get_cell_string(pos, 0, &value_string, &value_string_length);
            cell_length += value_string_length;

            char *cell_string = malloc(cell_length);
            cell_string[0] = '\0';
            strcat(cell_string, left_separator);
            strcat(cell_string, value_string);


            // Se è l'ultma cella appende un separatore a destra
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

            // Appende la cella alla riga
            string_row_length += strlen(cell_string);
            string_row = realloc(string_row, string_row_length);
            strcat(string_row, cell_string);

            free(cell_string);
            free(value_string);
            free(left_separator);
        }

        // Appende la riga alla griglia
        string_grid_length += strlen(string_row);
        char *temp = realloc(string_grid, string_grid_length);

        // Gestisce il leak se realloc non riesce ad alloccare
        if (temp != NULL) {
            string_grid = temp;
        }
        else {
            free(string_grid);
            return;
        }

        strcat(string_grid, string_row);

        free(string_row);
    }

    // Ultima riga in basso che chiude la griglia
    char* last_separator;
    horizontal_separator_line(row_count, &last_separator, &string_grid_length);

    char* temp = realloc(string_grid, string_grid_length);
    if (temp != NULL) {
        string_grid = temp;
    }
    else {
        free(string_grid);
        return;
    }
    strcat(string_grid, last_separator);

    write(STDOUT_FILENO, string_grid, strlen(string_grid));

    free(last_separator);
    free(string_grid);
    free(header_string);
}

inline void horizontal_separator_line(const int line_number, char** cell_string, int *print_length){
    // Ogni cella è lunga 4, e alla fine c'è
    // il separatore alla destra
    const int PRINT_LENGTH = col_count * 4 + 1;
    char buffer[PRINT_LENGTH + 1];

    for(int i = 0; i < PRINT_LENGTH; ++i){
        if(i == 0){ // Primo carattere (è sempre un angolo)
            if(line_number == 0){
                buffer[i] = GRID_CHARSET.CORNER_UP_SX;
            }
            else if (line_number == row_count){
                buffer[i] = GRID_CHARSET.CORNER_DOWN_SX;
            }
            else buffer[i] = GRID_CHARSET.CORNER_VERTICAL_SX;
        }
        else if(i == PRINT_LENGTH - 1){ // Ultimo carattere (sempre angolo)
            if(line_number == 0){
                buffer[i] = GRID_CHARSET.CORNER_UP_DX;
            }
            else if (line_number == row_count){
                buffer[i] = GRID_CHARSET.CORNER_DOWN_DX;
            }
            else buffer[i] = GRID_CHARSET.CORNER_VERTICAL_DX;
        }
        else{ // Caratteri in mezzo
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
    // Chiudo la stringa
    buffer[PRINT_LENGTH] = '\0';

    // Costruisco la stringa da stampare
    *print_length += strlen(ANSI_COLOR_GRAY_DK) + strlen(buffer) + strlen(ANSI_RESET) + 2;
    *cell_string = malloc(*print_length);
    (*cell_string)[0] = '\0';

    strcat(*cell_string, ANSI_COLOR_GRAY_DK);
    strcat(*cell_string, buffer);
    strcat(*cell_string, ANSI_RESET);
    strcat(*cell_string, "\n\0");
}

inline void get_cell_string(const int pos[2], const int is_current, char** cell_string, int *print_length){
    // Ritorna la stringa che rappresenta
    // il contenuto della cella
    const cell_info info = game_grid[pos[0]][pos[1]];
    const char* highlight = "";
    const char* color = "";
    char displayed_char = ' ';

    // Se è un numero
    if(info.content == ENGINE_CHARS.EMPTY && info.is_visible){
        const int mine_count = count_surrounding_mines(pos);
        get_number_string(mine_count, is_current, cell_string, print_length);
        return;
    }

    // Se l'utente ha perso le mine vengono scoperte
    if (info.content == ENGINE_CHARS.MINE && has_lost) {
        displayed_char = ENGINE_CHARS.MINE;
        color = ANSI_COLOR_RED;
    }
    else {
        // Se è la cella su cui l'utente si trova
        // cambia lo sfondo del carattere
        if(is_current){
            highlight = ANSI_COLOR_SELECTED;
        }

        // Cella nascosta
        if(!info.is_visible){
            displayed_char = ENGINE_CHARS.HIDDEN;
            color = ANSI_COLOR_GRAY_LG;
        }

        // Bandiera
        if (info.is_flagged) {
            displayed_char = ENGINE_CHARS.FLAGGED;
            color = ANSI_COLOR_RED;
        }
    }

    // Costruisco la stringa
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

inline void get_number_string(const short int mine_count, const short int is_current, char** cell_string, int *print_length){
    // Inserisci il numero corrispondente
    char displayed_char = '0' + mine_count;
    const char* highlight = "";
    const char* color = NULL;

    // Carattere cella vuota
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
        default:
    }

    // Costruisco la stringa da stampare
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

inline void get_header_string(char** print_string) {
    const int HEADER_LENGTH = col_count * 4 + 1;
    char* mid_padding = NULL;
    char* header_message = NULL;
    int visual_length;

    // Messaggio testuale dell'header.
    // Non posso usare strlen perchè quella tiene
    // conto dei caratteri ASCII di formattazione
    // che non sono visibili e quindi svasano i calcoli
    get_header_message(&header_message, &visual_length);

    int final_header_length;

    // L'header si adatta alla lunghezza della griglia
    // se la griglia è più lunga viene aggiunto un
    // padding di spazi. Altrimenti se la lunghezza
    // della griglia è troppo corta, non si adatta
    if (HEADER_LENGTH > visual_length) {
        // Calcola la differenza di lunghezza
        // e crea il padding
        final_header_length = HEADER_LENGTH;
        mid_padding = malloc(HEADER_LENGTH - visual_length + 1);
        memset(mid_padding, ' ', HEADER_LENGTH - visual_length);
        mid_padding[HEADER_LENGTH - visual_length] = '\0';
    }
    else {
        // Niente padding
        final_header_length = visual_length;
        mid_padding = malloc(1);
        mid_padding[0] = '\0';
    }

    // Separatori orizzontali dell'header
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


    // Costruisco la stringa da stampare
    *print_string = malloc( 1 +
        strlen(header_top) * 2 +
        strlen(ANSI_COLOR_GRAY_DK) * 2 +
        strlen(header_message) + 4 +  // il 4 per i \n
        strlen(mid_padding) + 1
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

inline int flagged_cells_count() {
    // Funzione ausiliaria usata dall'header
    // per vedere le mine rimanenti in
    // base alle bandiere piazzate
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

inline void get_header_message(char** header_message, int* visual_length) {
    // In tutti i casi c'è un +2 che sarebbero
    // il separatore verticale e lo spazio con
    // cui inizia la riga

    if (has_won) {
        // Stringa di vittoria
        const char* WIN = ANSI_COLOR_GREEN ANSI_FORMAT_BOLD "You won!" ANSI_RESET ANSI_COLOR_GRAY_DK " (Press any key to continue)";

        *header_message = malloc(strlen(WIN) + 1);
        strcpy(*header_message, WIN);
        *visual_length = 8 + 28 + 2 + 1;
    }
    else if (has_lost) {
        // Stringa di perdita
        const char* LOSE = ANSI_COLOR_RED  ANSI_FORMAT_BOLD "You lost!" ANSI_COLOR_GRAY_DK " (Press any key to continue)";

        *header_message = malloc(strlen(LOSE) + 1);
        strcpy(*header_message, LOSE);
        *visual_length = 9 + 28 + 2 + 1;
    }
    else {
        // L'utente è in game, stampa il numero di mine rimanenti.
        const char* IN_GAME = ANSI_FORMAT_BOLD ANSI_COLOR_GRAY_LG "Remaining mines: " ANSI_RESET ANSI_COLOR_WHITE;

        // La stringa è lunga 4 perchè il numero può essere negativo
        // (caso peggiore "-99\0").
        const int remaining_mines = mine_count - flagged_cells_count();
        char remaining_mines_string[4];
        itoa(remaining_mines, remaining_mines_string, 10);

        *header_message = malloc(strlen(IN_GAME) + strlen(remaining_mines_string) + 1);
        strcpy(*header_message, IN_GAME);
        strcat(*header_message, remaining_mines_string);
        *visual_length = 17 + strlen(remaining_mines_string) + 1 + 2;
    }

}

inline void show_change_setting(const short int setting_index) {
    // Stampa la stringa di quando vuoi cambiare righe/colonne/mine
    const char* PROMPT = ANSI_FORMAT_BOLD ANSI_COLOR_GRAY_LG "Insert new value ";
    char *title;
    int max_value;

    char* range_string = malloc(strlen(ANSI_COLOR_GRAY_DK) + 5 + 1);
    const char* RANGE_STRING_END = "]" ANSI_COLOR_GRAY_LG ": " ANSI_RESET ANSI_COLOR_WHITE;
    range_string[0] = '\0';
    strcat(range_string, ANSI_COLOR_GRAY_DK "[0 - ");

    // Tipo di valore da cambiare
    switch (setting_index) {
        default:
        case 0:
            title = ANSI_FORMAT_BOLD ANSI_COLOR_RED "EDIT ROWS COUNT\n\n";
        break;
        case 1:
            title = ANSI_FORMAT_BOLD ANSI_COLOR_RED "EDIT COLUMNS COUNT\n\n";
        break;
        case 2:
            title = ANSI_FORMAT_BOLD ANSI_COLOR_RED "EDIT MINE COUNT\n\n";
        break;
    }

    // Se si sta cambiando il numero di mine,
    // non si posso mettere più mine di quante
    // celle ci sono
    if (setting_index == 2) {
        max_value = row_count * col_count;
    }
    else max_value = MAX_COL_ROW_COUNT;

    // Crea la stringa col range di valori ammesso
    char range_string_value[3];
    itoa(max_value, range_string_value, 10);

    // Previene il memory leak nel caso in cui realloc non
    // riesca a trovare spazio
    char* temp = realloc(range_string, strlen(range_string) + 3 + strlen(RANGE_STRING_END));

    if (temp != NULL) {
        range_string = temp;
        temp = NULL;
    } else return free(range_string);

    strcat(range_string, range_string_value);
    strcat(range_string, RANGE_STRING_END);

    // Costruisco la stringa da stampare
    char* print_string = malloc(strlen(title) + strlen(PROMPT) + strlen(range_string));
    print_string[0] = '\0';

    strcat(print_string, title);
    strcat(print_string, PROMPT);
    strcat(print_string, range_string);

    write(STDOUT_FILENO, print_string, strlen(print_string));

    free(range_string);
    free(print_string);
    free(temp);
}

inline void show_menu(const int OPTIONS_NUMBER, const char **MENU_OPTIONS, const int selected_option){
    // Il menù è essenzialmente una grande stringa statica
    // ma me la costruisco per essere più organizzato
    const char* STRING_TITLE = ANSI_FORMAT_BOLD ANSI_COLOR_RED "MAIN MENU";
    const char* TOOLTIP = ANSI_RESET ANSI_COLOR_GRAY_LG " [Use W and S]\n\n" ANSI_RESET;
    const char* SELECTED_INDICATOR = ANSI_FORMAT_BOLD ANSI_COLOR_CYAN "> " ANSI_COLOR_WHITE;

    // Capisco la lunghezza della stringa finale
    // per vedere quanta memoria allocare
    int string_length = 1 + strlen(STRING_TITLE) +
                        strlen(TOOLTIP) +
                        strlen(SELECTED_INDICATOR) +
                        (strlen(ANSI_COLOR_GRAY_DK) + strlen(ANSI_COLOR_GRAY_LG) + 2) * 3;

    for(int i = 0; i < OPTIONS_NUMBER; i++) {
        string_length += strlen(MENU_OPTIONS[i]);
    }

    // Costruisco la stringa da stampare
    char *menu_string = malloc(string_length);
    menu_string[0] = '\0';
    strcat(menu_string, STRING_TITLE);
    strcat(menu_string, TOOLTIP);

    // Appendo le opzini
    for (int i = 0; i < OPTIONS_NUMBER; i++) {
        // Prima di ogni opzione metto l'indicatore
        // che fa vedere se è selezionata o meno
        if (i == selected_option) {
            strcat(menu_string, SELECTED_INDICATOR);
        }
        else {
            strcat(menu_string, ANSI_COLOR_GRAY_DK);
            strcat(menu_string, (char[]){254, ' ', '\0'});
            strcat(menu_string, ANSI_COLOR_GRAY_LG);
        }
        strcat(menu_string, MENU_OPTIONS[i]);
    }

    write(STDOUT_FILENO, menu_string, strlen(menu_string));

    free(menu_string);
}

inline void show_controls() {
    // Anche la schermata dei controlli è una grande
    // stringa statica che costruisco per essere
    // più organizzato
    const char* STRING_TITLE = ANSI_FORMAT_BOLD ANSI_COLOR_RED "GAME CONTROLS\n\n";
    const char* STRING_EXIT = "\n" ANSI_FORMAT_BOLD ANSI_COLOR_CYAN "> " ANSI_COLOR_WHITE "Back to menu";
    const char* CONTROLS[] = {
        ANSI_COLOR_WHITE "W" ANSI_COLOR_GRAY_DK " - " ANSI_COLOR_GRAY_LG "Move up\n",
        ANSI_COLOR_WHITE "A" ANSI_COLOR_GRAY_DK " - " ANSI_COLOR_GRAY_LG "Move left\n",
        ANSI_COLOR_WHITE "S" ANSI_COLOR_GRAY_DK " - " ANSI_COLOR_GRAY_LG "Move down\n",
        ANSI_COLOR_WHITE "D" ANSI_COLOR_GRAY_DK " - " ANSI_COLOR_GRAY_LG "Move right\n",
        ANSI_COLOR_WHITE "Q" ANSI_COLOR_GRAY_DK " - " ANSI_COLOR_GRAY_LG "Uncover cell\n",
        ANSI_COLOR_WHITE "Space" ANSI_COLOR_GRAY_DK " - " ANSI_COLOR_GRAY_LG "Place/remove flag\n",
    };

    // Capisco la lunghezza della stringa finale
    // per vedere quanta memoria allocare
    int print_length = 1 + strlen(STRING_TITLE) + strlen(STRING_EXIT);
    for(int i = 0; i < 6; i++) {
        print_length += strlen(CONTROLS[i]);
    }

    // Costruisco la stringa da stampare
    char* controls_string = malloc(print_length);
    controls_string[0] = '\0';

    strcat(controls_string, STRING_TITLE);
    for(int i = 0; i < 6; i++) {
        strcat(controls_string, CONTROLS[i]);
    }
    strcat(controls_string, STRING_EXIT);

    write(STDOUT_FILENO, controls_string, print_length);
    free(controls_string);
}

inline void show_settings(const int SETTING_NUMBER, const int selected_setting) {
    const char* STRING_TITLE = ANSI_FORMAT_BOLD ANSI_COLOR_RED "GAME SETTINGS\n\n";
    const char* SELECTED_INDICATOR = ANSI_FORMAT_BOLD ANSI_COLOR_CYAN "> " ANSI_COLOR_WHITE;
    const char* EDIT_TOOLTIP = ANSI_RESET ANSI_COLOR_GRAY_DK " (edit)\n";
    const char* SETTINGS[] = {
        "Rows count: " ANSI_RESET,
        "Columns count: " ANSI_RESET,
        "Mine count: " ANSI_RESET,
        "Back to menu" ANSI_RESET
    };

    // Creo la stringa con i valori di righe, colonne e mine
    char row_count_string[3];
    itoa(row_count, row_count_string, 10);

    char col_count_string[3];
    itoa(col_count, col_count_string, 10);

    char mine_count_string[3];
    itoa(mine_count, mine_count_string, 10);

    // Capisco la lunghezza per vedere quanta memoria allocare
    int string_length = strlen(STRING_TITLE) +
                        strlen(SELECTED_INDICATOR) +
                        strlen(row_count_string) +
                        strlen(col_count_string) +
                        strlen(mine_count_string) +
                        strlen(ANSI_COLOR_GRAY_LG) * 3 +
                        (strlen(ANSI_COLOR_GRAY_DK) + strlen(ANSI_COLOR_GRAY_LG) + 2) * 3;

    // Se l'ultima opzione è selezionata, non c'è l'indicatore "(edit)"
    if (selected_setting != SETTING_NUMBER - 1) {
        string_length += strlen(EDIT_TOOLTIP);
    }

    for(int i = 0; i < SETTING_NUMBER; i++) {
        string_length += strlen(SETTINGS[i]) + 1;
    }

    // Costruisco la stringa da stampare
    char* settings_string = malloc(string_length);
    settings_string[0] = '\0';
    strcat(settings_string, STRING_TITLE);

    for(int i = 0; i < SETTING_NUMBER; i++) {
        // Appendo l'indicatore di selezione corretto
        if (selected_setting == i) {
            strcat(settings_string, SELECTED_INDICATOR);
        }
        else {
            strcat(settings_string, ANSI_COLOR_GRAY_DK);
            strcat(settings_string, (char[]){254, ' ', '\0'});
            strcat(settings_string, ANSI_COLOR_GRAY_LG);
        }

        // Appendo il "nome" dell'impostazione
        strcat(settings_string, SETTINGS[i]);

        // Appendo il valore dell'impostazione
        if (i == 0) {
            strcat(settings_string, ANSI_COLOR_GRAY_LG);
            strcat(settings_string, row_count_string);
        }
        else if (i == 1) {
            strcat(settings_string, ANSI_COLOR_GRAY_LG);
            strcat(settings_string, col_count_string);
        }
        else if (i == 2) {
            strcat(settings_string, ANSI_COLOR_GRAY_LG);
            strcat(settings_string, mine_count_string);
        }

        // Se è un valore che può essere cambiato
        // (quindi NON l'ultima opzione) aggiungo "(edit)"
        if (i != SETTING_NUMBER - 1 && selected_setting == i) {
            strcat(settings_string, EDIT_TOOLTIP);
        }
        else strcat(settings_string, "\n");
    }

    write(STDOUT_FILENO, settings_string, string_length);

    free(settings_string);
}

#endif // CARATTERI_H_INCLUDED