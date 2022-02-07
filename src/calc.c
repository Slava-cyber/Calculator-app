#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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
int operation(char c, value_type_t *opearation);
int symbol(char c);
int function(char *str);
double form_function(char **str, value_type_t *func);


stack* parsing(char *str);


int input(char * str);

int main() {
    stack* result = NULL;
    stack* notation = NULL;
    double number = 0;
    char *str = (char*)malloc(100*sizeof(char));
    input(str);
    printf("str:%s\n", str);
    notation = parsing(str);
    printf("|||||||||||||||||\n");
    result = reverse_stack(notation);
    while (result != NULL) {
        printf("%.1f-%d||", peek_value(result), peek_operation(result));
        pop(&result);
    }

    return 0;
}

stack* reverse_stack(stack *first) {
    stack* result = NULL;
    while (first != NULL) {
        push(&result, peek_value(first), peek_operation(first));
        pop(&first);
    }
    return result;
}

stack* parsing(char *str) {
    int i = 0, error = 0;
    double number;
    stack* buffer = NULL;
    stack* notation = NULL;    
    char *tmp = str;
    value_type_t buffer_value_type;
    while (*tmp != '=' && *tmp != '\0' && !error) {
        printf("%d-%c\n", tmp, *(tmp));
        if (*tmp == ')') {
            while (peek_operation(buffer) != bracket_left && buffer != NULL) {
                printf("push:%0.1f-%d||", peek_value(buffer), peek_operation(buffer));
                push(&notation, peek_value(buffer), peek_operation(buffer));
                //printf("push:%0.1f-%d||", peek_value(buffer), peek_operation(buffer));
                pop(&buffer);
            }
            printf("h!!!!!!!!!!!!!s\n");
            if (buffer == NULL) {
                error = 1;
                break;
            }
            pop(&buffer);
            //printf("push:%0.1f-%d||", peek_value(buffer), peek_operation(buffer));
        } else if (digit(*tmp) > -1) {
            error = form_number(&tmp, &number);
            if (error)
                break;
            push(&notation, number, value);
            tmp--;
            printf("tmp:%s\n", tmp);
        } else if (*tmp == 'x') {
            push(&notation, 0.0, x);
        } else if (*tmp == '(') {
            push(&buffer, 0.0, bracket_left);
        } else if (*tmp == '+' && (!i || *(tmp-1) == '(')) {
            push(&buffer, 0.0, unary_plus);
        } else if (*tmp == '-' && (!i || *(tmp-1) == '(')) {
            printf("!!!\n");
            push(&buffer, 0.0, unary_minus);
        } else {
            printf("operation!\n");
            if (symbol(*tmp) && *tmp != 'x') {
                if (form_function(&tmp, &buffer_value_type)) {
                    error = 1;
                    break;
                } else {
                    //tmp -= 1;
                }
            } else if (operation(*tmp, &buffer_value_type) == 0) {
                error = 1;
                break;
            }
            printf("value:%d\n", buffer_value_type);
            if (buffer == NULL) {
                push(&buffer, 0.0, buffer_value_type);
            } else {
                if (prior(buffer->operation) < prior(buffer_value_type)) {
                    push(&buffer, 0.0, buffer_value_type);
                } else {
                    while (buffer != NULL && prior(buffer->operation) >= prior(buffer_value_type)) {
                        push(&notation, 0.0, peek_operation(buffer));
                        pop(&buffer);
                    }
                    push(&buffer, 0.0, buffer_value_type);
                }
            }
        }
        printf("error:%d\n", error);
        tmp++;
        i++;
        printf("%d-%c\n", tmp, *tmp);
        //printf("BUFFER:%d\n", peek_operation(buffer));
    }
    while (buffer != NULL) {
        push(&notation, peek_value(buffer), peek_operation(buffer));
        pop(&buffer);
    }
    return notation;
}

/*double calculate(stack* notation, int *error) {
    double result;
    stack 
    return result;
}*/



