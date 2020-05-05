//class function definitions moved from main.cpp
#include "includes/class_def.h"

bool  SHOW_DEBUG = false;

string tup2str(tuple<int,int>t){
    return to_string(get<0>(t)) + "," + to_string(get<1>(t));
}
string vec2str(vector<int>v){
    int i;
    string str = "";
    for(i=0;i<v.size();i++){
        str += v[i];
        if(i+1 != v.size())
            str += ",";
    }
    return str;
}

//copy constructors
Object::Object(const Object& o){
    name = o.getName();
    int f;
    for(f=0;f<6;f++){
        effect.push_back(o.getNeedValue(f));
    }
    coordinates = {get<0>(o.getCoordinates()),get<1>(o.getCoordinates())};

    //house and room will most likely be different - so set this manually later (within Room copy function)
    house = nullptr;
    room = nullptr;
    hasRoom = false;
}

Room::Room(const Room& r){
    name = r.getName();
    dimensions = r.getDimensions();

    //house will most likely be reset later
    house = nullptr;

    //duplicate any objects in the room
    vector<Object*> rObjs = r.getObjects();
    int o;
    for(o=0;o<rObjs.size();o++){
        Object *no = new Object(*(rObjs[o]));
        add_object(*no);
    }

}

House::House(const House& h){
    name = h.getName();
    fitness = getFitness();     //assuming everything's the same from the original room

    //add the rooms
    vector<Room *> hRooms = h.getRooms();
    int r;
    for(r=0;r<hRooms.size();r++){
        Room *nr = new Room(*(hRooms[0]));
        add_room(*nr);
    }

    //assume no sim in this house yet so don't copy it from the original house
}

//helper function that places a Sim in a Room and can change their Room
void Sim::current_room(Room* newroom){
        
    if (house == nullptr){
        // if the Sim currently doesnt have a house, we enter this conditional statement
        if(SHOW_DEBUG)
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
            if(SHOW_DEBUG)
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
            if(SHOW_DEBUG)
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
            if(SHOW_DEBUG)
                cout << name << " is currently in " << house->getName() << endl;
        }
    }
    else if((house != newhouse) && (house == nullptr)){
        // if the newhouse is not the same as the current House, and if the current house does not exists, we enter this conditional
        // Sim's current House is assigned to newhouse
        house = newhouse;
        //notifies the user what House the Sim is in
        if(SHOW_DEBUG)
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
            if(SHOW_DEBUG)
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
            if(SHOW_DEBUG)
                cout << "This object is in a different room." << endl;
        }
    }
        
    else{
        // if the Sim is already in a Room, we enter this conditional statement
        if (room->objectInside(&object)){
            //if the object is inside the same room as the Sim, they can interact
            if(SHOW_DEBUG)
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
            if(SHOW_DEBUG)
                cout << "This object is not in the same room as " << name << endl;
        }
    }
}

void Sim::changeCoordinates(tuple<int,int>& xy){
    // method that changes a sims coordinates in a room
    coordinates = xy;
}
        
void Sim::closestObject(){
    // method that finds the closest object to the sim
    if (room != nullptr){
        // call function that returns coordinates of each object in room;
        Object* closestobject = room->closestToSim(coordinates, *this);
        if(SHOW_DEBUG)
            cout << "The closest object to " << name << " is " << closestobject->getName() << endl;
        return;
    }
    if(SHOW_DEBUG)
        cout << "This sim is not inside a room yet." << endl;
}

void Sim::fufillNeeds(){
    // function that fills sims needs with objects in the room
    if(room != nullptr){
        // if the sim is inside a room, it will interact with all the objects in the room
        if(SHOW_DEBUG)
            cout << name << " will interact with all the objects in the room to fufill their needs." << endl;
        room->interactwithObjects(*this);
        }
    
}

void Sim::printNeeds(){
    int i;
    for(i=0;i<needs.size();i++){
        cout << needs[i];
        if(i < needs.size()-1)
            cout << " ";
    }
    cout << endl;
}

//changes a certain need by some amount
void Sim::alterNeed(int needIndex, int amt){
    needs[needIndex] += amt;

    //cap need at 0
    if(needs[needIndex] < 0)
        needs[needIndex] = 0;

    return;
}
  
//checks if the sim is at the target location and uses it if so
void Sim::atTarget(bool debug){
     //if no target set - don't even bother
    if(!hasTarget()){
        return;
    }       

    //if not at the target location - cancel
    if(get<0>(coordinates) != get<0>(target->getCoordinates()) || get<1>(coordinates) != get<1>(target->getCoordinates()) ){
        if(debug)
            cout << "    (navigating to target) - S: " << tup2str(getCoordinates()) << "  T: " << tup2str(target->getCoordinates()) << endl;
        return;
    }

    //if at the target - use it and remove the target and path
    interactWith(*target);
    //cout << "    INTERACTED WITH TARGET!" << endl;
    target = nullptr;
    navPath.clear();
    return;
}

