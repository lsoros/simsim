#include "prototype_data.h"

void showUltraRoom(UltraRoom ur){
	cout << "Furniture Count: " << endl;
	
	unordered_map<string, int> furnitureTally = boost::get<unordered_map<string, int> >(ur.properties.at("Furniture"));
	unordered_map<string, int> :: iterator itr;
	for(itr = furnitureTally.begin(); itr != furnitureTally.end(); itr++){
		cout << "\t" << itr->first << " = " << itr->second << endl;
	}
	
	int tCost = boost::get<int>(ur.properties.at("TotalCost"));
	cout << "Total Cost: $" << tCost << endl;
	double sHealth = boost::get<double>(ur.properties.at("SimHealth"));
	cout << "Sim Health: " << sHealth*100.0f << "%" << endl;

}




