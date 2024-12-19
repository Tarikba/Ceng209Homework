# Dungeon Adventure
A simple command-based dungeon adventure game written with c programming language. This project is made for Ceng209 Fall 2024 Homework.
## How to setup?
1. * First, open your terminal
2. * Clone this repository
```
git clone <https://github.com/Tarikba/DungeonAdventure>
```
2. * Go to the directory
```
cd DungeonAdventure
```
3. * Use Makefile (You can use with mignw32 to ensure, if you use Windows)
```
mingw32-make
```
4. * Run the game
```
.\DungeonAdventure
```
## About game
  Dungeon Adventure simulates basic features of text-based adventure games that are popular in the 20th century.
The main goal of this game is to escape from the dungeon we are in. There are two exits in the dungeon.
Players should find one of these exits to complete the game by wandering through rooms and checking them.
During this; they will encounter creatures, find unique items, and maybe confront with a fierce dragon.

## How to play
  If you installed the game successfully, you can play now.   
Since this is a command line based game, you should play this game on a terminal.
* When you run the game, menu screen of the game should be opened.
```
DUNGEON ADVENTURE
>>
```
* By typing `menu`, you can see the menu commands.
```
>> menu
1. start - Starts the game        
1. menu  - Shows menu commands    
3. keys  - Shows game instructions
4. exit  - Exits from game
```
* Type `start` to begin your journey
Game should be started. After that you should use game commands
n the game, there are 7 game commands to play the game.
(You can see these commands with explanation if you type `keys` on the menu srceen)
These commands are:

* `look` is used to see room descriptions and to check if there is any creature or item.
```
>> look
This is where all begins
It seems safe

```
* `pickup` to pick up an item if it exists within room.
```
>> pickup
Item added to inventory
```
* Use `inventory` to see your items.
```
>> inventory
Inventory:
1. Potion
(You can have 4 more items)
Select item (enter 0 to close bag)
>>
```
* While in your bag, you can select an item by entering its number on the left. Some items give you buffs.
(you can exit from inventory by typing `0`)
```
Inventory:
1. Potion
(You can have 4 more items)
Select item (enter 0 to close bag)
>> 1
Potion: A potion made of medicinal herbs and some magic
Hp is increased to 120
```
**Warning:** You can have at most 5 items. Use your bag wisely.

* `map` gives you a simple visualization of the dungeon. For instance, map of game at the beginning is: 
```
>> map
###0####
0C01C0C0
###CCCC0
You are here
```
  * `1` : player
  * `0` : empty rooms
  * `C` : creatures (when a room is "cleaned", it will be `0`)
  * `#` : walls
**Warning:** Some rooms might have no connection in each other. 
**Warning:** Empty room doesn't mean there is no item, there might be.

* You can go other rooms with `move` command. Its syntax is `move <direction>`.
<direction> is one of the characters from WASD. As an example:
```
>> move d
```
That means "go right". Other ways are:
  * `w`: go up
  * `a`: go left
  * `s`: go down
**Warning:** If you type a way room has no connection (or any nonexisting way/typo)
You will see this message:
```
>> move s
It is blocked here. You shall not pass!

>> move imadeupthis
It is blocked here. You shall not pass!
```
* If there is a creature in the room, you can fight it with `attack`.
```
>> look
You can barely see around and hear some creatures clattering
You see a creature!

>> attack
```
When you start a battle, it will be seem like:
```
>> attack
Battle begins!!!
>>
```
With `attack` command, you can start a fight, as well as hit the opponents. (It has two functions)
```
>> attack
Battle begins!!!
>> attack
You hit the creature
Creature's hp decreased to 60
You've been hit by creature
You have 95 hp remained
Your turn
>>
```
Battles are turned-based, a battle ends when one of the sides is defeated.
**Warning:** If you don't `attack` properly, you only take damage:
```
Your turn
>> attackannytypo
You've been hit by creature
You have 80 hp remained
Your turn
>>
```
Be careful to that!
When a battle ends and you win, you will return to the game screen such as:
```
Your turn
>> attack
You hit the creature
Creature's hp decreased to 0
Creature defeated

>> look
You can barely see around and hear some creatures clattering
It seems safe
```
And if you lose:
```
You've been hit by creature
You have 0 hp remained
You died

DUNGEON ADVENTURE
>>
```
**Warning:** If you kill a creature for the first time in a game, all creatures you will encounter until then will attack you first.
This is called "First Blood Mechanism". A showcase what will happen when you drew the first blood:
```
>> attack
You hit the creature
Creature's hp decreased to 0
Creature defeated

>> move d
This cave is crawling with creatures, be careful!
Watch out!! A creature is approaching you
Battle begins!!!
You've been hit by creature
You have 80 hp remained
Your turn
>>
```
* The last but the not least, you can go back to game menu by terminating current game with `quit`
```
>> quit

DUNGEON ADVENTURE
>>
```