//navigates to the next coordinate in the navigation path
void Sim::goToNext(){
    //if no target or path set - ignore
    if(!hasTarget())    
        return;
    if(!hasNavPath())
        return;

    //pop off the next coordinate and go to it
    changeCoordinates(navPath.front());
    navPath.pop_front();
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
            if(SHOW_DEBUG)
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

map<string, vector<int>> getfullObjList(){
    map<string, vector<int>> objList;

    std::ifstream file("simObjectList.txt");
    string fileLine;
    string delim = ",";
    while (getline(file, fileLine)){

        //set up variables to save
        string name = "";
        vector<int> fx;

        //split the line and save to a vector
        vector<string> parts;
        stringstream s_stream(fileLine);
        while(s_stream.good()){
            string data;
            getline(s_stream, data, ',');   //get the next part
            parts.push_back(data);
        }

        //assign the parts accordingly
        int i;
        for(i=0;i<parts.size();i++){
            if(i == 0)
                name = parts[i];
            else{
                int nv;
                stringstream(parts[i]) >> nv;
                fx.push_back(nv);
            }
        }


        objList.insert(pair<string,vector<int>>(name, fx));
    }

    return objList;
}

map<string, char> makeObjAsciiMap(map<string, vector<int>> fullObjList){
    map<string, char> amap;
    string curUsedChars = "";
    string extraChar = "1234567890!$%*+=[]{}:;?></|_^&()~";

    map<string, vector<int>>::iterator o;
    //assign a character representation for each object
    for(o=fullObjList.begin();o != fullObjList.end();o++){
        string name = o->first;
        int i;

        //use the name as the basis for the character representation
        for(i=0;i<name.length();i++){
            char c = name.at(i);

            //use lowercase
            if(curUsedChars.find_first_of(c) == string::npos){
                amap.insert(pair<string,char>(name, c));
                curUsedChars += c;
                break;
            }
            //use uppercase
            else if(curUsedChars.find_first_of(toupper(c)) == string::npos){
                amap.insert(pair<string,char>(name, toupper(c)));
                curUsedChars += toupper(c);
                break;
            }
        }


        //no assignment yet, use defaults
        if(amap.count(name) == 0){
            int i;
            for(i=0;i<extraChar.length();i++){
                char c = extraChar.at(i);

                //character was already used, pick the next one
                if(curUsedChars.find_first_of(c) != string::npos){
                    continue;
                }
                //new character can be saved, add to the map and pending list
                else{
                    amap.insert(pair<string,char>(name, c));
                    curUsedChars += c;
                    break;
                }
            }
        }
    }

    return amap;
}

//////// JSON EXPORTING /////////

//exports the house to a json file
string House::toJSON(){
   // map<string, vector<int>> fullObjList = getfullObjList();
   // map<string, char> charMap = makeObjAsciiMap(fullObjList);

    string json = "";
    json += ("{ \"house_" + to_string(getId()) + "\": {\n");           //print the name
    json += (" \"name\": \"" + getName() + "\",\n");             //print the id
    json += (" \"fitness\": " + to_string(getFitness()) + ",\n");   //print the fitness
    //json += (" \"map\": \"\n" + asciiRep(charMap) + "\",\n");     //multiline not supported in json
    
    json += ("  \"rooms\": [\n");       //start rooms
        vector<Room*>rooms = getRooms();
        int r;
        for(r=0;r<rooms.size();r++){
            json += rooms[r]->toJSON();
            if(r < rooms.size()-1)
                json += ",\n";
            else
                json += "\n";
        }
        json += "]\n";
   
    json += ("}}");         //close

    return json;
}

string Room::toJSON(){
    string json = "";
    json += ("{ \"name\": \"" + getName() + "\",\n");     //print the name
    json += (" \"dimensions\": ");                   //start dimensions
        tuple<int,int> d = getDimensions();
        json += ("{\"x\": " + to_string(get<0>(d)) + ", ");
        json += ("\"y\": " + to_string(get<1>(d)) + "},\n");
    json += (" \"objects\": [\n");                  //start objects
        vector<Object *>objs = getObjects();
        int o;
        for(o=0;o<objs.size();o++){
            json += objs[o]->toJSON();
            if(o < objs.size()-1)
                json += ",\n";
            else
                json += "\n";
        }
        json += "  ]\n";
    json += "}";
    return json;
}

string Object::toJSON(){
    string json = "";
    json += ("{ \"name\": \"" + getName() + "\",\n");     //print name
    json += (" \"position\": ");                   //start dimensions
        tuple<int,int> d = getCoordinates();
        json += ("{\"x\": " + to_string(get<0>(d)) + ", ");
        json += ("\"y\": " + to_string(get<1>(d)) + "},\n");
    json += (" \"needFX\": {\n");                   //start needs
        string needs[] = {"hunger", "hygeine", "bladder", "energy", "social", "fun"};
        int f;
        for(f=0;f<6;f++){
            json += ("  \"" + needs[f] + "\": " + to_string(getNeedValue(f)));
            if(f < 5)
                json += ",\n";
            else
                json += "\n";
        }
        json += " }\n";
    json += "}";

    return json;
}

