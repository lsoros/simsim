#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

int main() {
	Sim rachel("Rachel");
    cout << rachel << endl;
    
    //needs order: hunger, hygeine, bladder, energy, social, fun
    vector<int> tv_fx{0,-1,0,0,0,3};
    vector<int> phone_fx{0,0,0,0,3,2};
    vector<int> bed_fx{-1,-1,-1,3,-1,-1};
    vector<int> couch_fx{0,0,0,1,0,0};
    vector<int> decorations_1_fx{0,0,0,0,0,0};
    vector<int> decorations_2_fx{0,0,0,0,0,0};
        
    // room is 5 x 5
    tuple<int, int> tv_xy(5,5);
    tuple<int, int> phone_xy(-3,0);
    tuple<int, int> bed_xy(4,1);
    tuple<int, int> couch_xy(-3,4);
    tuple<int, int> decorations_1_xy(4,-5);
    tuple<int, int> decorations_2_xy(0,5);
    
    // objects the sim can interact with and affect their needs
    Object tv("tv", tv_fx, tv_xy);
    Object phone("phone", phone_fx, phone_xy);
    Object bed("bed", bed_fx, bed_xy);
    Object couch("couch", couch_fx, couch_xy);
    Object decorations_1("decorations_1", decorations_1_fx, decorations_1_xy);
    Object decorations_2("decorations_2", decorations_2_fx, decorations_2_xy);
    
    rachel.interactWith(tv);
    cout << rachel << endl;
    
    // Room object, which can hold different objects
    Room livingroom("Living Room");
    livingroom.add_object(tv);
    livingroom.add_object(phone);
    livingroom.add_object(couch);
    livingroom.add_object(decorations_1);
    livingroom.add_object(decorations_2);
    cout << livingroom << endl;
    
    // Place a sim in a room, can now only interact with the objects in that room
    livingroom.placeSim(rachel);
    
    
}
