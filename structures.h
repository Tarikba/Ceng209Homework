//Hasan Tarik Bakar, 22050111007

#define INVENTORY_CAPACITY 5 //use wisely
typedef enum item_type {HPBUFF, ATKBUFF, BOTHBUFF, TRIVIAL} item_type;

typedef struct Room {
    int x;
    int y;

    const char* roomMessage;
    const char* roomInfo;
    bool hasCreature;
    bool hasItem;

    bool goesUp;
    bool goesDown;
    bool goesLeft;
    bool goesRight;
} Room;

typedef struct Item {
    const char* itemName;
    item_type itemType;
    int itemValue;
    const char* itemDetails;
    bool isUsed;
} Item;

typedef struct Player {
    int hp;
    int dmg;
    Item* bag[INVENTORY_CAPACITY];
    int itemCount;
} Player;

typedef struct Creature {
    int c_hp;
    int c_dmg;
    bool isAlive;
} Creature;