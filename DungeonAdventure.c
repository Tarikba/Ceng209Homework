//Hasan Tarik Bakar, 22050111007

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "structures.h"

#define DEFAULT_INPUT_LENGHT 32
#define ROW_LENGTH 3
#define COLUMN_LENGTH 8
typedef enum game_statuses {COMPLETED, FAILED, RUNNING, GAMEOUT} game_status;
game_status currentStatus;

Room* currentRoom;
Room* roomMap[ROW_LENGTH][COLUMN_LENGTH];
Item* itemLocations[ROW_LENGTH][COLUMN_LENGTH];
Player* player;
Creature* roomEnemy;
Item* itemOfRoom;
static bool firstBlood = false;

// Function prototypes
game_status runGame(void);
char* getInput(void);
void menuParser(char* menuInput);
void inGameParser(char* commandInput);
void createMap(void);
Room* createRoom(int x, int y, bool gD, bool gL, bool gR, bool gU, bool hC, bool hI, const char* rM, const char* rI);
Player* createPlayer(void);
Creature* createCreature(void);
Creature* createDragon(void);
Item* createItem(const char* name, item_type type, int value, const char* details);
void pickupItem(void);
void useItem(int itemIndex);
void openBag(void);
void addItem(Room* room, Item* item);
void move(char way);
void hit(Creature* crtr, Player* plyr);
void getHitBy(Creature* crtr, Player* plyr);
void battle(Player* plyr, Creature* crtr);
void printMap(void);
void showMenu(void);
void instructions(void);


int main() {
    char* currentMenuCmd;
    printf("DUNGEON ADVENTURE");

    while(1) { 
        currentMenuCmd = getInput();
        menuParser(currentMenuCmd);
    }

    return 0;
}

game_status runGame(void) {
    currentStatus = RUNNING;
    createMap();
    char* currentGameCmd;
    player = createPlayer();
    
    sleep(1);
    printf("You woke up...\n");
    sleep(1.5);
    printf("... and you realized you are inside of a dungeon.\n");
    sleep(1.5);
    printf("%s", currentRoom->roomMessage);
    while (currentStatus == RUNNING) {
        currentGameCmd = getInput();
        inGameParser(currentGameCmd);
        if(!(currentRoom->goesDown) && !(currentRoom->goesLeft) && !(currentRoom->goesRight) && !(currentRoom->goesUp)) { //if currentRoom is an exit
            firstBlood = false;
            currentStatus = COMPLETED;
            sleep(1);
            printf("Your journey ends here for now\n"); sleep(1);
            printf("\nDUNGEON ADVENTURE");
        }
    }

    return COMPLETED;
}

char* getInput() {
    char* input = malloc(DEFAULT_INPUT_LENGHT * sizeof(char));
    while(1) {
        printf("\n>> ");
        fgets(input, DEFAULT_INPUT_LENGHT, stdin);
        input[strcspn(input, "\n")] = 0;
        return input;
    }
}

void menuParser(char* menuInput) {
    char menuCommand[DEFAULT_INPUT_LENGHT];
    sscanf(menuInput, "%s %[^\n]", menuCommand);

    if(strcmp(menuCommand, "exit") == 0) exit(EXIT_SUCCESS);
    else if (strcmp(menuCommand, "menu") == 0) showMenu();
    else if (strcmp(menuCommand, "keys") == 0) instructions();
    else if (strcmp(menuCommand, "start") == 0) runGame();
    else {
        printf("Enter valid command!");
        char* failedInput = getInput();
        menuParser(failedInput);
        free(failedInput);
    }
}

