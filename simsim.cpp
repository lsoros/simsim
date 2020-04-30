#include "includes/simsim_func.h"

///////////////////////////////////
//   MAIN LOOP EXPERIMENT CODE   //
///////////////////////////////////


void printNeeds(vector<int> needs){
	int i;
	for(i=0;i<needs.size();i++){
		cout << needs[i];
		if(i < needs.size()-1)
			cout << " ";
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

tuple<int,int> randPos(tuple<int,int>dimen){
	return {(rand() % get<0>(dimen)), (rand() % get<1>(dimen))};
}

void simulateTest(){
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
	vector<int> needsRanking{2,0,3,1,4,5};
	int _maxticks = 100;
    float fitness = simulate(&sim, _maxticks, decNeedRate, 30, needsRanking);

    cout << "House Fitness: " << fitness << endl;
}

int main(){
	srand((unsigned) time(0));
	//showObjList();
	//showTestRoom();
	simulateTest();

	return 1;
}








int runExp(){		//feel like some kind of arguments should go here; maybe file input?

	///////    PARAMETERS  //////
	int _popsize = 30;
	int _generations = 10;
	float _mut_add_prob = 0.3;
	float _mut_delete_prob = 0.1;
	float _mut_move_prob = 0.4;

	int curGen = 0;

	int _maxticks = 100;
	//needs order: hunger, hygeine, bladder, energy, social, fun
	vector<int> decNeedRate{5,10,5,7,15,15};
	vector<int> needsRanking{2,0,3,1,4,5};

//1. initial seed generation code 
	/* WRITE INIT CODE HERE */
	House initHouse("Laboratory");		//experiments done in labs :)






	/* END INIT CODE */


//2. initial population of popsize
	list<House *> population;
	int i;
	for(i=0;i<_popsize;i++){
		//mutate from the initial house
		House mutHouse("Mutated_House #" + to_string(i));
		initHouse.copyHouse(&mutHouse);
		vector<Room *> rooms = mutHouse.getRooms();
		int r;
		for(r=0;r<rooms.size();r++){
			rooms[r]->mutate_objects();			//function from the mutationsbranch (will be merged later)
		}

		//add to the population
		population.push_back(&mutHouse);
	}


	list<House *>novelHouses;

//3. While curGen < generations
	while(curGen < _generations){

		//3.5 i'm assuming the simulation would go somewhere here followed by novelty assignment
		list<House *>::iterator h;
		for(h=population.begin();h != population.end();h++){
			House *popHouse = (*h);
			Sim testSim("Chell");		//make a new sim everytime to reset needs

			float fitness = simulate(&testSim, _maxticks, decNeedRate, 30, needsRanking);

			bool foundNovelHouse = isNovel(novelHouses, popHouse, fitness);
			if(foundNovelHouse)
				addToNoveltySet(novelHouses, popHouse);
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
				House noobHouse(curHouse->getName());
				curHouse->copyHouse(&noobHouse);

				//mutate
				int r;
				vector<Room *>noobRooms = noobHouse.getRooms();
				for(r=0;r<noobRooms.size();r++){
					noobRooms[r]->mutate_objects();
				}

				//add
				newPop.push_back(&noobHouse);
			}
		}


		//e. replace old population (garbage collection should delete it)
		population = newPop;

		//f. iterate otherwise press F to pay respects to your machine
		curGen++;						
	}


//4. dump the rooms to archive

	/* ASCII (or something) REPRESENTATION TEXT GOES HERE */

}


