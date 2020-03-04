//Prototype data header file
//Code by Milk

#include <string>
#include <time.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

//sample object list to include in the room/house
const string OBJECT_TYPE_LIST[13] = {"chair", "table", "bed", "toilet", "fridge",
								"sink", "light", "TV", "shower", "oven",
								"stove", "sofa", "decor"};

//sample object cost - directly related to object type list
const int OBJECT_COST_LIST[13] = {20, 40, 150, 100, 300, 75, 10, 120, 250, 350, 300, 5, 60};

/*
class HouseObj{
	public:
		string name;	//name of the object
		int size;		//square area of the object
		int cost;		//cost of singular object
		int num;		//number of this type of object in the house
}
*/

class Room{
	public:
		int furnitureTally[13];		//tally of each type of furniture
		int totalCost;			//total cost of the house
		float simHealth;		//resulting health of the sim
};


//sample range for the number of objects in the room/house
const int NUM_OBJ_RANGE[2] = {0,5};

Room createRandomRoom();

void showRoom(Room r);




