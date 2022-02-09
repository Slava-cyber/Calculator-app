#include "calc.h"

    GtkWidget *label;
    GtkWidget *window;
    GtkWidget *fixed;
    GtkWidget *buttonDigits[11];
    GtkWidget *buttonEqual;
    GtkWidget *buttonFunctions[9];
    GtkWidget *buttonBrackets[2];
    GtkWidget *buttonOperations[6];
    GtkWidget *buttonDelete;
    GtkWidget *buttonDeleteAll;
    GtkWidget *buttonX;
    GtkWidget *buttonEmpty;
    char str[500] = "\0";
    int point = 0;


char *push_char(char *first, char *second, int *point) {
    while(*second != '\0' && *point < 500) {
        first[*point] = *second;
        *point += 1;
        second++;
    }
    first[*point] = '\0';
    // if (*point == 500)
    //     *error = 1;
    return first;
}

char *delete_char(char *first, int *point) {
    if (strlen(first) != 0 && *point != 0) {
        *point -= 1;
        first[*point] = '\0';
    }
    return first;
}

/*void delete_clicked(GtkWidget *button) {
    delete_char(str, &point);
    gtk_label_set_label((GtkLabel*)label, str);
}*/

/*void digits_clicked(GtkWidget *button) {
    //char *value = (char*)malloc(2 * sizeof(char));
    //gtk_button_get_label((GtkButton*)button);
    char value[5];
    strcpy(value, gtk_button_get_label((GtkButton*)button));
    value[2] = '\0';
    g_print("value:%s", value);
    g_print("strlen:%d", strlen(value));
    new_string(str, value, &point);
    gtk_label_set_label((GtkLabel*)label, str);
    g_print("new:%s", str);
}*/

void one_char_operation(char *str, char *value, int *point) {
    char buffer[2] = "\0\0";
    if ((value[0] >= '0' && value[0] <= '9') || value[0] == 'x') {
        if (*point != 0) {
            if (str[*point - 1] == ')') {
                buffer[0] = '*';
                push_char(str, buffer, point);
            }
        }
        push_char(str, value, point);
    } else if (value[0] == ',') {
        g_print(",!");
        if (*point != 0) {
            if (str[*point - 1] >= '0' && str[*point - 1] <= '9')
                push_char(str, value, point);
        }
    } else if (value[0] == '(' || value[0] == ')') {
        push_char(str, value, point);
    } else if (value[0] == '+' || value[0] == '-' || value[0] == '*' ||
               value[0] == '/' || value[0] == '^' || value[0] == '%') {
        if (*point != 0) {
            if (str[*point - 1] == '+' || str[*point - 1] == '-' || str[*point - 1] == '*' ||
                str[*point - 1] == '/' || str[*point - 1] == '^' || str[*point - 1] == '%')
                    delete_char(str, point);
            push_char(str, value, point);
        }
    }
}

void many_char_operation(char *str, char *value, int *point) {
    char buffer[2] = "\0\0";
    if (*point != 0) {
        if (str[*point - 1] != '+' && str[*point - 1] != '-' && str[*point - 1] != '*' &&
            str[*point - 1] != '/' && str[*point - 1] != '^' && str[*point - 1] != '%' && str[*point - 1] != '(') {
            buffer[0] = '*';
            push_char(str, buffer, point);
        }
    }
    push_char(str, value, point);
    buffer[0] = '(';
    push_char(str, buffer, point);
}

void button_clicked(GtkWidget *button) {
    char value[5] = "\0\0\0\0\0";
    strcpy(value, gtk_button_get_label((GtkButton*)button));
    if (strcmp(value, "=") == 0) {
        run(str);
    } else if (strlen(value) == 1) {
        g_print("digits\n");
        one_char_operation(str, value, &point);
    } else if (strlen(value) == 2 && strcmp(value, "AC") == 0) {
        delete_char(str, &point);
    } else if (strlen(value) == 2 && strcmp(value, "EC") == 0) {
        //str_zero(str);
    } else {
        many_char_operation(str, value, &point);
    }
    gtk_label_set_label((GtkLabel*)label, str);
}


