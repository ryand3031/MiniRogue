// Game.cpp

#include "Game.h"
#include "utilities.h"
#include "Actor.h"
#include "GameObject.h"
#include "Room.h"
#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

// Implement these and other Game member functions you may have added.

//constructor
//sets current level to 0 to start the game
Game::Game(int goblinSmellDistance): m_goblinSmellDistance(goblinSmellDistance), m_level(0)
{
     m_inventory.push_back(m_player.weapon());//puts the players current weapon into the players inventory
}
//destructor
Game::~Game() {//deletes all newed values in the vectors with pointers
    clearLevel();//clears m_monsters and m_gameObjects, deleting all of the gameObjects and Monsters stored inside
    for (size_t i = 0; i < m_inventory.size(); i++) {//loops through inventory vector
        if (m_inventory.at(i) != m_player.weapon()) {//don't delete the players weapon
            delete m_inventory.at(i);//deletes the weapon at the address of the current pointer
        }
    }
    m_inventory.clear();
}

//trivial getters
Player* Game::player() { return &m_player; }
int Game::goblinSmellDistance() const{ return m_goblinSmellDistance; }
//sets char of dungeon at the given row and col
void Game::setChar(int r, int c, char identifier) { m_dungeon[r][c] = identifier; }

//runs the game
void Game::play()
{
    newLevel();//sets the initial level
    char key = '/';//case that will never be used
    bool gameOver = false;//keeps track of whether the game is over or not
    while (key != 'q' && !gameOver) {//runs the players turn as long as q is not pressed and game is not over
        m_player.regenerate();//possibly regenerates players health by 1 at the start of turn
        display();//displays the board and output string(s) if any

        key = getCharacter();//takes the players input
        if (m_player.sleepTime() > 0) {//If player is asleep, decrease their sleep time by one and skip player turn
            m_player.setSleepTime(m_player.sleepTime() - 1);
        }
        else {
            Actor* monster;//monster pointer
            GameObject* gameObject;//gameObject pointer
            switch (key) {//checks the players input
            case ARROW_UP://move up
                monster = ActorInLocation(m_player.row() - 1, m_player.col());//checks for the actor above the player
                if (monster != nullptr) {//if the space above the player occupied by a monster, attack the monster
                    m_outputList.push(m_player.attack(monster));
                }
                else if (m_dungeon[m_player.row() - 1][m_player.col()] != '#') {//if the space is not occupied by a monster or a wall move the player up
                    m_player.moveUp();

                }
                break;
            case ARROW_DOWN://move down
                monster = ActorInLocation(m_player.row() + 1, m_player.col());//checks for the actor below the player
                if (monster != nullptr) {
                    m_outputList.push(m_player.attack(monster));
                }
                else if (m_dungeon[m_player.row() + 1][m_player.col()] != '#') {//if the space is not occupied by a monster or a wall move the player down
                    m_player.moveDown();
                }
                break;
            case ARROW_RIGHT://move right
                monster = ActorInLocation(m_player.row(), m_player.col() + 1);//checks for the actor to right of the player
                if (monster != nullptr) {
                    m_outputList.push(m_player.attack(monster));
                }
                else if (m_dungeon[m_player.row()][m_player.col() + 1] != '#') {//if the space is not occupied by a monster or a wall move the player right
                    m_player.moveRight();
                }
                break;
            case ARROW_LEFT://move left
                monster = ActorInLocation(m_player.row(), m_player.col() - 1);//checks for the actor to left of the player
                if (monster != nullptr) {
                    m_outputList.push(m_player.attack(monster));
                }
                else if (m_dungeon[m_player.row()][m_player.col() - 1] != '#') {//if the space is not occupied by a monster or a wall move the player left
                    m_player.moveLeft();
                }
                break;
            case 'c'://if key pressed is c run cheat method setting player health to 50 and strength to 9
                m_player.cheat();
                break;
            case '>'://next level
                if (m_level < 4 && m_player.row() == m_exitRow && m_player.col() == m_exitCol) {//if the level is not level 4 and the player presses'>' while on the exit row and collumn
                    m_level++;//increment level
                    newLevel();//set next level
                    continue;//skips back to the start of the players turn to start the level
                }
                break;
            case 'g'://grabbing items
                gameObject = ObjectInLocation(m_player.row(), m_player.col());//gets the gameObject at the players location
                if (gameObject != nullptr) {//checks if there is an item under the player
                    if (gameObject->identifier() == '&') {//chekcs if the player pickeud up the golden idol
                        m_outputList.push(gameObject->pickup());//picks up the golden idol
                        gameOver = true;
                    }
                    else if (m_inventory.size() >= 26) {//if inventory is full push inventory full message and dont pick up item
                        m_outputList.push("Your knapsack is full; you can't pick that up.");
                    }
                    else {
                        for (size_t i = 0; i < m_gameObjects.size(); i++) {
                            if (m_gameObjects.at(i) == gameObject) {//locate gameObject in m_gameObjects vector
                                m_inventory.push_back(gameObject);
                                //make string output happen
                                vector<GameObject*>::iterator itr = m_gameObjects.begin() + i;
                                m_gameObjects.erase(itr);//erases the pointer to picked up gameObject in m_gameObjects, doesn't deleted game object since it still exists inside m_inventory
                                m_outputList.push(gameObject->pickup());//might implement this another way later
                                break;
                            }
                        }
                    }
                }
                break;

            case 'i'://check inventory
                displayInventory();//displays inventory
                key = getCharacter();//gets character
                key = '/';//sets key to a value not equal to 'q' so program doesn't end
                break;
            case 'w'://swap weapon
                displayInventory();//displays inventory
                key = getCharacter();//gets character
                if (key - 97 < m_inventory.size() && m_inventory.at(key - 97)->identifier() == ')') {//checks that input char corresponds to a weapon in inventory
                    Weapon* w = dynamic_cast<Weapon*> (m_inventory.at(key - 97));//makes a weapon pointer to the weapon 
                    m_outputList.push(m_player.swapWeapon(w));//swaps the players weapon to the chosen weapon and pushes weilding string onto outputlist
                }
                key = '/';//sets key to a value not equal to 'q' so program doesn't end
                break;
            case 'r'://read scroll
                displayInventory();
                key = getCharacter();
                int convertedVal = key - 97;//converts the char to int starting with 'a' = 0
                if (convertedVal < m_inventory.size() && m_inventory.at(convertedVal)->identifier() == '?') {//checks that input char corresponds to a scroll in inventory
                    Scroll* s = dynamic_cast<Scroll*> (m_inventory.at(convertedVal));//makes a scroll pointer to the chosen scroll
                    m_outputList.push(s->use(&m_player));//uses the chosen scroll and pushes the scroll use string onto the outputlist
                    delete m_inventory.at(convertedVal);//deletes the used scroll
                    vector<GameObject*>::iterator itr = m_inventory.begin() + (convertedVal);//makes an iterator and uses iterator to erase usd scroll from inventory
                    m_inventory.erase(itr);
                }
                key = '/';//sets key to a value not equal to 'q' so program doesn't end
                break;
            }
        }
        runMonsterTurns();//runs through m_monster running the turn of each monster in the level
        if (m_player.isDead()){//if player is dead game is over
            gameOver = true;
        }
      
    };
    if (key != 'q') {//display exit message if key is not already 'q' 
        m_outputList.push("press q to exit game.");
    }
    display();//display output strings one last time
    while (key != 'q') {// end game once user presses 'q'
        key = getCharacter();
    }
}

