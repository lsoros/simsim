// Fourth commit to program
// some existing functions were changed (placeSim), and some were added to Room and Sim. Sims and objects now that coordinates, and the sim can see what object they are closest to, and interact with all the objects to fufill their needs
// might change so that a sim can identify their lowest need and only replenish that
<<<<<<< HEAD

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

// class forward declarations
class Room;
class House;
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
    Object(const string& name, vector<int>& effect, tuple<int, int> xy) : name(name), effect(effect), house(nullptr), room(nullptr), hasRoom(false), coordinates(xy) {}
    
    //getter for obtaining Object name
    const string& getName() const {return name;}
    // functions that change the color, cost and current room the object is in
    void changeColor(const string& newcolor) {color = newcolor;}
    void changeCost(const double& newcost) {cost = newcost;}
    void changeRoom(Room* newroom) {room = newroom;
        hasRoom = true;
    }
    // function to obtain the effects the Object has on a Sim
    const int getNeedValue(int i) {return effect[i];}
    // bool to see if the object is in a room
    tuple<float, float> getCoordinates() {return coordinates;}
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
        
    //getter for obtaining Sim name
    const string& getName() {return name;}
    //forward declarations of Sim methods
    void interactWith(Object& object);
    void current_room(Room* newroom);
    void current_house(House* newhouse);
    void closestObject();
    void changeCoordinates(tuple<int, int>& xy);
    void fufillNeeds();
    
