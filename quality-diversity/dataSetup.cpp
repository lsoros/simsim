#include "prototype_data.h"

Room createRandomRoom(){
	
	
	Room r;
	int totalCost = 0;
	//HouseObj[13] roomObjs;

	int i;
	for(i=0;i<13;i++){
		/*
		HouseObj o;

		o.name = OBJECT_TYPE_LIST[i];	//set name
		if(o.name == "table"){o.size = 2;}		//set size
		else if(o.name == "bed"){o.size = 4;}
		else if(o.name == "sofa"){o.size = 2;}
		else{o.size = 1;}
		o.cost = OBJECT_COST_LIST[i];		//set cost
		o.num = rand() % NUM_OBJ_RANGE[1] + NUM_OBJ_RANGE[0];	//set count of obj
		totalCost += (o.cost*o.num);
		*/

		r.furnitureTally[i] = rand() % NUM_OBJ_RANGE[1] + NUM_OBJ_RANGE[0];	//set count of obj
		totalCost += (OBJECT_COST_LIST[i]*r.furnitureTally[i]);

		//roomObjs[i] = o;
	}

	float simHealth = ((double) rand()/(RAND_MAX));	//random value between 0 and 1

	
	//r.furniture = roomObjs;
	r.totalCost = totalCost;
	r.simHealth = simHealth;

	return r;
}

void showRoom(Room r){
	int i;
	cout << "Furniture Count: " << endl;
	for(i=0;i<13;i++){
		cout << "\t" << OBJECT_TYPE_LIST[i] << " = " << r.furnitureTally[i] << endl;
	}

	cout << "Total Cost: $" << r.totalCost << endl;
	cout << "Sim Health: " << r.simHealth*100.0f << "%" << endl;
}