void Game::display() {//displays the dungeon
    clearScreen();//clears the screen
    updateBoard();//updates position of actors and gameObjects on the dungeon array
    for (int i = 0; i < MAX_ROWS; i++) {//loops through m_dungoen and prints the dungeon
        for (int j = 0; j < MAX_COLS; j++) {
                cout << m_dungeon[i][j];
        }
        cout << endl;
    }
    cout << "Dungeon Level: " << m_level << ", " << "Hit points: " << m_player.health() << ", " << "Armor: " <<//prints out level and player statistics
        m_player.armor() << ", " << "Strength: " << m_player.strength() << ", " << "Dexterity: " << m_player.dexterity() << endl;
        // cout << m_player.weapon()->action(); test

    //displays the output string(s)
    cout << endl;
    displayOutput();

}

void Game::displayOutput() {//prints output strings stored in output list and empties output list queue
    while (!m_outputList.empty()) {
        cout << m_outputList.front() << endl;
        m_outputList.pop();
    }
}

void Game::displayInventory() {//displayes the players inventory
    clearScreen();//clear screen
    cout << "Inventory:" << endl;
    for (size_t i = 0; i < m_inventory.size(); i++) {//prints out the gameObjects in the players inventory with each one preceded by a char from a-z in alphabetical order
       cout << char(97 + i) << ". "<< m_inventory.at(i)->name() <<endl;
    }
}

