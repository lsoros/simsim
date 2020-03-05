#include "prototype_data.h"

int main(){

	srand(time(NULL));
	UltraRoom sampleSet[100];

	int i;
	for(i=0;i<100;i++){
		cout << "ROOM #" << i+1 << endl;
		UltraRoom ur;
		showUltraRoom(ur);
		sampleSet[i] = ur;
	}
	
	return 1;
}