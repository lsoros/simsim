#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

int main() {
	Sim rachel("Rachel");
    Room livingroom("Living Room");
    cout << rachel << endl;
    
    //needs order: hunger, hygeine, bladder, energy, social, fun
    
    // objects the sim can interact with and affect their needs
    Object tv("tv", {0,-1,0,0,0,3}, (5,5));
    livingroom.add_object(tv);
    
    Object phone("phone", {0,0,0,0,3,2}, (-3,0));
    livingroom.add_object(phone);
    
    Object bed("bed", {-1,-1,-1,3,-1,-1}, (4,1));
    livingroom.add_object(bed);
    
    Object couch("couch", {0,0,0,1,0,0}, (-3,4));
    livingroom.add_object(couch);
    
    Object desk("desk", {0,0,0,0,0,1}, (3,4));
    livingroom.add_object(desk);
    
    Object chair("chair", {0,0,0,0,0,0}, (9,3));
    livingroom.add_object(chair);
    
    Object lamp("lamp", {0,0,0,0,0,0}, (2,0));
    livingroom.add_object(lamp);
    
    Object dresser("dresser", {0,0,0,0,0,0}, (0,1));
    livingroom.add_object(dresser);
    
    Object laptop("laptop", {0,0,0,-1,0,5}, (8,7));
    livingroom.add_object(laptop);
    
    Object musicstero("music stereo", {0,0,0,0,0,3}, (4,1));
    livingroom.add_object(musicstero);
    
    Object bookcase("bookcase", {0,0,0,0,0,2}, (0,10));
    livingroom.add_object(bookcase);
    
    Object painting("painting", {0,0,0,0,0,0}, (9,0));
    livingroom.add_object(painting);
    
    Object treadmill("treadmill", {0,-3,0,-5,0,2}, (-2,3));
    livingroom.add_object(treadmill);
    
    Object chesstable("chess table", {0,0,0,0,2,2}, (-3,-4));
    livingroom.add_object(chesstable);
    
    Object fireplace("fireplace", {0,0,0,0,0,1}, (-3,8));
    livingroom.add_object(fireplace);
    
    Object stove("stove", {4,0,0,0,0,1}, (9,-5));
    livingroom.add_object(stove);
    
    Object coffeemaker("coffee maker", {2,0,-3,3,0,0}, (8,8));
    livingroom.add_object(coffeemaker);
    
    Object fridge("fridge", {5,0,-1,0,0,1}, (0,3));
    livingroom.add_object(fridge);
    
    Object sink("sink", {0,2,0,0,0,0}, (1,7));
    livingroom.add_object(sink);
    
    Object trashcan("trashcan", {0,-1,0,0,0,0}, (-2,-4));
    livingroom.add_object(trashcan);
    
    Object toilet("toilet", {0,0,5,0,0,0}, (-8,-8));
    livingroom.add_object(toilet);
    
    Object bathtub("bathtub", {0,5,0,0,0,1}, (0,1));
    livingroom.add_object(bathtub);
    
    Object shower("shower", {0,5,0,0,0,0}, (9,0));
    livingroom.add_object(shower);
    
    Object mirror("mirror", {0,0,0,0,0,0}, (2,8));
    livingroom.add_object(mirror);
    
    Object hamper("hamper", {0,0,0,0,0,0}, (9,9));
    livingroom.add_object(hamper);
    
    Object washingmachine("washing machine", {0,0,0,0,0,0}, (8,9));
    livingroom.add_object(washingmachine);
    
    Object dryer("dryer", {0,0,0,0,0,0}, (7,9));
    livingroom.add_object(dryer);
    
    Object alarmclock("alarm clock", {0,0,0,0,0,-1}, (2,-3));
    livingroom.add_object(alarmclock);
    
    Object nightstand("night stand", {0,0,0,0,0,0}, (2,-9));
    livingroom.add_object(nightstand);
    
    Object videogameconsole("video game console", {0,0,0,-1,2,5}, (2,4));
    livingroom.add_object(videogameconsole);
    
    Object coffeetable("coffee table", {0,0,0,0,0,0}, (1,2));
    livingroom.add_object(coffeetable);
    
    Object buffettable("buffet table", {3,0,-1,0,0,1}, (3,-4));
    livingroom.add_object(buffettable);
    
    Object guitar("guitar", {0,0,0,0,1,4}, (3,-5));
    livingroom.add_object(guitar);
    
    Object piano("piano", {0,0,0,0,2,3}, (6,6));
    livingroom.add_object(piano);
    
    Object easel("easel", {0,0,0,0,0,3}, (-2,8));
    livingroom.add_object(easel);
    
    Object plant("plant", {0,0,0,0,0,0}, (6,-9));
    livingroom.add_object(plant);
    
    Object foosballtable("foosball table", {0,0,0,0,3,5}, (-2,6));
    livingroom.add_object(foosballtable);
    
    Object fishbowl("fish bowl", {0,0,0,0,0,1}, (9,9));
    livingroom.add_object(fishbowl);
    
    Object telescope("telescope", {0,0,0,0,0,2}, (9,2));
    livingroom.add_object(telescope);
    
    Object crib("crib", {0,0,0,5,0,0}, (6,8));
    livingroom.add_object(crib);
    
    Object dollhouse("doll house", {0,0,0,0,0,1}, (9,4));
    livingroom.add_object(dollhouse);
    
    Object teddybear("teddy bear", {0,0,0,0,1,1}, (4,4));
    livingroom.add_object(teddybear);
    
    Object grill("grill", {3,0,0,0,0,1}, (-3,1));
    livingroom.add_object(grill);
    
    Object newspaper("newspaper", {0,0,0,0,0,0}, (2,-4));
    livingroom.add_object(newspaper);
    
    Object bicycle("bicycle", {0,0,0,-1,0,3}, (8,2));
    livingroom.add_object(bicycle);
    
    Object drumset("drumset", {0,0,0,0,0,1}, (1,0));
    livingroom.add_object(drumset);
    
    Object camera("camera", {0,0,0,0,0,2}, (9,6));
    livingroom.add_object(camera);
    
    Object cellphone("cellphone", {0,0,0,0,3,2}, (4,7));
    livingroom.add_object(cellphone);
    
    Object microwave("microwave", {4,0,0,0,0,0}, (8,-3));
    livingroom.add_object(microwave);
    
    Object tissuebox("tissue box", {0,1,0,0,0,0}, (-3,2));
    livingroom.add_object(tissuebox);
    
    Object makeupbox("makeup box", {0,0,0,0,0,0}, (9,0));
    livingroom.add_object(makeupbox);
    
    Object magazine("magazine", {0,0,0,0,0,0}, (-8,1));
    livingroom.add_object(magazine);
    
    Object candle("candle", {0,0,0,0,0,0}, (5,-1));
    livingroom.add_object(candle);
    
    Object highchair("high chair", {0,0,0,0,0,0}, (-4,5));
    livingroom.add_object(highchair);
    
    Object book("book", {0,0,0,0,0,2}, (8,-7));
    livingroom.add_object(book);
    
    Object hottub("hot tub", {0,1,0,0,0,3}, (7,6));
    livingroom.add_object(hottub);
    
    Object megaphone("megaphone", {0,0,0,0,0,0}, (3,2));
    livingroom.add_object(megaphone);
    
    Object arcadegame("arcade game", {0,0,0,0,0,4}, (4,-5));
    livingroom.add_object(arcadegame);
    
    Object photobooth("photo booth", {0,0,0,0,2,4}, (-1,-1));
    livingroom.add_object(photobooth);
    
    Object pooltable("pool table", {0,0,0,0,2,4}, (-3,-4));
    livingroom.add_object(pooltable);
    
    Object tabletennis("table tennis", {0,0,0,0,3,5}, (8,4));
    livingroom.add_object(tabletennis);
    
    Object backpack("backpack", {0,0,0,0,0,0}, (0,6));
    livingroom.add_object(backpack);
    
    Object vendingmachine("vending machine", {3,0,0,0,0,0}, (3,0));
    livingroom.add_object(vendingmachine);
    
    Object karaokemachine("karaoke machine", {0,0,0,0,3,5}, (-6,0));
    livingroom.add_object(karaokemachine);
    
    Object smokedetector("smoke detector", {0,0,0,0,0,0}, (9,8));
    livingroom.add_object(smokedetector);
    
    Object dartboard("dartboard", {0,0,0,0,2,2}, (1,-2));
    livingroom.add_object(dartboard);
    
    Object foodbowl("food bowl", {0,0,0,0,0,0}, (9,2));
    livingroom.add_object(foodbowl);
    
    Object incense("incense", {0,0,0,0,0,1}, (3,6));
    livingroom.add_object(incense);
    
    Object clock("clock", {0,0,0,0,0,0}, (7,-7));
    livingroom.add_object(clock);
    
    Object calculator("calculator", {0,0,0,0,0,0}, (3,2));
    livingroom.add_object(calculator);
    
    // Place a sim in a room, can now only interact with the objects in that room
    livingroom.placeSim(rachel);
    
    
}
