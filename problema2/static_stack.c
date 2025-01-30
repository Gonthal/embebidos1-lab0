#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 50

/*
Item structure, as per the lab instructions
*/
typedef struct Item {
    uint32_t interrupt_time;
    uint16_t reference_pointer;
    uint8_t pin_name;
} Item;

/*
Stack structure, with an array of items and a top.
The top determines the size of the stack and
wheter it's empty or full or neither of those two
*/
typedef struct Stack {
    Item items[MAX_SIZE];
    int8_t top;
} Stack;

/*
States enumeration. This is for the instructions,
so the code is more readable.
*/
typedef enum States {
    STATE_TOGGLE,       // Toggle the value of pin 1
    STATE_PRINT_1,      // Print the value of pin 1
    STATE_ADD_COUNT,    // Increase the counter
    STATE_PRINT_COUNT,  // Print the counter
    STATE_VERIFY_NUM,   // Check the size of the stack
    STATE_EJECT         // If the stack is empty, quit the program
} States;

/*Functions prototypes, each is explained in more 
detail in its own code section.
*/
void stack_init(Stack *stack);
uint8_t is_empty(Stack *stack);
uint8_t is_full(Stack *stack);
void stack_add(Stack *stack, Item value);
int8_t stack_remove(Stack *stack);
Item peek_top_item(Stack *stack);
void print_stack(Stack *stack);
void populate_stack(Stack *stack);
Item item_init(void);
int8_t take_action(Item item);
int get_random_num(int min, int max);

// Variables affected by the instructions
uint8_t pin1 = 0;
size_t counter = 0;

int main (void) {
    Stack my_stack;             // Declare a stack
    stack_init(&my_stack);      // Initialize it as empty
    populate_stack(&my_stack);  // Populate it with items
    print_stack(&my_stack);     // Print it, just to take a look at it
    
    // Main loop
    while (!is_empty(&my_stack)) {                                  // 
        int return_value = take_action(peek_top_item(&my_stack));   // Get an item and read its instruction
        printf("Return value is %d\n", return_value);               // Print the instruction, just to check everything is ok
        stack_remove(&my_stack);                                    // That item has been taken care of, remove it from the stack
    }
    return 0;
}

// Initialize the stack as empty
void stack_init(Stack *stack) {
    stack->top = -1;
}

// If the stack's top is negative, that means is empty
uint8_t is_empty(Stack *stack) {
    return stack->top == -1;
}

// If the stack's top is the max size, that means is full
uint8_t is_full(Stack *stack) {
    return stack->top == MAX_SIZE - 1;
}

// Add an item to the top of the stack
void stack_add(Stack *stack, Item value) {
    if (is_full(stack)) {
        printf("Stack overflow\n");
        return;
    }
    // First add +1 to the top, then use the index to assign the new value
    stack->items[++stack->top] = value;
    //printf("Pushed item onto the stack\n");
}

// Remove the top from the stack
int8_t stack_remove(Stack *stack) {
    if (is_empty(stack)) {
        printf("Stack underflow\n");
        return -1;
    }
    // Reduce the stack top index,
    // but it does not erase the previous top value
    stack->top--;
    //printf("Popped item from the stack\n");
}

/*
Get the item from the top of the stack,
without removing it
*/
Item peek_top_item(Stack *stack) {
    if (is_empty(stack)) {
        printf("Stack is empty\n");
    } else {
        return stack->items[stack->top];
    }
}

// Print the whole stack
void print_stack(Stack *stack) {
    if (is_empty(stack)) {
        printf("Stack is empty\n");
        return;
    }
    
    int i = 0;
    // We use do while, because i can be the same as
    // the stack top index when top == 0, i.e., the stack
    // only has one item in it
    do {
        printf("%d. Pin name: %d, Interrupt time: %d, Reference pointer: %d\n",
                i,
                stack->items[i].pin_name,
                stack->items[i].interrupt_time,
                stack->items[i].reference_pointer);
    } while (i++ != stack->top);

    return;
}

// Fill the stack with Items
void populate_stack(Stack *stack) {
    if (is_full(stack)) {
        printf("Stack overflow!\n");
        return;
    }

    for (int i = 0; i < MAX_SIZE; i++) {
        Item next_item = item_init();   // Create an item with random pin name
        stack_add(stack, next_item);    // Add the item to the stack
    }
}

/*
Initialize an item with 0 interrupt time and reference pointer,
and a random pin name between 0 and 5
*/
Item item_init(void) {
    Item new_item = {0, 0, get_random_num(0, 5)};
    return new_item;
}

// Generate a random number between min and max
int get_random_num(int min, int max) {
    int random_num = rand() % (max - min + 1) + min;
    return random_num;
}

/*
Function that returns an instruction number
depending on the Item it receives.
This function does nothing by itself, it just
returns an instruction number.
*/
int8_t take_action(Item item) {
    // Get the item's pin name, a.k.a., the instruction
    States instruction = (States) item.pin_name;

    switch (instruction) {
        case STATE_TOGGLE:
            printf("Instruction: Toggle pin 1.\n");
            //pin1 = !pin1;
            break;
        
        case STATE_PRINT_1:
            printf("Instruction: Print pin 1 value.\n");
            break;
        
        case STATE_ADD_COUNT:
            printf("Instruction: Increase counter.\n");
            //counter++;
            break;
        
        case STATE_PRINT_COUNT:
            printf("Instruction: Print counter value.\n");
            break;

        case STATE_VERIFY_NUM:
            printf("Instruction: Verify size of stack.\n");
            break;

        case STATE_EJECT:
            printf("Instruction: If stack is full, quit the program\n");
            break;

        default:
            printf("You shouldn't be here: %d\n", instruction);
            return -1;
    }
    return instruction;
}