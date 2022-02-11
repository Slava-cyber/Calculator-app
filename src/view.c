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
    
    // GtkWidget *drawing_area;
    // GtkWidget *windowGraph;
    
    char str[500] = "\0";
    char str2[500] = "\0";
    int point = 0;
    int x_status;
    double x1[500];
    double y2[500];

double scale_y(int height) {
    double min = y2[0];
    double max = y2[0];
    for (int i = 0; i < 500; i++) {
        if (y2[i] < min)
            min = y2[i];
        if (y2[i] > max)
            max = y2[i];
    }
    return max - min;
}

int check_graph(char *str) {
    int result = 0;
    int i = 0;
    while(str[i] != '\0') {
        if (str[i] == 'x')
            result = 1;
        i += 1;
    }
    return result;
}

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
    } else if (value[0] == '.') {
        //g_print("!");
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

// void on_draw_event (GtkWidget *widget, cairo_t *cr, gpointer data)
// {
//    /* Отрисовка неба */
//     cairo_set_source_rgb(cr, 0.0, 0.0, 1.0);
//     cairo_rectangle(cr, 0, 0, 700, 300);
//     cairo_fill(cr);
//     /* Отрисовка земли */
//     cairo_set_source_rgb(cr, 0.3, 0.3, 0.0);
//     cairo_rectangle(cr, 0, 300, 700, 200);
//     cairo_fill(cr);
 
//     /* Отрисовка солнца */
//     cairo_set_source_rgb(cr, 1, 1, 0.0);
//     cairo_arc(cr, 500, 0, 50, 0, M_PI);
//     cairo_fill(cr);
//     /* Отрисовка лучей */
//     cairo_move_to(cr, 500, 0);
//     cairo_line_to(cr, 400, 50);
//     cairo_move_to(cr, 500, 0);
//     cairo_line_to(cr, 500, 100);
//     cairo_move_to(cr, 500, 0);
//     cairo_line_to(cr, 600, 50);
//     cairo_stroke(cr);
// }


void draw_axes(cairo_t *cr, int width, int height, int x_shift, int y_shift) {
    // int x_area = width - x_shift;
    // int y_area = height - y_shift;

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 0.5);
    // оси координат    
    cairo_move_to(cr, x_shift, (height - y_shift) / 2);
    cairo_line_to(cr, width, (height - y_shift) / 2);
    cairo_move_to(cr, (width + x_shift) / 2, 0);
    cairo_line_to(cr, (width + x_shift) / 2, height - y_shift);
    // оси масштаба
    cairo_move_to(cr, x_shift, 0);
    cairo_line_to(cr, x_shift, height - y_shift);
    cairo_move_to(cr, x_shift, height - y_shift);
    cairo_line_to(cr, width, height - y_shift);

    cairo_stroke(cr);
}

void draw_graph(cairo_t *cr, int width, int height, int x_shift, int y_shift) {
    int x_area = width - x_shift;
    int y_area = height - y_shift;
    double x_step = x_area / 499.0;
    double x_start = x_shift;
     double y_step = (height - y_shift - 200) / scale_y(height);
    //double y_step = 200 / scale_y(height);
    g_print("y_step:%f\n", y_step);
    cairo_set_source_rgb(cr, 0, 0, 1);
    cairo_set_line_width(cr, 3);
    for (int i = 0; i < 500 - 1; i++) {
        
        g_print("i:%d-x:%f-y:%f\n", i, x_start, y2[i]);
        cairo_move_to(cr, x_start, (height - y_shift) / 2 - y2[i] * y_step);
        x_start += x_step;
        cairo_line_to(cr, x_start, (height - y_shift) / 2 - y2[i + 1] * y_step);
    }
    //g_print("width:%d-height:%d\n", width, height);
    cairo_stroke(cr);
}

void on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data) {
    int width = 1200, height = 700, x_shift = 50, y_shift = 50;
    draw_axes(cr, width, height, x_shift, y_shift);
    draw_graph(cr, width, height, x_shift, y_shift);
}

void graph() {
    int graph = check_graph(str);
    if (graph) {
        g_print("tutgraph\n");
        int width = 1200, height = 700;
        // создаем новое окно
        //gtk_init(&argc, &argv);
        GtkWidget *windowGraph;
        windowGraph = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(windowGraph), "Graph");
        gtk_window_set_position(GTK_WINDOW(windowGraph), GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(windowGraph), width, height);
        g_signal_connect(G_OBJECT(windowGraph), "destroy", G_CALLBACK(gtk_main_quit), NULL);
        /* создать виджет - область для рисования   */
        GtkWidget *drawing_area;
        drawing_area = gtk_drawing_area_new();
        gtk_container_add(GTK_CONTAINER(windowGraph), drawing_area);
        /* Событие отрисовки содержимого области */
        //         for (int i = 0; i < 500; i++) {
        //    g_print("xx:%f\nyy:%f\n", x1[i], y1[i]);
        // }
        g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw_event), NULL);

        gtk_widget_show_all(windowGraph);
        //gtk_main();
    }
}

void form_x_points(double scale, double *x) {
    double shift = 2 * scale / (500 - 1);
    g_print("scale:%f\n", scale);
    double start = scale*(-1) - shift;
    g_print("start:%f\n", start);
    for (int i = 0; i < 500; i++) {
        start += shift;
        x[i] = start;
        //g_print("xx:%f\n", x[i]);
    }
}

void button_clicked(GtkWidget *button) {
    char value[5] = "\0\0\0\0\0";
    strcpy(value, gtk_button_get_label((GtkButton*)button));
    if (strcmp(value, "=") == 0) {
        if (strlen(str) != 0) {
            if (check_graph(str) == 0) {
                g_print("str2:%s-str:%s\n", str2, str);
                run(str, str2, &point);
                str_zero(str2);
                x_status = 0;
            } else if (x_status == 0) {
                g_print("x1\n");
                str_zero(str2);
                strcpy(str2, str);
                str_zero(str);
                point = 0;
                x_status += 1;
            } else {
                str_zero(str);
                strcpy(str, "error");
                point = 5;
                str_zero(str2);
                x_status = 0;
            }
        }
    } else if (strcmp(value, "f(x)") == 0) {
        form_x_points(100, x1);
        // for (int i = 0; i < 500; i++) {
        //    //g_print("xx:%f\n", x[i]);
        // }
        //graph_build(str, &point, x, y);

        //for (int i = 0; i < 500; i++) {
           //g_print("xx:%f\nyy:%f\n", x[i], y[i]);
        //}
        if (!graph_build(str, &point, x1, y2))
            graph();
    } else if (strlen(value) == 1) {
        //g_print("digits\n");
        one_char_operation(str, value, &point);
    } else if (strlen(value) == 2 && strcmp(value, "AC") == 0) {
        delete_char(str, &point);
    } else if (strlen(value) == 2 && strcmp(value, "EC") == 0) {
        str_zero(str);
        point = 0;
    } else {
        many_char_operation(str, value, &point);
    }
    g_print("str:%s\n", str);
    g_print("strlen:%d\n", strlen(str));
    
    gtk_label_set_label((GtkLabel*)label, str);
}

void create_digit_button(GtkWidget **buttonDigits, int width, int height, int SizeButton, int SizeSpace, GtkWidget *fixed) {
    char* title[11] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "."};
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
    buttonEmpty = gtk_button_new_with_label("f(x)");
    g_signal_connect(G_OBJECT(buttonEmpty), "clicked", G_CALLBACK(button_clicked), NULL);
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
