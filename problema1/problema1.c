#include <stdint.h>
#include <stdio.h>

/*
Struct to store the 12 bytes received in a single variable.
Memory padding is taken into account:
4 + 4 + 2 + 2 + 1 + 3 (padding) = 16
*/
typedef struct Item {
    int32_t reserve_tank;
    uint32_t main_tank;
    int16_t temperature;
    uint16_t checksum;
    uint8_t battery;
} Item;

/*
Union to store the 12 bytes inside an Item, but without
the need to typecast the data.
With this union, data[] and item occupy the same memory like this:
data[0-2] = reserve water tank
data[3] = 0x00, padding because an int32_t is 4 bytes
data[4-7] = main water tank
data[8-9] = temperature
data[10-11] = checksum
data[12] = battery
data[13-15] = padding, so that data[] and item are the same size
*/
typedef union Copycat {
    uint8_t data[16];
    Item item;
} Copycat;

Copycat receiver(uint8_t input[]);
void item_printer(Item item);

int main (void) {
    // Test data "sets", where n == 12
    uint8_t first_input[12] = {0x56, 0xA0, 0x00, 0x12, 0xFF, 0x00, 0x0F, 0xED, 0x00, 0x00, 0xCB, 0xA9};
    uint8_t second_input[12] = {86, -1, -1, 0, -5, -1, 0, 0, 5, 8, 10, 11};

    Copycat first_container = receiver(first_input);
    item_printer(first_container.item);

    Copycat second_container = receiver(second_input);
    item_printer(second_container.item);

    return 0;
}

/*
This functions receives input data, creates an organizer Copycat
and assigns the input data to the Copycat
*/
Copycat receiver(uint8_t input[]) {
    Copycat organizer; 
    // Reserve tank
    organizer.data[0] = input[3];
    organizer.data[1] = input[4];
    organizer.data[2] = input[5];
    organizer.data[3] = 0x00;
    // Main tank
    organizer.data[4] = input[6];
    organizer.data[5] = input[7];
    organizer.data[6] = input[8];
    organizer.data[7] = input[9];
    // Temperature
    organizer.data[8] = input[1];
    organizer.data[9] = input[2];
    // Checksum
    organizer.data[10] = input[10];
    organizer.data[11] = input[11];
    // Battery
    organizer.data[12] = input[0];

    return organizer;
}

/*
Utility function that prints an Item
*/
void item_printer(Item item) {
    printf("0. Battery is %d\n", item.battery);
    printf("1-2. Temperature is %d\n", item.temperature);
    printf("3-4-5. Reserve tank is %d\n", item.reserve_tank);
    printf("6-7-8-9. Main tank is %d\n", item.main_tank);
    printf("10-11. Checksum is %d\n", item.checksum);
}