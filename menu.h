#ifndef MENU_H
#define MENU_H
#define KEY_ENTER 13
#define KEY_SPACEBAR 32

#include <unistd.h>

int row_count = 10;
int col_count = 10;
int mine_count = 10;

//
// SIGNATURES
//
void print_menu();
void handle_menu();
void select_option();
void show_controls();
void print_settings();
void handle_settings();
void select_setting();

//
// GLOBAL VARS
//
const int OPTIONS_NUMBER = 4;
const char* MENU_OPTIONS[] = {
    "Play\n" ANSI_RESET,
    "Settings\n" ANSI_RESET,
    "Controls\n" ANSI_RESET,
    "Quit\n" ANSI_RESET
};

short int selected_option = 0;

const int SETTING_NUMBER = 4;
short int selected_setting = 0;

//
// FUNCTION
//
inline void handle_menu() {
    char command;

    print_menu();

    do{
        fflush(stdin);
        command = _getch();
        printf("%d", command);
    }
    while(command != KEY_ENTER && command != KEY_SPACEBAR && command != ENGINE_COMMANDS.KEY_UP && command != ENGINE_COMMANDS.KEY_DOWN);

    system("@cls||clear");

    switch(command) {
        case KEY_UP:
            if (selected_option > 0)
                --selected_option;
            break;
        case KEY_DOWN:
            if (selected_option < OPTIONS_NUMBER - 1)
                ++selected_option;
            break;
        default:
        case KEY_SPACEBAR:
        case KEY_ENTER:
            select_option();
            break;
    }

    system("@cls||clear");
}

inline void select_option() {
    switch(selected_option) {
        default:
            case 1:
            selected_setting = 0;
            game_state =  GAME_STATE.OPTIONS;

            do {
                handle_settings();
            }while(game_state == GAME_STATE.OPTIONS);
            break;
        case 2:
            show_controls();
            break;
        case 3:
            game_state = GAME_STATE.QUIT;
            break;
    }
}

inline void show_controls() {
    const char* STRING_TITLE = ANSI_FORMAT_BOLD ANSI_COLOR_RED "GAME CONTROLS\n\n";
    const char* STRING_EXIT = "\n" ANSI_FORMAT_BOLD ANSI_COLOR_CYAN "> " ANSI_COLOR_WHITE "Back to menu";
    const char* CONTROLS[] = {
        ANSI_COLOR_WHITE "W" ANSI_COLOR_GRAY_DK " - " ANSI_COLOR_GRAY_LG "Move up\n",
        ANSI_COLOR_WHITE "A" ANSI_COLOR_GRAY_DK " - " ANSI_COLOR_GRAY_LG "Move left\n",
        ANSI_COLOR_WHITE "S" ANSI_COLOR_GRAY_DK " - " ANSI_COLOR_GRAY_LG "Move down\n",
        ANSI_COLOR_WHITE "D" ANSI_COLOR_GRAY_DK " - " ANSI_COLOR_GRAY_LG "Move right\n",
        ANSI_COLOR_WHITE "Q" ANSI_COLOR_GRAY_DK " - " ANSI_COLOR_GRAY_LG "Uncover cell\n",
        ANSI_COLOR_WHITE "E" ANSI_COLOR_GRAY_DK " - " ANSI_COLOR_GRAY_LG "Place/remove flag\n",
    };

    int print_length = 1 + strlen(STRING_TITLE) + strlen(STRING_EXIT);

    for(int i = 0; i < 6; i++) {
        print_length += strlen(CONTROLS[i]);
    }

    char* controls_string = malloc(print_length);
    controls_string[0] = '\0';

    strcat(controls_string, STRING_TITLE);

    for(int i = 0; i < 6; i++) {
        strcat(controls_string, CONTROLS[i]);
    }

    strcat(controls_string, STRING_EXIT);

    write(STDOUT_FILENO, controls_string, print_length);
    free(controls_string);

    char command = '\0';
    do {
         command = _getch();
    }
    while(command != KEY_ENTER && command != KEY_SPACEBAR);

    game_state = GAME_STATE.MAIN_MENU;
}

