#include "calc.h"

// int main() {
//     stack* result = NULL;
//     stack* notation = NULL;
//     double number = 0, answer;
//     char *str = (char*)malloc(100*sizeof(char));
//     int error;
//     input(str);
//     printf("str:%s\n", str);
//     notation = parsing(str, &error);
//         //printf("answer:%f\n", answer);
//     printf("error:%d\n", error);
//     printf("\n|||||||||||||||||\n");
//     result = reverse_stack(notation);
//     error = calculate(result, &answer);
//     // while (result != NULL) {
//     //     printf("%.1f-%d||", peek_value(result), peek_operation(result));
//     //     pop(&result);
//     // }
//     printf("answer:%f\n", answer);
//     printf("error:%d\n", error);
//     return 0;
// }

int str_zero(char *str) {
    for (int i = 0; i < 500; i++)
        str[i] = '\0';
    return 1;
}

int run(char *str) {
    stack* result = NULL;
    stack* notation = NULL;
    int error = 0, point = 0;
    double answer;
    notation = parsing(str, &error);
    printf("\nerror1:%d\n", error);
    if (!error) {
        result = reverse_stack(notation);
        error = calculate(result, &answer);
    } 
    printf("\nerror2:%d\n", error);
    // if (error) {
    //     str_zero(str);
    //     strcpy(str, "error");
    // } else {
        str_zero(str);
        sprintf(str, "%f", answer);
    // }
    return 1;
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
    int i = 0;
    *error = 0;
    double number;
    stack* buffer = NULL;
    stack* notation = NULL;    
    char *tmp = str;
    value_type_t buffer_value_type;
    while (*tmp != '=' && *tmp != '\0' && !*error) {
        //printf("%d-%c\n", tmp, *(tmp));
        if (*tmp == ')') {
            while (peek_operation(buffer) != bracket_left && buffer != NULL) {
                printf("push:%0.1f-%d||", peek_value(buffer), peek_operation(buffer));
                push(&notation, peek_value(buffer), peek_operation(buffer));
                //printf("push:%0.1f-%d||", peek_value(buffer), peek_operation(buffer));
                pop(&buffer);
            }
            //printf("h!!!!!!!!!!!!!s\n");
            if (buffer == NULL) {
                *error = 1;
                break;
            }
            pop(&buffer);
            //printf("push:%0.1f-%d||", peek_value(buffer), peek_operation(buffer));
        } else if (digit(*tmp) > -1) {
            *error = form_number(&tmp, &number);
            if (*error)
                break;
            push(&notation, number, value);
            tmp--;
            //printf("tmp:%s\n", tmp);
        } else if (*tmp == 'x') {
            push(&notation, 0.0, x);
        } else if (*tmp == '(') {
            push(&buffer, 0.0, bracket_left);
        } else if (*tmp == '+' && (!i || *(tmp-1) == '(')) {
            push(&buffer, 0.0, unary_plus);
        } else if (*tmp == '-' && (!i || *(tmp-1) == '(')) {
            //printf("!!!\n");
            push(&buffer, 0.0, unary_minus);
        } else {
            //printf("operation!\n");
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
            //printf("value:%d\n", buffer_value_type);
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
        // printf("errorrrrr:%d\n", *error);
        tmp++;
        i++;
        //printf("%d-%c\n", tmp, *tmp);
        //printf("BUFFER:%d\n", peek_operation(buffer));
    }
    //printf("errorrrrrrs:%d\n", *error);
    while (buffer != NULL) {
        push(&notation, peek_value(buffer), peek_operation(buffer));
        pop(&buffer);
    }
    return notation;
}

int calculate(stack* notation, double *result) {
    int error = 0;
    stack *value = NULL;
    printf("tut\n");
    while (notation != NULL && error == 0) {
        printf("k");
        value_type_t operation = peek_operation(notation);
        //printf("operation:%d\n", operation);
        if (operation == -2 || operation == -1) {
            //printf("p");
            push(&value, peek_value(notation), operation);
        } else if (operation >= 2 && operation < 8) {
            action_two_arguments(&value, operation, &error);
            printf("errorcalc:%d\n", error);
            //printf("errorin:%d", error);
        } else {
            action_one_arguments(&value, operation, &error);
        }
        if (error)
            break;
        //printf("value:%f||", peek_value(value));
        pop(&notation);
    }
    if (!error) {
        *result = peek_value(value);
    //printf("result:%f\n", *result);
        pop(&value);
        if (value != NULL) {
            printf("tuterror%f\n", peek_value(value));
            error = 1;
        }
    }
    printf("errorin:%d\n", error);
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
        } else if (operation == cosine) {
            result = cos(number);
        } else if (operation == sine) {
            result = sin(number);
        } else if (operation == tangent) {
            result = tan(x); 
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
    printf("strfactor:%s\n", str);
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
    //char *factor = (char*)malloc(10 * sizeof(char));
    char factor[10] = "\0";
    int error = 0, i = 0;;
    printf("symbol:%c\n", **str);
    while (symbol(**str) && i < 5) {
        factor[i] = **str;
        i++;
        //factor += 1;
        if (**str == '(')
            break;
        *str += 1;
    }
    factor[i] = '\0';
    //factor = factor - 2;
    //strcpy(factor, factor1);
    printf("factor:%s\n", factor);
    if (!(*func = function(factor)))
        error = 1;
    printf("error:%d\n", error);
    //free(factor);
    return error;
}
