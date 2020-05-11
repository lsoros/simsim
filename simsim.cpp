#include "includes/simsim_func.h"

void runExp();

////////////////////////////////
//   TESTER EXPERIMENT CODE   //
////////////////////////////////


tuple<int,int> randPos(tuple<int,int>dimen){
	return {(rand() % get<0>(dimen)), (rand() % get<1>(dimen))};
}

void printNeeds(vector<int> needs){
	cout << vec2str(needs);
}

void printObjCt(map<string, int>objsCt){
    map<string, int>::iterator oi;

    for(oi=objsCt.begin();oi!=objsCt.end();++oi){
        cout << oi->first << ": " << oi->second << endl;
    }
}

void showObjList(){
	cout << "Getting object list...\n";
	map<string, vector<int>> fullObjList = getfullObjList();

	cout << "Making ascii character map...\n";
	map<string, char> charMap = makeObjAsciiMap(fullObjList);

	map<string, vector<int>>::iterator f;
	for(f=fullObjList.begin();f!=fullObjList.end();++f){
		cout << f->first << ": [";
		printNeeds(f->second);
		cout << "]\n";
	}

	 map<string, char>::iterator c;
	 for(c=charMap.begin();c!=charMap.end();++c){
	 	cout << c->first << " : " << c->second << "\n";
	 }
}

void showTestRoom(){

	//cout << "Getting object list...\n";
	map<string, vector<int>> fullObjList = getfullObjList();

	//cout << "Making ascii character map...\n";
	map<string, char> charMap = makeObjAsciiMap(fullObjList);


	Sim sim("Foo Bar");
    Room livingroom("Test Room");
    House testHouse("Some House");
    testHouse.add_room(livingroom);
    livingroom.placeSim(sim);
    
    //needs order: hunger, hygeine, bladder, energy, social, fun
    
   	// objects the sim can interact with and affect their needs
    Object fridge("fridge", fullObjList["fridge"], {6,5});
    Object toilet("toilet", fullObjList["toilet"], {9,8});
    Object bed("bed", fullObjList["bed"], {2,0});

    livingroom.add_object(fridge);
    livingroom.add_object(toilet);
    livingroom.add_object(bed);

    //print the house
    cout << testHouse.asciiRep(charMap) << endl;
    
}

void addRandObjects(Room& room){
	//getting the full object list
	map<string, vector<int> > fullObjList = getfullObjList();
	// map iterator
	map<string, vector<int> >::iterator it;
	// get a random number out of the 80 objects
	int objnum = rand() % 80
	// get the object name by using iterator arithmetic, then dereferencing, and getting the key
	string objname = (*(it + objnum)).first;
	// get object FX needs by using iterator arithmetic, then dereferencing, and getting mapped value
	vector<int> objFX = (*(it + objnum)).second;
	// new object of the random object
	Object newobj(objname, objFX, randPos(room.getDimensions()));
	// add object to room
	room.add_object(newobj);

}



void simulateTest(){
	//cout << "Getting object list...\n";
	map<string, vector<int>> fullObjList = getfullObjList();

	//cout << "Making ascii character map...\n";
	map<string, char> charMap = makeObjAsciiMap(fullObjList);


	Sim sim("Foo Bar");
    Room livingroom("Test Room", {5,5});
    House testHouse("Some House");
    testHouse.add_room(livingroom);
    livingroom.placeSim(sim);
    
    //needs order: hunger, hygeine, bladder, energy, social, fun
    
   	// objects the sim can interact with and affect their needs

    Object fridge("fridge", fullObjList["fridge"], randPos(livingroom.getDimensions()));
    Object toilet("toilet", fullObjList["toilet"], randPos(livingroom.getDimensions()));
    Object bed("bed", fullObjList["bed"], randPos(livingroom.getDimensions()));

    livingroom.add_object(fridge);
    livingroom.add_object(toilet);
    livingroom.add_object(bed);

    //print the house
    cout << testHouse.asciiRep(charMap) << endl;

    //simulate
    vector<int> decNeedRate{5,10,5,7,15,15};
	vector<int> needsRanking{0,2,3,1,4,5};
	int _maxticks = 100;
    float fitness = simulate(&sim, _maxticks, decNeedRate, 3, needsRanking);

    cout << "House Fitness: " << fitness << endl;
}

