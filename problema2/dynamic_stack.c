#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
wheter it's empty or full or neither of those two.
The capacity is for initialization purporses.
*/
typedef struct Stack {
    Item *items;
    int8_t top;
    int8_t capacity;
} Stack;

/*
States enumeration. This is for the instructions,
so the code is more readable.
*/
typedef enum States {
    STATE_TOGGLE,
    STATE_PRINT_1,
    STATE_ADD_COUNT,
    STATE_PRINT_COUNT,
    STATE_VERIFY_NUM,
    STATE_EJECT
} States;

Stack* create_stack(int8_t capacity);
uint8_t is_empty(Stack *stack);
uint8_t is_full(Stack *stack);
//void double_stack(Stack *stack);
//void halve_stack(Stack *stack);
void stack_add(Stack *stack, Item value);
int8_t stack_remove(Stack *stack);
Item peek_top_item(Stack *stack);
void populate_stack(Stack *stack);
void populate_stack(Stack *stack);
void print_stack(Stack *stack);
Item item_init(void);
int8_t take_action(Item item);
int get_random_num(int min, int max);

uint8_t pin1 = 0;
size_t counter = 0;

int main (void) {

    Stack* my_stack = create_stack(20);  // Create an item stack of size 20
    populate_stack(my_stack);            // Populate it with items
    print_stack(my_stack);               // Print it, just to look at it

    // Thiw while loops does the same as in the static stack case
    while (!is_empty(my_stack)) {
        int return_value = take_action(peek_top_item(my_stack));
        printf("Return value is %d\n", return_value);
        stack_remove(my_stack);
    }
    free(my_stack->items); // Once stack is empty, free the array
    free(my_stack);        // Once stack is empty, free the memory it occupied
    return 0;
}

Stack* create_stack(int8_t capacity) {
    Stack *stack = (Stack*) malloc(sizeof(Stack));                  // Allocate memory for the stack
    stack->capacity = capacity;                                     // Assign the capacity to the stack
    stack->top = -1;                                                // The stack starts empty
    stack->items = (Item*) malloc(stack->capacity * sizeof(Item));  // Allocate memory for the Item array
    return stack;
}

uint8_t is_empty(Stack *stack) {
    return stack->top == -1;
}

uint8_t is_full(Stack *stack) {
    return stack->top == stack->capacity - 1;
}

/*void double_stack(Stack *stack) {
    stack->capacity *= 2;
    stack->items = (Item*) realloc(stack->items, stack->capacity * sizeof(Item));
}*/

/*void halve_stack(Stack *stack) {
    if (stack->top < 1) {
        printf("The stack is too small, can't get any smaller\n");
        return;
    }
    
    printf("Reducing stack size in memory... Size: %d, Capacity: %d\n",
           sizeof(stack->items), stack->capacity);

    stack->capacity /= 2;
    stack->items = (Item*) realloc(stack->items, stack->capacity * sizeof(Item));

    printf("Stack size in memory reduced. New size: %d, New capacity: %d\n",
            sizeof(stack->items) / sizeof(stack->items[0]), stack->capacity);
}*/

void stack_add(Stack *stack, Item value) {
    if (is_full(stack)) {
        printf("Stack is full!\n");
        //resize_stack(stack);
    }
    stack->items[++stack->top] = value;
}

int8_t stack_remove(Stack *stack) {
    if (is_empty(stack)) {
        printf("Stack is empty!\n");
    }
    stack->top--;
}

Item peek_top_item(Stack *stack) {
    if (is_empty(stack)) {
        printf("The stack is empty, nothing to peek\n");
    }
    return stack->items[stack->top];
}

void populate_stack(Stack *stack) {
    if (is_full(stack)) {
        printf("Stack overflow!\n");
        return;
    }

    srand(time(NULL));
    for (int i = 0; i < stack->capacity; i++) {
        Item next_item = item_init();
        stack_add(stack, next_item);
    }
}

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

Item item_init(void) {
    Item new_item = {0, 0, get_random_num(0, 5)};
    return new_item;
}

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
            printf("Instruction: Toggle pin 1. ");
            //pin1 = !pin1;
            break;
        
        case STATE_PRINT_1:
            printf("Instruction: Print pin 1 value. ");
            break;
        
        case STATE_ADD_COUNT:
            printf("Instruction: Increase counter. ");
            //counter++;
            break;
        
        case STATE_PRINT_COUNT:
            printf("Instruction: Print counter value. ");
            break;

        case STATE_VERIFY_NUM:
            printf("Instruction: Verify size of stack. ");
            break;

        case STATE_EJECT:
            printf("Instruction: If stack is full, quit the program. ");
            break;

        default:
            printf("You shouldn't be here: %d ", instruction);
            return -1;
    }
    return instruction;
}