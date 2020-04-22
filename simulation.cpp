//simulation for the SIM living in a build house
//code by Milk
#include "includes/class_def.h"


class Node;

float calculateFitness(Sim* s);
void findNeedObj(Sim *s, int needIndex);
//void findNeedObj(House* house, int needIndex);
float objDist(Sim *s, Object *o);
float objDistManhattan(Sim *s, Object *o);


list<tuple<int,int>> getBFSPath(tuple<int, int>start, tuple<int, int>end, tuple<int, int> boundary, list<tuple<int,int>>xs);
list<Node *> getNeighbors(Node* n, tuple<int,int> bounds, list<tuple<int,int>> xs);
bool visited(list<Node *> v, Node* n);
bool inSet(list<tuple<int,int>>s, tuple<int,int>e);

//for use with the BFS algorithm
class Node{
	public:
		string id;				//string = "x,y" for ease of access 
		Node *parent;
		tuple<int, int>xy;

		Node(tuple<int,int> c, Node* par_node){
			xy = c;
			id = to_string(get<0>(c)) + "," + to_string(get<1>(c));
			parent = par_node;
		}
};




// ----- REMEMBER -----
//needs: hunger, hygeine, bladder, energy, social, fun


//simulates a sim living in a house
//input: 
//	Sim* sims 					: array of sims to simulate on
//  int numSims					: number of sims in the array [sims]
//	House house 				: the house to simulate in
//	int maxTicks 				: the number of ticks to simulate for - at the end returns the end state of the sim
//	vector<int> rate			: how often to decrease the energy levels ( applies to the vector needs accordingly )
//	int threshold				: the value sim starts to attend to needs (i.e. when bladder reaches 3 - search for toilet)
//  vector<int> needsRanking	: how to rank the priority of the needs when looking for an object to fulfill it

//output:
//	float fitness	: evaluation of the Sim's end condition (see calculateFitness)
float simulate(Sim* simChar, int maxTicks, vector<int> rate, int threshold, vector<int> needsRanking){
	int tick;
	for(tick=0;tick<maxTicks;tick++){

		//0. check if dead
		if(simChar->isDead())		//miss keisha? Miss Keeisshaaa? MISS KEISHA! 
			break;

		//1. set the target path if not already set
		if(simChar->hasTarget() && !simChar->hasNavPath()){
			//get the room objects' coordinates
			vector<Object *>roomObjs = simChar->getRoom()->getObjects();
			list<tuple<int,int>> objCoords;
			int o;
			for(o=0;o<roomObjs.size();o++){
				objCoords.push_back(roomObjs[o]->getCoordinates());
			}

			//make the path
			list<tuple<int,int>> bfs_path = getBFSPath(simChar->getCoordinates(), simChar->getTarget()->getCoordinates(), simChar->getRoom()->getDimensions(), objCoords);
			simChar->setNavPath(bfs_path);
		}

		//1.2 go to the target
		simChar->goToNext();

		//2. check if at the target object (if has one) - and use the object if so
		simChar->atTarget();

		//3. apply the needs decrement
		int n;
		for(n=0;n<rate.size();n++){
			if(tick == 0)				//skip the first iteration
				break;

			if(rate[n] % tick == 0)		//if the rate matches - decrement a need
				simChar->alterNeed(n, -1);
		}

		//4. if the target has been set - ignore other needs until it is fulfilled
		if(simChar->hasTarget())
			continue;

		//4. find next need to fulfill
		for(n=0;n<needsRanking.size();n++){
			int needIndex = needsRanking[n];
			vector<int> simNeeds = simChar->getNeeds();
			if(simNeeds[needIndex] < threshold){
				findNeedObj(simChar, needIndex);
			}
		}
		
	}

	return calculateFitness(simChar);
}



//simulates multiple sims living in a house
//input: 
//	Sim* sims 					: array of sims to simulate on
//  int numSims					: number of sims in the array [sims]
//	House house 				: the house to simulate in
//	int maxTicks 				: the number of ticks to simulate for - at the end returns the end state of the sim
//	vector<int> rate			: how often to decrease the energy levels ( applies to the vector needs accordingly )
//	int threshold				: the value sim starts to attend to needs (i.e. when bladder reaches 3 - search for toilet)
//  vector<int> needsRanking	: how to rank the priority of the needs when looking for an object to fulfill it

