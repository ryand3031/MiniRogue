#include "GameObject.h"
#include <string>
#include "utilities.h"
#include "Actor.h"
#include "Game.h"
using namespace std;
//constructor
GameObject::GameObject(char identifier, std::string name, string action, int row, int col):m_identifier(identifier),m_name(name),m_action(action),m_row(row),m_col(col) {}
GameObject::~GameObject(){}
//trivial getters
string GameObject::name()const { return m_name; }
string GameObject::action()const { return m_action; }
int GameObject::row()const { return m_row; }
int GameObject::col()const { return m_col; }
char GameObject::identifier()const { return(m_identifier); }

//sets row and collumn position
void GameObject::setPosition(int r, int c) {
    m_row = r;
    m_col = c;
}

//Weapon implementation
//Constructor
Weapon::Weapon(int row, int col, std::string name,string action, int dexterityBonus, int damage): GameObject(')',name,action,row, col),
m_dexterityBonus(dexterityBonus), m_damage(damage) {}

//trivial getters
int Weapon::dexterityBonus() const{ return m_dexterityBonus; }
int Weapon::damage()const { return m_damage; }
string Weapon::pickup() {//returns pickup string
    string output = "You pick up " + name();//creates proper pickup string for weapon
    setPosition(-1, -1);//positions of -1 indicate that the obect is not on the dungeon floor
    return output;//returns output
}

//Weapon Types implementations
//Constructors
ShortSword::ShortSword(int row, int col): Weapon(row, col,"short sword", "slashes short sword", 0, 2 ){}
Mace::Mace(int row, int col) : Weapon(row, col, "mace", "swings mace", 0, 2) {}
LongSword::LongSword(int row, int col) : Weapon(row, col, "long sword", "swings long sword", 2, 4) {}
MagicAxe::MagicAxe(int row, int col) : Weapon(row, col, "magic axe", "chops magic axe", 5, 5) {}
MagicFangs::MagicFangs(int row, int col) : Weapon(row, col, "magic fangs of sleep", "strikes magic fangs", 3, 2) {}



//Scroll Implementation
//constructors
Scroll::Scroll(int row, int col, string name, string action, int boostVal):GameObject('?',name,action,row,col), m_boostVal(boostVal){}//figure out this error latter
//trivial getter
int Scroll::boostVal()const { return m_boostVal; }
string Scroll::pickup() {//returns pickup string
    string output = "You pick up a " + name();//creates proper pickup string for scroll
    setPosition(-1, -1);//positions of -1 indicate that the obect is not on the dungeon floor
    return output;//returns output string
}

//scroll type implementations
//constructors
ScrollOfTeleportation::ScrollOfTeleportation(Game* game,int row , int col):Scroll(row,col, "scroll called scroll of teleportation", "You feel your body wrenched in space and time.",-1), m_game(game){}
ScrollOfRaiseStrength::ScrollOfRaiseStrength(int row, int col) : Scroll(row, col, "scroll called scroll of raise strength", "Your muscles bulge.",randInt(1,3) ) {}
ScrollOfEnhanceArmor::ScrollOfEnhanceArmor(int row , int col) : Scroll(row, col, "scroll called scroll of enhance armor", "Your armor glows blue.",randInt(1,3) ) {}
ScrollOfEnhanceHealth::ScrollOfEnhanceHealth(int row , int col) : Scroll(row, col, "scroll called scroll of enhance health", "You feel your heart beating stronger.", randInt(3,8)) {}
ScrollOfEnhanceDexterity::ScrollOfEnhanceDexterity(int row , int col) : Scroll(row, col, "scroll called scroll of enhance dexterity", "You feel like less of a klutz.",1) {}


//moves the player to a random unoccupied place in the dungeon and returns use output string
string ScrollOfTeleportation::use(Player* player) {
    string outputMessage = "You read the " + name() + '\n' + action();//creates use output string
    int row, col;
    do {//checks random positions untill it finds an open space to move the player to
        row = randInt(0, MAX_ROWS - 1);
        col = randInt(0, MAX_COLS - 1);
    } while (m_game->charInLocation(row,col) == '#' || m_game->ActorInLocation(row, col)!= nullptr);
    player->setCol(col);//sets player to the new position
   player->setRow(row);
        return outputMessage;//returns output message
}

string ScrollOfRaiseStrength::use(Player* player){
    string outputMessage = "You read the " + name() + '\n' + action();//creates use output string
    player->setStrength(player->strength() + boostVal());//boosts the players strength by boostVal
    return outputMessage;//returns output string

}

string ScrollOfEnhanceArmor::use(Player* player){
    string outputMessage = "You read the " + name() + '\n' + action();//creates use output string
    player->setArmor(player->armor() + boostVal());//boosts the players armor value by boostVal
    return outputMessage;//returns output string
}

string ScrollOfEnhanceHealth::use(Player* player) {
    string outputMessage = "You read the " + name() + '\n' + action();//creates use output string
    player->setMaxHealth(player->maxHealth() + boostVal());//boosts the players max Health by boostVal
    return outputMessage;//returns output string
}

string ScrollOfEnhanceDexterity::use(Player* player){
    string outputMessage = "You read the " + name() + '\n' + action() ;//creates use output string
    player->setDexterity(player->dexterity() + boostVal());//boosts the players dexterity by boostVal
    return outputMessage;//returns output string
}

GoldenIdol::GoldenIdol(int row, int col):GameObject('&', "the golden idol", "Congratulations, you won!", row, col){}
string GoldenIdol::pickup() {
    string output = "You pick up " + name() + '\n' + action();//creates proper pickup string for idol
    return output;//returns output string
}