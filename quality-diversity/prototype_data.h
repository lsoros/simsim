//Prototype data header file
//Code by Milk

#include <string>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <unordered_map>
#include "boost/variant.hpp"

using namespace std;

//sample object list to include in the room/house
const string OBJECT_TYPE_LIST[13] = {"chair", "table", "bed", "toilet", "fridge",
								"sink", "light", "TV", "shower", "oven",
								"stove", "sofa", "decor"};

//sample object cost - directly related to object type list
const int OBJECT_COST_LIST[13] = {20, 40, 150, 100, 300, 75, 10, 120, 250, 350, 300, 5, 60};

//sample range for the number of objects in the room/house
const int NUM_OBJ_RANGE[2] = {0,5};


//unordered map version of room (properties accessed by string name)
class UltraRoom{
	public:
		unordered_map<
			string, 
			boost::variant<unordered_map<string, int>, int, double> 
		> properties;

	//creates a room with random values
	UltraRoom(){
		int totalCost = 0;

		int i;
		unordered_map<string, int> furnitureTally;
		for(i=0;i<13;i++){
			int ct = rand() % NUM_OBJ_RANGE[1] + NUM_OBJ_RANGE[0];	//set count of obj
			furnitureTally[OBJECT_TYPE_LIST[i]] = ct;
			totalCost += ct*OBJECT_COST_LIST[i];
		}

		double shealth = ((double) rand()/(RAND_MAX));

		properties["Furniture"] = furnitureTally;
		properties["TotalCost"] = totalCost;			//set total cost of the room
		properties["SimHealth"] = shealth;				//random value between 0 and 1
	}

};

void showUltraRoom(UltraRoom ur);

