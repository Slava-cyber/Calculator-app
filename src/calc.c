#include <stdio.h>
#include <math.h>

typedef struct stack {
    double value;
    int operation;
    struct stack *next;
} stack;

stack* push(stack *head, double number, int oper);
stack* pop(stack **head);
stack peek_value(const stack *head);
stack peek_opearation(const stack *head);

double form_number(char **str, double *number);
int priority(char a);
int digit(char c);
int operation(char c);

int main() {

    return 0;
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

stack peek_value(const stack *head) {
    if (head == NULL)
        exit(-1);
    return head->value;
}

stack peek_opearation(const stack *head) {
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
    int result = 0;
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

char* symbol_form(char *str, int *len, char *factor) {
    char *tmp = str;
    int len1 = 0;
    while (symbol(*tmp)) {
        *factor = *tmp;
        //  printf("%c ", *tmp);
        factor++;
        len1++;
        if (*tmp == '(') {
            break;
        }
        tmp++;
    }
    *len = len1;
    *factor = '\0';
    return factor;
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
        if (*tmp == '(')
            break;
        *str += 1;
    }
    *factor = '\0';
    if (!(*func = function(factor)))
        error = 1;
    free(factor);
    return error;
}

double form_number(char **str, double *number) {
    *number = 0;
    int digit, error = 0;
    while ((digit = digit(**str))) {
        *number = 10 * *number + digit;
        *str += 1;
    }
    if (**str == ',') {
        *str += 1;
        if (digit = digit(**str)) {
            int i = 0;
            while ((digit = digit(**str))) {
                i++;
                *number += *number + digit / pow(10, i);
                *str += 1;
            }
        } else {
            error = 1;
        }
    } else if (!opertion(**str)) {
        error = 1;
    }
    if (!operation(**str))
        error = 1;
    return error;
}

double parsing(char *str, char *out, stack *value, stack *opers) {
    int i = 0, j = 0;
    while (str[i] != '\0') {
        if (str[i] == ')') {
            while ((opers->c) != '(') {
                out[j] = del(&opers);
                j++;
            }
            out[j] = ')';
            j++;
            del(&opers);
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
                        out[j] = del(&opers);
                        j++;
                    }
                    opers = push(opers, str[i]);
                }
            }
        }
        i++;
    }
    while (opers != NULL) {
        out[j] = del(&opers);
        j++;
    }
    out[j] = '\0';
    *n = j;
    return 1;
}
