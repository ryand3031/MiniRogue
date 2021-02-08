#include "Actor.h"
#include "GameObject.h"
#include "utilities.h"
#include <string>
#include "Game.h"
#include <iostream>
using namespace std;

//constructor
Actor::Actor(char identifier,string name, int row, int col, int health, int armor, int strength, int dexterity, Weapon* weapon): m_identifier(identifier), m_name(name), m_row(row), 
m_col(col), m_health(health),m_armor(armor), m_strength(strength), m_dexterity(dexterity), m_sleepTime(0), m_weapon(weapon) {//check this
}

Actor::~Actor() { delete m_weapon; }//deconstructor just deletes the actors weapon


//trivial getter methods
char Actor::identifier() const{ return (m_identifier); }
string Actor::name() const { return (m_name); }
int Actor::row() const { return (m_row); }
void Actor::setRow(int r) { m_row = r; }
int Actor::col()const { return (m_col); }
int Actor::health() const { return (m_health); }
int Actor::armor() const { return (m_armor); }
int Actor::strength() const { return (m_strength); }
int Actor::dexterity()const { return (m_dexterity); }
int Actor::sleepTime()const { return (m_sleepTime); }
Weapon* Actor::weapon() { return m_weapon; }

bool Actor::isDead() const { return(m_health <= 0); }//returns if health is zero or lower

//trivial setter methods
void Actor::setCol(int c) { m_col = c; }
void Actor::setWeapon(Weapon* w) { m_weapon = w; }
void Actor::setHealth(int health) { 
    m_health =health;
    if (m_health > 99) {//health is capped at 99
        m_health = 99;
    }
}
void Actor::setStrength(int strength) { 
    m_strength = strength;
    if (m_strength > 99) {//strength is capped at 99
        m_strength = 99;
    }
}
void Actor::setArmor(int armor) { 
    m_armor = armor; 
    if (m_armor > 99) {//armor is capped at 99
        m_armor = 99;
    }
}
void Actor::setDexterity(int dexterity) { 
    m_dexterity = dexterity; 
    if (m_dexterity > 99) {//dexterity is capped at 99
        m_dexterity = 99;
    }
}
void Actor::setSleepTime(int sleepTime) { m_sleepTime = sleepTime; }

//Attacks another Actor
string Actor::attack(Actor* actor) {
    string outputPhrase = name() + " " + weapon()->action() + " at " + actor->name();//stores the start of the attack message
   //runs attack calculation to see if the attack hits
    int attackerPoints = m_dexterity + m_weapon->dexterityBonus();
    int defenderPoints = actor->dexterity() + actor->armor();
    if (randInt(1, attackerPoints) >= randInt(1, defenderPoints)) {//check if attack hits
    int damagePoints = randInt(0, m_strength + m_weapon->damage() - 1);//calculate the damage of the attack
    actor->setHealth(actor->health() - damagePoints);//lower the health of the actor being attacked
    if (actor->isDead()) {//if the actors health is below 0 display special final blow message
        outputPhrase += " dealing a final blow.";
    }
    else {//if actor being attacked is not dead, use the and hits message
        outputPhrase += " and hits";
        if (weapon()->name()=="magic fangs of sleep" && trueWithProbability(0.2)) {//if the weapon is magic fangs check for chance to put the actor to sleep
            int newSleepTime = randInt(2, 6);//calculate turns of sleep
            if (newSleepTime > actor->sleepTime()) {//only update the actors sleep time if the actor is not already asleep for at least as long as newSleepTime
                actor->setSleepTime(newSleepTime);
            }
            outputPhrase += ", putting " + actor->name() + " to sleep";//special sleep output phrase
        }
        outputPhrase += '.';//ends the output phrase with '.'
    }
    }
    else {//if attack misses add the and misses phrase 
        outputPhrase += " and misses.";
    }
    return outputPhrase;//return outputPhrase
}


//Moves the actor in the direction specified
void Actor::moveUp() { setRow(row() - 1); }
void Actor::moveDown() { setRow(row() + 1); }
void Actor::moveRight() { setCol(col() + 1); }
void Actor::moveLeft() { setCol(col() - 1); }

//Monster Constructor
Monster::Monster(char identifier, std::string name, int row, int col, int health, int armor, int strength, int dexterity, Weapon* weapon,Game* game):
	Actor(identifier, name, row, col, health, armor, strength, dexterity, weapon) ,m_game(game){}
