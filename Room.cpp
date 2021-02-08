#include"Room.h"
Room::Room(int x1, int y1, int w, int h) {
	//sets m_x1 and m_x2 to given values
	m_x1 = x1;
	m_y1 = y1;
	//sets m_x2 and m_y2 based on given x and y and given height and width
	m_x2 = x1+w-1;
	m_y2 = y1 - h + 1;
	//sets mid point values for x and y
	//if w or h is even mid point will not be i exact middle
	m_midx = (m_x1 + m_x2) / 2;
	m_midy = (m_y1 + m_y2) / 2;

}
//trivial getters
int Room::x1() { return m_x1; }
int Room::midx() { return m_midx; }
int Room::midy() { return m_midy; }

void Room::copyRoom( char dungeon[MAX_ROWS][MAX_COLS]) {//copys the room onto an array the size of the dungeon
	for (int i = m_y1; i >= m_y2; i--) {//loops through all coordinates in the room
		for (int j = m_x1; j <= m_x2; j++) {
			dungeon[i][j] = ' ';//sets the value of dungon at each point in room to ' '
		}
	}
}

bool Room::intersects(const Room& room) {//returns true if this room intersects the given room or is out of bounds of the total dungeon, returns false otherwise
	if (m_x1 <= 0 || m_x2 >= MAX_COLS - 2 || m_y1 >= MAX_ROWS - 2 || m_y2 <= 0) {//returns true if this room is out of bounds of the dungeon
		return true;
	}
	else if (m_x1 <= room.m_x2+1 && m_x2 >= room.m_x1-1 && m_y1 >= room.m_y2-1 && m_y2 <= room.m_y1+1) {//returns true if this room intersects given room
		return true;
	}
	return false;//returns false otherwise
}

//returns true if the leftmost wall of room1 is in a row less than the leftmost wall of room 2
//returns false otherwise
bool furthestLeft(Room room1, Room room2) {
	return(room1.x1()< room2.x1());
}