#ifndef MENU_H
#define MENU_H
#define KEY_ENTER 13
#define KEY_SPACEBAR 32
#define MAX_COL_ROW_COUNT 20

#include <unistd.h>

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
void change_value();

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
        case 0:
            game_state = GAME_STATE.PLAYING;
            break;
        case 1:
            selected_setting = 0;
            game_state =  GAME_STATE.OPTIONS;
            break;
        case 2:
            show_controls();
            break;
        default:
        case 3:
            game_state = GAME_STATE.QUIT;
            break;
    }
}

inline void change_value() {
    const char* PROMPT = ANSI_FORMAT_BOLD ANSI_COLOR_GRAY_LG "Insert new value ";
    char *title;
    int max_value;
    int user_value;
    char* range_string = malloc(strlen(ANSI_COLOR_GRAY_DK) + 5 + 1);
    const char* RANGE_STRING_END = "]" ANSI_COLOR_GRAY_LG ": " ANSI_RESET ANSI_COLOR_WHITE;
    range_string[0] = '\0';
    strcat(range_string, ANSI_COLOR_GRAY_DK "[0 - ");

    switch (selected_setting) {
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

    if (selected_setting == 2) {
        max_value = row_count * col_count;
    }
    else max_value = MAX_COL_ROW_COUNT;

    char range_string_value[3];
    itoa(max_value, range_string_value, 10);
    range_string = realloc(range_string, strlen(range_string) + 3 + strlen(RANGE_STRING_END));
    strcat(range_string, range_string_value);
    strcat(range_string, RANGE_STRING_END);

    char *print_string = malloc(strlen(title) + strlen(PROMPT) + strlen(range_string));
    print_string[0] = '\0';

    strcat(print_string, title);
    strcat(print_string, PROMPT);
    strcat(print_string, range_string);

    do {
        write(STDOUT_FILENO, print_string, strlen(print_string));
        scanf("%d", &user_value);
    }while (user_value < 0 || user_value > max_value);

    switch (selected_setting) {
        case 0:
            row_count = user_value;
            break;
        case 1:
            col_count = user_value;
            break;
        case 2:
            mine_count = user_value;
            break;
        default:
    }


    if (mine_count > row_count * col_count) {
        mine_count = row_count * col_count;
    }

    free(range_string);
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
        ANSI_COLOR_WHITE "Space" ANSI_COLOR_GRAY_DK " - " ANSI_COLOR_GRAY_LG "Place/remove flag\n",
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
        case 0:
        case 1:
        case 2:
            change_value();
        break;
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
    itoa(col_count, col_count_string, 10);

    char mine_count_string[3];
    itoa(mine_count, mine_count_string, 10);

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