double form_number(char **str, double *number) {
    *number = 0;
    value_type_t buffer;
    int dig, error = 0;
    while ((dig = digit(**str)) > -1) {
        *number = 10 * *number + dig;
        *str += 1;
        //printf("%d", dig);
    }
    if (**str == ',') {
        *str += 1;
        if ((dig = digit(**str)) > -1) {
            int i = 0;
            while ((dig = digit(**str)) > -1) {
                i +=1;
                //printf("number:%f\n", dig / pow(10, i));
                //printf("%d", dig);
                double buf = dig / pow(10, i);
                *number += buf;
                //printf("number:%f\n", *number);
                *str += 1;
            }
        } else {
            error = 1;
        }
    } 
    //else if ((!operation(**str, &buffer) || **str == ')') && **str != '\0') {
    //    error = 1;
    //}
    //printf("number:%f\n", *number);
    if (!(operation(**str, &buffer) || **str == ')' || **str == '\0'))
        error = 1;
    return error;
}

int input(char * str) {
    char c;
    while (1) {
        scanf("%c", &c);
        if (c == '\n')
            break;
        *str = c;
        str++;
    }
    *str = '\0';
    return 1;
}

void push(stack **head, double number, value_type_t oper) {
    stack *st = malloc(sizeof(stack));
    if (st == NULL) {
        puts("memory fault");
        exit(-1);
        //exit(STACK_OVERFLOW);
    }
    st->next = *head;
    st->value = number;
    st->operation = oper;
    *head = st;
}

stack* pop(stack **head) {
    stack *out;
    if ((*head) == NULL)
        exit(-1);
    out = *head;
    *head = (*head)->next;
    return out;
}

double peek_value(const stack *head) {
    if (head == NULL)
        exit(-1);
    return head->value;
}

value_type_t peek_operation(const stack *head) {
    if (head == NULL)
        exit(-1);
    return head->operation;
}

int prior(value_type_t operation) {
    int flag = 0;
    if (operation == bracket_left) {
        flag = 0;
    } else if (operation == additional || operation == subtraction) {
        flag = 1;
    } else if (operation == multiplication || operation == division || operation == modulus)  {
        flag = 2;
    } else if (operation == power) {
        flag = 3;
    } else if (operation == unary_minus || operation == unary_plus) {
        flag = 4;
    } else {
        flag = 5;
    }
    return flag;
}

int digit(char c) {
    int result = -1;
    if (c >= '0' && c <= '9')
        result = c - 48;
    return result;
}

int operation(char c, value_type_t *opearation) {
    int result = 0;
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^')
        result = 1;
    if (c == '+')
        *opearation = 2;
    if (c == '-')
       *opearation = 3;
    if (c == '*')
        *opearation = 4;
    if (c == '/')
        *opearation = 5;
    if (c == '^')
        *opearation = 6;
    return result;
}

int symbol(char c) {
    int result = 0;
    if ((c >= 'a' && c <= 'z') || c == '(') {
        result = 1;
    }
    return result;
}
int function(char *str) {
    int result = 0;
    if (strcmp(str, "cos(") == 0)
        result = 10;
    if (strcmp(str, "sin(") == 0)
        result = 11;
    if (strcmp(str, "tan(") == 0)
        result = 12;
    if (strcmp(str, "acos(") == 0)
        result = 13;
    if (strcmp(str, "asin(") == 0)
        result = 14;
    if (strcmp(str, "atan(") == 0)
        result = 15;
    if (strcmp(str, "sqrt(") == 0)
        result = 16;
    if (strcmp(str, "log(") == 0)
        result = 17;
    if (strcmp(str, "ln(") == 0)
        result = 18;
    return result;
}

double form_function(char **str, value_type_t *func) {
    char *factor = (char*)malloc(10 * sizeof(char));
    int error = 0;
    while (symbol(**str)) {
        *factor = **str;
        factor++;
        if (**str == '(')
            break;
        *str += 1;
    }
    *factor = '\0';
    if (!(*func = function(factor)))
        error = 1;
    free(factor);
    return error;
}

