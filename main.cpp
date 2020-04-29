#include "includes/class_def.h"

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


    Object taco2("taco", taco_fx, taco_xy);
    kitchen.add_object(taco2);
    for (auto const& pair: rachelhouse.getObjectCt()) {
        std::cout << pair.first << ": " << pair.second << "\n";
    }
        
    return 0;
}


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
