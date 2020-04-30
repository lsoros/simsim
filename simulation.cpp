//simulation for the SIM living in a build house
//code by Milk
#include "includes/simsim_func.h"

bool debug = true;
bool debug2 = false;

//for use with the BFS algorithm
class Node{
	friend ostream& operator<<(ostream& os, const Node& n){
        // output operator for Node class
        os << "[" << n.id << "]  <-  ";
        if(n.parentId != "")
      		os << "[" << n.parentId << "]" << endl;
        else
        	os << "null" << endl;

        return os;
    };

	public:
		string id;				//string = "x,y" for ease of access 
		string parentId;
		tuple<int, int>xy;

		Node(tuple<int,int> c, string p){
			xy = c;
			id = tup2str(c);
			parentId = p;
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

			if(debug){
				list<tuple<int,int>>::iterator i;
				cout << "OBJS: ";
				for(i=objCoords.begin();i!=objCoords.end();++i){
					cout << "[" << get<0>(*i) << "," << get<1>(*i) << "]  ";
				}
				cout << endl;
			}
			
			

			//make the path
			list<tuple<int,int>> bfs_path = getBFSPath(simChar->getCoordinates(), simChar->getTarget()->getCoordinates(), simChar->getRoom()->getDimensions(), objCoords);
			
			if(debug){
				list<tuple<int,int>>::iterator i;
				cout << "BFS PATH: ";
				for(i=bfs_path.begin();i!=bfs_path.end();++i){
					cout << "[" << get<0>(*i) << "," << get<1>(*i) << "]  ";
				}
				cout << endl;
			}

			simChar->setNavPath(bfs_path);

			
		}

		//1.2 go to the target
		simChar->goToNext();


		//2. check if at the target object (if has one) - and use the object if so
		simChar->atTarget(debug);


		//3. apply the needs decrement
		int n;
		for(n=0;n<rate.size();n++){
			if(tick == 0)				//skip the first iteration
				break;

			if((tick+1) % rate[n] == 0)		//if the rate matches - decrement a need
				simChar->alterNeed(n, -1);
		}

		if(debug){
			cout << tick << ": ";
			simChar->printNeeds();
		}
		
		//4. if the target has been set - ignore other needs until it is fulfilled
		if(simChar->hasTarget())
			continue;

		//5. find next need to fulfill
		for(n=0;n<needsRanking.size();n++){
			int needIndex = needsRanking[n];
			vector<int> simNeeds = simChar->getNeeds();
			if(simNeeds[needIndex] < threshold){
				findNeedObj(simChar, needIndex);
			}
		}
		
