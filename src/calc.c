#include "calc.h"

void delete_stack(stack **head) {
    while (*head != NULL)
        pop(head);
}

int check_graph(char *str) {
    int result = 0;
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == 'x')
            result = 1;
        i += 1;
    }
    return result;
}

int str_zero(char *str) {
    for (int i = 0; i < 300; i++)
        str[i] = '\0';
    return 1;
}

double run(char *str, char *str2, int *point) {
    stack* result = NULL;
    stack* notation = NULL;
    int error = 0;
    double answer = 0;
    notation = parsing(str, &error);
    // printf("\nerror1:%d\n", error);
    result = reverse_stack(notation);
    if (!error) {
        error = calculate(result, &answer, 0);
    } else {
        delete_stack(&result);
    }
    // printf("\nerror2:%d\n", error);
    if (check_graph(str2) && !error) {
        double answer2 = 0;
        notation = parsing(str2, &error);
        result = reverse_stack(notation);
        if (!error) {
            error = calculate(result, &answer2, answer);
        } else {
            delete_stack(&result);
        }
        if (!error)
            answer = answer2;
    }
    if (error) {
        str_zero(str);
        snprintf(str, sizeof(str), "%s", "error");
        *point = 5;
    } else {
        str_zero(str);
        snprintf(str, sizeof(str), "%f", answer);
        *point = strlen(str);
    }
    return answer;
}

int graph_build(char *str, int *point, double *x, double *y) {
    stack* result = NULL;
    stack* notation = NULL;
    int error = 0;
    if (check_graph(str)) {
        for (int i = 0; i < numberpoints; i++) {
            notation = parsing(str, &error);
            result = reverse_stack(notation);
            if (!error) {
                error += calculate(result, &y[i], x[i]);
            } else {
                delete_stack(&result);
            }
            if (error)
                break;
        }
    } else {
        error = 1;
    }
    if (error) {
        str_zero(str);
        snprintf(str, sizeof(str), "%s", "error");
        *point = 5;
    }
    return error;
}

stack* reverse_stack(stack *first) {
    stack* result = NULL;
    while (first != NULL) {
        push(&result, peek_value(first), peek_operation(first));
        pop(&first);
    }
    return result;
}