void cloneTest(){
	//cout << "Getting object list...\n";
	map<string, vector<int>> fullObjList = getfullObjList();

	//cout << "Making ascii character map...\n";
	map<string, char> charMap = makeObjAsciiMap(fullObjList);


	Sim sim("Foo Bar");
    Room livingroom("Test Room", {5,5});
    House testHouse("Some House");
    testHouse.add_room(livingroom);
    livingroom.placeSim(sim);
    
    //needs order: hunger, hygeine, bladder, energy, social, fun
   	Object fridge("fridge", fullObjList["fridge"], randPos(livingroom.getDimensions()));
    Object toilet("toilet", fullObjList["toilet"], randPos(livingroom.getDimensions()));
    Object bed("bed", fullObjList["bed"], randPos(livingroom.getDimensions()));

    livingroom.add_object(fridge);
    livingroom.add_object(toilet);
    livingroom.add_object(bed);


// TEST CLONE OBJECT
    Object bed2 = bed;
    bed2.changeCoordinates({100,100});	//check if seperate entity

    //cout << bed << endl;
    //cout << bed2 << endl;

// TEST CLONE ROOM

    //duplicate rooms
    Room room2 = livingroom;
    Object athing("video game console", fullObjList["video game console"], randPos(room2.getDimensions()));
    room2.add_object(athing);

    //cout << livingroom << "\n" << endl;
    //cout << room2 << endl;


// TEST CLONE HOUSE
    //make a copy of the house
    House otherHouse = testHouse;
    Room* otherRoom = otherHouse.getRooms()[0];

   
    Object cthing("microwave", fullObjList["microwave"], randPos(otherRoom->getDimensions()));
    Object bthing("bed", fullObjList["bed"], randPos(otherRoom->getDimensions()));

    otherRoom->add_object(cthing);
    otherRoom->add_object(bthing);


     //print the house
    //cout << testHouse.asciiRep(charMap) << endl;
    //cout << otherHouse.asciiRep(charMap) << endl;
    
}

