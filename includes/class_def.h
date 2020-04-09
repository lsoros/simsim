//taken from main.cpp to be used in other classes
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

// class forward declarations
class House;
class Room;
class Object;
class Sim;


class Object {
    friend ostream& operator<<(ostream& os, const Object& theObject){
        //output operator for Object class
        os << theObject.name << " has the following needs: " << endl;
        for(size_t i = 0; i < theObject.effect.size(); i++){
            os << theObject.effect[i] << endl;
        }
        return os;
    };
    
public:
    // Object constructor, initializes with name and vector of need effects, and two pointers that will point to the house and room they are in
    Object(const string& name, vector<int>& effect, tuple<float, float> xy) : name(name), effect(effect), house(nullptr), room(nullptr), hasRoom(false), coordinates(xy) {}
    
    //getter for obtaining Object name
    const string& getName() const {return name;}
    const House* getHouse() const {return house;}
    const Room* getRoom() const {return room;}
    const tuple<float, float> getCoordinates() const{return coordinates;}

    // functions that change the color, cost and current room the object is in
    void changeColor(const string& newcolor) {color = newcolor;}
    void changeCost(const double& newcost) {cost = newcost;}
    void changeRoom(Room* newroom) {room = newroom;
        hasRoom = true;
    }
    void changeCoordinates(tuple<float, float> newcoords){coordinates = newcoords;}
    // function to obtain the effects the Object has on a Sim
    const int getNeedValue(int i) {return effect[i];}
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
    tuple<float, float> coordinates;
    
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
    tuple <float, float> getCoordinates(){return coordinates;}
    
    //forward declarations of Sim methods
    void interactWith(Object& object);
    void current_room(Room* newroom);
    void current_house(House* newhouse);

    void closestObject();
    void changeCoordinates(tuple<float, float>& xy);
    void fufillNeeds();

    //simulation functions
    bool isDead(){return needs[0] == 0 || needs[3] == 0;}   //check if the sim is dead (hunger = 0 or energy = 0)
    void alterNeed(int needIndex, int amt);             	//alters the value of a need by some amount
    
    bool hasTarget(){return target != nullptr;}		//check if target object is set
    void setTarget(Object* o){target = o;}				//sets the current target object
    void atTarget();									//checks if can interact with a target
    
    
private:
    //needs: hunger, hygeine, bladder, energy, social, fun
    string name;
    vector<int> needs{5,5,5,5,5,5};
    House* house;
    Room* room;
    Object* target;
    tuple <float, float> coordinates;
    
};

class Room{
    friend ostream& operator<<(ostream& os, const Room& theRoom){
        //output operator for Room class
        os << "The " << theRoom.name << " has the following objects inside: " << endl;
        for(size_t i = 0; i < theRoom.objects.size(); i++){
            os << theRoom.objects[i]->getName() << endl;
        }
        return os;
    };
public:
    //Room constructor, initializes with a name, and a pointer to the house it is in
    Room(const string& name) : name(name), house(nullptr), dimensions(tuple<int, int>{10,10}) {}
    Room(const string& name, const tuple<int, int>wh) : name(name), house(nullptr), dimensions(wh) {}
    
    // forward declarations of Room methods
    void changeHouse(House* newhouse);
    void placeSim(Sim& sim);
    Object* closestToSim(tuple<float,float>& coordinates, Sim& sim);
    void interactwithObjects(Sim& sim);
        
    //getters
    const string& getName() {return name;}
    vector<Object*> getObjects(){return objects;}


    // method that returns house pointer
    House* getHouse() {return house;}
    // method that adds objects to Room
    void add_object(Object& object){
        if (!objectInside(&object)){
            //if the object isnt already in the room, add it to the Rooms vector of Object*
         
        	//check if outside the boundary of the room
        	tuple<float, float> c = object.getCoordinates();
        	if(get<0>(c) >= get<0>(dimensions) || get<0>(c) < 0){		//x out of bounds
        		cout << "** Warning ** : " << object.getName() << " X coordinate is out of bounds! Moving to edge of boundary" << endl;
        		object.changeCoordinates(tuple<float,float>{(get<0>(c) < 0 ? 0 : get<0>(dimensions)-1),get<1>(c)});
        	}
        	if(get<1>(c) >= get<1>(dimensions) || get<1>(c) < 0){		//x out of bounds
        		cout << "** Warning ** : " << object.getName() << " Y coordinate is out of bounds! Moving to edge of boundary" << endl;
        		object.changeCoordinates(tuple<float,float>{get<0>(c),((get<1>(c) < 0) ? 0 : get<1>(dimensions)-1)});
        	}

        	//add the object
            objects.push_back(&object);
        }
        else{
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
    House(const string& name) : name(name) {}
    
    // getters
    const string& getName() {return name;}
    const vector<Room*> getRooms(){return rooms;};
    
    // method that adds a room to House
    void add_room(Room& room){
       
        if (roomInside(&room)){
            //if room already exists in the house, user is notified
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
        
private:
    vector<Room*> rooms;
    vector<Sim*> sims;
    string name;
};
