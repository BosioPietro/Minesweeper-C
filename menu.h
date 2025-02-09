#ifndef MENU_H
#define MENU_H
#define KEY_ENTER 13
#define KEY_SPACEBAR 32

//
// SIGNATURES
//
void handle_menu();
void select_option();
void handle_controls();
void handle_settings();
void change_value();
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

const int SETTING_NUMBER = 4;
short int selected_option = 0;
short int selected_setting = 0;

//
// FUNCTIONS
//
inline void handle_menu() {
    char command;

    show_menu(OPTIONS_NUMBER, MENU_OPTIONS, selected_option);

    do{
        fflush(stdin);
        command = _getch();
    }
    // L'utente può solo muoversi in alto e in basso e selezionare
    while(command != KEY_ENTER && command != KEY_SPACEBAR && command != ENGINE_COMMANDS.KEY_UP && command != ENGINE_COMMANDS.KEY_DOWN);

    system("@cls||clear");

    switch(command) {
        // Movimento
        case KEY_UP:
            if (selected_option > 0)
                --selected_option;
            break;
        case KEY_DOWN:
            if (selected_option < OPTIONS_NUMBER - 1)
                ++selected_option;
            break;

        // Selezione
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
            // show_contols non ha il suo stato
            // perchè è una semplice pagina dalla quale
            // si può solo tornare indetro
            handle_controls();
            break;
        default:
        case 3:
            game_state = GAME_STATE.QUIT;
            break;
    }
}

inline void change_value() {
    int user_value, max_value;

    // Se si stanno cambiando le mine,
    // non si possono avere più mine che celle
    if (selected_setting == 2) {
        max_value = row_count * col_count;
    }
    else max_value = MAX_COL_ROW_COUNT;

    do {
        system("@cls||clear");
        show_change_setting(selected_setting);
        fflush(stdin);
        scanf("%d", &user_value);
    }while (user_value < 0 || user_value > max_value);

    // Setto il valore alla variabile che si
    // è deciso di editare
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

    // Se si diminuisce il numero di righe o
    // colonne, controllo sempre che le mine non
    // siano di più del numero di celle
    if (mine_count > row_count * col_count) {
        mine_count = row_count * col_count;
    }
}

inline void handle_controls() {
    // Non si può fare niente nei controlli se non uscire
    char command = '\0';

    show_controls();
    do {
         command = _getch();
    }
    while(command != KEY_ENTER && command != KEY_SPACEBAR);

    game_state = GAME_STATE.MAIN_MENU;
}

inline void handle_settings() {
    char command;

    show_settings(SETTING_NUMBER, selected_setting);

    do{
        fflush(stdin);
        command = _getch();
    }
    // Permette di muoversi o selezionare un'opzione
    while(command != KEY_ENTER && command != KEY_SPACEBAR && command != ENGINE_COMMANDS.KEY_UP && command != ENGINE_COMMANDS.KEY_DOWN);

    system("@cls||clear");

    switch(command) {
        // Movimento
        case KEY_UP:
            if (selected_setting > 0)
                --selected_setting;
        break;
        case KEY_DOWN:
            if (selected_setting < SETTING_NUMBER - 1)
                ++selected_setting;
        break;

        // Selezione
        default:
        case KEY_SPACEBAR:
        case KEY_ENTER:
            if (selected_setting == 3) {
                game_state = GAME_STATE.MAIN_MENU;
            }
            else change_value();
        break;
    }

    system("@cls||clear");
}

#endif //MENU_H