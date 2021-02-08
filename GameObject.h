#ifndef GAMEOBJECT_INCLUDED
#define GAMEOBJECT_INCLUDED
#include <string>

class Game;
class Player;


class GameObject {
public:
	//Constructor
	GameObject(char identifier, std::string name, std::string action, int row, int col);
	//Deconstructor
	virtual ~GameObject();
	//trivial getters
	std::string name()const;
	std::string action()const;
	int row()const;
	int col()const;
	char identifier()const;
	//Sets row and column position
	void setPosition(int r, int c);
	//returns pickup string when gameObject is picked up
	virtual std::string pickup() = 0;

private:
	char m_identifier;//char that is displayed on the board for the gameObject
	std::string m_name;//gives name of the object
	std::string m_action;//gives the action that is displayed when the object is attacked with(weapon) or is used(scroll)
	//gives location in the dungeon
	//m_row and m_col of -1 signifies that the object is not on the dungeon floor
	int m_row;
	int m_col;

};

//GameObject Subclasses
class Weapon : public GameObject {
public:
	//constructor
	Weapon(int row, int col, std::string name, std::string action, int dexterityBonus, int damage);

	//trivial getters
	int dexterityBonus()const;
	int damage()const;
	//returns pickup string when gameObject is picked up
	virtual std::string pickup();
private:
	int m_dexterityBonus;//weapons dexterity boost
	int m_damage;//weapons damage
};

class Scroll : public GameObject {
public:
	//constructor
	Scroll(int row, int col, std::string name, std::string action, int boostVal);
	//trivial getter
	int boostVal()const;
	//implements the scrolls effect on the player and returns the cooresponding message
	virtual std::string use(Player* player) = 0;
	//returns pickup string when gameObject is picked up
	virtual std::string pickup();

private:
	//boost value for the scroll to tell how much it will boost the players attribute
	//set to -1 and not used for teleportation scroll
	int m_boostVal;
};

class GoldenIdol : public GameObject {
public:
	GoldenIdol(int row = -1, int col=-1);
	virtual std::string pickup();
};


//Weapon Subclasses
//Weapons can be initialized with specific coordinated or are default set to (row, col)=(-1,-1) to signify not yet placed on the map
class ShortSword : public Weapon {
public:
	ShortSword(int row =-1, int col = -1);
};

class Mace : public Weapon {
public:
	Mace(int row = -1, int col = -1);
};

class LongSword : public Weapon {
public:
	LongSword(int row = -1, int col = -1);
};

class MagicAxe : public Weapon {
public:
	MagicAxe(int row = -1, int col = -1);
};

class MagicFangs : public Weapon {
public:
	MagicFangs(int row = -1, int col = -1);
};



//Scroll subclasses
//Scrolls can be initialized with specific coordinated or are default set to (row, col)=(-1,-1) to signify not yet placed on the map
class ScrollOfTeleportation : public Scroll {
public:
	ScrollOfTeleportation(Game* game,int row = -1, int col = -1);
	virtual std::string use(Player* player);//uses the scroll
private:
	Game* m_game;//pointer to the game in order to properly move the player to a new location when the scroll is used
};

class ScrollOfEnhanceArmor : public Scroll {
public:
	ScrollOfEnhanceArmor(int row = -1, int col = -1);
	virtual std::string use(Player* player);//uses the scroll
};

class ScrollOfRaiseStrength : public Scroll {
public:
	ScrollOfRaiseStrength(int row = -1, int col = -1);
	virtual std::string use(Player* player);//uses the scroll
};

class ScrollOfEnhanceHealth : public Scroll {
public:
	ScrollOfEnhanceHealth(int row = -1, int col = -1);
	virtual std::string use(Player* player);//uses the scroll
};

class ScrollOfEnhanceDexterity : public Scroll {
public:
	ScrollOfEnhanceDexterity(int row = -1, int col = -1);
	virtual std::string use(Player* player);//uses the scroll
};

#endif // !GAMEOBJECT_INCLUDED
