#include "includes/simsim_func.h"

//based on instructions from http://eplex.cs.ucf.edu/noveltysearch/userspage/
void printHouseMap(map<House *, float> v){
	map<House *, float>::iterator i;
	for(i=v.begin();i!=v.end();++i){
		cout << i->first->getName() << ": " << i->second << endl;
	}
}
void printHouseVec(vector<pair<House *, float>> v){
	int i;
	for(i=0;i<v.size();i++){
		cout << v[i].first->getName() << ": " << v[i].second << endl;
	}
}

bool isNovel(list<House *> novelSet, House* h, float simFitness){
	//if nothing in the novel set, then it's the novel-est!
	if(novelSet.size() == 0)
		return true;

	//check minimum criteria
	if(simFitness < MINIMUM_SIM_FITNESS_CRITERIA){	//did not pass
		return false;
	}

	int real_k = (K_VALUE > novelSet.size() ? novelSet.size() : K_VALUE);

	float nd = avg_knn_dist(novelSet, h, real_k);
	if(nd >= MINIMUM_NOVEL_DISTANCE)				//if distance is big enough, return as novel
		return true;
	else
		return false;
}

bool sortAsc(const pair<House *, float> &a, const pair<House *, float> &b){return (a.second < b.second); }


//returns the euclidean distance between 2 integer vectors (assume for the object count)
//if an object is missing from one of the vectors that is found in the other, the default for the missing object = 0
//if both are missing, then 0 is default and will be (0-0)^2 for that item
float euclidDist(map<string, int> a, map<string, int>b){
	float d_2 = 0;

	/// iterate through both arrays ///

	//a house objects - look for similar objects and missing objects
	for(map<string,int>::iterator i = a.begin();i != a.end(); ++i){
		string key = i->first;
		if(b.find(key) != a.end())			//if the key was found, add the distance between values
			d_2 += pow(a[i->first]-b[i->first],2);
		else										//else add the entire value (x-0)^2
			d_2 += pow(a[i->first]-0,2);	
	}

	//b house objects - look missing objects only
	for(map<string,int>::iterator i = b.begin();i != b.end(); ++i){
		string key = i->first;
		if(a.find(key) != b.end())			//if the key was found, it was already calculated so skip
			continue;
		else										//else add the entire value (0-y)^2
			d_2 += pow(0-b[i->first],2);	
	}
	return sqrt(d_2);
}

//returns the average distance to the k nearest neighbors from a set
float avg_knn_dist(list<House*> neighbors, House* noob, int k){
/// get the distances ///
	map<House *, float> neighborDist;
	list<House *>::iterator n;
	for(n=neighbors.begin();n != neighbors.end();n++){
		neighborDist.insert(pair<House *, float>((*n), euclidDist(noob->getObjectCt(), (*n)->getObjectCt())));
	}

	if(SHOW_DEBUG){
		cout << "--Original--" << endl;
		printHouseMap(neighborDist);
	}
	

/// sort in ascending order ///
	vector<pair<House *, float>> kvec;

	// copy rom the map to the vector
	map<House *, float>::iterator i2;
	for (i2=neighborDist.begin(); i2!=neighborDist.end(); i2++){
		kvec.push_back(make_pair(i2->first, i2->second));
	}

	//sort the vector
	sort(kvec.begin(), kvec.end(), sortAsc); 

	if(SHOW_DEBUG){
		cout << "--Sorted--" << endl;
		printHouseVec(kvec);
	}

	/// average the first k distances ///
	float d_sum = 0;
	int i;

	int k_min = std::min((int)kvec.size(), k);

	for(i=0;i<k_min;i++){
		d_sum += kvec[i].second;
	}


	return d_sum/k_min;

}



//int main(){return 0;}