void create_digit_button(GtkWidget **buttonDigits, int width, int height, int SizeButton, int SizeSpace, GtkWidget *fixed) {
    char* title[11] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", ","};
    int positionX = width - 4 * SizeButton - 3 * SizeSpace;
    int positionY = height - 4 * SizeButton - 3 * SizeSpace;
    GtkWidget *button;
    for (int i = 0; i < 11; i++) {
        button = gtk_button_new_with_label(title[i]);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(button_clicked), NULL);
        gtk_widget_set_size_request(button, SizeButton, SizeButton);
        gtk_fixed_put(GTK_FIXED(fixed), button, positionX, positionY);
        buttonDigits[i] = button;
        positionX += SizeButton + SizeSpace;
        if ((i + 1) % 3 == 0) {
            positionY += SizeButton + SizeSpace;
            positionX = width - 4 * SizeButton - 3 * SizeSpace;
        }
    }
}

void create_function_button(GtkWidget **buttonFunctions, int width, int height, int SizeButton, int SizeSpace, GtkWidget *fixed) {
    char* title[9] = {"cos", "sin", "tan", "acos", "asin", "atan", "sqrt", "ln", "log"};
    int positionX = width - 7 * SizeButton - 6 * SizeSpace;
    int positionY = height - 3 * SizeButton - 2 * SizeSpace;
    GtkWidget *button;
    for (int i = 0; i < 9; i++) {
        button = gtk_button_new_with_label(title[i]);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(button_clicked), NULL);
        gtk_widget_set_size_request(button, SizeButton, SizeButton);
        gtk_fixed_put(GTK_FIXED(fixed), button, positionX, positionY);
        buttonFunctions[i] = button;
        positionX += SizeButton + SizeSpace;
        if ((i + 1) % 3 == 0) {
            positionY += SizeButton + SizeSpace;
            positionX = width - 7 * SizeButton - 6 * SizeSpace;
        }
    }
}

void create_operation_button(GtkWidget **buttonOperations, int width, int height, int SizeButton, int SizeSpace, GtkWidget *fixed) {
    char* title[6] = {"^", "%", "/", "*", "+", "-"};
    int positionX = width - 4 * SizeButton - 3 * SizeSpace;
    int positionY = height - 5 * SizeButton - 4 * SizeSpace;
    GtkWidget *button;
    for (int i = 0; i < 6; i++) {
        button = gtk_button_new_with_label(title[i]);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(button_clicked), NULL);
        gtk_widget_set_size_request(button, SizeButton, SizeButton);
        gtk_fixed_put(GTK_FIXED(fixed), button, positionX, positionY);
        buttonOperations[i] = button;
        if (i < 2) 
            positionX += SizeButton + SizeSpace;
        if (i == 1) {
            positionX = width - SizeButton;
            positionY = height - 4 * SizeButton - 3 * SizeSpace; 
        }
        if (i > 1)
            positionY += SizeButton + SizeSpace;
    }
}

void create_brackets_button(GtkWidget **buttonBrackets, int width, int height, int SizeButton, int SizeSpace, GtkWidget *fixed) {
    char* title[2] = {"(", ")"};
    int positionX = width - 7 * SizeButton - 6 * SizeSpace;
    int positionY = height - 4 * SizeButton - 3 * SizeSpace;
    GtkWidget *button;
    for (int i = 0; i < 2; i++) {
        button = gtk_button_new_with_label(title[i]);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(button_clicked), NULL);
        gtk_widget_set_size_request(button, SizeButton, SizeButton);
        gtk_fixed_put(GTK_FIXED(fixed), button, positionX, positionY);
        buttonBrackets[i] = button;
        positionX += SizeButton + SizeSpace;
    }
}

void create_delete_button(GtkWidget *buttonDelete, int width, int height, int SizeButton, int SizeSpace, GtkWidget *fixed) {
    int positionX = width -  SizeButton;
    int positionY = height - 5 * SizeButton - 4 * SizeSpace;
    buttonDelete = gtk_button_new_with_label("AC");
    g_signal_connect(G_OBJECT(buttonDelete), "clicked", G_CALLBACK(button_clicked), NULL);
    gtk_widget_set_size_request(buttonDelete, SizeButton, SizeButton);
    gtk_fixed_put(GTK_FIXED(fixed), buttonDelete, positionX, positionY);
}

