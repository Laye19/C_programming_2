#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

typedef struct Equipment {
    char index[50];
    char name[50];
    char equipment_category_index[50];
    char equipment_category_name[50];
    char equipment_category_url[256]; // Adjust buffer size as needed
    char vehicle_category[50];
    double cost_quantity;
    char cost_unit[10];
    double cost_weight;
    char cost_url[256]; // Adjust buffer size as needed
    double weight; // weight of the equipment
    double value;  
    struct Equipment* next; // Pointer to the next equipment
} Equipment;

typedef struct Coins {
    int cp;
    int sp;
    int ep;
    int gp;
    int pp;
} Coins;

typedef struct Inventory {
    Equipment* head;
    Equipment* tail;
} Inventory;

typedef struct spell {
    char name[50];
    double power;
} spell;

typedef struct spellNode {
    spell data;
    struct spellNode* next;
} spellNode;

void parse_json_file(const char* filename, Equipment* equipment);
void add_equipment(Inventory* inventory, Equipment* new_equipment);
void display_inventory(const Inventory* inventory);
double calculate_total_weight(const Inventory* inventory);
double calculate_total_value(const Inventory* inventory, const Coins* coins);
void parse_command_line(int argc, char* argv[], Inventory* inventory, double* max_weight, Coins* coins, char** camp_file);
void cycle(spellNode** head);
void push(spellNode** head, spell* new_spell);
spell* pop(spellNode** head);
void free_inventory(Inventory* inventory);

int main(int argc, char* argv[]) {
    Inventory inventory = {NULL, NULL};
    double max_weight = 0.0;
    Coins coins = {0, 0, 0, 0, 0};
    char* camp_file = NULL;

    Equipment Equipment;
    spellNode* spell_stack = NULL;

    parse_json_file("equipment.json", &Equipment);

    parse_command_line(argc, argv, &inventory, &max_weight, &coins, &camp_file);

    display_inventory(&inventory);

       // Display inventory
    display_inventory(&inventory);

    // Push spells onto the spell stack
    spell new_spell1 = {"Fireball", 50.0};
    push(&spell_stack, &new_spell1);

    spell new_spell2 = {"Heal", 30.0};
    push(&spell_stack, &new_spell2);

    // Cycle through the spell stack
    cycle(&spell_stack);

    spell* popped_spell = pop(&spell_stack);
    if (popped_spell) {
        printf("Popped Spell: Name: %s, Power: %.2f\n", popped_spell->name, popped_spell->power);
        free(popped_spell); // Free memory allocated for the popped spell
    } else {
        printf("Spell stack is empty.\n");
    }

    double total_weight = calculate_total_weight(&inventory);
    double total_value = calculate_total_value(&inventory, &coins);

    printf("Total Weight: %.2f\n", total_weight);
    printf("Total Value: %.2f\n", total_value);
    if (max_weight > 0 && total_weight > max_weight) {
        printf("You are encumbered!\n");
    }

    free_inventory(&inventory);
    free(camp_file);
    return 0;
}

// Function to parse JSON file and populate equipment structure
void parse_json_file(const char* filename, Equipment* equipment) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char buffer[1024]; // Adjust buffer size as needed
    char* token;

    while (fgets(buffer, sizeof(buffer), file)) {
        if ((token = strstr(buffer, "\"index\""))) {
            sscanf(token, "\"index\": \"%[^\"]\"", equipment->index);
        } else if ((token = strstr(buffer, "\"name\""))) {
            sscanf(token, "\"name\": \"%[^\"]\"", equipment->name);
        } else if ((token = strstr(buffer, "\"equipment_category\""))) {
            if ((token = strstr(buffer, "\"index\""))) {
                sscanf(token, "\"index\": \"%[^\"]\"", equipment->equipment_category_index);
            }
            if ((token = strstr(buffer, "\"name\""))) {
                sscanf(token, "\"name\": \"%[^\"]\"", equipment->equipment_category_name);
            }
            if ((token = strstr(buffer, "\"url\""))) {
                sscanf(token, "\"url\": \"%[^\"]\"", equipment->equipment_category_url);
            }
            if ((token = strstr(buffer, "\"vehicle_category\""))) {
                sscanf(token, "\"vehicle_category\": \"%[^\"]\"", equipment->vehicle_category);
            }
        } else if ((token = strstr(buffer, "\"cost\""))) {
            if ((token = strstr(buffer, "\"quantity\""))) {
                sscanf(token, "\"quantity\": %lf", &equipment->cost_quantity);
            }
            if ((token = strstr(buffer, "\"unit\""))) {
                sscanf(token, "\"unit\": \"%[^\"]\"", equipment->cost_unit);
            }
            if ((token = strstr(buffer, "\"weight\""))) {
                sscanf(token, "\"weight\": %lf", &equipment->cost_weight);
            }
            if ((token = strstr(buffer, "\"url\""))) {
                sscanf(token, "\"url\": \"%[^\"]\"", equipment->cost_url);
            }
        }
    }

    fclose(file);
}

