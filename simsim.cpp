#include "includes/class_def.h"

///////////////////////////////////
//   MAIN LOOP EXPERIMENT CODE   //
///////////////////////////////////

int main(){		//feel like some kind of arguments should go here; maybe file input?

	///////    PARAMETERS  //////
	int _popsize = 30;
	int _generations = 10;
	float _mut_add_prob = 0.3;
	float _mut_delete_prob = 0.1;
	float _mut_move_prob = 0.4;

	int curGen = 0;

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
		//a. create an empty list for the next generation's population
		list<House *> newPop;			


		//b + c. randomly select (popsize/6) houses from the novel archive and the population to be parents
		// (shuffles the list then pick the first n elements)
		int parentNum = round(_popsize/6);

		list<House *>pseudoPop(population);		//copy for safety
		random_shuffle(pseudoPop.begin(), pseudoPop.end());

		list<House *>pseudoNovel(novelHouses);	//copy archive likewise
		random_shuffle(pseudoNovel.begin(), pseudoNovel.end());

		list<House *>parentSet;					//add both to the parent set
		int p;
		for(p=0;p<parentNum;p++){
			House *popHouse = pseudoPop.front();
			pseudoPop.pop_front();	

			House *novHouse = pseudoNovel.front();
			pseudoNovel.pop_front();

			parentSet.push_back(popHouse);
			parentSet.push_back(novHouse);
		}


		//d. mutate each parent 3x and add generated offspring to new population
		
		//could use a while loop here, but meh same result (theoretically)
		for (int i = 0; i < _popsize; ++i){
			//pop off
			House *curHouse = parentSet.front();
			curHouse.pop_front();

			int m;
			for(m=0;m<3;m++){
				//copy
				House noobHouse(curHouse->getName());
				curHouse.copyHouse(&noobHouse);

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
		population = newPop

		//f. iterate otherwise press F to pay respects to your machine
		curGen++;						
	}


//4. dump the rooms to archive

	/* ASCII (or something) REPRESENTATION TEXT GOES HERE */

}


