//class function definitions moved from main.cpp
#include "includes/class_def.h"

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
        }
        else{
            //if the object is not in the same room, attempt fails and we notify the user
            cout << "This object is not in the same room as " << name << endl;
        }
    }
}

//changes a certain need by some amount
void Sim::alterNeed(int needIndex, int amt){
    needs[needIndex] += amt;
    return;
}
  
//checks if the sim is at the target location and uses it if so
void Sim::atTarget(){
     //if no target set - don't even bother
    if(!hasTarget())       
        return;

    //if not at the target location - cancel
    if(coordinates[0] != target.coordinates[0] || coordinates[1] != target.coordinates[1] )
        return;

    //if at the target - use it and remove the target
    interactWith(target);
    target = null;
    return;
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
    // in any other case, the Sims house and room are changed and Sim is added to the Room
    sim.current_house(house);
    sim.current_room(this);
    sims.push_back(&sim);
            
}

//helper function for changing a Room's house
void Room::changeHouse(House* newhouse){
    house = newhouse;
    for (size_t i = 0; i < sims.size(); i++){
        //for each Sim in the current Room, their House is changed
        house->placeSim(*sims[i]);
    }
        //tell house vector to remove this room if not equal to prev house?
}