inline void handle_settings() {
    char command;

    print_settings();

    do{
        fflush(stdin);
        command = _getch();
    }
    while(command != KEY_ENTER && command != KEY_SPACEBAR && command != ENGINE_COMMANDS.KEY_UP && command != ENGINE_COMMANDS.KEY_DOWN);

    system("@cls||clear");

    switch(command) {
        case KEY_UP:
            if (selected_setting > 0)
                --selected_setting;
        break;
        case KEY_DOWN:
            if (selected_setting < SETTING_NUMBER - 1)
                ++selected_setting;
        break;
        default:
        case KEY_SPACEBAR:
        case KEY_ENTER:
            select_setting();
        break;
    }

    system("@cls||clear");
}

inline void select_setting() {
    switch (selected_setting) {
        default:
        case 3:
            game_state = GAME_STATE.MAIN_MENU;
            break;
    }
}

inline void print_settings() {
    const char* STRING_TITLE = ANSI_FORMAT_BOLD ANSI_COLOR_RED "GAME SETTINGS\n\n";
    const char* SELECTED_INDICATOR = ANSI_FORMAT_BOLD ANSI_COLOR_CYAN "> " ANSI_COLOR_WHITE;
    const char* EDIT_TOOLTIP = ANSI_RESET ANSI_COLOR_GRAY_DK " (edit)\n";

    char row_count_string[3];
    itoa(row_count, row_count_string, 10);

    char col_count_string[3];
    itoa(row_count, col_count_string, 10);

    char mine_count_string[3];
    itoa(row_count, mine_count_string, 10);

    const char* SETTINGS[] = {
        "Rows count: " ANSI_RESET,
        "Columns count: " ANSI_RESET,
        "Mine count: " ANSI_RESET,
        "Back to menu" ANSI_RESET
    };

    int string_length = strlen(STRING_TITLE) +
                        strlen(SELECTED_INDICATOR) +
                        strlen(row_count_string) +
                        strlen(col_count_string) +
                        strlen(mine_count_string) +
                        strlen(ANSI_COLOR_GRAY_LG) * 3 +
                        (strlen(ANSI_COLOR_GRAY_DK) + strlen(ANSI_COLOR_GRAY_LG) + 2) * 3;

     if (selected_setting != SETTING_NUMBER - 1) {
         string_length += strlen(EDIT_TOOLTIP);
     }

    for(int i = 0; i < SETTING_NUMBER; i++) {
        string_length += strlen(SETTINGS[i]) + 1;
    }

    char* settings_string = malloc(string_length);
    settings_string[0] = '\0';
    strcat(settings_string, STRING_TITLE);

    for(int i = 0; i < SETTING_NUMBER; i++) {
        if (selected_setting == i) {
            strcat(settings_string, SELECTED_INDICATOR);
        }
        else {
            strcat(settings_string, ANSI_COLOR_GRAY_DK);
            strcat(settings_string, (char[]){254, ' ', '\0'});
            strcat(settings_string, ANSI_COLOR_GRAY_LG);
        }

        strcat(settings_string, SETTINGS[i]);

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

        if (i != SETTING_NUMBER - 1 && selected_setting == i) {
            strcat(settings_string, EDIT_TOOLTIP);
        }
        else strcat(settings_string, "\n");
    }

    write(STDOUT_FILENO, settings_string, string_length);

    free(settings_string);
}

inline void print_menu() {
    const char* STRING_TITLE = ANSI_FORMAT_BOLD ANSI_COLOR_RED "MAIN MENU";
    const char* TOOLTIP = ANSI_RESET ANSI_COLOR_GRAY_LG " [Use W and S]\n\n" ANSI_RESET;
    const char* SELECTED_INDICATOR = ANSI_FORMAT_BOLD ANSI_COLOR_CYAN "> " ANSI_COLOR_WHITE;

    int string_length = 1 + strlen(STRING_TITLE) +
                        strlen(TOOLTIP) +
                        strlen(SELECTED_INDICATOR) +
                        (strlen(ANSI_COLOR_GRAY_DK) + strlen(ANSI_COLOR_GRAY_LG) + 2) * 3;

    for(int i = 0; i < OPTIONS_NUMBER; i++) {
        string_length += strlen(MENU_OPTIONS[i]);
    }

    char *menu_string = malloc(string_length);
    menu_string[0] = '\0';
    strcat(menu_string, STRING_TITLE);
    strcat(menu_string, TOOLTIP);


    for (int i = 0; i < OPTIONS_NUMBER; i++) {
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

#endif //MENU_H
