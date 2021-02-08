#ifndef ACTOR_INCLUDED
#define ACTOR_INCLUDED
#include "GameObject.h" 
#include <string>




class Game;

class Actor {
public:
	//Actor constructor
	Actor(char identifier, std::string name, int row, int col, int health, int armor, int strength, int dexterity, Weapon* weapon);
	//Trivial Getter methods
	char identifier()const;
	std::string name()const;
	int row()const; 
	int col()const; 
	int health()const; 
	int armor()const; 
	int strength()const;
	int dexterity()const; 
	int sleepTime()const;
	Weapon* weapon();

	//Trivial Setter methods
	void setStrength(int strength);//sets strength
	void setRow(int r);
	void setCol(int c);
	void setWeapon(Weapon* w);
	void setArmor(int armor);
	void setDexterity(int dexterity);
	void setHealth(int health);
	void setSleepTime(int sleepTime);

	bool isDead()const;//checks if health has dropped below 0

	//moves the actors location by one in the direction specified
	void moveUp();
	void moveDown();
	void moveRight();
	void moveLeft();

	//changes the actors health to a new value

	//attacks another actor
	std::string attack(Actor* actor);

	virtual ~Actor();

private:
	char m_identifier;//char printed on the level
	std::string m_name;//name of the actor ex "Player", "the Goblin"
	int m_row;//current row that the actor is located at
	int m_col;//current collumn that the actor is located at
	int m_health;//health of the actor
	int m_armor;//armor value of the actor
	int m_strength;//strength value of the actor
	int m_dexterity;//dexterity value of the actor
	int m_sleepTime;//amount of turns the actor will be asleep for, when m_sleepTime<1, the actor is awake and can make a move
	Weapon* m_weapon;//pointer to the weapon of the actor
};

//Subclasses of Actors
class Player : public Actor {
public:
	//constructor
	Player();
	void cheat();//sets health to 50 and strength to 9
	std::string swapWeapon(Weapon* weapon);//swaps the current weapon of the player with another weapon
	int maxHealth();//trivial getter for the players maxHealth
	void setMaxHealth(int maxHealth);//trivial setter for the players max health
	void regenerate();//checks if the player regenerates 1 health, called at the start of the players turn
private:
	int m_maxHealth;//max health of the player
};

class Monster : public Actor {
public:
	//constructor
	Monster(char identifier, std::string name, int row, int col, int health, int armor, int strength, int dexterity, Weapon* weapon,Game* game);
	//runs the monsters turn, outputs attack string if the monster attacks, outputs blank string otherwise
	virtual std::string takeTurn()=0;

	//possible item drops when a monster is killed
	virtual void dropOnDeath() = 0;
	//getter of game pointer
	Game* game();
	//moves over one row/collumn in the direction negative or positive indicated by direction if possibe. returns true if moved, returns false if the monster can't move
	//direction positive if direction>0 direction negative if direction<0
	//if direction = 0 do nothing and return false
	bool moveRow(int direction);
	bool moveCol(int direction);
private:
	//pointer to the game that the monster is a part of
	Game* m_game;

};

//Subclasses of Monster
class Bogeyman : public Monster {
public:
	//constructor
	Bogeyman(Game* game);
	//runs the monsters turn, outputs attack string if the monster attacks, outputs blank string otherwise
	std::string takeTurn();
	//runs the deathdrop of bogeyman
	void dropOnDeath();
};

class Snakewoman : public Monster {
public:
	Snakewoman(Game* game);
	//runs the monsters turn, outputs attack string if the monster attacks, outputs blank string otherwise
	std::string takeTurn();
	//runs the death drop of Snakewoman
	void dropOnDeath();
};

class Dragon : public Monster {
public:
	//constructor
	Dragon(Game* game);
	//runs the monsters turn, outputs attack string if the monster attacks, outputs blank string otherwise
	std::string takeTurn();
	//runs the death drop of Dragon
	void dropOnDeath();
private:
	int m_maxHealth;
};


class Goblin : public Monster {
public:
	//constructor
	Goblin(Game* game);
	//runs the monsters turn, outputs attack string if the monster attacks, outputs blank string otherwise
	std::string takeTurn();
	//runs the death drop of Goblin
	void dropOnDeath();
	//returns the minimum path to the player from current position if that path is less than or equal to the given amount of moves
	//if not possible to reach player in less than the given number of moves returns -1 
	//parameters: 
	//r:starting row, c:starting col, moves: max numbers of moves allowed to reach the player
	//direction is changed to represent the direction in which the goblin should move to reach the player
		//0 is up, 1 is down, 2 is left, 3 is right, -1 is no possible moves
	int pathToPlayer(int r, int c,int moves , int& direction);

};



#endif // !ACTOR_INCLUDED
