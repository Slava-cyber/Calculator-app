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
    GtkWidget *fixedGraph;
    GtkWidget *drawing_area;
    GtkWidget *windowGraph;
    GtkWidget *scaleUp;
    GtkWidget *scaleDown;

    char str[300] = "\0";
    char str2[300] = "\0";
    int point = 0;
    int x_status;
    double x1[2001];
    double y2[2001];
    double scale = 100;

void scaleUp_clicked(GtkWidget *button, gpointer *data) {
    if (scale <= 100000)
        scale = scale * 10;
    form_x_points(scale, x1);
    graph_build(str, &point, x1, y2);
}

void scaleDown_clicked(GtkWidget *button, gpointer *data) {
    if (scale >= 1e-1)
        scale /= 10;
    form_x_points(scale, x1);
    graph_build(str, &point, x1, y2);
}

double scale_y(int height) {
    double max = 0;
    for (int i = 0; i < numberpoints; i++) {
        if (!isnan(y2[i]) && !isinf(y2[i])) {
            max = fabs(y2[i]);
            break;
        }
    }
    for (int i = 0; i < numberpoints; i++) {
        if (fabs(y2[i]) > max && !isnan(y2[i]) && !isinf(y2[i]))
            max = fabs(y2[i]);
    }
    return max;
}

char *push_char(char *first, char *second, int *point) {
    while (*second != '\0' && *point < 300) {
        first[*point] = *second;
        *point += 1;
        second++;
    }
    first[*point] = '\0';
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
            if (str[*point - 1] == ')' || str[*point - 1] == 'x') {
                buffer[0] = '*';
                push_char(str, buffer, point);
            }
            if (value[0] == 'x' && str[*point - 1] >= '0' && str[*point - 1] <= '9') {
                buffer[0] = '*';
                push_char(str, buffer, point);
            }
        }
        push_char(str, value, point);
    } else if (value[0] == '.') {
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
            str[*point - 1] != '/' && str[*point - 1] != '^' && str[*point - 1] != '%' &&
            str[*point - 1] != '(') {
            buffer[0] = '*';
            push_char(str, buffer, point);
        }
    }
    push_char(str, value, point);
    buffer[0] = '(';
    push_char(str, buffer, point);
}

void draw_axes(cairo_t *cr, int width, int height, int x_shift, int y_shift) {
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 1);
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
    double x_step = x_area * 1.0 / numberpoints;
    double x_start = x_shift;
    double y_step = (height - y_shift) / (2 * scale_y(height));
    cairo_set_source_rgb(cr, 1, 0, 0);
    cairo_set_line_width(cr, 0.5);
    int line = 0;
    for (int i = 0; i < numberpoints; i++) {
        double y = (height - y_shift) / 2 - y2[i] / (x_step / 7);
        // g_print("i:%d-x:%f-y:%f\n", i, x_start, y2[i]);
        if (isnan(y2[i]) == 0 && isinf(y2[i]) == 0 /* && y < 550 && y > 0 */) {
            if (line == 0) {
                cairo_move_to(cr, x_start, (height - y_shift) / 2 - y2[i] * y_step);
                // cairo_move_to(cr, x_start, y);
                line = 1;
            } else {
                cairo_move_to(cr, x_start, (height - y_shift) / 2 - y2[i] * y_step);
                // cairo_move_to(cr, x_start, y);
                cairo_line_to(cr, x_start - x_step, (height - y_shift) / 2 - y2[i - 1] * y_step);
                // cairo_line_to(cr, x_start - x_step, (height - y_shift) / 2 - y2[i - 1] / (x_step / 7));
            }
        } else {
            line = 0;
        }
        x_start += x_step;
    }
    cairo_stroke(cr);
}