void Game::updateBoard() {//updates the m_dungeon array before it is printed out
    for (int i = 0; i < MAX_ROWS; i++) {//sets every non '#' char to ' ' so that all actors and object chars can be filled in
        for (int j = 0; j < MAX_COLS; j++) {
            if (m_dungeon[i][j] != '#' ) {//might add more for stationary chars
                m_dungeon[i][j] = ' ';

            }
           
        }
    }

    //IMPLEMENT ADDING GAMEOBJECTS
    for (size_t i = 0; i < m_gameObjects.size(); i++) {//sets the board positions of gameObjects to their corresponding chars
        m_dungeon[m_gameObjects.at(i)->row()][m_gameObjects.at(i)->col()] = m_gameObjects.at(i)->identifier();//display the objects identifing char
    }

    if (m_level < 4) {//if the level is less than 4, the exit row and col represent the stair way so the stairway char is set at that position
        m_dungeon[m_exitRow][m_exitCol] = '>';
    }
 

    //ADD ACTORS
    m_dungeon[m_player.row()][m_player.col()] = '@';//sets player char
    for (size_t i = 0; i < m_monsters.size(); i++) {//sets monster chars
        m_dungeon[m_monsters.at(i)->row()][m_monsters.at(i)->col()] = m_monsters.at(i)->identifier();//display the monsters identifing char
    }
}

void Game::checkForDeaths() {//checks if monsters died
    for (size_t i = 0; i < m_monsters.size(); i++) {//loop through vector of monsters
        if (m_monsters.at(i)->isDead()) {//if a monsters health is less than or equal to zero, that monster is deleted from the vector
            m_monsters.at(i)->dropOnDeath();//chance to drop item on death
            delete(m_monsters.at(i));//delete the monster that died 
            vector<Monster*>::iterator itr = m_monsters.begin() + i;
            m_monsters.erase(itr);//erase the monster pointer from the vector
            i--;//moves down i by one to account for the fact that one pointer was removed from the array
        }
    }
}


void Game::clearLevel() {
    //clear m_monsters
    for (size_t i = 0; i < m_monsters.size(); i++) {//loop through vector
        delete m_monsters.at(i);//delete every object in the vector
    }
    m_monsters.clear();//clear vector

    for (size_t i = 0; i < m_gameObjects.size(); i++) {//loop through vector
        delete m_gameObjects.at(i);//delete every object in the vector
    }
    m_gameObjects.clear();//clear vector

}

void Game::newLevel() {
    //first clear old m_dungeon and m_gameObjects
    clearLevel();
    buildDungeon();

    //find random unnoccupied location for the exit position of the stairs or the golden idol position depending on the level
    int row, col;
        do {
            row = randInt(MAX_ROWS);
            col = randInt(MAX_COLS);
        } while (m_dungeon[row][col] == '#');
        if(m_level<4){//if level is less than 4 set the exitRow and exitCol to signify stair location 
            m_exitRow = row;
            m_exitCol = col;
        }
        else {//otherwise place the golden idol
            GameObject* gameObject = new GoldenIdol(row, col);
            m_gameObjects.push_back(gameObject);
           //set to exit positions to - 1 to signify no stairs on level 4
            m_exitRow = -1;
            m_exitCol = -1;
        }

    

    //place the player randomly
    do {
        m_player.setRow(randInt(MAX_ROWS));
        m_player.setCol(randInt(MAX_COLS));
    } while (m_dungeon[m_player.row()][m_player.col()] == '#');

    //places random items
    int numItems = randInt(2, 3);//number of items to spawn per level
    for (int l = 0; l < numItems; l++) {
        placeRandObject();
    }


    //spawn random monsters
    int numMonsters = randInt(2, 5 * (m_level + 1) + 1);//number of monsters to randomly spawn on the level
    //randomly create and place the monsters
    for (int k = 0; k < numMonsters; k++) {
        placeRandMonster();
    }

}