//trivial getter
Game* Monster::game() { return m_game; }

//Moves the monster one row over in the direction specified by the sign of int direction
// if direction < 0 move up, if direction>0 move down, else dont move 
bool Monster::moveRow(int direction) {
    int moveVal = 1;//rows to move from current position
    if (direction == 0)//if direction is 0 nothing is done and returns false;
        return false;
    else if (direction < 0) {//sets amount of rows to move to -1 if direction is negative
        moveVal = -1;
    }
    if (game()->charInLocation(row() + moveVal, col()) != '#' && game()->ActorInLocation(row() + moveVal, col()) == nullptr) {//checks if the desired space to move is already occupied
        setRow(row() + moveVal);//moves row value by one in desired direction and returns true
        return true;
    }
    return false;//otherwise returns false
}

//Moves the monster one col over in the direction specified by the sign of int direction
// if direction < 0 move left, if direction>0 move right, else dont move 
bool Monster::moveCol(int direction) {
        int moveVal = 1;//cols to move from current position
        if (direction == 0)//if direction is 0 nothing is done and returns false;
            return false;
        else if (direction < 0) {//sets amount of cols to move to -1 if direction is negative
            moveVal = -1;
        }
        if (game()->charInLocation(row(), col()+moveVal) != '#' && game()->ActorInLocation(row(), col()+moveVal) == nullptr) {//checks if the desired space to move is already occupied
            setCol(col() + moveVal);//moves col value by one in desired direction and returns true
            return true;
        }
        return false;//otherwise returns false
    
}

//Player Constructor
Player::Player() : Actor('P',"Player", 0, 0, 20,  2, 2, 2, new ShortSword(-1,-1)), m_maxHealth(20) {}

void Player::cheat() {//Sets player health to 50 and player strength to 9
    setHealth(50);
    setStrength(9);
}
//trivial getter method
int Player::maxHealth() { return m_maxHealth; }
//trivial setter method
void Player::setMaxHealth(int maxHealth) { 
    m_maxHealth = maxHealth;
    if (m_maxHealth > 99) {//health is capped at 99
        m_maxHealth = 99;
    }
}
//Sets the players weapon pointer to the address of a different weapon
//returns the weilding message
string Player::swapWeapon(Weapon* w) {
    setWeapon(w);//sets player weapon pointer to w
    string outputPhrase = "You are wielding " + w->name();//creates weilding message
        return(outputPhrase);//returns outputphrase
   
    
}
//To be called at the start of every player turn
//1 in 10 chance to regenerate 1 health if health<maxHealth
void Player::regenerate() {
    if (trueWithProbability(0.1)) {
        if (health() < m_maxHealth) {
            setHealth(health() + 1);
        }
    }
}


Bogeyman::Bogeyman(Game* game) : Monster('B',"the Bogeyman", 0, 0, randInt(5,10), 2, randInt(2,3), randInt(2,3), new ShortSword(-1, -1), game) {}
string Bogeyman::takeTurn() {    //returns empty string ("") if monster doesn't attack
    //run movement algorith
    //check if its movement is into player and if so attacks otherwise increments position

    if (sleepTime() > 0) {//checks if the monster is asleep
        setSleepTime(sleepTime() - 1);//decreases sleep time by one
    }
    else {//If not asleep
        Actor* player = game()->player();//get pointer to the games player
        int rowsAway = player->row() - row();
        int colsAway = player->col() - col();
        int spacesAway = abs(rowsAway) + abs(colsAway);  //absolute value of distance between player and monster
        if (spacesAway == 1) {//if player is adjacent to monster, attack player  
            return attack(player);
        }
        else if (spacesAway <= 5) {//Boogiemen can smell player from up to 5 spaces away
                if (!moveRow(rowsAway)) {//moves in row direction first if possible
                    moveCol(colsAway);//if can't move to a closer row, it moves to closer collumn
                }
            
        }
    }
    return"";//if monster doesn't attack, return empty string
}

//To be called on death of boogieman
void Bogeyman::dropOnDeath() {
    //1 in 10 chance of dropping magic axe
    if (trueWithProbability(0.1)) {
        GameObject* gameObject = new MagicAxe();
        if (!(game()->placeObject(row(), col(), gameObject))) {//places object in the monsters location if there is space
            delete gameObject;//if there is not open space, object is deleted
        }
    }
}

