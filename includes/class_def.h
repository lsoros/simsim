//taken from main.cpp to be used in other classes
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <sstream>
#include <string>
#include <cmath>
#include <map>
#include <algorithm>
using namespace std;

extern bool SHOW_DEBUG;
extern bool SHOW_MUT_DEBUG;


// class forward declarations
class House;
class Room;
class Object;
class Sim;

//global functions
map<string, vector<int>> getfullObjList();  //retrieves the object list in the format [name, fx]
map<string, char> makeObjAsciiMap(map<string, vector<int>> fullObjList);        //ascii representation for the object list in the form [name, ascii_rep]
string tup2str(tuple<int,int>t);
string vec2str(vector<int>v);
tuple<int,int> randPos(tuple<int,int>dimen);        //random position within dimensions
int randVal(tuple<int,int>r);            //random number between x and y
tuple<int,int> randDim(tuple<int,int>r);            //random dimensions between x and y

//////////////////     CLASS DEFINITIONS     ///////////////////


class Object {
    friend ostream& operator<<(ostream& os, const Object& theObject){
        os << theObject.getName() << " @ (" << tup2str(theObject.getCoordinates()) << ") = [";

        for(size_t i = 0; i < 6; i++){
            os << theObject.getNeedValue(i);
            if(i <= 4)
                os << ",";
        }
        os << "]";
        return os;

        /*
        //output operator for Object class
        os << theObject.name << " has the following needs: " << endl;
        for(size_t i = 0; i < theObject.effect.size(); i++){
            os << theObject.effect[i] << endl;
        }
        return os;
        */
    };
    
public:
    // Object constructor, initializes with name and vector of need effects, and two pointers that will point to the house and room they are in
    Object(const string& name, vector<int>& effect, tuple<int, int> xy) : name(name), effect(effect), house(nullptr), room(nullptr), hasRoom(false), coordinates(xy) {}
    
    //Object copy function
    Object(const Object& o);

    //getter for obtaining Object name
    const string& getName() const {return name;}
    const House* getHouse() const {return house;}
    const Room* getRoom() const {return room;}
    const tuple<int, int> getCoordinates() const{return coordinates;}
    const vector<int> getEffect() const{return effect;}

    void setRoom(Room *r){room = r;}
    void setHouse(House *h){house = h;}

    string toJSON();

    // functions that change the color, cost and current room the object is in
    void changeColor(const string& newcolor) {color = newcolor;}
    void changeCost(const double& newcost) {cost = newcost;}
    void changeRoom(Room* newroom) {room = newroom;
        hasRoom = true;
    }
    void changeCoordinates(tuple<int, int> newcoords){coordinates = newcoords;}
    // function to obtain the effects the Object has on a Sim
    const int getNeedValue(int i) const {return effect[i];}
    // bool to see if the object is in a room
    bool inRoom() {return hasRoom;}
    
    
private:
    string name;
    double cost;
    string color;
    vector<int> effect;
    House* house;
    Room* room;
    bool hasRoom;
    tuple<int, int> coordinates;
    
};

class Sim{
    friend ostream& operator<<(ostream& os, const Sim& theSim){
        //output operator for Sim class
        os << theSim.name << " has the following needs: " << endl;
        for(size_t i = 0; i < theSim.needs.size(); i++){
            os << theSim.needs[i] << endl;
        }
        return os;
    };
    
public:

    //Sim constructor, initializes with a name, and two pointers that will point to the house and room they are in
    Sim(const string& name) : name(name), house(nullptr), room(nullptr) {}
        
            
    const string& getName() {return name;}                //getter for obtaining Sim name
   	vector<int> getNeeds(){return needs;}           //getter for obtaining Sim needs
   	House* getHouse(){return house;}
    Room* getRoom(){return room;}
    tuple<int,int> getCoordinates(){return coordinates;}
    
    //forward declarations of Sim methods
    void interactWith(Object& object);
    void current_room(Room* newroom);
    void current_house(House* newhouse);

    void closestObject();
    void changeCoordinates(tuple<int, int>& xy);
    void fufillNeeds();
    void printNeeds();

    //simulation functions
    bool isDead(){return needs[0] == 0 || needs[3] == 0;}   //check if the sim is dead (hunger = 0 or energy = 0)
    void alterNeed(int needIndex, int amt);             	//alters the value of a need by some amount
    
    Object* getTarget(){return target;}
    bool hasTarget(){return target != nullptr;}		//check if target object is set
    void setTarget(Object* o){target = o;}				//sets the current target object
    void atTarget(bool debug);									//checks if can interact with a target
    
