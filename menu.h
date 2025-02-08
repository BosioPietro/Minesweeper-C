#ifndef MENU_H
#define MENU_H

#include <unistd.h>

//
// SIGNATURES
//
void print_menu();
void handle_menu();
void select_option();
void show_controls();

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
    while(command != 13 && command != 119 && command != 115);

    system("@cls||clear");

    switch(command) {
        // W
        case 119:
            if (selected_option > 0)
                --selected_option;
            break;
        // S
        case 115:
            if (selected_option < OPTIONS_NUMBER - 1)
                ++selected_option;
            break;
        default:
        case 13:
            select_option();
            break;
    }

    system("@cls||clear");
}

inline void select_option() {
    switch(selected_option) {
        default:
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
    const char* STRING_EXIT = "\n" ANSI_FORMAT_BOLD ANSI_COLOR_CYAN "> " ANSI_COLOR_WHITE " Back to menu";
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

    while(_getch() != 13){}
    game_state = GAME_STATE.MAIN_MENU;
    free(controls_string);
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