void inGameParser(char* commandInput) {
    char gameCommand[DEFAULT_INPUT_LENGHT];
    char additionalCmd[DEFAULT_INPUT_LENGHT];
    sscanf(commandInput, "%s %[^\n]", gameCommand, additionalCmd);

    if(strcmp(gameCommand, "attack") == 0) {
        if(currentRoom->hasCreature) roomEnemy = createCreature();
        battle(player, roomEnemy);
    }
    else if(strcmp(gameCommand, "look") == 0) {
        printf("%s\n", currentRoom->roomInfo);
        sleep(1);
        if (currentRoom->hasCreature ? printf("You see a creature!\n") : printf( "It seems safe\n"));
        sleep(1);
        if (currentRoom->hasItem && !(currentRoom->hasCreature)) printf("Oh, look! You found something\n");
    }
    else if(strcmp(gameCommand, "inventory") == 0) openBag();
    else if(strcmp(gameCommand, "pickup") == 0) pickupItem();
    else if(strcmp(gameCommand, "map") == 0) printMap();
    else if(strcmp(gameCommand, "move") == 0) {
        char direction = additionalCmd[0];
        move(direction);

        //boss domain
        if(currentRoom->x == 1 && currentRoom->y == 1 && currentRoom->hasCreature) {
            roomEnemy = createDragon();
            sleep(1);
            if (firstBlood) printf("Guess this is the end...\n");
            sleep(1);
            battle(player, roomEnemy);
        }
        else if(currentRoom->hasCreature && firstBlood) {
            sleep(1);
            printf("Watch out!! A creature is approaching you\n");
            sleep(1);
            if(currentRoom->hasCreature) roomEnemy = createCreature();
            battle(player, roomEnemy);
        }
    }
    else if (strcmp(gameCommand, "quit") == 0) {
        firstBlood = false;
        currentStatus = GAMEOUT;
        printf("\nDUNGEON ADVENTURE");
    }
    else {
        printf("Enter valid command!");
        char* failedInput = getInput();
        inGameParser(failedInput);
        free(failedInput);
    }
}

void createMap(void) {
    //      room                    x/ y/ down/ left/ right/ up/ creature/ item/  message/                                             info
    Room* startingRoom = createRoom(1, 3, true, true, true, true, false, false, "It seems this place is the middle of a crossroads", "This is where all begins");
    
    Room* potionRoom = createRoom(0, 3, true, false, false, false, false, true, "You found an abandoned alchemy laboratory", "You see old cauldrons, broken glass bottles and some bones");

    Item* potion = createItem("Potion", HPBUFF, 20, "A potion made of medicinal herbs and some magic");
    addItem(potionRoom, potion);

    Room* dragonHallWay = createRoom(1, 2, false, true, true, false, false, true, "You started to feel it's getting hotter inside", "You see several valuables scattered around\n and wonder if there are more ahead");

    //      item                  name/       type/ value/   detail   
    Item* goldCoin = createItem("Gold Coin", TRIVIAL, 100, "A gold coin worth 100 units, might be a part of a great treasure");
    addItem(dragonHallWay, goldCoin);

    Room* dragonHall = createRoom(1, 1, false, true, true, false, true, true, "You see massive amounts of gold, jewelry, chests...\nand a DRAGON!!!", "Fly, you fool!");
    
    Item* treasure = createItem("Great Treasure", TRIVIAL, INT_MAX, "The great treasure once guarded by the dragon.\nThis is the highest wealth one can attain in this realm");
    addItem(dragonHall, treasure);

    Room* leftExit = createRoom(1, 0, false, false, false, false, false, false, "You see a giant gate leads to outer world.\nYou think no one will believe what just happened", "GG");

    Room* rightHallWay1 = createRoom(1, 4, false, true, true, false, true, false, "You are going through a dim hallway,\n yet it's getting brighter as you are getting closer", "You can barely see around and hear some creatures clattering");

    Room* rightHallWay2 = createRoom(1, 5, false, true, true, false, false, true, "You are going through a dim hallway,\nyet it's getting brighter as you are getting closer", "You can barely see around and hear some creatures clattering");

    Item* sword  = createItem("Rusty Sword", ATKBUFF, 5, "An old, rusty sword. Once the guide of the brave,\nnow... not half bad");
    addItem(rightHallWay2, sword);

    Room* rightHallWay3 = createRoom(1, 6, false, true, true, false, true, false, "You are going through a dim hallway,\nBut this time light is brighter than ever", "You can barely see around and hear some creatures clattering");

    Room* rightExit = createRoom(1, 7, false, false, false, false, false, false, "The light coming from the exit ahead is blinding you.\nYou are free now!", "GG");

    Room* caveMouth = createRoom(2, 3, false, false, true, true, true, true, "You found a cave, and thought what is inside", "Inside of cave seems uncanny\nBut you feel there must be something worth it");

    Item* spellBook = createItem("C-pellbook", TRIVIAL, 0, "An ancient spellbook about ancient arts\nThere is a big blue 'C' letter on it\nIt will take time to master");
    addItem(caveMouth, spellBook);

    Room* cave1 = createRoom(2, 4, false, true, true, false, true, false, "This cave is crawling with creatures, be careful!", "It's so narrow and stuffy,\n and you can feel the creatures lurking in the shadows");

    Room* cave2 = createRoom(2, 5, false, true, true, false, true, false, "This cave is crawling with creatures, be careful!", "It's so narrow and stuffy,\n and you can feel the creatures lurking in the shadows");

    Room* cave3 = createRoom(2, 6, false, true, true, false, true, false, "This cave is crawling with creatures, be careful!", "It's so narrow and stuffy,\n and you can feel the creatures lurking in the shadows");
    
    Room* endOfCave = createRoom(2, 7, false, true, false, false, false, true, "You reached end of the cave", "You can feel the divine aura of the room");

    Item* armor = createItem("Slayer Armor", BOTHBUFF, 150, "A sacred armor that makes its wearer almost invincible\nMoreover, it makes one immune to dragons");
    addItem(endOfCave, armor);

    currentRoom = startingRoom;
}