Snakewoman::Snakewoman(Game* game) : Monster('S',"the Snakewoman", 0, 0, randInt(3,6), 3, 2, 3, new MagicFangs(-1, -1), game) {}//change to magic fangs of sleep
string Snakewoman::takeTurn() {    //returns empty string ("") if monster doesn't attack
    //run movement algorith
    //check if its movement is into player and if so attacks otherwise increments position
    if (sleepTime() > 0) {//checks if the monster is asleep
        setSleepTime(sleepTime() - 1);//decreases sleep time by one
    }
    else {
        Actor* player = game()->player();//gets pointer to the games player
        int rowsAway = player->row() - row();
        int colsAway = player->col() - col();
        int spacesAway = abs(rowsAway) + abs(colsAway);  //Absolute value of distacne between player and monster
        if (spacesAway == 1) {//if player is adjacent to monster, attack player  
            return attack(player);
        }
        else if (spacesAway <= 3) {

                if (!moveRow(rowsAway)) {//moves in row direction first if possible
                    moveCol(colsAway);//if can't move to a closer row, it moves to closer collumn
                }
            
        }
    }
    return"";//if monster doesn't attack, return empty string
}

//To be called on death of Snakewoman
void Snakewoman::dropOnDeath() {
    //1 in 3 chance of dropping magic fangs of sleep
    if (trueWithProbability(1.0/3)) {
        GameObject* gameObject = new MagicFangs();
        if (!(game()->placeObject(row(), col(), gameObject))) {//places object in the monsters location if there is space
            delete gameObject;//if there is not open space, object is deleted
        }
    }
}

//Dragon Constructor
Dragon::Dragon(Game* game) : Monster('D', "the Dragon", 0, 0, randInt(20, 25), 4, 4, 4, new LongSword(-1, -1), game) {
    m_maxHealth = health();//max health starts at the initial health
}

//runs Dragons turn
string Dragon::takeTurn() {//returns empty string ("") if monster doesn't attack
    //run movement algorith
    //check if its movement is into player and if so attacks otherwise increments position
    if (trueWithProbability(0.1)) {//possible regeneration at the start of every dragons turn
        if (health() < m_maxHealth) {
            setHealth(health() + 1);
        }
    }
    if (sleepTime() > 0) {//checks if the monster is asleep
        setSleepTime(sleepTime() - 1);//decreases sleep time by one
    }
    else {//if dragon not asleep
        Actor* player = game()->player();//gets pointer to the games player
        int rowsAway = player->row() - row();
        int colsAway = player->col() - col();
        int spacesAway = abs(rowsAway) + abs(colsAway);  //Absolute value of distacne between player and dragon
        if (spacesAway == 1) {//if player is adjacent to monster, attack player  
            return attack(player);
        }
    }

    return"";//if monster doesn't attack, return empty string
}
//To be called on death of Dragon
void Dragon::dropOnDeath() {
    //Always Drops a scroll
    int rand = randInt(0, 4);
    GameObject* gameObject = nullptr;
    switch (rand){

    case 0://ScrollOfEnhanceArmor
        gameObject = new ScrollOfEnhanceArmor();
        break;
    case 1://ScrollOfRaiseStrength
        gameObject = new ScrollOfRaiseStrength();
        break;
    case 2://ScrollOfEnhanceHealth
        gameObject = new ScrollOfEnhanceHealth();
        break;
    case 3://ScrollOfEnhanceDexterity
        gameObject = new ScrollOfEnhanceDexterity();
        break;
    case 4://ScrollOfTeleportation
        gameObject = new ScrollOfTeleportation(game());
        break;
    }
    if (!(game()->placeObject(row(), col(), gameObject))) {//places object in the monsters location if there is space
        delete gameObject;//if there is not open space, object is deleted
    }
}


//Goblin Constructor
Goblin::Goblin(Game* game) : Monster('G' ,"the Goblin", 0, 0, randInt(15, 20), 1, 3, 1, new ShortSword(-1, -1), game) {}
//Runs goblins turn
string Goblin::takeTurn() {
    if (sleepTime() > 0) {//checks if the monster is asleep
        setSleepTime(sleepTime() - 1);//decreases sleep time by one
    }
    else {
        Actor* player = game()->player();//gets pointer to the games player
        int rowsAway = player->row() - row();
        int colsAway = player->col() - col();
        int spacesAway = abs(rowsAway) + abs(colsAway);  //Absolute value of distance between player and monster
        if (spacesAway > 15) {
            return "";
        }
        if (spacesAway == 1) {//if player is adjacent to monster, attack player  
            return attack(player);
        }
        //Run Goblin Recursive Algorithm
        game()->updateBoard();
        game()->setChar(row(), col(), ' ');//takes out the goblins space so goblin doesn't run into himself
        int direction= -1;
        if (pathToPlayer(row(), col(), game()->goblinSmellDistance(), direction) != -1) {//based on the retun of pathToPlayer, decides which direction to move
            switch (direction) {
            case 0://move up
                setRow(row() - 1);
                break;
            case 1://move down
                setRow(row() + 1);
                break;
            case 2://move left
                setCol(col() - 1);
                break;
            case 3://move right
                setCol(col() + 1);
                break;

            }
        }
    }


    return"";//returns empty if it doesnt attack the player
}

