#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    float* data;
    int top;
    int capacity;
} Stack;

void initStack(Stack* stack, int capacity) {
    stack->data = (float*)malloc(capacity * sizeof(float));
    stack->top = -1;
    stack->capacity = capacity;
}

void push(Stack* stack, float value) {
    if (stack->top == stack->capacity - 1) {
        stack->capacity *= 2;
        stack->data = (float*)realloc(stack->data, stack->capacity * sizeof(float));
    }
    stack->data[++stack->top] = value;
}

float pop(Stack* stack) {
    return stack->data[stack->top--];
}

float top(Stack* stack) {
    return stack->data[stack->top];
}

int isEmpty(Stack* stack) {
    return stack->top == -1;
}

void freeStack(Stack* stack) {
    free(stack->data);
}

float calculate(char* expression) {
    Stack values;
    Stack ops;
    initStack(&values, 10);
    initStack(&ops, 10);

    for (int i = 0; i < strlen(expression); i++) {
        char ch = expression[i];
        
        if (isspace(ch)) {
            continue;
        }

        if (isdigit(ch) || ch == '.') {
            char buffer[20];
            int j = 0;
            while (isdigit(expression[i]) || expression[i] == '.') {
                buffer[j++] = expression[i++];
            }
            buffer[j] = '\0';
            push(&values, atof(buffer));
            i--; // Вернуться назад
        } else {
            while (!isEmpty(&ops) && 
                   (ch == '+' || ch == '-') && 
                   (top(&ops) == '*' || top(&ops) == '/' || top(&ops) == '+' || top(&ops) == '-')) {
                char op = pop(&ops);
                float b = pop(&values);
                float a = pop(&values);
                if (op == '+') push(&values, a + b);
                if (op == '-') push(&values, a - b);
                if (op == '*') push(&values, a * b);
                if (op == '/') {
                    if (b != 0) push(&values, a / b);
                    else {
                        printf("Ошибка: деление на ноль\n");
                        freeStack(&values);
                        freeStack(&ops);
                        return 0;
                    }
                }
            }
            push(&ops, ch);
        }
    }

    while (!isEmpty(&ops)) {
        char op = pop(&ops);
        float b = pop(&values);
        float a = pop(&values);
        if (op == '+') push(&values, a + b);
        if (op == '-') push(&values, a - b);
        if (op == '*') push(&values, a * b);
        if (op == '/') {
            if (b != 0) push(&values, a / b);
            else {
                printf("Ошибка: деление на ноль\n");
                freeStack(&values);
                freeStack(&ops);
                return 0;
            }
        }
    }

    int result = pop(&values);
    freeStack(&values);
    freeStack(&ops);
    return result;
}

int main() {
    char expression[100];

   
    fgets(expression, sizeof(expression), stdin);
    
    int result = calculate(expression);
    printf("%d\n", result);  
  return 0;
}