Room* createRoom(int x, int y, bool gD, bool gL, bool gR, bool gU, bool hC, bool hI, const char* rM, const char* rI) {
    Room* room = malloc(sizeof(Room));
    room->x = x;
    room->y = y;

    room->goesDown = gD;
    room->goesLeft = gL;
    room->goesRight = gR;
    room->goesUp = gU;

    room->hasCreature = hC;
    room->hasItem = hI;
    room->roomMessage = rM;
    room->roomInfo = rI;

    roomMap[room->x][room->y] = room;
    return room;
}

Player* createPlayer() {
    Player* player = malloc(sizeof(Player));
    player->hp = 100;
    player->dmg = 20;
    player->itemCount = 0;
    return player;
}

Creature* createCreature() {
    if(currentRoom->hasCreature) {
        Creature* creature = malloc(sizeof(Creature));
        creature->c_hp = 80;
        creature->c_dmg = 5;
        creature->isAlive = 1;
        return creature;
    }
    return NULL;
}

Creature* createDragon() {
    if(currentRoom->x == 1 && currentRoom->y == 1 && currentRoom->hasCreature) {
        Creature* dragon = malloc(sizeof(Creature));
        dragon->c_dmg = 250;
        dragon->c_hp = 195;
        dragon->isAlive = true;
        return dragon;
    }
}

Item* createItem(const char* name, item_type type, int value, const char* details) {
    Item* item = malloc(sizeof(Item));
    item->itemName = name;
    item->itemType = type;
    item->itemValue = value;
    item->itemDetails = details;
    return item;

}

