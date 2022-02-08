#include "calc.h"

void create_digit_button(GtkWidget **buttonDigits, int positionX, int positionY, GtkWidget *fixed) {
    char* title[12] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ",", ""};
    GtkWidget *button;
    for (int i = 0; i < 12; i++) {
        button = gtk_button_new_with_label(title[i]);
        gtk_widget_set_size_request(button, 50, 50);
        gtk_fixed_put(GTK_FIXED(fixed), button, positionX, positionY);
        buttonDigits[i] = button;
        positionX += 54;
    }
    //return 1;
}

void init(int argc, char *argv[]) {
    int positionX = 0;
    int positionY = 0;
    GtkWidget *window;
    GtkWidget *fixed;
    GtkWidget *buttonDigits[12];
    
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Calculator");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 300);

    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);

    create_digit_button(buttonDigits, positionX, positionY, fixed);
    //GtkWidget *button = gtk_button_new_with_label(text);
    
    gtk_widget_show_all(window);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();
    //return 0;
}