		if(debug){
			cout << "    Target: ";
			if(simChar->hasTarget())
				cout << simChar->getTarget()->getName() << endl;
			else
				cout << "(none)" << endl;
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
	string startID = tup2str(start);
	string endID = tup2str(end);

	//intialize lists and map
	list<Node>queue;
	list<Node>visitedList;
	map<string, tuple<int,int>>famTree;		//key: child, value: parent

	Node initnode(start, "");
	queue.push_front(initnode);
	visitedList.push_front(initnode);

	//solution node
	string matchNodeId = "";

	//search the entire map
	while(queue.size() > 0){
		Node curNode = queue.front();
		queue.pop_front();

		if(debug2){
			cout << "CUR: " << (curNode);
		}

		visitedList.push_front(Node(curNode.xy, curNode.parentId));

		//check the neighbors for unfound points 
		list<Node> neighbors;
		getNeighbors(&curNode,boundary, neighbors);
		
		list<Node>::iterator n;
		for(n=neighbors.begin();n != neighbors.end();++n){
			Node node = (*n);

			//add to branching tree if not already added
			if(famTree.count(node.id) == 0)
				famTree.insert(pair<string,tuple<int,int>>(node.id, curNode.xy));

			//if not already visited - add to the queue and the family tree
			if(!visited(visitedList, &node) && !visited(queue, &node) && !inSet(xs, node.xy)){
				queue.push_back(Node(node.xy, node.parentId));
				if(debug2){
					cout << "     " << (node) << endl;
				}
			}


			//found the point! seems dumb to save to a string...
			if(node.id == endID){
				matchNodeId = node.id;
				break;
			}
		}

		if(debug2){
			list<Node>::iterator v;
			cout << "VISITED: " << endl; 
			for(v=visitedList.begin();v != visitedList.end();v++){
				cout << (v)->id << " ";
			}
			cout << endl;

			list<Node>::iterator t;
			cout << "QUEUE: " << endl;
			for(t=queue.begin();t != queue.end();t++){
				cout << (t)->id << " ";
			}
			cout << "\n" << endl;
		}

		


		//found a match
		if(matchNodeId != "")
			break;
	}

	//no match - return empty handed
	if(matchNodeId == "")
		return list<tuple<int,int>>();



	if(debug2){
		map<string, tuple<int,int>>::iterator m;
		for(m=famTree.begin();m!=famTree.end();m++){
			cout << m->first << " -> " << tup2str(m->second) << endl;
		}
	}

	//trace it back to the source to get the path
	list<tuple<int, int>> back_path;
	back_path.push_front(end);

	while(matchNodeId != startID){
		tuple<int,int> p = famTree[matchNodeId];
		back_path.push_front(p);
		matchNodeId = tup2str(p);
	}
	

	return back_path;
}

//gets the neighboring nodes (in 8 directions) of a given node
//checks for boundary area and if an object is already occupying the space
void getNeighbors(Node* p, tuple<int,int> bounds, list<Node>& neighbors){

	int x = get<0>(p->xy);
	int y = get<1>(p->xy);
	int w = get<0>(bounds);
	int h = get<1>(bounds);

	//cout << x << "," << y << " " << w << "x" << h << endl;
	
	//top
	/*
	if(x-1 >= 0 && y-1 >= 0 && !inSet(xs, {x-1,y-1})){
		Node n({x-1,y-1}, p);
		neighbors.push_back(&n);
	}
	*/
	if(((y-1) >= 0)){
		int nx = x;
		int ny = y-1;

		//cout << "new xy: [" << nx << "," << ny << "]" << endl;

		Node n({nx,ny}, p->id);
		neighbors.push_back(n);
		// Node n({x,y-1}, p);
		// neighbors.push_back(&n);
	}
	/*
	if(x+1 < w  && y-1 >= 0 && !inSet(xs, {x+1,y-1})){
		Node n({x+1,y-1}, p);
		neighbors.push_back(&n);
	}
	*/

	//middle
	if(((x-1) >= 0)){
		int nx = x-1;
		int ny = y;

		//cout << "new xy: [" << nx << "," << ny << "]" << endl;

		Node n({nx,ny}, p->id);
		neighbors.push_back(n);

		// Node n({x-1,y}, p);
		// neighbors.push_back(&n);
	}
	if(((x+1) < w)){
		int nx = x+1;
		int ny = y;

		//cout << "new xy: [" << nx << "," << ny << "]" << endl;

		Node n({nx,ny}, p->id);
		neighbors.push_back(n);

		// Node n({x+1,y-1}, p);
		// neighbors.push_back(&n);
	}

	/*
	//bottom
	if((x-1 >= 0) && y+1 < h && !inSet(xs, {x-1,y+1})){
		Node n({x-1,y+1}, p);
		neighbors.push_back(&n);
	}
	*/
	if(((y+1) < h)){
		int nx = x;
		int ny = y+1;

		//cout << "new xy: [" << nx << "," << ny << "]" << endl;

		//cout << "*** " << x << "," << (y+1) << endl;
		Node n({nx,ny}, p->id);
		neighbors.push_back(n);
	}
	/*
	if((x+1 < w) && y+1 < h  && !inSet(xs, {x+1,y+1})){
		Node n({x+1,y+1}, p);
		neighbors.push_back(&n);
	}
	*/

}

//returns if a node has already been visited or not
bool visited(list<Node> v, Node* n){
	list<Node>::iterator i;
	for(i=v.begin();i != v.end();i++){
		//cout << n->id << " vs. " << i->id << endl;
		if(n->id.compare(i->id) == 0){
			//cout << "MATCH!" << endl;
			return true;
		}
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



//int main(){return 0;}