    bool hasNavPath(){return !navPath.empty() || navPath.size() > 0;}	//check if a navigation path has been set
    void setNavPath(list<tuple<int,int> > path){navPath = path;}		//set the navigation path for the sim
    void goToNext();			//changes the coordinates of the SIM and removes the step from the path queue
    
private:
    //needs: hunger, hygeine, bladder, energy, social, fun
    string name;
    vector<int> needs{5,5,5,5,5,5};
    House* house;
    Room* room;
    Object* target;
    tuple <int, int> coordinates;
    list<tuple<int,int>> navPath;
};

class Room{
    friend ostream& operator<<(ostream& os, const Room& theRoom){
        //output operator for Room class
        os << "The " << theRoom.name << " has the following objects inside: " << endl;
        for(size_t i = 0; i < theRoom.objects.size(); i++){
            os << (*theRoom.objects[i]) << endl;
        }
        return os;
    };
public:
    //Room constructor, initializes with a name, and a pointer to the house it is in
    Room(const string& name) : name(name), house(nullptr), dimensions(tuple<int, int>{10,10}) {}
    Room(const string& name, const tuple<int, int>wh) : name(name), house(nullptr), dimensions(wh) {}
    
    //Room copy function
    Room(const Room& r);

    // forward declarations of Room methods
    void changeHouse(House* newhouse);
    void placeSim(Sim& sim);
    Object* closestToSim(tuple<int, int>& coordinates, Sim& sim);
    void interactwithObjects(Sim& sim);

    string toJSON();
        
    //getters
    const string& getName() const{return name;}
    const vector<Object*> getObjects() const{return objects;}
    const tuple<int, int> getDimensions() const{return dimensions;}
    vector<Sim*> getSims(){return sims;}



    // method that returns house pointer
    House* getHouse() {return house;}
    // method that adds objects to Room
    void add_object(Object& object){
        if (!objectInside(&object)){
            //if the object isnt already in the room, add it to the Rooms vector of Object*
         
        	//check if outside the boundary of the room
        	tuple<int, int> c = object.getCoordinates();
        	if(get<0>(c) >= get<0>(dimensions) || get<0>(c) < 0){		//x out of bounds
        		if(SHOW_DEBUG)
                    cout << "** Warning ** : " << object.getName() << " X coordinate is out of bounds! Moving to edge of boundary" << endl;
        		object.changeCoordinates(tuple<int,int>{(get<0>(c) < 0 ? 0 : get<0>(dimensions)-1),get<1>(c)});
        	}
        	if(get<1>(c) >= get<1>(dimensions) || get<1>(c) < 0){		//x out of bounds
        		if(SHOW_DEBUG)
                    cout << "** Warning ** : " << object.getName() << " Y coordinate is out of bounds! Moving to edge of boundary" << endl;
        		object.changeCoordinates(tuple<int,int>{get<0>(c),((get<1>(c) < 0) ? 0 : get<1>(dimensions)-1)});
        	}

        	//add the object
            objects.push_back(&object);

            //set the object's room to this room and house to this house
            object.setRoom(this);
            object.setHouse(this->getHouse());
        }
        else{
            if(SHOW_DEBUG)
                cout << object.getName() << " is already in " << name << endl;
        }
    }
    // helper function that checks if an object is currently inside the room
    bool objectInside(Object* object){
        for (size_t i = 0; i < objects.size(); i++){
            if (objects[i] == object){
                return true;
            }
        }
        return false;
    }
        
    //when a Sim changes rooms, it notifies the previous room it was in to remove itself from their vector of Sim*
    void removeSim(Sim* sim){
        int simindex = 0;
        for (size_t i = 0; i < sims.size(); i++){
            if (sims[i] == sim){
                // if the Sim is indeed inside the Room, keep track of the index
                simindex = i;
                break;
                
            }
        }
        
        while (simindex != sims.size()){
            // loop that overwrites Sim to be removed, and pops back last position in vector. moves everything down and keeps order
            sims[simindex] = sims[simindex +1];
            simindex += 1;

        }
        sims.pop_back();
    }

    bool check_position(tuple<int,int> coordinates)
    {
        // Check if coordinates lie within the bounding box. If they don't return false. Else continue.

        tuple<int, int> dimensions = getDimensions();
        
        if(get<0>(coordinates) < 0 || get<0>(coordinates) >= get<0>(dimensions) || get<1>(coordinates) <0 || get<1>(coordinates) >= get<1>(dimensions)){
            return false;
        }
        
        for(auto obj:objects) if(obj->getCoordinates() == coordinates) return false;
        
        return true;
    }