void create_deleteAll_button(GtkWidget *buttonDelete, int width, int height, int SizeButton, int SizeSpace, GtkWidget *fixed) {
    int positionX = width -  2 * SizeButton - SizeSpace;
    int positionY = height - 5 * SizeButton - 4 * SizeSpace;
    buttonDelete = gtk_button_new_with_label("EC");
    g_signal_connect(G_OBJECT(buttonDelete), "clicked", G_CALLBACK(button_clicked), NULL);
    gtk_widget_set_size_request(buttonDelete, SizeButton, SizeButton);
    gtk_fixed_put(GTK_FIXED(fixed), buttonDelete, positionX, positionY);
}

void create_equal_button(GtkWidget *buttonEqual, int width, int height, int SizeButton, int SizeSpace, GtkWidget *fixed) {
    int positionX = width - 2 * SizeButton - SizeSpace;
    int positionY = height - SizeButton;
    buttonEqual = gtk_button_new_with_label("=");
    g_signal_connect(G_OBJECT(buttonEqual), "clicked", G_CALLBACK(button_clicked), NULL);
    gtk_widget_set_size_request(buttonEqual, SizeButton, SizeButton);
    gtk_fixed_put(GTK_FIXED(fixed), buttonEqual, positionX, positionY);
}

void create_x_button(GtkWidget *buttonX, int width, int height, int SizeButton, int SizeSpace, GtkWidget *fixed) {
    int positionX = width - 5 * SizeButton - 4* SizeSpace;
    int positionY = height - 4 * SizeButton - 3 * SizeSpace;
    buttonX = gtk_button_new_with_label("x");
    g_signal_connect(G_OBJECT(buttonX), "clicked", G_CALLBACK(button_clicked), NULL);
    gtk_widget_set_size_request(buttonX, SizeButton, SizeButton);
    gtk_fixed_put(GTK_FIXED(fixed), buttonX, positionX, positionY);
}

void create_empty_button(GtkWidget *buttonEmpty, int width, int height, int SizeButton, int SizeSpace, GtkWidget *fixed) {
    int positionX = width - 7 * SizeButton - 6 * SizeSpace;
    int positionY = height - 5 * SizeButton - 4 * SizeSpace;
    buttonEmpty = gtk_button_new_with_label(" ");
    gtk_widget_set_size_request(buttonEmpty, 3 * SizeButton + 2 * SizeSpace, SizeButton);
    gtk_fixed_put(GTK_FIXED(fixed), buttonEmpty, positionX, positionY);
}



void init(int argc, char *argv[]) {
    int width = 724, height = 700;
    int SizeButton = 100, SizeSpace = 4;
// create window    
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Calculator");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);
// create fixed
    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);
// create label
    label = gtk_label_new(str);
    gtk_widget_set_size_request(label, width, height - 5 * SizeButton - 5 * SizeSpace);
    gtk_fixed_put(GTK_FIXED(fixed), label, 0, 0);
// create buttons 
    create_digit_button(buttonDigits, width, height, SizeButton, SizeSpace, fixed);
    create_equal_button(buttonEqual, width, height, SizeButton, SizeSpace, fixed);
    create_function_button(buttonFunctions, width, height, SizeButton, SizeSpace, fixed);
    create_delete_button(buttonDelete, width, height, SizeButton, SizeSpace, fixed);
    create_deleteAll_button(buttonDeleteAll, width, height, SizeButton, SizeSpace, fixed);
    create_operation_button(buttonOperations, width, height, SizeButton, SizeSpace, fixed);
    create_brackets_button(buttonBrackets, width, height, SizeButton, SizeSpace, fixed);
    create_x_button(buttonX, width, height, SizeButton, SizeSpace, fixed);
    create_empty_button(buttonEmpty, width, height, SizeButton, SizeSpace, fixed);


    gtk_widget_show_all(window);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();
    //return 0;
}