void Game:: placeRandMonster() {
    int n = 1;//can only spawn Goblins and Snakewomen if level is 0 or 1
    if (m_level >= 2)//from level 2 onward boogiemen can spawn
        n++;
    if (m_level >= 3)//from level 3 onward dragons can spawn
        n++;
    int rand = randInt(0, n);//randomly decides which monster to spawn with each integer value corresponding to a different monster
        Monster* monster = nullptr;//pointer to the new monster
        switch (rand) {
        case 0://Goblin
            monster = new Goblin(this);//creates monster with pointer to this game
            break;
        case 1://Snakewoman
            monster = new Snakewoman(this);//creates monster with pointer to this game
            break;
        case 2://Bogeyman
            monster = new Bogeyman(this);//creates monster with pointer to this game
            break;
        case 3://Dragon
            monster = new Dragon(this);//creates monster with pointer to this game
            break;
        }
        int row, col;//position that the monster will be placed at
        do {//randomly assigns the monster to an unnocupied space
            row = randInt(0, MAX_ROWS - 1);
            col = randInt(0, MAX_COLS - 1);
        } while (m_dungeon[row][col]== '#' || ActorInLocation(row,col));
        monster->setCol(col);//sets monster position
        monster->setRow(row);
        m_monsters.push_back(monster);//pushes monster onto the monsters vector
}

void Game::placeRandObject() {
    int rand = randInt(0, 6);//randomly decides which monster to spawn with each of the 7 possible integer value corresponding to a different gameObject
    GameObject* gameObject = nullptr;//makes pointer that will point to the new gameObject
    switch (rand) {
    case 0://Mace
        gameObject = new Mace();
        break;
    case 1://Short Sword
        gameObject = new ShortSword();
        break;
    case 2://Long Sword
        gameObject = new LongSword();
        break;
    case 3://ScrollOfEnhanceArmor
        gameObject = new ScrollOfEnhanceArmor();
        break;
    case 4://ScrollOfRaiseStrength
        gameObject = new ScrollOfRaiseStrength();
        break;
    case 5://ScrollOfEnhanceHealth
        gameObject = new ScrollOfEnhanceHealth();
        break;
    case 6://ScrollOfEnhanceDexterity
        gameObject = new ScrollOfEnhanceDexterity();
        break;
    }
    int row, col;//position that the monster will be placed at
    do {//randomly sets the position to an unnocupied gameObject location
        row = randInt(0, MAX_ROWS - 1);
        col = randInt(0, MAX_COLS - 1);
    } while (m_dungeon[row][col] == '#' || (row == m_exitRow && col == m_exitCol) || ObjectInLocation(row, col));
    gameObject->setPosition(row, col);//sets gameObjects position
    m_gameObjects.push_back(gameObject);//pushes the gameObject onto gameObject vector

}

//places down a passed in object 
//returns true if object is placed down, false otherwise
bool Game::placeObject(int r, int c, GameObject* gameObject) {
    if (m_dungeon[r][c] != '#' && (r != m_exitRow || c != m_exitCol) && ObjectInLocation(r, c) == nullptr) {//checks that the given location is open
        gameObject->setPosition(r, c);//sets objects position
        m_gameObjects.push_back(gameObject);//pushes object onto gameObject vector
        return true;//object was placed down
  }
    else {
        return false;//object was not placed down
    }
}

//returns the char at the given location in the m_dungeon array
char Game::charInLocation(int r, int c) {
    return (m_dungeon[r][c]);
}

//returns a pointer to the actor at a given location in the dungeon
//returns nullptr if their is no actor at that position
Actor* Game:: ActorInLocation(int r, int c) {

    for (size_t i = 0; i < m_monsters.size(); i++) {//loops through monsters vector
        if ((m_monsters.at(i)->row() == r) &&  (m_monsters.at(i)->col() == c)) {//if any monster is at the given location return pointer to that monster
            return m_monsters.at(i);
        }
    }
    if (m_player.row() == r && m_player.col() == c) {//if the games player is at given location return poiter to the games player
       return &m_player;
    }

    return(nullptr);//otherwise return nullptr
}

//returns a pointer to the gameObject at a given location in the dungeon
//returns nullptr if their is no gameObject at that position
GameObject* Game::ObjectInLocation(int r, int c) {

    for (size_t i = 0; i < m_gameObjects.size(); i++) {//loops through gameObject vector
        if (m_gameObjects.at(i)->row() == r && m_gameObjects.at(i)->col() == c) {//if any gameObject is at the given location return pointer to that gameObject
            return m_gameObjects.at(i);
        }
    }

    return(nullptr);//otherwise return nullptr
}