    //inline bool place_object(Object& object, tuple<int, int> coordinates)
    inline bool place_object(string objName, tuple<int, int> coordinates)
    {
        map<string, vector<int>> object_list = getfullObjList();
        //graph search to find next position?
        if(check_position(coordinates)) {
             Object *no = new Object(objName, object_list[objName], coordinates);
            add_object(*no);
            //objects.push_back(&object);
            return true;
        }
        else {
            // check neighbours
            int x[] = {0,1,0,-1};
            int y[] = {1,0,-1,0};
            for(int i=0;i<4;i++) {
                if(check_position({get<0>(coordinates) + x[i], get<1>(coordinates) + y[i]})){
                    tuple<int, int> pnew = {get<0>(coordinates) + x[i], get<1>(coordinates) + y[i]};
                    Object *no = new Object(objName, object_list[objName], pnew);
                    add_object(*no);
                    //object.changeCoordinates(pnew);
                    //objects.push_back(&object);
                    return true;
                }
            }
        }
        return false;
    }
    inline bool move_object(Object* object, tuple<int, int> coordinates)
    {
        //graph search to find next position?
        if(check_position(coordinates)) {
            object->changeCoordinates(coordinates);
            return true;
        }
        else {
            // check neighbours
            int x[] = {0,1,0,-1};
            int y[] = {1,0,-1,0};
            for(int i=0;i<4;i++) {
                if(check_position({get<0>(coordinates) + x[i], get<1>(coordinates) + y[i]})){
                    tuple<int, int> pnew = {get<0>(coordinates) + x[i], get<1>(coordinates) + y[i]};
                    object->changeCoordinates(pnew);
                    return true;
                }
            }
        }
        return false;
    }     
    
    inline Object* delete_object(int position){
        Object *obj = objects[position];
        //objects.erase(objects.begin()+position);
        objects.erase(remove(objects.begin(), objects.end(), obj),objects.end());
        return obj;
    }

    inline void mutate_objects(){
        float _mut_add_prob = 30;
        //float _mut_add_prob = 100;
	    float _mut_delete_prob = 10;
	    float _mut_move_prob = 40;
        int randomNum = (rand() %101);

        vector<Object*> object_list = getObjects();
        for (int i = 0; i < object_list.size(); i++){
            if(randomNum <= (_mut_delete_prob)){
                //deleting an object

                Object* obj = delete_object(i);
                if(obj != nullptr && SHOW_MUT_DEBUG){
                    cout << "** DELETED "<< obj->getName() << " **" << endl;
                }
                //object obj was deleted

            }
            else if(randomNum <= (_mut_delete_prob + _mut_move_prob)){
                //moving an object
                tuple<int, int> new_coordinates = randPos(getDimensions());
                Object* current_obj = objects[i];
                bool did_it_happen = move_object(current_obj, new_coordinates);

                if(did_it_happen && SHOW_MUT_DEBUG){
                    cout << "** MOVED " << objects[i]->getName() << " **" << endl;
                }

            }
            else{
                //do no mutation
            }
         randomNum = (rand() %101);
        }

        randomNum = (rand() %101);
        if(randomNum <= (_mut_add_prob)){
            //add an object
            map<string, vector<int>> object_list = getfullObjList();
            auto it = object_list.begin();
            std::advance(it, rand() % object_list.size());
            string new_object_p = it->first;
            //generate coordinates and then place object
            tuple<int, int> new_coordinates = randPos(getDimensions());
            //Object new_object(new_object_p, object_list[new_object_p], new_coordinates);
            bool did_it_happen = place_object(new_object_p, new_coordinates); 
            if(did_it_happen && SHOW_MUT_DEBUG){
                //cout << "** ADDED OBJECT " << new_object.getName() << " **" << endl; 
                cout << "** ADDED " << new_object_p << "**" << endl; 
            }  
            
        }

    }
    
private:
    string name;
    vector<Object*> objects;
    vector<Sim*> sims;
    House* house;
    tuple<int, int> dimensions;
};

class House{
    friend ostream& operator<<(ostream& os, const House& theHouse){
        // output operator for House class
        os << "Current status of " << theHouse.name << endl;
        os << "There's " << theHouse.rooms.size() << " room(s) in this house: " << endl;
        for (size_t i = 0; i < theHouse.rooms.size(); i++){
        os << theHouse.rooms[i]->getName() << endl;
        }
        
        os << "Current sims inside the house: " << endl;
        for (size_t i = 0; i < theHouse.sims.size(); i++){
            os << theHouse.sims[i]->getName() << endl;
        }
        
        return os;
    };
public:
    // House constructor, initializes with a name
    House(const string& name) : name(name), fitness(0.0), houseId(-1) {}
    House(const string& name, const int id) : name(name), fitness(0.0), houseId(id) {}