void add_equipment(Inventory* inventory, Equipment* new_equipment) {
    new_equipment->next = NULL;
    if (!inventory->head) {
        inventory->head = new_equipment;
        inventory->tail = new_equipment;
    } else {
        inventory->tail->next = new_equipment;
        inventory->tail = new_equipment;
    }
}

// Function to display the inventory
void display_inventory(const Inventory* inventory) {
    if (!inventory->head) return;
    Equipment* current = inventory->head;
    do {
        printf("Name: %s, Weight: %.2f, Value: %.2f\n", current->name, current->weight, current->value);
        current = current->next;
    } while (current);
}

// Function to calculate the total weight of the inventory
double calculate_total_weight(const Inventory* inventory) {
    double total_weight = 0.0;
    if (!inventory->head) return total_weight;
    Equipment* current = inventory->head;
    do {
        total_weight += current->weight;
        current = current->next;
    } while (current);
    return total_weight;
}

//Function to calculate the total value of the inventory
double calculate_total_value(const Inventory* inventory, const Coins* coins) {
    double total_value = 0.0;
    if (!inventory->head) return total_value;
    Equipment* current = inventory->head;
    do {
        total_value += current->value;
        current = current->next;
    } while (current);
    total_value += coins->cp * 0.01 + coins->sp * 0.1 + coins->ep * 0.5 + coins->gp * 1 + coins->pp * 10;
    return total_value;
}

// Function to parse command line
void parse_command_line(int argc, char* argv[], Inventory* inventory, double* max_weight, Coins* coins, char** camp_file) {
int option;
while ((option = getopt(argc, argv, "w:m:c:")) != -1) {
switch (option) {
case 'w':
*max_weight = atof(optarg);
break;
case 'm':
sscanf(optarg, "%dcp %dsp %dep %dgp %dpp", &coins->cp, &coins->sp, &coins->ep, &coins->gp, &coins->pp);
break;
case 'c':
*camp_file = strdup(optarg);
break;
default:
fprintf(stderr, "Usage: %s equipment-files [number-of-items] [-w max-weight] [-m money] [-c camp-file]\n", argv[0]);
exit(EXIT_FAILURE);
}
}

for (int index = optind; index < argc; index++) {
    Equipment* new_equipment = (Equipment*)malloc(sizeof(Equipment));
    if (!new_equipment) {
        fprintf(stderr, "Memory allocation failed for equipment\n");
        exit(EXIT_FAILURE);
    }

    parse_json_file(argv[index], new_equipment);
    add_equipment(inventory, new_equipment);
}
}

void cycle(spellNode** head) {
if (!head || !*head) return;
*head = (*head)->next;
}

void push(spellNode** head, spell* new_spell) {
spellNode* new_node = (spellNode*)malloc(sizeof(spellNode));
if (!new_node) {
fprintf(stderr, "Memory allocation failed for spellNode\n");
exit(EXIT_FAILURE);
}
new_node->data = *new_spell;
new_node->next = *head;
*head = new_node;
}

spell* pop(spellNode** head) {
if (!head || !head) return NULL;
spellNode* temp = *head;
spell* popped_spell = (spell*)malloc(sizeof(spell));
if (!popped_spell) {
fprintf(stderr, "Memory allocation failed for spell\n");
exit(EXIT_FAILURE);
}
*popped_spell = temp->data;
*head = (*head)->next;
free(temp);
return popped_spell;
}

void free_inventory(Inventory* inventory) {
if (!inventory->head) return;
Equipment* current = inventory->head;
while (current) {
Equipment* next = current->next;
free(current);
current = next;
}
inventory->head = NULL;
inventory->tail = NULL;
}

//Flowchart: https://miro.com/app/board/uXjVKDegJEM=/
