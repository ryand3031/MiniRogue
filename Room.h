#ifndef ROOM_INCLUDED
#define ROOM_INCLUDED
//Max dimensions of the dungeon
const int MAX_ROWS = 18;// 18
const int MAX_COLS = 70;//70
class Room {
	public:
	Room(int x1, int y1, int w, int h);
	//copys the room onto an array the size of the dungeon
	//used to copy rooms to Games m_dungeon array
	void copyRoom(char[MAX_ROWS][MAX_COLS]);
	//returns true if this room intersects the given room,or is out of bounds of the total dungeon, returns false otherwise
	bool intersects(const Room& room);
	//trivial getters
	int x1();
	int midx();
	int midy();
private:
	int m_x1;//collumn of the left side of the room
	int m_x2;//collumn of the right side of the room
	int m_y1;//bottom row of the room
	int m_y2;//top row of the room
	int m_midx;//col halfway between m_x1 and m_x2, for even width m_midx is closer to m_x1 by one col
	int m_midy;//row halfway between m_y1 and m_y2, for even height m_midy is closer to m_y2 by one row
};

//returns true if the leftmost wall of room1 is in a row less than the leftmost wall of room 2
//returns false otherwise
bool furthestLeft(Room room1, Room room2); 

#endif // !ROOM_INCLUDED