//output:
//	float fitness	: evaluation of the Sim's end condition (see calculateFitness)
/*
float* multiSimulate(Sim** sims, int numSims, int maxTicks, vector<int> rate, int threshold, vector<int> needsRanking){
	int tick;
	for(tick=0;tick<maxTicks;tick++){
		int s;
		for(s=0;s<numSims;s++){
			Sim* simChar = sims[s];		//current sim

			//0. check if dead
			if(simChar->isDead())		//miss keisha? Miss Keeisshaaa? MISS KEISHA! 
				break;

			//1. set the target path if not already set
			if(simChar->hasTarget() && !simChar->hasNavPath()){
				//get the room objects' coordinates
				vector<Object *>roomObjs = simChar->getRoom()->getObjects();
				vector<tuple<int,int>> objCoords;
				int o;
				for(o=0;o<roomObjs.size();o++){
					objCoords.push_back(roomObjs[o]->getCoordinates());
				}

				//make the path
				simChar->setNavPath(getBFSPath(simChar->getCoordinates(), simChar->getTarget()->getCoordinates(), simChar->getRoom()->getDimensions(), objCoords));
			}

			//1.2 go to the target
			simChar->goToNext();


			//2. check if at the target object (if has one) - and use the object if so
			simChar->atTarget();

			//3. navigate to the target if applicable
			simChar->goToNext();

			//3. apply the needs decrement
			int n;
			for(n=0;n<rate.size();n++){
				if(tick == 0)				//skip the first iteration
					break;

				if(rate[n] % tick == 0)		//if the rate matches - decrement a need
					simChar->alterNeed(n, -1);
			}

			//4. find next need to fulfill if no target is set
			if(simChar->hasTarget())
				continue;

			for(n=0;n<needsRanking.size();n++){
				int needIndex = needsRanking[n];
				vector<int> simNeeds = simChar->getNeeds();
				if(simNeeds[needIndex] < threshold){
					findNeedObj(simChar, needIndex);
				}
			}
		}
	}

	//calculate all of the fitnesses for the sims
	float* fitnesses = new float[numSims];
	int f;
	for(f=0;f<numSims;f++){
		fitnesses[f] = calculateFitness(sims[f]);
	}

	return fitnesses;
}

*/


//calculates a fitness value based on the Sim's end condition
//1 = best fitness + health | 0 = dead
float calculateFitness(Sim* s){
	//if the sim died - it's the worst fitness
	if(s->isDead())
		return 0;

	//tally all of the needs and divide them by 60 (needs [6] * maximum value [10])
	int n;
	float avg = 0;
	vector<int> needs = s->getNeeds();
	for(n=0;n<needs.size();n++){
		avg += needs[n];
	}
	avg /= 60;

	return avg;
}

//fulfills a need by identifying the closest object in the house that 
// can satisfy the particular need and setting it as the Sim's target object to navigate to

//void findNeedObj(Sim *s, House* house, int needIndex){
void findNeedObj(Sim *s, int needIndex){
	//need array = [hunger, hygeine, bladder, energy, social, fun]

//1. iterate through all of the object and find the closest, need-beneficial object 

	//1.1 find objects within current room
	Room *curRoom = s->getRoom();
	vector<Object *>roomObjs = curRoom->getObjects();	//replace later with ALL objects in the house


	vector<Object *>beneficialObjs;
	int o;
	for(o=0;o<roomObjs.size();o++){
		if(roomObjs[o]->getNeedValue(needIndex) > 0){
			beneficialObjs.push_back(roomObjs[o]);
		}
	}

	//1.2 find object within house - if none in the current room
	/* coming soon

	if()

	*/

	//if no good object found - return
	if(beneficialObjs.size() == 0)
		return;

//2. use the closest object as the target for the Sim
	float smallDist = 10000.0;
	int bestObjIndex = 0;
	int b;
	for(b=0;b<beneficialObjs.size();b++){
		Object *o = beneficialObjs[b];
		float d = objDist(s, o);
		if(d < smallDist){
			smallDist = d;
			bestObjIndex = b;
		}
	}

	s->setTarget(beneficialObjs[bestObjIndex]);		//set the target

	return;
}

//returns navigation queue from xy1 to xy2
// Input: 
//   tuple<int, int>start 				: starting point on the map
//   tuple<int, int>end 				: ending target point on the map
//   tuple<int, int>boundary 			: boundary of the map (w, h) starting from 0,0 in top left
//   list<tuple<int,int>>xs			: blocked off points of the map (objects or walls)

