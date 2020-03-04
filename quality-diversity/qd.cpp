#include "prototype_data.h"

int main(){
	srand(time(NULL));
	Room sampleSet[100];

	int i;
	for(i=0;i<100;i++){
		Room r = createRandomRoom();
		cout << "ROOM #" << i+1 << endl;
		showRoom(r);
		sampleSet[i] = r;
	}
	
	
}