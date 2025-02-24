#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_INPUT_SIZE 1024

// Stack implementation for integers
int stack[MAX_INPUT_SIZE];
int top = -1;

void push(int value) {
    if (top >= MAX_INPUT_SIZE - 1) {
        fprintf(stderr, "Stack overflow\n");
        exit(EXIT_FAILURE);
    }
    stack[++top] = value;
}

int pop() {
    if (top < 0) {
        fprintf(stderr, "Stack underflow\n");
        exit(EXIT_FAILURE);
    }
    return stack[top--];
}

// Stack implementation for operators
char op_stack[MAX_INPUT_SIZE];
int op_top = -1;

void push_op(char op) {
    if (op_top >= MAX_INPUT_SIZE - 1) {
        fprintf(stderr, "Operator stack overflow\n");
        exit(EXIT_FAILURE);
    }
    op_stack[++op_top] = op;
}

char pop_op() {
    if (op_top < 0) {
        fprintf(stderr, "Operator stack underflow\n");
        exit(EXIT_FAILURE);
    }
    return op_stack[op_top--];
}

// Function to return precedence of operators
int precedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            return 0;
    }
}

// Function to perform arithmetic operations
int apply_op(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        default:
            fprintf(stderr, "Unknown operator: %c\n", op);
            exit(EXIT_FAILURE);
    }
}

// Function to evaluate the expression
int evaluate(const char* expression) {
    int i;
    for (i = 0; expression[i]; i++) {
        // Skip whitespace
        if (isspace(expression[i])) {
            continue;
        }

        // If the character is a digit, push it to the stack
        if (isdigit(expression[i])) {
            int num = 0;
            while (isdigit(expression[i])) {
                num = num * 10 + (expression[i] - '0');
                i++;
            }
            i--; // Because the for loop will increment i
            push(num);
        }
        // If the character is an opening parenthesis, push it to the operator stack
        else if (expression[i] == '(') {
            push_op(expression[i]);
        }
        // If the character is a closing parenthesis, solve the expression inside the parentheses
        else if (expression[i] == ')') {
            while (op_top >= 0 && op_stack[op_top] != '(') {
                int val2 = pop();
                int val1 = pop();
                char op = pop_op();
                push(apply_op(val1, val2, op));
            }
            if (op_top >= 0 && op_stack[op_top] == '(') {
                pop_op(); // Pop the '('
            } else {
                fprintf(stderr, "Mismatched parentheses\n");
                exit(EXIT_FAILURE);
            }
        }
        // If the character is an operator
        else {
            while (op_top >= 0 && precedence(op_stack[op_top]) >= precedence(expression[i])) {
                int val2 = pop();
                int val1 = pop();
                char op = pop_op();
                push(apply_op(val1, val2, op));
            }
            push_op(expression[i]);
        }
    }

    // Apply remaining operators to the remaining values
    while (op_top >= 0) {
        int val2 = pop();
        int val1 = pop();
        char op = pop_op();
        push(apply_op(val1, val2, op));
    }

    // The final result is on the top of the stack
    return pop();
}

int main() {
    char expression[MAX_INPUT_SIZE];
    if (fgets(expression, MAX_INPUT_SIZE, stdin) {
        // Remove newline character if present
        expression[strcspn(expression, "\n")] = 0;
        int result = evaluate(expression);
        printf("%d\n", result);
    } else {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
