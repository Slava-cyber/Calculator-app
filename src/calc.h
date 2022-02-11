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
    //int operation;
    struct stack *next;
} stack;

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

int run(char *str, int *point);
int str_zero(char *str);

int graph_build(char *str, int *point, double *x, double *y);

stack* parsing(char *str, int *error);
int calculate(stack* notation, double *result, double x_value);

double action_two_arguments(stack **value, value_type_t operation, int *error);
double action_one_arguments(stack **value, value_type_t operation, int *error);

int input(char * str);


    // view
void init(int argc, char *argv[]);
int check_graph(char *str);


#endif  // SRC_CALC_H