void noveltyTest(){
	map<string, vector<int>> fullObjList = getfullObjList();
	map<string, char> charMap = makeObjAsciiMap(fullObjList);

	//create house 1
    Room livingroom("Test Room", {3,3});
    House testHouse("Some House");
    testHouse.add_room(livingroom);
   	Object fridge("fridge", fullObjList["fridge"], randPos(livingroom.getDimensions()));
    Object toilet("toilet", fullObjList["toilet"], randPos(livingroom.getDimensions()));
    Object bed("bed", fullObjList["bed"], randPos(livingroom.getDimensions()));
	livingroom.add_object(fridge);
    livingroom.add_object(toilet);
    livingroom.add_object(bed);

    //make a second smaller house
    House house2("Another House");
    Room bedroom("Bed Room", {3,3});
    house2.add_room(bedroom);
    Object bed2 = bed;
    bedroom.add_object(bed2);

    //make a second smaller house
    House house3("Shitty House");
    Room bathroom("Bath Room", {3,3});
    house3.add_room(bathroom);
    Object toilet2 = toilet;
    bathroom.add_object(toilet2);

    cout << testHouse.asciiRep(charMap) << endl;
    cout << house2.asciiRep(charMap) << endl;
    cout << house3.asciiRep(charMap) << endl;
    


	 //make a copy of the house
    House otherHouse = testHouse;
    otherHouse.setName("Copy House");
    Room* otherRoom = otherHouse.getRooms()[0];
    Object athing("video game console", fullObjList["video game console"], randPos(otherRoom->getDimensions()));
    Object cthing("microwave", fullObjList["microwave"], randPos(otherRoom->getDimensions()));
    Object bthing("bed", fullObjList["bed"], randPos(otherRoom->getDimensions()));
    otherRoom->add_object(cthing);
    otherRoom->add_object(bthing);
    otherRoom->add_object(athing);

    


    list<House *>novelHouses;
    novelHouses.push_back(&testHouse);
    novelHouses.push_back(&house2);
    novelHouses.push_back(&house3);

    //simulate the first house
    int _maxticks = 5;
	vector<int> decNeedRate{5,10,5,7,15,15};
	vector<int> needsRanking{2,0,3,1,4,5};

    Sim s("Foo Bar");
    otherRoom->placeSim(s);

    cout << otherHouse.asciiRep(charMap) << endl;

    float f1 = simulate(&s, _maxticks, decNeedRate, 30, needsRanking);

    cout << "AVG DIST: " << avg_knn_dist(novelHouses, &otherHouse, 2) << endl;
    cout << "IS NOVEL: " << isNovel(novelHouses, &otherHouse, f1) << endl;

    cout << "\n" << endl;


    cout << "\nHouse 1" << endl;
    printObjCt(testHouse.getObjectCt());
    cout << "\nHouse 2" << endl;
    printObjCt(house2.getObjectCt());
    cout << "\nHouse 3" << endl;
    printObjCt(house3.getObjectCt());
    cout << "\nTest House" << endl;
    printObjCt(otherHouse.getObjectCt());
    
}

void jsonTEST(){
	map<string, vector<int>> fullObjList = getfullObjList();
	map<string, char> charMap = makeObjAsciiMap(fullObjList);

	//create house 1
    Room livingroom("Test Room", {3,3});
    House testHouse("Some House", 100);
    testHouse.add_room(livingroom);
   	Object fridge("fridge", fullObjList["fridge"], randPos(livingroom.getDimensions()));
    Object toilet("toilet", fullObjList["toilet"], randPos(livingroom.getDimensions()));
    Object bed("bed", fullObjList["bed"], randPos(livingroom.getDimensions()));
	livingroom.add_object(fridge);
    livingroom.add_object(toilet);
    livingroom.add_object(bed);

    //cout << "---OBJ:---\n" << bed.toJSON() << endl;
    //cout << "---ROOM:---\n" << livingroom.toJSON() << endl;
   	cout << "---HOUSE:---\n" << testHouse.toJSON() << endl;

    ofstream houseFile("NOVEL_OUTPUT/House_" + to_string(testHouse.getId()) + ".json");
	houseFile << testHouse.toJSON() << "\n";
	houseFile.close();
		
}

int main(){
	srand((unsigned) time(0));

// TEST FUNCTIONS 
	//showObjList();
	//showTestRoom();
	//simulateTest();
	//cloneTest();
	//noveltyTest();
	jsonTEST();

// ACTUAL EXPERIMENT
	//runExp();			//seg faults unless run (still missing initializer function and mutator)

	return 1;
}




///////////////////////////////////
//   MAIN LOOP EXPERIMENT CODE   //
///////////////////////////////////


