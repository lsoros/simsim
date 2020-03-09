// Second draft commit of sim sim
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

class Sim;

class Object {
    friend ostream& operator<<(ostream& os, const Object& theObject){

        return os;
    };
    
public:
    Object(const string& name, vector<int>& effect) : name(name), effect(effect) {}
    
    const string& getName() const {return name;}
    void changeColor(const string& newcolor) {color = newcolor;}
    void changeCost(const double& newcost) {cost = newcost;}
    const int getNeedValue(int i) {return effect[i];}
    
private:
    string name;
    double cost;
    string color;
    vector<int> effect;
    
};

class Sim{
    friend ostream& operator<<(ostream& os, const Sim& theSim){
        os << theSim.name << " has the following needs: " << endl;
        for(size_t i = 0; i < theSim.needs.size(); i++){
            os << theSim.needs[i] << endl;
        }
        return os;
    };
    
public:

    
    Sim(const string& name) : name(name) {}
    
    const string& getName() {return name;}
    
    void interactWith(Object& object){
        cout << name << " interacts with " << object.getName() << endl;
        for (size_t i = 0; i < needs.size(); i++){
            needs[i] += object.getNeedValue(i);
        }
        
    }
    

    
private:
    //needs: hunger, hygeine, bladder, energy, social, fun
    string name;
    vector<int> needs{5,5,5,5,5,5};
    
};

class Room{
    friend ostream& operator<<(ostream& os, const Room& theRoom){
        os << "The " << theRoom.name << " has the following objects inside: " << endl;
        for(size_t i = 0; i < theRoom.room.size(); i++){
            os << theRoom.room[i]->getName() << endl;
        }
        return os;
    };
public:
    Room(const string& name) : name(name) {}
    
    void add_object(Object& object){
        room.push_back(&object);
    }
    
    void placeSim(Sim& sim){
        //places sim in room, makes them interact w their environment
        cout << sim.getName() << "has been placed in the " << name << endl;
        for (size_t i = 0; i < room.size(); i++){
            sim.interactWith(*room[i]);
        }
    }
    
private:
    string name;
    vector<Object*> room;
    
};

// House Class to keep rooms

class House{
    
public:
    
    void add_room(Room& room){
        house.push_back(&room);
    }
    
    void placeSim(Sim& sim){
        cout << sim.getName() << " has been placed in the house" << endl;
        for (size_t i = 0; i < house.size(); i++){
            house[i]->placeSim(sim);
        }
    }
    
private:
    vector<Room*> house;
};

int main() {
    // Sim object, Rachel
    Sim rachel("Rachel");
    cout << rachel << endl;
    
    //needs order: hunger, hygeine, bladder, energy, social, fun
    vector<int> fridge_fx{3,-1,-2,1,0,0};
    vector<int> toilet_fx{0,-1,3,0,0,0};
    vector<int> bed_fx{-1,-1,-1,3,-1,-1};
    
    // objects the sim can interact with and affect their needs
    Object fridge("fridge", fridge_fx);
    Object toilet("toilet", toilet_fx);
    Object bed("bed", bed_fx);
    
    // sims can interact with individual objects, which will affect their needs
    rachel.interactWith(fridge);
    cout << rachel << endl;
    
    // Room object, which can hold different objects
    Room livingroom("Living Room");
    livingroom.add_object(fridge);
    livingroom.add_object(toilet);
    livingroom.add_object(bed);
    cout << livingroom << endl;
    
    // Place a sim in a room, all the objects will affect the sims needs
    livingroom.placeSim(rachel);
    cout << rachel << endl;
    

    return 0;
}
/*
 Next steps:
 > making cost a factor in need effect?
 > House class? Interact with whole house? Or just container for rooms?
 > If a room is in a house can we only interact with a certain room?
 > Chamh
 */