void Game::runMonsterTurns() {//runs the turn of each of the dungeons monsters
    for (size_t i = 0; i < m_monsters.size(); i++) {//loops through monsters vector
        if (!m_monsters.at(i)->isDead()) {//if the monster is not dead take the monsters turn
            string outputMessage = m_monsters.at(i)->takeTurn();
            if (!outputMessage.empty()) {//if the monsters turn returns a nonempty string(their attack string) push it onto the outputList
                m_outputList.push(outputMessage);
            }
        }
    }
    checkForDeaths();//checks for monsters who may have died and updates m_monsters accordingly
}

void Game::buildDungeon(){
    //initialize dungeon with all '#'
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
                m_dungeon[i][j] = '#';
           
        }
    }

 //Make Rectangle rooms about 4-7 rooms
//Heights random number around 4-9
//Widths random number around 8-15
    int numRooms = randInt(4, 7);//number of rooms to make for the level random from 4-7
    int minHeigth = 4;
    int maxHeigth = 9;
    int minWidth = 8;
    int maxWidth = 15;
    if (numRooms >= 6) {//if makeing 6 or 7 rooms, the max sizes of rooms are decreased to make it easier for everything to fit
        maxHeigth = 7;
        maxWidth = 12;
    }

    vector<Room> rooms;//vector of all the rooms on the current level
    int timesIntersected = 0;//used to make sure that it doesn't get caught in an infinite loop
    for (int i = 0; i < numRooms; i++) {//loops for number of rooms to make
        Room room(randInt(1, MAX_COLS - maxWidth-1), randInt(0+maxHeigth, MAX_ROWS -2), randInt(minWidth, maxWidth), randInt(minHeigth, maxHeigth));//makes room of random positions and size within the dungeons bounds
        bool intersects = false;//keeps track of if the current room intersects any previously created rooms
        for (size_t j = 0; j < rooms.size(); j++) {//loops through the previously created rooms
            if (room.intersects(rooms[j])) {//checks if the created room interects any of the already made rooms and makes intersect bool appropriately
                intersects = true;
                break;
            }
        }
        if (intersects ) {//checks if the room intesected any other rooms
            if (timesIntersected < 10000) {//if the rooms intersects with another room and has tried making this room 1000 times or more, try making the room again
               timesIntersected++;//increment amount of times this room has failed to be created correctly
                i--;//decrement i to indicate that that a new room was not succesfully created and the loop has to run again
            }
            else {//if the program fails to create a new room that doesn't overlap with the other rooms after 1000 trys give up and make one less room than planned
                timesIntersected = 0;//resets timesIntersected for the next potential room 
            }
        }
        else {//if the room doesn't intersect anny others push it onto the rooms vector
            rooms.push_back(room);
        }
    }

    for (size_t k = 0; k < rooms.size(); k++) {//loops through the rooms array copying every room onto the m_dungeon array
        rooms.at(k).copyRoom(m_dungeon);
    }
   sort(rooms.begin(), rooms.end(), furthestLeft);//sorts the rooms in order of the collumn of their furthest left wall in order of closest to 0 to furthest from 0
    //make the paths
    for (size_t l = 0; l < rooms.size()-1; l++) {//loops from the first room to the second to last room
        for (int i = rooms.at(l).midx(); i <= rooms.at(l + 1).midx(); i++) {//makes a path from the the x midpoint of the current room to the x midpoint of the next room at in the row of the current rooms y midpoint
            m_dungeon[rooms.at(l).midy()][i] = ' ';
        }
        if (rooms.at(l).midy() < rooms.at(l + 1).midy()) {//if the next rooms y midpoint is below that of the current room
            for (int i = rooms.at(l).midy(); i <= rooms.at(l + 1).midy(); i++) {//makes a path from the the y midpoint of the current room to the y midpoint of the next room in the collumn of the next rooms x midpoint
                m_dungeon[i][rooms.at(l + 1).midx()] = ' ';
            }
        }
        else {//if the next rooms y midpoint is below or the same as that of the current room
            for (int i = rooms.at(l).midy(); i >= rooms.at(l + 1).midy(); i--) {//makes a path from the the y midpoint of the current room to the y midpoint of the next room in the collumn of the next rooms x midpoint
                m_dungeon[i][rooms.at(l + 1).midx()] = ' ';
            }
        }
    }
}


// You will presumably add to this project other .h/.cpp files for the
// various classes of objects you need to play the game:  player, monsters,
// weapons, etc.  You might have a separate .h/.cpp pair for each class
// (e.g., Player.h, Boegeyman.h, etc.), or you might put the class
// declarations for all actors in Actor.h, all game objects in GameObject.h,
// etc.