void runExp(){		//feel like some kind of arguments should go here; maybe file input?

	///////    PARAMETERS  //////
	int _popsize = 30;
	int _generations = 10;
	/*
	float _mut_add_prob = 0.3;
	float _mut_delete_prob = 0.1;
	float _mut_move_prob = 0.4;
	*/

	int curGen = 0;

	int _maxticks = 100;
	//needs order: hunger, hygeine, bladder, energy, social, fun
	vector<int> decNeedRate{5,10,5,7,15,15};
	vector<int> needsRanking{2,0,3,1,4,5};

//1. initial seed generation code 
	/* WRITE INIT CODE HERE */
	int houseIdIndex = 1;
	House initHouse("Laboratory",houseIdIndex);		//experiments done in labs :)
	houseIdIndex++;

	Room r("Experiment Room");
    initHouse.add_room(r);

    map<string, vector<int>> fullObjList = getfullObjList();
	Object fridge("fridge", fullObjList["fridge"], randPos(r.getDimensions()));
    Object toilet("toilet", fullObjList["toilet"], randPos(r.getDimensions()));
    Object bed("bed", fullObjList["bed"], randPos(r.getDimensions()));
	r.add_object(fridge);
    r.add_object(toilet);
    r.add_object(bed);



	/* END INIT CODE */


//2. initial population of popsize
	list<House *> population;
	int i;
	for(i=0;i<_popsize;i++){
		//mutate from the initial house
		House mutHouse = initHouse;
		mutHouse.setId(houseIdIndex);
		mutHouse.setName("Mutated_House #" + to_string(i));
		vector<Room *> rooms = mutHouse.getRooms();
		int r;
		for(r=0;r<rooms.size();r++){
			rooms[r]->mutate_objects();			//function from the mutationsbranch (will be merged later)
		}

		//add to the population
		population.push_back(&mutHouse);

		//increment index
		houseIdIndex++;
	}


	list<House *>novelHouses;

//3. While curGen < generations
	while(curGen < _generations){

		//3.5 i'm assuming the simulation would go somewhere here followed by novelty assignment
		list<House *>::iterator h;
		for(h=population.begin();h != population.end();h++){
			House *popHouse = (*h);
			Sim testSim("Foo Bar");		//make a new sim everytime to reset needs

			float fitness = simulate(&testSim, _maxticks, decNeedRate, 30, needsRanking);

			bool foundNovelHouse = isNovel(novelHouses, popHouse, fitness);
			if(foundNovelHouse)
				novelHouses.push_back(popHouse);
		}

		//a. create an empty list for the next generation's population
		list<House *> newPop;			


		//b + c. randomly select (popsize/6) houses from the novel archive and the population to be parents
		// (shuffles the list then pick the first n elements)
		int parentNum = round(_popsize/6);

		vector<House *>pseudoPop(population.begin(), population.end());		//copy for safety
		random_shuffle(pseudoPop.begin(), pseudoPop.end());

		vector<House *>pseudoNovel(novelHouses.begin(), population.end());	//copy archive likewise
		random_shuffle(pseudoNovel.begin(), pseudoNovel.end());

		list<House *>parentSet;					//add both to the parent set
		int p;
		for(p=0;p<parentNum;p++){
			House *popHouse = pseudoPop[p];
			House *novHouse = pseudoNovel[p];

			parentSet.push_back(popHouse);
			parentSet.push_back(novHouse);
		}


		//d. mutate each parent 3x and add generated offspring to new population

		//could use a while loop here, but meh same result (theoretically)
		for (int i = 0; i < _popsize; ++i){
			//pop off
			House *curHouse = parentSet.front();
			parentSet.pop_front();

			int m;
			for(m=0;m<3;m++){
				//copy
				House noobHouse = (*curHouse);
				noobHouse.setId(houseIdIndex);

				//mutate
				int r;
				vector<Room *>noobRooms = noobHouse.getRooms();
				for(r=0;r<noobRooms.size();r++){
					noobRooms[r]->mutate_objects();
				}

				//add
				newPop.push_back(&noobHouse);

				//increment counter
				houseIdIndex++;
			}
		}


		//e. replace old population (garbage collection should delete it)
		population = newPop;

		//f. iterate otherwise press F to pay respects to your machine
		curGen++;						
	}


//4. dump the rooms to archive
	list<House*>::iterator n;
	for(n=novelHouses.begin();n!=novelHouses.end();n++){
		House *nov_house = (*n);
		ofstream houseFile("NOVEL_OUTPUT/House_" + to_string(nov_house->getId()) + ".json");
		houseFile << nov_house->toJSON() << "\n";
		houseFile.close();
	}
	
	



	return;
}


