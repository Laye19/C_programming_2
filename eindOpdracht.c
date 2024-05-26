#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>


#define MAX_LIST_SIZE (100) // Define the maximum list size
// Custom boolean type
typedef enum { FALSE, TRUE } Bool;
// Custom structure for any key
struct KeyInfo {
    uint8_t level;
    char* Value;
};

enum GeometryComponents { VOLUME, SURFACE, MASS };// Enumerated components
enum ShapeType { SPHERE, CONE, CYLINDER, LINE, CUBE }; // Add more shapes if needed

// Custom strsep function
char *myCustomStrsep(char **stringPtr, const char *delimiter) {
    char *result = *stringPtr;
    if (result) {
        *stringPtr += strcspn(*stringPtr, delimiter); // Skip leading delimiters if any 
        if (**stringPtr)
            *(*stringPtr)++ = '\0';
        else
            *stringPtr = NULL;}
    return result;
}

// Custom structure for shape informat
 struct shape{ 
    enum ShapeType shapeType; // Shape type (SPHERE, CONE, CYLINDER, LINE, CUBE)
    int size;  // Size of the shape (e.g. radius, height, length, etc.) 
 };

struct AttackType{ // Custom structure for attack type information 
    char* name; // Name of the attack type 
    char* index; // Index of the attack type     
    char* url; // URL of the attack type 
};

// Custom structure for a list of attack types
enum AttackTypeList{attack_at_characterelevel, attack_at_slot_level}; // Enumerated attack types 

 struct attack{ 
    enum AttackTypeList AttackTypeList; // Attack type information 
    struct KeyInfo *keyvalues; // Pointer to the next attack type 
 };

 struct response{ // Custom structure for response information 
    char *name; // Name of the response 
    char *index; // Index of the response 
    char *url; // URL of the response
};

struct responseList{ // Custom structure for a list of responses 
    struct response response_type; // Response information 
    char* rep_succes; // Pointer to the next response 
};

// Custom structure for a list of shapes
struct game{
    char* name; // Name of the game
    char* index; // Index of the game
    char* url; // URL of the game
    char** describtion; // Description of the game
    uint8_t describtion_count; // Number of descriptions
    char** highScore; // Highscore of the game
    uint8_t highScore_count; // Number of highscores
    char* range; // Range of the game
    uint8_t componentsCounter; // Number of components
    enum GeometryComponents components[MAX_LIST_SIZE]; // List of components
    char* material; // Material of the game 
    struct attack* attack; // Attack information 
    char* defense; // Defense information
    char* speed; // Speed information
    struct shape* shape; // Shape information 
    struct response* classes; // Response information
    struct game* next; // Pointer to the next game

};


struct gameNode{ // Custom structure for a list of games
    struct game* data; // Game information
    struct gameNode* next; // Pointer to the next game
};

//function prototypes
void push(struct gameNode** head, struct game* game); // push a game to the list of games ***1
void printGames(struct gameNode* head); // print the list of games ***2
void freeGames(struct gameNode* head); // free the list of games ***3 
//void addShape(struct shape** head, struct shape* shape); // push a shape to the list of shapes ***13
struct game* popGame(struct gameNode** head); // pop a game from the list of games*** 4
void printfGameOptions(struct gameNode* head, char* optionstring); // print the game options ***5
void freeList(struct gameNode* head); // free the list of games ***6
void cycleGame(struct gameNode** head); // cycle through the list of games ***7
void jsonParse(FILE* filePointer, struct game*); // parse the json string ***8
uint8_t parseArray(FILE* filePointer, char*** element ); // parse the array ***9 
uint8_t parseEnumGeometryComponentsArray(FILE* filePointer, enum GeometryComponents** element); // parse the enum geometry components array ***10
uint8_t parseKeyInfoArray(FILE* filePointer, struct KeyInfo** KeyInfo); // parse the key info array ***11
void parseNameUrlIndex(FILE* filePointer, char** name, char** url, char** index); // parse the name url index ***13
uint8_t parseNameUrlIndexArray(FILE* filePointer, struct responseList** classes); // parse the name url index array ***12


int main(int argc, char *argv[]) 
{
    struct gameNode* gameList = NULL; // Initialize the list of games
    uint8_t characterLevel = 0; // Initialize the counter
    char* filename; // Replace with your JSON file
    uint8_t amontOfGamesClose [9] = {0}; // Initialize the counter for the amount of games close to the character level 
    char* nameHistoryFile; // Name of the history file
    uint8_t writeLogFiles = 0; // Initialize the counter for the log files      

    // Parse the command line arguments
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-") == 0) { // Check if the argument is a flag 
            if(strcomp(argv[i + 1], "s") == 0) { // Check if the flag is -f
                i++; // Increment the counter
            for(int j = 0; j < 9; j++) { // Loop through the amount of games close to the character level
                if(isdigit(argv[i + j]) == 0){ // Check if the argument is a digit
                    amontOfGamesClose[j] = atoi(argv[i + j]); // Parse the amount of games close to the character level
                    i++; // Increment the counter
                }else{
                break; // Break the loop
              }
            }
           
        } if(strcmp(argv[i + 1], "l") == 0) { // Check if the flag is -l
            characterLevel = atoi(argv[i + 2]); // Parse the character level
            i++; // Increment the counter
        } else if(strcmp(argv[i + 1], "h") == 0) { // Check if the flag is -h
            nameHistoryFile = argv[i + 1]; // Set the name of the history file
            writeLogFiles = 1; // Set the counter for the log files
            i++; // Increment the counter
        } 
        }else {
            struct game* currentGame = (struct game*)Calloc(1, sizeof(struct game)); // Allocate memory for the game
            char* filename = calloc(sizeof(argv[i])/sizeof(char)+7, sizeof(char)); // Allocate memory for the filename
            strcpy(filename, "JSON/"); // Copy the filename to the JSON folder 
            strcat(filename, argv[i]); // Concatenate the filename
        }
    }
   
    return 0;
}