void draw_value(cairo_t *cr, int width, int height, int x_shift, int y_shift) {
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 0.25);
    double y_step = (height - y_shift) / 10;
    double y_start = height - y_shift;
    double x_step = (width - x_shift) / 10;
    double x_start = x_shift;
    double scaleY = scale_y(height);
    double scaleYstep = 2 * scaleY / 10;
    double scaleYstart = (-1) * scale_y(height);
    double scaleXstep = 2 * scale / 10;
    double scaleXstart = (-1) * scale;

    char textY[10][20], textX[10][20];
    for (int i = 0; i < 10; i++) {
        snprintf(textY[i], sizeof(textY[i]), "%.2e", scaleYstart);
        scaleYstart += scaleYstep;
        snprintf(textX[i], sizeof(textX[i]), "%.2f", scaleXstart);
        scaleXstart += scaleXstep;
    }

    for (int i = 0; i < 10; i++) {
        cairo_move_to(cr, x_shift - 5, y_start);
        cairo_line_to(cr, width, y_start);
        cairo_move_to(cr, 0, y_start);
        cairo_show_text(cr, textY[i]);
        y_start -= y_step;
        cairo_move_to(cr, x_start, height - y_shift + 5);
        cairo_line_to(cr, x_start, 0);
        cairo_move_to(cr, x_start, height - y_shift + 20);
        cairo_show_text(cr, textX[i]);
        x_start += x_step;
    }
    cairo_stroke(cr);
}

void on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data) {
    int width = 1200, height = 700, x_shift = 50, y_shift = 150;
    draw_value(cr, width, height, x_shift, y_shift);
    draw_axes(cr, width, height, x_shift, y_shift);
    draw_graph(cr, width, height, x_shift, y_shift);
}

void graph() {
        int width = 1200, height = 700;
        // создаем новое окно
        windowGraph = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(windowGraph), "Graph");
        gtk_window_set_position(GTK_WINDOW(windowGraph), GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(windowGraph), width, height);
        gtk_window_set_modal(GTK_WINDOW(windowGraph), TRUE);
        // g_signal_connect(G_OBJECT(windowGraph), "closed", G_CALLBACK(gtk_main_quit), NULL);

        // create fixed
        fixedGraph = gtk_fixed_new();
        gtk_container_add(GTK_CONTAINER(windowGraph), fixedGraph);

        /* создать виджет - область для рисования   */
        drawing_area = gtk_drawing_area_new();
        gtk_widget_set_size_request(drawing_area, width, height - 100);
        gtk_fixed_put(GTK_FIXED(fixedGraph), drawing_area, 0, 0);
        g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw_event), NULL);

        // button ScaleUp
        scaleUp = gtk_button_new_with_label("scale+");
        g_signal_connect(G_OBJECT(scaleUp), "clicked", G_CALLBACK(scaleUp_clicked), NULL);
        gtk_widget_set_size_request(scaleUp, width/2, 100);
        gtk_fixed_put(GTK_FIXED(fixedGraph), scaleUp, 0, height - 100);

        // button ScaleDown
        scaleDown = gtk_button_new_with_label("scale-");
        g_signal_connect(G_OBJECT(scaleDown), "clicked", G_CALLBACK(scaleDown_clicked), NULL);
        gtk_widget_set_size_request(scaleDown, width/2, 100);
        gtk_fixed_put(GTK_FIXED(fixedGraph), scaleDown, width/2, height - 100);

        gtk_widget_show_all(windowGraph);
}

void form_x_points(double scale, double *x) {
    double shift = 2 * scale / (numberpoints - 1);
    double buf;
    // g_print("scale:%f\n", scale);
    double start = scale*(-1) - shift;
    // g_print("start:%f\n", start);
    for (int i = 0; i < numberpoints; i++) {
        start += shift;
        x[i] = start;
        x[i] = modf(x[i] * 10e5, &buf);
        x[i] = buf / 10e5;
    }
}

