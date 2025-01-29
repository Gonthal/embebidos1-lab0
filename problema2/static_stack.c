#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_SIZE 15

typedef struct Item {
    uint32_t interrupt_time;
    uint16_t reference_pointer;
    uint8_t pin_name;
} Item;

typedef struct Stack {
    Item items[MAX_SIZE];
    int8_t top;
} Stack;

typedef enum States {
    STATE_TOGGLE,
    STATE_PRINT_1,
    STATE_ADD_COUNT,
    STATE_PRINT_COUNT,
    STATE_VERIFY_NUM,
    STATE_EJECT
} States;

// Stack functions prototypes
void stack_init(Stack *stack);
uint8_t is_empty(Stack *stack);
uint8_t is_full(Stack *stack);
void stack_add(Stack *stack, Item value);
int8_t stack_remove(Stack *stack);
Item peek_top_item(Stack *stack);
void print_stack(Stack *stack);

uint8_t take_action(Stack *stack);

// Variables affected by the instructions
bool pin1 = true;
size_t counter = 0;

int main (void) {

    Stack my_stack;
    stack_init(&my_stack);
    Item test_item;
    test_item.interrupt_time = 0x01;
    test_item.reference_pointer = 0x02;
    test_item.pin_name = 0x03;

    //printf("Stack is empty = %d\n", is_empty(&my_stack));
    //printf("Stack top is = %d\n", my_stack.top);
    stack_add(&my_stack, test_item);
    //printf("Stack is empty = %d\n", is_empty(&my_stack));
    //printf("Stack top is = %d\n", my_stack.top);
    print_stack(&my_stack);
    Item read_item = peek_top_item(&my_stack);
    printf("Item is: %d, %d, %d", read_item.pin_name, read_item.interrupt_time, read_item.reference_pointer);
    return 0;
}

void stack_init(Stack *stack) {
    stack->top = -1;
}

uint8_t is_empty(Stack *stack) {
    return stack->top == -1;
}

uint8_t is_full(Stack *stack) {
    return stack->top == MAX_SIZE - 1;
}

void stack_add(Stack *stack, Item value) {
    if (is_full(stack)) {
        printf("Stack overflow\n");
        return;
    }
    stack->items[++stack->top] = value;
    printf("Pushed item onto the stack\n");
}

int8_t stack_remove(Stack *stack) {
    if (is_empty(stack)) {
        printf("Stack underflow\n");
        return -1;
    }

    Item removed = stack->items[stack->top];

    stack->top--;
    printf("Popped item from the stack\n");
}

Item peek_top_item(Stack *stack) {
    if (is_empty(stack)) {
        printf("Stack is empty\n");
        Item null_item;
        return; // WE NEED TO ADDRESS THIS
    }

    return stack->items[stack->top];
}

void print_stack(Stack *stack) {
    if (is_empty(stack)) {
        printf("Stack is empty\n");
        return;
    }
    
    int i = 0;
    do {
        printf("%d. Pin name: %d, Interrupt time: %d, Reference pointer: %d\n",
                i,
                stack->items[i].pin_name,
                stack->items[i].interrupt_time,
                stack->items[i].reference_pointer);
    } while (i++ != stack->top);

    return;
}

/*uint8_t take_action(Stack *stack) {
    switch ()
}*/