private:
    //needs: hunger, hygeine, bladder, energy, social, fun
    string name;
    vector<int> needs{5,5,5,5,5,5};
    House* house;
    Room* room;
    tuple <int, int> coordinates;
    
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
    Room(const string& name) : name(name), house(nullptr) {}
    
    // forward declarations of Room methods
    void changeHouse(House* newhouse);
    void placeSim(Sim& sim);
    Object* closestToSim(tuple<int,int>& coordinates, Sim& sim);
    void interactwithObjects(Sim& sim);
    //getter for obtaining Room name
    const string& getName() {return name;}
    // method that returns house pointer
    House* getHouse() {return house;}
    // method that adds objects to Room
    void add_object(Object& object){
        if (!objectInside(&object)){
            //if the object isnt already in the room, add it to the Rooms vector of Object*
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
    
    // getter for obtaining House name
    const string& getName() {return name;}
    
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
        /*
        if (simindex != sims.size()){
        //  if sim exists in room and needs to be removed, its coordinates are reset to be 0,0 for when they are placed. might not be necessary, since placeSim does this
            tuple<float,float> newxy(0,0);
            sim->changeCoordinates(newxy);
        }
         */
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
=======
#include "includes/class_def.h"
>>>>>>> 0bcf771e69784d24937498ca25dec12efb783f31

int main() {
    // Sim object, Rachel
    Sim rachel("Rachel");
    cout << rachel << endl;
    
    //needs order: hunger, hygeine, bladder, energy, social, fun
    vector<int> fridge_fx{3,-1,-2,1,0,0};
    vector<int> toilet_fx{0,-1,3,0,0,0};
    vector<int> bed_fx{-1,-1,-1,3,-1,-1};
        
    tuple<int, int> fridge_xy(5,5);
    tuple<int, int> toilet_xy(3,2);
    tuple<int, int> bed_xy(4,1);
    
    // objects the sim can interact with and affect their needs
    Object fridge("fridge", fridge_fx, fridge_xy);
    Object toilet("toilet", toilet_fx, toilet_xy);
    Object bed("bed", bed_fx, bed_xy);
    
    // sims can interact with individual objects, which will affect their needs
    rachel.interactWith(fridge);
    cout << rachel << endl;
    
    // Room object, which can hold different objects
    Room livingroom("Living Room");
    livingroom.add_object(fridge);
    livingroom.add_object(toilet);
    livingroom.add_object(bed);
    cout << livingroom << endl;
    
    // Place a sim in a room, can now only interact with the objects in that room
    livingroom.placeSim(rachel);
    
    // House object, can add rooms and keep track of sims in house
    House rachelhouse("Rachel's House");
    rachelhouse.add_room(livingroom);
    cout << rachelhouse << endl;

    // if you try to double add a room to a house or double add a sim to a room, the action will fail
    rachelhouse.add_room(livingroom);
    livingroom.placeSim(rachel);
    cout << endl;
        
    // when you place a sim in a different room in the house, it takes that sim out of the room it is currently in
    Room kitchen("Kitchen");
    rachelhouse.add_room(kitchen);
    kitchen.placeSim(rachel);
        
    // adding a new object to the room
    vector<int> taco_fx{3,-1,-1,0,0,0};
    tuple<int, int> taco_xy(1,1);
    Object taco("taco", taco_fx, taco_xy);
    kitchen.add_object(taco);
    rachel.interactWith(taco);
        
    // trying to interact with an object that is not in the same room will fail
    rachel.interactWith(bed);
        
    cout << rachel << endl;
    cout << rachelhouse << endl;
    
    rachel.closestObject();
    cout << rachel << endl;
    rachel.fufillNeeds();
    cout << rachel << endl;
        
    return 0;
}
<<<<<<< HEAD
        
//helper function that places a Sim in a Room and can change their Room
void Sim::current_room(Room* newroom){
        
    if (house == nullptr){
        // if the Sim currently doesnt have a house, we enter this conditional statement
        cout << name << " has been placed in the " << newroom->getName() << endl;
        if (room != nullptr){
            // if they were in a previous before, remove the Sim from the previous room
            room->removeSim(this);
        }
        // Sim's current Room is assigned to newroom
        room = newroom;
        //house = newroom->getHouse();
    }
    else{
        // if the Sim was previously in a different house, we enter this conditional statement
        if (house->roomInside(newroom)){
            // if the Room exists inside the House the Sim is currently inside, they can change rooms
            cout << name << " has been placed in the " << newroom->getName() << endl;
            if (room != nullptr){
                // if they were in a previous before, remove the Sim from the previous room
                room->removeSim(this);
            }
            // Sim's current Room is assigned to newroom
            room = newroom;
        }
        else{
        // if the Room does not exist inside the House the Sim is currently in, the attempt fails. You can only switch to rooms that are in the same House
            cout << "The " << newroom->getName() << " is not in the same house!" << endl;
        }
    }
    
}
        
//helper function that places a Sim in a House and can change their House
void Sim::current_house(House* newhouse){

    if ((house != newhouse) && (house != nullptr)){
        //if the newhouse is not the same as the current House, and if the current house exists, we enter this conditional
        //before changing the house, we have to remove the Sim from the previous House and Room it was in
        house->removeSim(this);
        room->removeSim(this);
        // Sim's current House is assigned to newhouse
        house = newhouse;
        if (house != nullptr){
            //notifies the user what House the Sim is in
            cout << name << " is currently in " << house->getName() << endl;
        }
    }
    else if((house != newhouse) && (house == nullptr)){
        // if the newhouse is not the same as the current House, and if the current house does not exists, we enter this conditional
        // Sim's current House is assigned to newhouse
        house = newhouse;
        //notifies the user what House the Sim is in
        cout << name << " is currently in " << house->getName() << endl;
    }
    // for any other case, reassign house to newhouse
    house = newhouse;
}
        
// method that makes a Sim interact with an Object
void Sim::interactWith(Object& object){
    //check if they have same room and house
    if (room == nullptr){
        //if Sim is not currently inside a Room, we enter this conditional statement
        if(!object.inRoom()){
            //if the object does not have a Room either, the two can interact
            cout << name << " interacts with " << object.getName() << endl;
            for (size_t i = 0; i < needs.size(); i++){
                int newfx = needs[i] + object.getNeedValue(i);
                if (newfx > 10){
                    // if the needs exceed 10, reassign the needs to be 10
                    needs[i] = 10;
                }
                else{
                    needs[i] = newfx;
                }
            }
            coordinates = object.getCoordinates();
            // changes coordinates to move to object
        }
        else{
            // if the object is in a different Room, attempt fails and we notify the user
            cout << "This object is in a different room." << endl;
        }
    }
        
    else{
        // if the Sim is already in a Room, we enter this conditional statement
        if (room->objectInside(&object)){
            //if the object is inside the same room as the Sim, they can interact
            cout << name << " interacts with " << object.getName() << " in the " << room->getName() << endl;
            for (size_t i = 0; i < needs.size(); i++){
                int newfx = needs[i] + object.getNeedValue(i);
                if (newfx > 10){
                    // if the needs exceed 10, reassign the needs to be 10
                    needs[i] = 10;
                }
                else{
                    needs[i] = newfx;
                }
            }
        coordinates = object.getCoordinates();
        // changes coordinates to move to object
        }
        
        
        else {
            //if the object is not in the same room, attempt fails and we notify the user
            cout << "This object is not in the same room as " << name << endl;
        }
    }
}
void Sim::changeCoordinates(tuple<int, int>& xy){
    // method that changes a sims coordinates in a room
    coordinates = xy;
    }
        
void Sim::closestObject(){
    // method that finds the closest object to the sim
    if (room != nullptr){
        // call function that returns coordinates of each object in room;
        Object* closestobject = room->closestToSim(coordinates, *this);
        cout << "The closest object to " << name << " is " << closestobject->getName() << endl;
        return;
    }
    cout << "This sim is not inside a room yet." << endl;
}

void Sim::fufillNeeds(){
    // function that fills sims needs with objects in the room
    if(room != nullptr){
        // if the sim is inside a room, it will interact with all the objects in the room
        cout << name << " will interact with all the objects in the room to fufill their needs." << endl;
        room->interactwithObjects(*this);
        }
    
}
  
void Room::interactwithObjects(Sim& sim){
    // sim inside the room interacts with all the objects
    for(size_t i = 0; i < objects.size(); i++){
        sim.interactWith(*objects[i]);
        }
    }

Object* Room::closestToSim(tuple<int,int>& coordinates, Sim& sim){
    float difference = 100.0;
    float x_2 = get<0>(coordinates);
    float y_2 = get<1>(coordinates);
    Object* closest = nullptr;
    // x and y coordinates for the sim, and object pointer for the closest object
    for (size_t i = 0; i < objects.size(); i++){
        float x_1 = get<0>(objects[i]->getCoordinates());
        float y_1 = get<1>(objects[i]->getCoordinates());
        // x and y coordinates for the object
        float distance = sqrt( pow(x_2 - x_1, 2) + pow(y_2 - y_1, 2) );
        // distance formula for the two x,y coordinates
        if (distance < difference){
            // if we find an object whos distance is closer to the sim, we reassign it, and set the closest to that object
            difference = distance;
            closest = objects[i];
        }
    }
    // return closest Object*
    return closest;
}
// method for placing a Sim in a Room
void Room::placeSim(Sim& sim){
    //places sim in room, makes them interact w their environment
    for(size_t i = 0; i < sims.size(); i++){
        if (sims[i] == &sim){
            // if Sim is already inside the Room, attempt fails and the user is notified
            cout << sim.getName() << " is already inside the " << name << "!" << endl;
            return;
        }
    }
    // in any other case, the Sims house and room are changed and Sim is added to the Room, at coordinate 0,0
    sim.current_house(house);
    sim.current_room(this);
    tuple<int,int> newxy(0,0);
    sim.changeCoordinates(newxy);
    sims.push_back(&sim);
            
}
=======
>>>>>>> 0bcf771e69784d24937498ca25dec12efb783f31


/*
 Next steps:
 > making cost a factor in need effect?
 > House class? Interact with whole house? Or just container for rooms?
 > If a room is in a house can we only interact with a certain room?
 > go on house object
 > sim has to individually interact w/ objects, not entire room at once
 > list out all objects in rooms, method interact with object in room
 > sim has to know what room it is in
 > sim can only interact with objects in the room, and it can change rooms if wanted
 > tree to organize needs?
 > add x,y coordinate
 > heap for needs
 > position for objects
 > if sim bladder is low, find the nearest toilet in room
 > death is hunger at 0, they die
 > sims need to priortize where to go
 > initialize vector inside of making object
 > needs * multiplier (?)
 > function in Sim that asks what object is it closest to, calls function in Object that returns coordinates in room
 */
