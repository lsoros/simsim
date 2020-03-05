// First draft commit of sim sim
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
    Object(const string& name) : name(name) {}
    
    void moodEffect(int value){
        happiness += value;
    }
    
    const string& getName() const {return name;}
    void changeColor(const string& newcolor) {color = newcolor;}
    void changeCost(const double& newcost) {cost = newcost;}
    virtual void characteristics(const string& itemcolor, const double& itemcost) = 0;
    virtual int interact(Sim& sim) = 0;
    
private:
    string name;
    double cost;
    string color;
    int happiness;
    
    //bool effect;
    //positive = true
    //negative = false
    
};



class HungerObject : public Object{
public:
    HungerObject(const string& name, int interactval) : Object(name), val(interactval) {}
        
    void characteristics(const string& itemcolor, const double& itemcost) {
        this->changeColor(itemcolor);
        this->changeCost(itemcost);
            //environment = environmentval;
    }
        
    int interact(Sim& sim){
        return val;
    }
        
private:
    int val;

};

class BladderObject : public Object{
public:
    BladderObject(const string& name, int interactval) : Object(name), val(interactval) {}
        
    void characteristics(const string& itemcolor, const double& itemcost) {
        this->changeColor(itemcolor);
        this->changeCost(itemcost);
            //environment = environmentval;
    }
        
    int interact(Sim& sim){
        return val;
    }
        
private:
    int val;

};

class EnergyObject : public Object{
public:
    EnergyObject(const string& name, int interactval) : Object(name), val(interactval) {}
        
    void characteristics(const string& itemcolor, const double& itemcost) {
        this->changeColor(itemcolor);
        this->changeCost(itemcost);
            //environment = environmentval;
    }
        
    int interact(Sim& sim){
        return val;
    }
        
private:
    int val;

};


class Sim{
    friend ostream& operator<<(ostream& os, const Sim& theSim){
        os << theSim.name << " has the following needs: " << endl;
        os << "Hunger: " << theSim.hunger << "\n" << "Hygeine: " << theSim.hygeine << "\n" << "Bladder: " << theSim.bladder << "\n" << "Energy: " << theSim.energy << "\n" << "Social: " << theSim.social << "\n" << "Fun: " << theSim.fun << endl;
        return os;
    };
public:
    
    // void interact
    // if a sim interacts w an object, it affects their needs
    Sim(const string& name) : name(name) {}
    
    void interactWith(HungerObject& object){
        cout << name << " interacts with " << object.getName() << endl;
        int valeffect = object.interact(*this) + hunger;
        
        if (valeffect > 10){
            hunger = 10;
        }
        
        else{
            hunger = valeffect;
        }
        
    }
    
    void interactWith(BladderObject& object){
        cout << name << " interacts with " << object.getName() << endl;
        int valeffect = object.interact(*this) + hunger;
        if (valeffect > 10){
            bladder = 10;
        }
        
        else{
            bladder = valeffect;
        }
    }
    
    void interactWith(EnergyObject& object){
        cout << name << " interacts with " << object.getName() << endl;
        int valeffect = object.interact(*this) + hunger;
        if (valeffect > 10){
            energy = 10;
        }
        
        else{
            energy = valeffect;
        }
    }
    
private:
    //needs: hunger, hygeine, bladder, energy, social, fun
    string name;
    int hunger = 5;
    int hygeine = 5;
    int bladder = 5;
    int energy = 5;
    int social = 5;
    int fun = 5;
    
};

class Room{
    
public:
    Room(const string& name) : name(name) {}
    
    void add_object(Object& object){
        room.push_back(&object);
    }
    
private:
    string name;
    vector<Object*> room;
    
};


int main() {
    HungerObject fridge("fridge",3);
    Sim rachel("Rachel");
    cout << rachel << endl;
    rachel.interactWith(fridge);
    cout << rachel << endl;
    
    Room r1("Living Room");

    return 0;
}
/*
 methods for happiness/positive item
 methods for negative bad item
 
 method for appearance
 method for build, clothes, funiture etc
 method for expansion packs
 method for type ex: food, wallpaper, etc
 
 think as a Sim, not a Player
 needs interact with objects
 make sim object
 needs with objects
 */

/*
class Build : public Object {
    
public:
    Build(const string& name) : Object(name) {}
    void characteristics(const string& itemcolor, const double& itemcost) const{
        
    }
private:
    
};
*/

//generic object that increases a specific need, only one object
//positive and negative effect on objects
//keep room vector option
//vector of needs = [-1,2,3,4,-3,0]
// negative 1 effect on hunger, plus 2 on energy, 3 on social, no effect on last one