stack* parsing(char *str, int *error) {
    int i = 0, left_bracket = 0, right_bracket = 0;
    *error = 0;
    double number;
    stack* buffer = NULL;
    stack* notation = NULL;
    char *tmp = str;
    value_type_t buffer_value_type;
    while (*tmp != '=' && *tmp != '\0' && !*error) {
        if (*tmp == ')') {
            right_bracket += 1;
            if (buffer == NULL) {
                *error = 1;
                break;
            }
            while (peek_operation(buffer) != bracket_left && buffer != NULL) {
                push(&notation, peek_value(buffer), peek_operation(buffer));
                pop(&buffer);
                if (buffer == NULL) {
                    *error = 1;
                    break;
                }
            }
            if (*error != 1)
                pop(&buffer);
        } else if (digit(*tmp) > -1) {
            *error = form_number(&tmp, &number);
            if (*error)
                break;
            push(&notation, number, value);
            tmp--;
        } else if (*tmp == 'x') {
            push(&notation, 0.0, x);
        } else if (*tmp == '(') {
            left_bracket += 1;
            push(&buffer, 0.0, bracket_left);
        } else if (*tmp == '+' && (i == 0 || *(tmp-1) == '(')) {
            push(&buffer, 0.0, unary_plus);
        } else if (*tmp == '-' && (i == 0 || *(tmp-1) == '(')) {
            push(&buffer, 0.0, unary_minus);
        } else {
            if (symbol(*tmp) && *tmp != 'x') {
                if (form_function(&tmp, &buffer_value_type)) {
                    *error = 1;
                    break;
                } else {
                    tmp -= 1;
                }
            } else if (operation(*tmp, &buffer_value_type) == 0) {
                *error = 1;
                break;
            }
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
        tmp++;
        i++;
    }
    if (left_bracket != right_bracket || notation == NULL)
        *error = 1;
    while (buffer != NULL) {
        push(&notation, peek_value(buffer), peek_operation(buffer));
        pop(&buffer);
    }
    return notation;
}

int calculate(stack* notation, double *result, double x_value) {
    int error = 0;
    stack *value = NULL;
    while (notation != NULL && error == 0) {
        value_type_t operation = peek_operation(notation);
        if (operation == -1) {
            push(&value, x_value, -2);
        } else if (operation == -2) {
            push(&value, peek_value(notation), -2);
        } else if (operation >= 2 && operation < 8) {
            action_two_arguments(&value, operation, &error);
        } else {
            action_one_arguments(&value, operation, &error);
        }
        if (error)
            break;
        pop(&notation);
    }
    if (!error) {
        *result = peek_value(value);
        pop(&value);
        if (value != NULL) {
            error = 1;
        }
    }
    while (value != NULL)
        pop(&value);
    while (notation != NULL)
        pop(&notation);
    return error;
}

double action_two_arguments(stack **value, value_type_t operation, int *error) {
    double number1, number2, result;
    if (*value != NULL) {
        number1 = peek_value(*value);
        pop(value);
    } else {
        *error = 1;
    }
    if (*value != NULL) {
        number2 = peek_value(*value);
        pop(value);
    } else {
        *error = 1;
    }
    if (*error != 1) {
        if (operation == additional) {
            result = number1 + number2;
        } else if (operation == subtraction) {
            result = number2 - number1;
        } else if (operation == multiplication) {
            result = number1 * number2;
        } else if (operation == division) {
            result = number2 / number1;
        } else if (operation == power) {
            result = pow(number2 * 1.0, number1 * 1.0);
        } else if (operation == modulus) {
            result = fmod(number2, number1);
        }
        push(value, result, -2);
    }
    return result;
}

double action_one_arguments(stack **value, value_type_t operation, int *error) {
    double number, result;
    if (*value != NULL) {
        number = peek_value(*value);
        pop(value);
    } else {
        *error = 1;
    }
    if (*error != 1) {
        if (operation == unary_minus) {
            result = number * (-1);
        } else if (operation == unary_plus) {
            result = number;
        } else if (operation == cosine) {
            result = cos(number);
        } else if (operation == sine) {
            result = sin(number);
        } else if (operation == tangent) {
            result = tan(number);
        } else if (operation == arc_cosine) {
            result = acos(number);
        } else if (operation == arc_sine) {
            result = asin(number);
        } else if (operation == arc_tangent) {
            result = atan(number);
        } else if (operation == square) {
            result = sqrt(number);
        } else if (operation == natural_logarithm) {
            result = log(number);
        } else if (operation == common_logarithm) {
            result = log10(number);
        }
        push(value, result, -2);
    }
    return result;
}

double form_number(char **str, double *number) {
    *number = 0;
    value_type_t buffer;
    int dig, error = 0;
    while ((dig = digit(**str)) > -1) {
        *number = 10 * *number + dig;
        *str += 1;
    }
    if (**str == '.') {
        *str += 1;
        if ((dig = digit(**str)) > -1) {
            int i = 0;
            while ((dig = digit(**str)) > -1) {
                i +=1;
                double buf = dig / pow(10, i);
                *number += buf;
                *str += 1;
            }
        } else {
            error = 1;
        }
    }
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
        // exit(STACK_OVERFLOW);
    }
    st->next = *head;
    st->value = number;
    st->operation = oper;
    *head = st;
}

void pop(stack **head) {
    stack *out;
    if ((*head) == NULL)
        exit(-1);
    out = *head;
    *head = (*head)->next;
    free(out);
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

int operation(char c, value_type_t *operation) {
    int result = 0;
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%')
        result = 1;
    if (c == '+')
        *operation = 2;
    if (c == '-')
       *operation = 3;
    if (c == '*')
        *operation = 4;
    if (c == '/')
        *operation = 5;
    if (c == '^')
        *operation = 6;
    if (c == '%')
        *operation = 7;
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
    // printf("strfactor:%s\n", str);
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
    if (strcmp(str, "ln(") == 0)
        result = 17;
    if (strcmp(str, "log(") == 0)
        result = 18;
    return result;
}

double form_function(char **str, value_type_t *func) {
    char factor[10] = "\0";
    int error = 0, i = 0;;
    while (symbol(**str) && i < 5) {
        factor[i] = **str;
        i++;
        if (**str == '(')
            break;
        *str += 1;
    }
    factor[i] = '\0';
    if (!(*func = function(factor)))
        error = 1;
    return error;
}