void button_clicked(GtkWidget *button) {
    char value[5] = "\0\0\0\0\0";
    // strcpy(value, gtk_button_get_label((GtkButton*)button));
    snprintf(value, sizeof(value), "%s", gtk_button_get_label((GtkButton*)button));
    if (strcmp(value, "=") == 0) {
        if (strlen(str) != 0) {
            if (check_graph(str) == 0) {
                run(str, str2, &point);
                str_zero(str2);
                x_status = 0;
            } else if (x_status == 0) {
                str_zero(str2);
                // strcpy(str2, str);
                snprintf(str2, sizeof(str2), "%s", str);
                str_zero(str);
                point = 0;
                x_status += 1;
            } else {
                str_zero(str);
                // strcpy(str, "error");
                snprintf(str, sizeof(str), "%s", "error");
                point = 5;
                str_zero(str2);
                x_status = 0;
            }
        }
    } else if (strcmp(value, "f(x)") == 0) {
        form_x_points(scale, x1);
        if (strlen(str) != 0 && !graph_build(str, &point, x1, y2))
            graph();
    } else if (strlen(value) == 1) {
        one_char_operation(str, value, &point);
    } else if (strlen(value) == 2 && strcmp(value, "AC") == 0) {
        delete_char(str, &point);
    } else if (strlen(value) == 2 && strcmp(value, "EC") == 0) {
        str_zero(str);
        point = 0;
    } else {
        many_char_operation(str, value, &point);
    }
    gtk_label_set_label((GtkLabel*)label, str);
}

void create_digit_button(GtkWidget **buttonDigits, int width, int height,
                         int SizeButton, int SizeSpace, GtkWidget *fixed) {
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

void create_function_button(GtkWidget **buttonFunctions, int width, int height,
                            int SizeButton, int SizeSpace, GtkWidget *fixed) {
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

void create_operation_button(GtkWidget **buttonOperations, int width, int height,
                             int SizeButton, int SizeSpace, GtkWidget *fixed) {
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

void create_brackets_button(GtkWidget **buttonBrackets, int width, int height,
                            int SizeButton, int SizeSpace, GtkWidget *fixed) {
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

void create_delete_button(GtkWidget *buttonDelete, int width, int height,
                          int SizeButton, int SizeSpace, GtkWidget *fixed) {
    int positionX = width -  SizeButton;
    int positionY = height - 5 * SizeButton - 4 * SizeSpace;
    buttonDelete = gtk_button_new_with_label("AC");
    g_signal_connect(G_OBJECT(buttonDelete), "clicked", G_CALLBACK(button_clicked), NULL);
    gtk_widget_set_size_request(buttonDelete, SizeButton, SizeButton);
    gtk_fixed_put(GTK_FIXED(fixed), buttonDelete, positionX, positionY);
}

void create_deleteAll_button(GtkWidget *buttonDelete, int width, int height,
                             int SizeButton, int SizeSpace, GtkWidget *fixed) {
    int positionX = width -  2 * SizeButton - SizeSpace;
    int positionY = height - 5 * SizeButton - 4 * SizeSpace;
    buttonDelete = gtk_button_new_with_label("EC");
    g_signal_connect(G_OBJECT(buttonDelete), "clicked", G_CALLBACK(button_clicked), NULL);
    gtk_widget_set_size_request(buttonDelete, SizeButton, SizeButton);
    gtk_fixed_put(GTK_FIXED(fixed), buttonDelete, positionX, positionY);
}

void create_equal_button(GtkWidget *buttonEqual, int width, int height,
                         int SizeButton, int SizeSpace, GtkWidget *fixed) {
    int positionX = width - 2 * SizeButton - SizeSpace;
    int positionY = height - SizeButton;
    buttonEqual = gtk_button_new_with_label("=");
    g_signal_connect(G_OBJECT(buttonEqual), "clicked", G_CALLBACK(button_clicked), NULL);
    gtk_widget_set_size_request(buttonEqual, SizeButton, SizeButton);
    gtk_fixed_put(GTK_FIXED(fixed), buttonEqual, positionX, positionY);
}

void create_x_button(GtkWidget *buttonX, int width, int height, int SizeButton,
                     int SizeSpace, GtkWidget *fixed) {
    int positionX = width - 5 * SizeButton - 4* SizeSpace;
    int positionY = height - 4 * SizeButton - 3 * SizeSpace;
    buttonX = gtk_button_new_with_label("x");
    g_signal_connect(G_OBJECT(buttonX), "clicked", G_CALLBACK(button_clicked), NULL);
    gtk_widget_set_size_request(buttonX, SizeButton, SizeButton);
    gtk_fixed_put(GTK_FIXED(fixed), buttonX, positionX, positionY);
}

void create_empty_button(GtkWidget *buttonEmpty, int width, int height,
                         int SizeButton, int SizeSpace, GtkWidget *fixed) {
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
}
