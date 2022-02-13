#ifndef SRC_CALC_H_
#define SRC_CALC_H_

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <string.h>

#include <gtk/gtk.h>
#include <cairo.h>

#define numberpoints 2001

typedef enum {
    value = -2,
    x = -1,
    bracket_left = 0,
    bracket_right = 1,
    additional = 2,
    subtraction = 3,
    multiplication = 4,
    division = 5,
    power = 6,
    modulus = 7,
    unary_plus = 8,
    unary_minus = 9,
    cosine = 10,
    sine = 11,
    tangent = 12,
    arc_cosine = 13,
    arc_sine = 14,
    arc_tangent = 15,
    square = 16,
    natural_logarithm = 17,
    common_logarithm = 18
} value_type_t;

typedef struct stack {
    double value;
    value_type_t operation;
    struct stack *next;
} stack;

    // calc.c
void push(stack **head, double number, value_type_t oper);
stack* pop(stack **head);
double peek_value(const stack *head);
value_type_t peek_operation(const stack *head);
stack* reverse_stack(stack *first);

double form_number(char **str, double *number);
int prior(value_type_t operation);
int digit(char c);
int operation(char c, value_type_t *operation);
int symbol(char c);
int function(char *str);
double form_function(char **str, value_type_t *func);
int str_zero(char *str);

double run(char *str, char *str2, int *point);
int graph_build(char *str, int *point, double *x, double *y);

stack* parsing(char *str, int *error);
int calculate(stack* notation, double *result, double x_value);
double action_two_arguments(stack **value, value_type_t operation, int *error);
double action_one_arguments(stack **value, value_type_t operation, int *error);

    // view.c
void init(int argc, char *argv[]);
int check_graph(char *str);
void scaleUp_clicked(GtkWidget *button, gpointer *data);
void scaleDown_clicked(GtkWidget *button, gpointer *data);
double scale_y(int height);
char *push_char(char *first, char *second, int *point);
char *delete_char(char *first, int *point);
void one_char_operation(char *str, char *value, int *point);
void draw_axes(cairo_t *cr, int width, int height, int x_shift, int y_shift);
void draw_graph(cairo_t *cr, int width, int height, int x_shift, int y_shift);
void on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data);
void graph();
void form_x_points(double scale, double *x);
void button_clicked(GtkWidget *button);
void create_digit_button(GtkWidget **buttonDigits, int width, int height,
                         int SizeButton, int SizeSpace, GtkWidget *fixed);
void create_function_button(GtkWidget **buttonFunctions, int width, int height,
                            int SizeButton, int SizeSpace, GtkWidget *fixed);
void create_operation_button(GtkWidget **buttonOperations, int width, int height,
                             int SizeButton, int SizeSpace, GtkWidget *fixed);
void create_brackets_button(GtkWidget **buttonBrackets, int width, int height,
                            int SizeButton, int SizeSpace, GtkWidget *fixed);
void create_delete_button(GtkWidget *buttonDelete, int width, int height,
                          int SizeButton, int SizeSpace, GtkWidget *fixed);
void create_deleteAll_button(GtkWidget *buttonDelete, int width, int height,
                             int SizeButton, int SizeSpace, GtkWidget *fixed);
void create_equal_button(GtkWidget *buttonEqual, int width, int height,
                         int SizeButton, int SizeSpace, GtkWidget *fixed);
void create_x_button(GtkWidget *buttonX, int width, int height, int SizeButton,
                     int SizeSpace, GtkWidget *fixed);
void create_empty_button(GtkWidget *buttonEmpty, int width, int height,
                         int SizeButton, int SizeSpace, GtkWidget *fixed);
void init(int argc, char *argv[]);
#endif  // SRC_CALC_H_