void useItem(int itemIndex) {
    Item* selectedItem = player->bag[itemIndex - 1];
    printf("%s: %s\n", selectedItem->itemName, selectedItem->itemDetails);
    if(selectedItem->isUsed) printf("(This item has been used)\n");
    else {
        if(selectedItem->itemType == HPBUFF) {
        player->hp += selectedItem->itemValue;
        printf("Hp is increased to %d\n", player->hp);
        selectedItem->itemValue = 0;
        selectedItem->isUsed = true;
    } else if(selectedItem->itemType == ATKBUFF) {
        player->dmg += selectedItem->itemValue;
        printf("Attack is increased to %d\n", player->dmg);
        selectedItem->itemValue = 0;
        selectedItem->isUsed = true;
    } else if(selectedItem->itemType == BOTHBUFF) {
        player->dmg += selectedItem->itemValue;
        player->hp += selectedItem->itemValue;
        printf("Attack is increased to %d\n", player->dmg);
        printf("Hp is increased to %d\n", player->hp);
        selectedItem->itemValue = 0;
        selectedItem->isUsed = true;
    } else {
        printf("Item: %s\nValue: %d\n", selectedItem->itemName, selectedItem->itemValue);
    }
    }
}

void openBag() {
    printf("Inventory:\n");
    if(player->itemCount == 0) printf("Wow, look! Nothing!\n");
    else {
        for(int i = 0; i < player->itemCount; i++) {
        printf("%d. %s\n", i + 1, player->bag[i]->itemName);
    }
    printf("(You can have %d more items)\n", INVENTORY_CAPACITY - player->itemCount); //shows remaining capacity
    printf("Select item (enter 0 to close bag)");
    char* indexInput = malloc(DEFAULT_INPUT_LENGHT * sizeof(char));
    int itemIndex;
    printf("\n>> ");
    fgets(indexInput, DEFAULT_INPUT_LENGHT, stdin);
    indexInput[strcspn(indexInput, "\n")] = 0;
    itemIndex = atoi(indexInput);
    free(indexInput);
    if(itemIndex == 0) printf("Bag is closed\n");
    else if(0 < itemIndex && itemIndex < INVENTORY_CAPACITY + 1) useItem(itemIndex);
    }
}

void pickupItem() {
    if (currentRoom->hasItem && !(currentRoom->hasCreature)) {
        if(player->itemCount < INVENTORY_CAPACITY) {
            Item* foundItem = itemLocations[currentRoom->x][currentRoom->y];
            Item* newItem = malloc(sizeof(Item));
            newItem->itemName = malloc(strlen(foundItem->itemName) + 1);
            strcpy((char*)newItem->itemName, (char*)foundItem->itemName);
            newItem->itemDetails = malloc(strlen(foundItem->itemDetails) + 1);
            strcpy((char*)newItem->itemDetails, (char*)foundItem->itemDetails);
            newItem->itemValue = foundItem->itemValue;
            newItem->itemType = foundItem->itemType;
            player->bag[player->itemCount] = newItem;
            player->itemCount++;
            
            currentRoom->hasItem = false; //item is taken by player
            if(!(currentRoom->hasItem) && (itemLocations[currentRoom->x][currentRoom->y] != NULL)) 
            itemLocations[currentRoom->x][currentRoom->y] = NULL; 
            free(foundItem);
            printf("Item added to inventory\n");
        } else printf("Your bag is full, you can't have this item unfortunately\n");
    } else printf("You found nothing to obtain\n");
}

void addItem(Room* room, Item* item) {
    if(room->hasItem) itemLocations[room->x][room->y] = item;
    else itemLocations[room->x][room->y] = NULL;
}

void move(char way) {
    if (currentRoom->goesLeft && way == 'a') {
        currentRoom = roomMap[currentRoom->x][currentRoom->y - 1];
        printf("%s\n", currentRoom->roomMessage);
    } 
    else if (currentRoom->goesRight && way == 'd') {
        currentRoom = roomMap[currentRoom->x][currentRoom->y + 1];
        printf("%s\n", currentRoom->roomMessage);
    } 
    else if (currentRoom->goesUp && way == 'w') {
        currentRoom = roomMap[currentRoom->x - 1][currentRoom->y];
        printf("%s\n", currentRoom->roomMessage);
    } 
    else if (currentRoom->goesDown && way == 's') {
        currentRoom = roomMap[currentRoom->x + 1][currentRoom->y];
        printf("%s\n", currentRoom->roomMessage);
    } 
    else {
        printf("It is blocked here. "); sleep(1);
        printf("You shall not pass!\n"); sleep(1);
    }
}