//returns the minimum path to the player from current position if that path is less than or equal to the given amount of moves
//if not possible to reach player in less than the given number of moves returns -1 
//parameters: 
//r:starting row, c:starting col, moves: max numbers of moves allowed to reach the player
//direction is changed to represent the direction in which the goblin should move to reach the player
    //0 is up, 1 is down, 2 is left, 3 is right, -1 is no possible moves
int Goblin::pathToPlayer(int r, int c, int moves,  int& direction) {//Goblin Recursive Algorithm
   
    Actor* player = game()->player();
    int rowsAway = player->row() - r;
    int colsAway = player->col() - c;
    int spacesAway = abs(rowsAway) + abs(colsAway);
    
    if (spacesAway==0) {//if goblin reached player
        return 0;//return 0 distance away
    }

    if (spacesAway>moves || (game()->charInLocation(r, c) != ' ' && game()->charInLocation(r, c) != ')' && game()->charInLocation(r, c) != '?' && game()->charInLocation(r, c) != '&' && game()->charInLocation(r, c) != '>')) {//checks if current space is already occupied
        direction = -1;//sets direction to -1
        return -1;//returns -1 to indicate there is no possible path from this direction
    }
        
    int paths[4] = { -1,-1,-1,-1 };//stores the value returned from the pathToPlayer function called at the four possible directions from the current position {up, down, left, right}
    //stores the direction from the last call to the function
    //indicates the direction you came from to get to the current spot so you don't have to check the path you came from
    int lastDirection = direction;
    
    if (lastDirection != 1) {//if not coming from above
        direction = 0;//coming from above
        paths[0] = pathToPlayer(r - 1, c, moves - 1, direction);//up
    }
    if (lastDirection != 0) {//if not coming from below
        direction = 1;//coming from below
        paths[1] = pathToPlayer(r + 1, c, moves - 1, direction);//down
    }
    if (lastDirection != 3) {//if not coming from the right
        direction = 2;//coming from the left
        paths[2] = pathToPlayer(r, c - 1, moves - 1, direction);//left
    }
    if (lastDirection != 2) {//if not coming from the left
        direction = 3;//coming from the right
        paths[3] = pathToPlayer(r, c + 1, moves - 1, direction);//right
    }

    int min = moves+1;//sets moves greater than moves
    for (int i = 0; i < 4; i++) {
        if (paths[i] < min && paths[i] != -1) {//finds the minimum path to the player from this spot out of the four directions
            min = paths[i];
        }
    }
    if (min > moves) {//if there is no possible path to player in the amount of moves return -1
        direction = -1;
        return -1;
    }
    else if (min == paths[0]) {//if up path is shortest set direction to 0 for up
        direction = 0;
    }
    else if (min == paths[1]) {//if down path is shortest set direction to 1 for down
        direction = 1;
    }
    else if (min == paths[2]) {//if left path is shortest set direction to 2 for left
        direction = 2;
    }
    else if (min == paths[3]) {//if right path is shortest set direction to 3 for right
        direction = 3;
    }
    return min +1;//return the minimum path distance plus 1
}



//To be called on death of boogieman
void Goblin::dropOnDeath() {
    //1 in 3 chance of dropping either magic fangs of sleep or magic axe
    if (trueWithProbability(1.0/3.0)) {//1/3 chance of dropping item
        GameObject* gameObject;
        if (trueWithProbability(0.5)) {//if dropping an item, 50/50 chance of dropping either item
            gameObject = new MagicFangs();
        }
        else {
            gameObject = new MagicAxe();
        }
        if (!(game()->placeObject(row(), col(), gameObject))) {//places object in the monsters location if there is space
            delete gameObject;//if there is not open space, object is deleted
        }
    }
}