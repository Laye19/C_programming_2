#include <stdio.h> //voor het printen van tekst
#include <stdlib.h> //voor malloc en free
#include <string.h> //voor het werken met strings
#include <getopt.h> //voor het parsen van command line arguments
#include <unistd.h> //voor getopt


#define MAX_ITEMS 100

// Structs voor deEquiments_items en de lijst
typedef struct {
    char naam[100];
    int aantal;
    float prijs;
} equipmentItem;

//Struct for representing a money amount
typedef struct {
    int cp;
    int sp;
    int gp;
    int pp;
    int ep;
} Coins;

//Struct for representing a node in a linked list
typedef struct node {
    equipmentItem item;
    struct node *next;
} node;

//Struct for representing the inventory
typedef struct {
    equipmentItem items[MAX_ITEMS];
    int count;
    float max_weight;
    Coins money;
    node *head; //Head of the linked list
} Inventory;

Inventory inventory = {{0}, 0, 0, {0}, NULL};

//Function prototypes
void push(node **head, equipmentItem item);
equipmentItem pop(node **head);
void displayInventory();
void displayLindedList(node *head);
void displayCoins(Coins coins);
void parseJsonFile(char *filename);
void moveItemToCamp(char *itemname);
void saveItemInventoryToFile(char *filename);
void loadItemInventoryFromFile(char *filename);

int main(int argc, char *argv[])
{
    int opt;
    float max_weight = 0;
    char* campFile = NULL;
    while ((opt = getopt(argc, argv, "w:c:")) != -1) {
        switch (opt) {
            case 'w':
                max_weight = atof(optarg); //convert string to float
                break;
            case 'm':
                //Parse coin options
                sscanf(optarg, "%dgp,%dsp,%dep,%dgp,%dpp", &inventory.money.cp, &inventory.money.sp, &inventory.money.gp, &inventory.money.pp, &inventory.money.ep);
                break;
            case 'c':
                campFile = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s [-w max_weight] [-c campFile]\n", argv[0]); //print usage
                exit(EXIT_FAILURE); //exit with error
        }
    }

    //load equipment from file and populate the inventory
    for(int i = optind; i < argc; i++) {
        parseJsonFile(argv[i]);
    }

    //display the inventory
    printf("Inventory:\n");
    displayInventory( inventory.head);

    //display the linked list
    printf("Linked List:\n");
    displayLindedList(inventory.head);

    //display the coins
    printf("Coins:\n");
    displayCoins(inventory.money);

    //Save the inventory to a file
    if(campFile != NULL) {
        saveItemInventoryToFile(campFile);
    }

    //Clean up memory
    node *current = inventory.head;
    while(current != NULL) {
        node *next = current->next;
        free(current);
        current = next;
    }


    return 0;
}

void push(node **head, equipmentItem item) {
    node *new_node = (node *)malloc(sizeof(node));
    new_node->item = item;
    if(*head == NULL) {
        *head = new_node;
        (*head)->next = *head;
    }
    else {
        node *current = *head;
        while(current->next != *head) {
            current = current->next;
        }
        current->next = new_node;
        new_node->next = *head;
    }
}

equipmentItem pop(node **head) {
    if(*head == NULL){
        printf("Inventory is empty\n");
        exit(EXIT_FAILURE);
    }

    equipmentItem item = (*head)->item;
    if((*head)->next == *head) {
        free(*head);
        *head = NULL;
    }
    else {
        node* temp = *head;
        node *current = *head;
        while(current->next->next != *head) {
            current = current->next;
        }
        *head = (*head)->next;
        current->next = *head;
        free(temp);
    }
    return item;
}

void displayInventory(node* head){
    node *current = inventory.head;
    do{
        printf("Naam: %s, Aantal: %d, Prijs: %.2f\n", current->item.naam, current->item.aantal, current->item.prijs);
        current = current->next;
    } while(current != head);
}

void displayCoins(Coins coins) {
    printf("cp: %d, sp: %d, ep: %d, gp: %d, pp: %d\n", coins.cp, coins.sp, coins.ep, coins.gp, coins.pp);
}

void displayLindedList(node *head) {
    node *current = head;
    while(current != NULL) {
        printf("Naam: %s, Aantal: %d, Prijs: %.2f\n", current->item.naam, current->item.aantal, current->item.prijs);
        current = current->next;
        if(current == head) {
            break;
        }
    }
}

void moveItemToCamp(char *itemname) {
    node *current = inventory.head;
    node *prev = NULL;
    while(current != NULL) {
        if(strcmp(current->item.naam, itemname) == 0) {
            if(prev == NULL) {
                inventory.head = current->next;
            }
            else {
                prev->next = current->next;
            }
            push(&inventory.head, current->item);
            break;
        }
        prev = current;
        current = current->next;
        if(current == inventory.head) {
            break;
        }
    }
}


