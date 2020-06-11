#include "class_def.h"

////// SIMULATION.CPP CODE ///////
class Node;

float calculateFitness(Sim* s);
void findNeedObj(Sim *s, int needIndex);
float objDist(Sim *s, Object *o);
float objDistManhattan(Sim *s, Object *o);
list<tuple<int,int>> getBFSPath(tuple<int, int>start, tuple<int, int>end, tuple<int, int> boundary, list<tuple<int,int>>xs);
void getNeighbors(Node* n, tuple<int,int> bounds, list<Node>& neighbors);
bool visited(list<Node> v, Node* n);
bool inSet(list<tuple<int,int>>s, tuple<int,int>e);

float simulate(Sim* simChar, int maxTicks, vector<int> rate, int threshold, vector<int> needsRanking);



////// NOVELTY.CPP CODE ////////
const float MINIMUM_SIM_FITNESS_CRITERIA = 0.0;
const float MINIMUM_NOVEL_DISTANCE = 3.0;
const int K_VALUE = 10;

float euclidDist(map<string, int> a, map<string, int>b);
float avg_knn_dist(list<House*> neighbors, House* noob, int k);
bool sortAsc(const pair<House *, float> &a, const pair<House *, float> &b);

bool isNovel(list<House *> novelSet, House* h, float simFitness);