    //House copy function
    House(const House& h);
    
    // getters
    const string& getName() const{return name;}
    const vector<Room*> getRooms()const{return rooms;};
    const float getFitness()const{return fitness;}
    const int getId()const{return houseId;}
    
    void setFitness(float f){fitness = f;}
    void setName(string n){name = n;}
    void setId(int i){houseId = i;}

    string toJSON();
    
    // method that adds a room to House
    void add_room(Room& room){
       
        if (roomInside(&room)){
            //if room already exists in the house, user is notified
            if(SHOW_DEBUG)
                cout << room.getName() << " is already in the house!" << endl;
        }
        else{
            // else, add the room to the House
            rooms.push_back(&room);
            room.changeHouse(this);
            
        }
    }
    
    // helper function that places Sim inside House
    void placeSim(Sim& sim){
        //flag that tracks if a Sim is present
        bool simPresent = false;
        
        for (size_t j = 0; j < sims.size(); j++){
            if (sims[j] == &sim){
                if(SHOW_DEBUG)
                    cout << sim.getName() << " is already inside the house!" << endl;
                simPresent = true;
                // if sim is already inside house, flag is set to true
                break;
            }
        }
        
        if (!simPresent){
            //if sim is not already in the house, add it to the vector of Sim*, and change Sim's house
            sims.push_back(&sim);
            sim.current_house(this);
        
        }
    }
    
    //helper function that checks if a Room is already inside the current House
    bool roomInside(Room* room){

        for(size_t i = 0; i < rooms.size(); i++){
            if (rooms[i] == room){
                return true;
            }
        }
        return false;
    }
        
    //when a Sim changes rooms, it notifies the previous room it was in to remove itself from their vector of Sim*
    void removeSim(Sim* sim){
        int simindex = 0;
        for (size_t i = 0; i < sims.size(); i++){
            if (sims[i] == sim){
                // if the Sim is indeed inside the Room, keep track of the index
                simindex = i;
                break;
            }
        }
        
        while (simindex != sims.size()){
            // loop that overwrites Sim to be removed, and pops back last position in vector. moves everything down and keeps order
            sims[simindex] = sims[simindex +1];
            simindex += 1;
        }
        sims.pop_back();

    }

    map<string, int> getObjectCt(){
        int i;
        map<string, int> objectMap;

        for(i=0;i<rooms.size();i++){
            vector<Object *>objs = rooms[i]->getObjects();
            int v;
            for(v=0;v<objs.size();v++){
                objectMap[objs[v]->getName()]++;
            }
        }
        return objectMap;
    }

    //copies the contents of the house into another house object (DEEP COPY)
    /*
    void copyHouse(House* newHouse){

        //copy the rooms first
        int r;
        vector<Room *>rooms = getRooms();
        for(r=0;r<rooms.size();r++){
            Room newRoom(rooms[r]->getName());

            //copy the internal objects
            vector<Object *>objects = rooms[r]->getObjects();
            int o;
            for(o=0;o<objects.size();o++){
                Object *oldObj = objects[o];
                vector<int> newFX{oldObj->getNeedValue(0), oldObj->getNeedValue(1),oldObj->getNeedValue(2),oldObj->getNeedValue(3),oldObj->getNeedValue(4)};     //rebuild the fx vector
                tuple<int,int> oldXY = oldObj->getCoordinates();        
                tuple<int, int>newXY{get<0>(oldXY), get<1>(oldXY)};

                Object newObject(oldObj->getName(), newFX, newXY);
                newRoom.add_object(newObject);
            }

            //ignore placing the sim for now (?) 

            //add the room to the house
            newHouse->add_room(newRoom);
        }
    }
    */


