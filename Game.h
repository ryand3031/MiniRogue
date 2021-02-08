// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED
#include <vector>
#include <queue>
#include "utilities.h"
#include "Actor.h"
#include"GameObject.h"
#include "Room.h"

// You may add data members and other member functions to this class.



class Game
{
public:
    //constructor
    Game(int goblinSmellDistance);
    //destructor
    ~Game();
    //runs the game
    void play();
    //trivial getter
    int goblinSmellDistance() const;
    //returns a pointer to the actor in the given location in the dungeon, if there is no actor returns nullptr
    Actor* ActorInLocation(int r, int c);
    //returns a pointer to the gameObject in the given location in the dungeon, if there is no gameObject returns nullptr
    GameObject* ObjectInLocation(int r, int c);
    //returns the char in the given location in the dungeon
    char charInLocation(int r, int c);
    //return a pointer to the games player
    Player* player();
    //places a game object in the given row and collumn of the dungeon if the space is not filled, does nothing otherwise
    //returns true if the object was place, returns false otherwise
    bool placeObject(int r, int c, GameObject* gameObject);
    //sets the char at the given location in the dungeon
    void setChar(int r, int c, char identifier);
    void updateBoard();//updates the board with the current position of all actors and objects

private:
    //minimum distance that goblins can smell the player from
    int m_goblinSmellDistance;
    //2d array that is used to display the positions of all things on the board 
    char m_dungeon[MAX_ROWS][MAX_COLS];
    Player m_player;//the games player
    int m_level;//keeps track of the current level

    //row and col position variables of the stairway or idol depending on the level
    //if level<4 they represents stairs position
    //if level = 4 they represents the idol position
    int m_exitCol;
    int m_exitRow;

    std::vector<Monster*> m_monsters;//vector of all monsters in the current level
    std::vector<GameObject*> m_gameObjects;//vector of all of the gameObjects in the current level
    std::vector<GameObject*> m_inventory;//vector that stores the players inventory for the game
    std::queue<std::string> m_outputList;//queue that stores and then outputs the string outputs for each turn


    void runMonsterTurns();//runs the monsters turns
    void placeRandMonster();//places a random monster in a random open location on the board
    void placeRandObject();//places a random gameObject in a random open location on the board
    void newLevel();//sets the game to the next level
    void display();//displays the board and string outputs
    void displayOutput();//displayes string outputs stored in the outputlist and clears the output list
    void checkForDeaths();//checks if monsters have died and if so runs item drop and deletes monster from monsters vector
    void displayInventory();//displayes the players inventory
    void clearLevel();//clears all the objects in m_monsters and m_gameObjects and deletes thier pointers
    void buildDungeon();//randomly builds the dungeon rooms and paths for the current level

};

#endif // GAME_INCLUDED