void hit(Creature* crtr, Player* plyr) {
    char* strike = malloc(DEFAULT_INPUT_LENGHT * sizeof(char));
    printf("\n>> ");
    fgets(strike, DEFAULT_INPUT_LENGHT, stdin);
    strike[strcspn(strike, "\n")] = 0;

    if(strcmp(strike, "attack") == 0) { //typo may be mortal
        crtr->c_hp -= plyr->dmg;
        if(crtr->c_hp < 0) crtr->c_hp = 0;
        printf("You hit the creature\n"); sleep(1);
        printf("Creature's hp decreased to %d\n", crtr->c_hp);
    }
}

void getHitBy(Creature* crtr, Player* plyr) {
    plyr->hp -= crtr->c_dmg;
    if(plyr->hp < 0) plyr->hp = 0;
    printf("You've been hit by creature\n"); sleep(1);
    printf("You have %d hp remained\n", plyr->hp);
}

void battle(Player* plyr, Creature* crtr) {
    if(currentRoom->hasCreature && roomEnemy->isAlive) {
        printf("Battle begins!!!"); sleep(1);
        while(plyr->hp > 0 && crtr->c_hp > 0) {
            if (firstBlood) {
                printf("\n");
                while(plyr->hp > 0 && crtr->c_hp > 0) {
                    getHitBy(crtr, plyr);
                    if(plyr->hp <= 0) break;
                    printf("Your turn");
                    hit(crtr, plyr);
                    if(crtr->c_hp <= 0) break;
                    sleep(1.5);
                }
            } else {
                while(plyr->hp > 0 && crtr->c_hp > 0) {
                    hit(crtr, plyr);
                    if(crtr->c_hp <= 0) break;
                    sleep(1.5);
                    getHitBy(crtr, plyr); 
                    if(plyr->hp <= 0) break;
                    printf("Your turn");
                }
                firstBlood = true;
            }
        }
        if(plyr->hp <= 0) {
            printf("You died\n");
            firstBlood = false;
            currentStatus = FAILED;
            printf("\nDUNGEON ADVENTURE");
        } else if(crtr->c_hp <= 0) {
            printf("Creature defeated\n");
            crtr->isAlive = false;
            currentRoom->hasCreature = false;
            free(crtr);
        }
    } else printf("There is no creature in this room\n"); sleep(1);
    
}

void printMap() {
    for(int i = 0; i < ROW_LENGTH; i++) {
        for(int j = 0; j < COLUMN_LENGTH; j++) {
            char location = '0';
            if(roomMap[i][j] == NULL) location = '#';
            else{
                if(roomMap[i][j]->hasCreature) location = 'C';
                if(i == currentRoom->x && j == currentRoom->y) location = '1';
            }
            printf("%c", location);
        }
        printf("\n");
    }
    printf("You are here\n");
}

void showMenu() {
    printf("1. start - Starts the game\n");
    printf("1. menu  - Shows menu commands\n");
    printf("3. keys  - Shows game instructions\n");
    printf("4. exit  - Exits from game\n");
}

void instructions() {
    printf("\nHow to play:");
    printf("\n1. move + (one of WASD) - move through a direction");
    printf("\n          ( w: up, s: down, a: left, d: right)");
    printf("\n2. look - gives you a brief description");
    printf(" \n3. map - shows your location and existing creatures on a simple map");
    printf("\n4. attack - starts a fight against creatures you encountered");
    printf("\n5. pickup - adds item to your bag");
    printf("\n6. inventory - shows what you have in your bag");
    printf("\n7. quit - close the game and back to menu");
}