#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

// 0 - (
// 1 - )
// 2 - cos
// 3 - sin
// 4 - tg
// 5 - ctg
// 6 - log
// 7 - ln
// 8 - sqrt

typedef struct stack {
    double value;
    int operation;
    struct stack *next;
} stack;

stack* push(stack *head, double number, int oper);
stack* pop(stack **head);
double peek_value(const stack *head);
double peek_opearation(const stack *head);

double form_number(char **str, double *number);
int priority(char a);
int digit(char c);
int operation(char c);
int symbol(char c);
int function(char *str);
double form_function(char **str, int *func);


double parsing(char *str, char *out, stack *value, stack *opers);


int input(char * str);

int main() {
    stack* opers = NULL;
    stack* value = NULL;
    // char str[100] = "lsff+32,35+23,42";
    // char out[100] = "\0";
    double number = 0;
    char *str = (char*)malloc(100*sizeof(char));
    char *out = (char*)malloc(100*sizeof(char));
    //parsing(str, out, value, opers);
    input(str);
    int error = form_number(&str, &number);
    printf("\n");
    printf("error:%d\nnumber:%f\n", error, number);
    printf("%s\n", str);
    return 0;
}

/*double parsing(char *str, char *out, stack *value, stack *opers) {
    int i = 0, j = 0;
    while (str[i] != '\0') {
        if (str[i] == ')') {
            while ((opers->c) != '(') {
                out[j] = peek_value(opers);
                pop(&opers);
                j++;
            }
            out[j] = ')';
            j++;
            pop(&opers);
        }

        if ((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'z')) {
            out[j] = str[i];
            j++;
        }

        if (str[i] == '(') {
            opers = push(opers, '(');
            out[j] = '(';
            j++;
        }

        if (str[i] == '+' || str[i] == '-' || str[i] == '/' || str[i] == '*' || str[i] == '^') {
            if (opers == NULL) {
                opers = push(opers, str[i]);
            } else {
                if (prior(opers->c) < prior(str[i])) {
                    opers = push(opers, str[i]);
                } else {
                    while (opers != NULL && prior(opers->c) >= prior(str[i])) {
                        out[j] = peek_value(opers);
                        pop(&opers);
                        j++;
                    }
                    opers = push(opers, str[i]);
                }
            }
        }
        i++;
    }
    while (opers != NULL) {
        out[j] = peek_value(opers);
        pop(&opers);
        j++;
    }
    out[j] = '\0';
    *n = j;
    return 1;
}*/
double form_number(char **str, double *number) {
    *number = 0;
    int dig, error = 0;
    while ((dig = digit(**str)) > -1) {
        *number = 10 * *number + dig;
        *str += 1;
        printf("%d", dig);
    }
    if (**str == ',') {
        *str += 1;
        if ((dig = digit(**str)) > -1) {
            int i = 0;
            while ((dig = digit(**str)) > -1) {
                i +=1;
                printf("number:%f\n", dig / pow(10, i));
                //printf("%d", dig);
                double buf = dig / pow(10, i);
                *number += buf;
                printf("number:%f\n", *number);
                *str += 1;
            }
        } else {
            error = 1;
        }
    } else if (!operation(**str) && **str != '\0') {
        error = 1;
    }
    printf("number:%f\n", *number);
    if (!operation(**str) && **str != '\0')
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

stack* push(stack *head, double number, int oper) {
    stack *st = malloc(sizeof(stack));
    if (st == NULL) {
        puts("memory fault");
        exit(-1);
        //exit(STACK_OVERFLOW);
    }
    st->value = number;
    st->operation = oper;
    st->next = head;
    return st;
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

double peek_opearation(const stack *head) {
    if (head == NULL)
        exit(-1);
    return head->operation;
}

int priority(char a) {
    int flag;
    switch (a) {
        case '^': {
            flag = 4;
            break;
        }
        case '*': case '/': {
            flag = 3;
            break;
        }
        case '+': case '-': {
            flag = 2;
            break;
        }
        case '(': {
            flag = 1;
            break;
        }
        case 'c': {
            flag = 0;
            break;
        }
    }
    return flag;
}

int digit(char c) {
    int result = -1;
    if (c >= '0' && c <= '9')
        result = c - 48;
    return result;
}

int operation(char c) {
    int result = 0;
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^')
        result = 1;
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
        result = 1;
    if (strcmp(str, "sin(") == 0)
        result = 2;
    if (strcmp(str, "tan(") == 0)
        result = 3;
    if (strcmp(str, "ctg(") == 0)
        result = 4;
    if (strcmp(str, "log(") == 0)
        result = 5;
    if (strcmp(str, "sqrt(") == 0)
        result = 6;
    return result;
}

double form_function(char **str, int *func) {
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