// Output:
//	 list<tuple<int,int>> outPath		: resulting path to take from start to end
list<tuple<int,int>> getBFSPath(tuple<int, int>start, tuple<int, int>end, tuple<int, int> boundary, list<tuple<int,int>>xs){
	string startID = to_string(get<0>(start)) + "," + to_string(get<1>(start));
	string endID = to_string(get<0>(end)) + "," + to_string(get<1>(end));

	//intialize queue
	list<Node *>queue;
	list<Node *>visitedList;
	Node initnode(start, nullptr);
	queue.push_front(&initnode);
	visitedList.push_front(&initnode);

	//solution node
	Node *matchNode;

	//search the entire map
	while(queue.size() > 0){
		Node *curNode = queue.front();
		queue.pop_front();

		//check the neighbors for unfound points 
		list<Node *> neighbors = getNeighbors(curNode,boundary, xs);
		
		list<Node *>::iterator n;
		for(n=neighbors.begin();n != neighbors.end();n++){
			Node *node = *n;

			//found the point!
			if(node->id == endID){
				matchNode = node;
				break;
			}

			//if not already visited - add to the queue
			if(!visited(visitedList, node)){
				queue.push_front(node);
				visitedList.push_front(node);
			}
		}

		//found a match
		if(matchNode != nullptr)
			break;
	}

	//no match - return empty handed
	if(matchNode == nullptr)
		return list<tuple<int,int>>();
	

	//trace it back to the source to get the path
	list<tuple<int, int>> back_path;
	while(matchNode != nullptr){
		back_path.push_front(matchNode->xy);
		matchNode = matchNode->parent;
	}

	return back_path;
}

//gets the neighboring nodes (in 8 directions) of a given node
//checks for boundary area and if an object is already occupying the space
list<Node *> getNeighbors(Node* p, tuple<int,int> bounds, list<tuple<int,int>> xs){
	list<Node *> neighbors;

	float x = get<0>(p->xy);
	float y = get<1>(p->xy);
	float w = get<0>(bounds);
	float h = get<1>(bounds);
	
	//top
	if(x-1 >= 0 && y-1 >= 0 && !inSet(xs, tuple<int,int>{x-1,y-1})){
		Node n(tuple<int,int>{x-1,y-1}, p);
		neighbors.push_back(&n);
	}
	if(y-1 >= 0  && !inSet(xs, tuple<int,int>{x,y-1})){
		Node n(tuple<int,int>{x,y-1}, p);
		neighbors.push_back(&n);
	}
	if(x+1 < w  && y-1 >= 0 && !inSet(xs, tuple<int,int>{x+1,y-1})){
		Node n(tuple<int,int>{x+1,y-1}, p);
		neighbors.push_back(&n);
	}

	//middle
	if(x-1 >= 0 && !inSet(xs, tuple<int,int>{x-1,y})){
		Node n(tuple<int,int>{x-1,y}, p);
		neighbors.push_back(&n);
	}
	if(x+1 < w  && !inSet(xs, tuple<int,int>{x+1})){
		Node n(tuple<int,int>{x+1,y-1}, p);
		neighbors.push_back(&n);
	}

	//bottom
	if(x-1 >= 0 && y+1 < h && !inSet(xs, tuple<int,int>{x-1,y+1})){
		Node n(tuple<int,int>{x-1,y+1}, p);
		neighbors.push_back(&n);
	}
	if(y+1 < h  && !inSet(xs, tuple<int,int>{x,y+1})){
		Node n(tuple<int,int>{x,y+1}, p);
		neighbors.push_back(&n);
	}
	if(x+1 < w && y+1 < h  && !inSet(xs, tuple<int,int>{x+1,y+1})){
		Node n(tuple<int,int>{x+1,y+1}, p);
		neighbors.push_back(&n);
	}

	return neighbors;

}

//returns if a node has already been visited or not
bool visited(list<Node *> v, Node* n){
	list<Node *>::iterator i;
	for(i=v.begin();i != v.end();i++){
		if(n->id == (*i)->id)
			return true;
	}
	return false;
}

//check if coordinates is in a set of coordinates
bool inSet(list<tuple<int,int>>s, tuple<int,int>e){
	list<tuple<int,int>>::iterator i;
	for(i=s.begin();i != s.end();i++){
		if(get<0>(*i) == get<0>(e) && get<1>(*i) == get<1>(e))
			return true;
	}
	return false;
}


//euclidean distance between Sim and Object
float objDist(Sim *s, Object *o){
	float x_1 = get<0>(o->getCoordinates());
    float y_1 = get<1>(o->getCoordinates());

    float x_2 = get<0>(s->getCoordinates());
    float y_2 = get<1>(s->getCoordinates());
        
    return sqrt( pow(x_2 - x_1, 2) + pow(y_2 - y_1, 2) );
}	

//manhattan distance between Sim and Object
float objDistManhattan(Sim *s, Object *o){
	float x_1 = get<0>(o->getCoordinates());
    float y_1 = get<1>(o->getCoordinates());

    float x_2 = get<0>(s->getCoordinates());
    float y_2 = get<1>(s->getCoordinates());

    return abs(x_2-x_1) + abs(y_2-y_1);
}



int main(){return 0;}