    //prints the house in ascii format
    string asciiRep(map<string, char> nameRep){
       
        //do for one room for now
        tuple<int,int> dim = this->getRooms()[0]->getDimensions();
        int w = get<0>(dim);
        int h = get<1>(dim);

        //make an empty house first
        string houseRep = "";
        int r;
        int c;

        char wall = '#';
        char empty = '.';

        for(r=0;r<w;r++){
            for(c=0;c<h;c++){

                //all empty space first
                houseRep += empty;
                
            }
        }

        //map the objects
        vector<Object *> objs = this->getRooms()[0]->getObjects(); //only one room for now so just use that 
        int o;
        for(o=0;o<objs.size();o++){
            tuple<int,int>pos = objs[o]->getCoordinates();
            string name = objs[o]->getName();
            int index = get<0>(pos) + w*get<1>(pos);
            houseRep[index] = nameRep[name];
        }

            
        //add the sims
        char sim = '@';
        vector<Sim *> sims = this->getRooms()[0]->getSims();
        int s;
        for(s=0;s<sims.size();s++){
            Sim *curSim = sims[s];
            tuple<int,int>pos = curSim->getCoordinates();
            int index = get<0>(pos) + w*get<1>(pos);
            houseRep[index] = sim;
        }


        //add finishing format touches
        string houseRepFinal = "";
        int a;
        //top wall
        for(a=0;a<(w+2);++a){
            houseRepFinal += wall;
        }
        houseRepFinal += "\n";

        for(a=0;a<houseRep.length();++a){
            //add side wall
            if(a % (w) == 0){
                houseRepFinal += wall;
            }

            houseRepFinal += houseRep[a];

            //add side wall and new line
            if((a+1) % w == 0){
                houseRepFinal += wall;
                houseRepFinal += "\n";
            }
        }

        //bottom wall
        for(a=0;a<(w+2);++a){
            houseRepFinal += wall;
        }
        houseRepFinal += "\n";


        //add a key for good measure
        houseRepFinal += "\nKey\n";
        if(sims.size() > 0)
            houseRepFinal += "@ : SIM\n";

        map<string, int> objsCt = getObjectCt();
        map<string, int>::iterator oi;

        for(oi=objsCt.begin();oi!=objsCt.end();++oi){
            string name = oi->first;

            //[ascii_char] : [object_name]
            houseRepFinal += nameRep[name];
            houseRepFinal += " : ";
            houseRepFinal += name;
            houseRepFinal += "\n";
        }

        return houseRepFinal;
    }

    //prints the house in ascii format (literal with "\n")
    string asciiRepLit(map<string, char> nameRep){
       
        //do for one room for now
        tuple<int,int> dim = this->getRooms()[0]->getDimensions();
        int w = get<0>(dim);
        int h = get<1>(dim);

        //make an empty house first
        string houseRep = "";
        int r;
        int c;

        char wall = '#';
        char empty = '.';

        for(r=0;r<w;r++){
            for(c=0;c<h;c++){

                //all empty space first
                houseRep += empty;
                
            }
        }

        //map the objects
        vector<Object *> objs = this->getRooms()[0]->getObjects(); //only one room for now so just use that 
        int o;
        for(o=0;o<objs.size();o++){
            tuple<int,int>pos = objs[o]->getCoordinates();
            string name = objs[o]->getName();
            int index = get<0>(pos) + w*get<1>(pos);
            houseRep[index] = nameRep[name];
        }

            
        //add the sims
        char sim = '@';
        vector<Sim *> sims = this->getRooms()[0]->getSims();
        int s;
        for(s=0;s<sims.size();s++){
            Sim *curSim = sims[s];
            tuple<int,int>pos = curSim->getCoordinates();
            int index = get<0>(pos) + w*get<1>(pos);
            houseRep[index] = sim;
        }


        //add finishing format touches
        string houseRepFinal = "";
        int a;
        //top wall
        for(a=0;a<(w+2);++a){
            houseRepFinal += wall;
        }
        houseRepFinal += "\\n";

        for(a=0;a<houseRep.length();++a){
            //add side wall
            if(a % (w) == 0){
                houseRepFinal += wall;
            }

            houseRepFinal += houseRep[a];

            //add side wall and new line
            if((a+1) % w == 0){
                houseRepFinal += wall;
                houseRepFinal += "\\n";
            }
        }

        //bottom wall
        for(a=0;a<(w+2);++a){
            houseRepFinal += wall;
        }
        houseRepFinal += "\\n";


        //add a key for good measure
        houseRepFinal += "\\nKey\\n";
        if(sims.size() > 0)
            houseRepFinal += "@ : SIM\\n";

        map<string, int> objsCt = getObjectCt();
        map<string, int>::iterator oi;

        for(oi=objsCt.begin();oi!=objsCt.end();++oi){
            string name = oi->first;

            //[ascii_char] : [object_name]
            houseRepFinal += nameRep[name];
            houseRepFinal += " : ";
            houseRepFinal += name;
            houseRepFinal += "\\n";
        }

        return houseRepFinal;
    }
        
private:
    vector<Room*> rooms;
    vector<Sim*> sims;
    string name;
    float fitness;
    int houseId;
};






