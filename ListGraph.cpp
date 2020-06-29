#include "ListGraph.h"
#include "helperFunctions.h"
#include <cstdlib>

bool isSorted(List arr, int start, int end) { // funkcja pomocnicza pomagaj¹ca w przyœpieszeniu sortowania
	for (int i = start + 1; i < end; i++) {
		if (arr[i]->weight < arr[i - 1]->weight) {
			return false;
		}
	}
	return true;
}

void qsort_helper(List arr, int l, int r) //funkcja sortuj¹ca
{
	if (isSorted(arr,l,r)) {
		return;
	}
	int v = arr[(l + r) / 2]->weight;
	int i, j;
	i = l;
	j = r;
	do {
		while (arr[i]->weight < v) i++;
		while (arr[j]->weight > v) j--;
		if (i <= j) {
			arr.swapElements(arr[i], arr[j]);
			i++;
			j--;
		}
	} while (i <= j);
	if (j > l) qsort_helper(arr, l, j);
	if (i < r) qsort_helper(arr, i, r);
}

ListGraph::ListGraph(int size) { //konstruktor
	start = 0;
	this->size = size;
	list = new List[size];
	sE = List();
}

ListGraph ListGraph::readFromFileMST(ListGraph graph, std::string fileName) { //£adowanie z pliku dla problemu MST
	ifstream file;
	file.open(fileName);
	if (!file) {
		cout << "Nie mozna wczytac z pliku" << endl;
		return graph;
	}
	int numberOfVertices, numberOfEdges;
	file >> numberOfEdges;
	file >> numberOfVertices;
	ListGraph newGraph = ListGraph(numberOfVertices);
	newGraph.edgesNumber = numberOfEdges;
	for (int i = 0; i < numberOfEdges; i++) {
		int start, end, weight;
		file >> start;
		file >> end;
		file >> weight;
		newGraph.list[start].pushBack(end, weight, start);
		newGraph.sE.pushBack(end, weight, start);
		newGraph.list[end].pushBack(start, weight, end);
		newGraph.sE.pushBack(start, weight, end);
	}
	return newGraph;
}

ListGraph ListGraph::readFromFileSP(ListGraph graph, std::string fileName) { //£adowanie z pliku dla problemu SP
	ifstream file;
	file.open(fileName);
	if (!file) {
		cout << "Nie mozna wczytac z pliku" << endl;
		return graph;
	}
	int numberOfVertices, numberOfEdges, startPoint;
	file >> numberOfEdges;
	file >> numberOfVertices;
	file >> startPoint;
	ListGraph newGraph = ListGraph(numberOfVertices);
	newGraph.start = startPoint;
	newGraph.edgesNumber = numberOfEdges;
	for (int i = 0; i < numberOfEdges; i++) {
		int start, end, weight;
		file >> start;
		file >> end;
		file >> weight;
		newGraph.list[start].pushBack(end, weight, start);
		newGraph.sE.pushBack(end, weight, start);
	}
	return newGraph;
}

ListGraph ListGraph::convertFromMatrix(MatrixGraph graph) { //Konwersja z postaci macierzowej na listow¹
	ListGraph newGraph = ListGraph(graph.getSize());
	newGraph.edgesNumber = graph.getEdgesNumber();
	newGraph.start = graph.getStart();
	for (int i = 0; i < graph.getSize(); i++) {
		for (int j = 0; j < graph.getSize(); j++) {
			if (graph.getMatrix()[i][j] != 0) {
				newGraph.list[i].pushBack(j, graph.getMatrix()[i][j],i);
				newGraph.sE.pushBack(j, graph.getMatrix()[i][j],i);
			}
		}
	}
	return newGraph;
}

ListGraph ListGraph::generateRandomMST(int v, int density) { //tworzenie losowej listy dla problemu MST
	int numberOfEdges = (density * (v*(v - 1) / 2) / 100);
	ListGraph newGraph = ListGraph(v);
	for (int i = 0; i < v - 1; i++) {
		newGraph.list[i].pushBack(i + 1, rand() % 10 + 1,i);
		newGraph.sE.pushBack(i+1, newGraph.list[i].tail->weight, i);
		newGraph.list[i + 1].pushBack(i, newGraph.list[i].tail->weight,i+1);
		newGraph.sE.pushBack(i, newGraph.list[i+1].tail->weight, i+1);
	}
	numberOfEdges -= (v - 1);
	while (numberOfEdges > 0) {
		int x, y;
		x = rand() % v;
		y = rand() % v;
		if (x == y || newGraph.list[x].find(y) != NULL) {
			continue;
		}
		newGraph.list[x].pushBack(y, rand() % 10 + 1,x);
		newGraph.sE.pushBack(y, newGraph.list[x].tail->weight,x);
		newGraph.list[y].pushBack(x, newGraph.list[x].tail->weight,y);
		newGraph.sE.pushBack(x, newGraph.list[x].tail->weight,y);
		numberOfEdges--;
	}
	return newGraph;
}

ListGraph ListGraph::generateRandomSP(int v, int density) { //tworzenie losowej listy dla problemu SP
	int numberOfEdges = (density * (v*(v - 1) / 2) / 100);
	ListGraph newGraph = ListGraph(v);
	for (int i = 0; i < v - 1; i++) {
		newGraph.list[i].pushBack(i + 1, rand() % 10 + 1,i);
	}
	numberOfEdges -= (v - 1);
	while (numberOfEdges > 0) {
		int x, y;
		x = rand() % v;
		y = rand() % v;
		if (x == y || newGraph.list[x].find(y) != NULL) {
			continue;
		}
		newGraph.list[x].pushBack(y, rand() % 10 + 1,x);
		newGraph.sE.pushBack(y, newGraph.list[x].tail->weight,y);
		numberOfEdges--;
	}
	return newGraph;
}

void ListGraph::displayGraph() { //Wyœwietlanie grafu
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < list[i].size; j++) {
			cout << i << " - " << list[i][j]->name << " w: " << list[i][j]->weight << endl;
		}
	}
}

void ListGraph::displayMST(int parent[]) { //Wyœietlanie wyników algorytmu Prima
	int total = 0;
	for (int i = 0; i < size; i++) {
		if (parent[i] < 0) continue;
		cout << parent[i] << " - " << i << " w: " << list[i].find(parent[i])->weight << endl;
		total += list[i].find(parent[i])->weight;
	}
	cout << "Sumaryczny koszt: " << total << endl;
}

void ListGraph::displaySP(int b[], bool isBF) { //Wyœwietlanie wyników algorytmu Dijkstry i Forda Bellmana
	if (isBF) {
		if (b[size] == 1) {
			cout << "Graf zawiera cykl ujemny" << endl;
			return;
		}
	}
	for (int i = 0; i < size; i++) {
		cout << start << " -> " << i << " odleglosc: " << b[i] << endl;
	}
}

int ListGraph::findMinKey(int key[], bool inMST[]) { //szukanie minimalnego klucza
	int curMin = INT_MAX, index;
	for (int i = 0; i < size; i++) {
		if (inMST[i] == false && key[i] < curMin) {
			curMin = key[i];
			index = i;
		}
	}
	return index;
}

int* ListGraph::prim() { //algorytm Prima dla listy s¹siedztwa
	int* parent = new int[size];
	int* key = new int[size];
	bool* inMST = new bool[size];

	for (int i = 0; i < size; i++) {
		inMST[i] = false;
		key[i] = INT_MAX;
	}

	key[0] = 0;
	parent[0] = -1;

	for (int i = 0; i < size - 1; i++) {
		int u = findMinKey(key, inMST);
		inMST[u] = true;
		for (int v = 0; v < list[u].size; v++) {
			if (inMST[list[u][v]->name] == false && list[u][v]->weight < key[list[u][v]->name]) {
				key[list[u][v]->name] = list[u][v]->weight;
				parent[list[u][v]->name] = u;
			}
		}
	}

	return parent;
}


//Funkcje pomocnicze dla algorytmu kruskala
void ListGraph::makeSet(int parent[], int rank[]) {
	for (int i = 0; i < size; i++) {
		parent[i] = i;
		rank[i] = 0;
	}
}

void ListGraph::Union(int x, int y, int parent[], int rank[]) {
	int a = findSet(x, parent);
	int b = findSet(y, parent);
	if (rank[a] < rank[b]) {
		parent[a] = b;
	}
	else {
		parent[b] = a;
	}

	if (rank[a] == rank[b]) {
		rank[a]++;
	}
}

int ListGraph::findSet(int x, int parent[]) {
	if (parent[x] != x) {
		parent[x] = findSet(parent[x], parent);
	}
	return parent[x];
}



void ListGraph::qsort() {
	qsort_helper(sE, 0, sE.size - 1);
}

void ListGraph::kruskal(bool isTest) { //algorytm Kruskala dla listy s¹siedztwa
	int* parent = new int[size];
	int* rank = new int[size];
	int total = 0;

	makeSet(parent, rank);
	
	for (int i = 0; i < sE.size; i++) {
		int u = sE[i]->start;
		int v = sE[i]->name;
		if (findSet(u,parent) != findSet(v,parent)) {
			Union(u, v, parent, rank);
			if (!isTest) {
				cout << u << " - " << v << " w: " << sE[i]->weight << endl;
				total += sE[i]->weight;
			}
		}
	}

	if (!isTest) {
		cout << "Sumaryczny koszt: " << total << endl;
	}
}

//funkcja pomocnicza dla algorytmu dijkstry
int ListGraph::extractMin(int dist[], bool p[]) { 
	int min = INT_MAX, index;
	for (int i = 0; i < size; i++) {
		if (p[i] == false && dist[i] <= min) {
			min = dist[i];
			index = i;
		}
	}
	return index;
}

int* ListGraph::dijkstra() { //algorytm Dijkstry dla listy s¹siedztwa
	bool* p = new bool[size];
	int* b = new int[size];

	for (int i = 0; i < size; i++) {
		b[i] = INT_MAX;
		p[i] = false;
	}

	b[start] = 0;

	for (int i = 0; i < size; i++) {
		int u = extractMin(b, p);
		p[u] = true;
		for (int v = 0; v < list[u].size; v++) {
			if (b[u] + list[u][v]->weight < b[list[u][v]->name]) {
				b[list[u][v]->name] = b[u] + list[u][v]->weight;
			}
		}
	}

	return b;
}

int* ListGraph::fordBellman() { //algorytm Forda-Bellmana dla listy s¹siedztwa
	bool* p = new bool[size];
	int* b = new int[size + 1];

	for (int i = 0; i < size; i++) {
		b[i] = INT_MAX;
		p[i] = false;
	}

	b[start] = 0;
	b[size] = 0;
	for (int i = 0; i < size - 1; i++) {
		for (int u = 0; u < size; u++) {
			for (int v = 0; v < list[u].size; v++) {
				if (b[list[u][v]->name] > b[u] + list[u][v]->weight) {
					b[list[u][v]->name] = b[u] + list[u][v]->weight;
					p[list[u][v]->name] = true;
				}
			}
		}
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < list[i].size; j++) {
			if (list[i][j]->weight != 0 && b[list[i][j]->name] > b[i] + list[i][j]->weight) {
				b[size] = 1;
				return b;
			}
		}
	}

	return b;
}

//Pomocnicze funkcje do testowania
double ListGraph::testPrim(ListGraph graph) {
	counterStart();
	graph.prim();
	return getCounter();
}

double ListGraph::testKruskal(ListGraph graph) {
	counterStart();
	graph.kruskal(true);
	return getCounter();
}

double ListGraph::testDijkstra(ListGraph graph) {
	counterStart();
	graph.dijkstra();
	return getCounter();
}

double ListGraph::testFB(ListGraph graph) {
	counterStart();
	graph.fordBellman();
	return getCounter();
}

void ListGraph::testMST() { //Testowanie MST
	double timePrim = 0.0;
	double timeKruskal = 0.0;
	ListGraph newGraph = ListGraph(0);
	int testSize = 40;
	for (int i = 1; i < 8; i++) {
		testSize = 40 + (i - 1) * 40;
		newGraph = ListGraph::generateRandomMST(testSize, 20);
		newGraph.qsort();
		for (int j = 0; j < 100; j++) {
			timePrim = newGraph.testPrim(newGraph);
			timeKruskal = newGraph.testKruskal(newGraph);
		}
		cout << "Czas wykonania algorytmu Prima dla " << testSize << " wierzcholkow i gestosci 20%: " << timePrim / 100 << endl;
		cout << "Czas wykonania algorytmu Kruskala dla " << testSize << " wierzcholkow i gestosci 20%: " << timeKruskal / 100 << endl;
		timePrim = 0.0;
		timeKruskal = 0.0;

		newGraph = ListGraph::generateRandomMST(testSize, 60);
		newGraph.qsort();
		for (int j = 0; j < 100; j++) {
			timePrim = newGraph.testPrim(newGraph);
			timeKruskal = newGraph.testKruskal(newGraph);
		}
		cout << "Czas wykonania algorytmu Prima dla " << testSize << " wierzcholkow i gestosci 60%: " << timePrim / 100 << endl;
		cout << "Czas wykonania algorytmu Kruskala dla " << testSize << " wierzcholkow i gestosci 60%: " << timeKruskal / 100 << endl;
		timePrim = 0.0;
		timeKruskal = 0.0;

		newGraph = ListGraph::generateRandomMST(testSize, 99);
		newGraph.qsort();
		for (int j = 0; j < 100; j++) {
			timePrim = newGraph.testPrim(newGraph);
			timeKruskal = newGraph.testKruskal(newGraph);
		}
		cout << "Czas wykonania algorytmu Prima dla " << testSize << " wierzcholkow i gestosci 99%: " << timePrim / 100<< endl;
		cout << "Czas wykonania algorytmu Kruskala dla " << testSize << " wierzcholkow i gestosci 99%: " << timeKruskal /100 << endl;
		timePrim = 0.0;
		timeKruskal = 0.0;
	}
}

void ListGraph::testSP() { //Testowanie SP
	double timeDijkstra = 0.0;
	double timeFB = 0.0;
	ListGraph newGraph = ListGraph(0);
	int testSize = 40;
	for (int i = 1; i < 8; i++) {
		testSize = 40 + (i - 1) * 40;
		newGraph = ListGraph::generateRandomMST(testSize, 20);
		for (int j = 0; j < 100; j++) {
			timeDijkstra = newGraph.testDijkstra(newGraph);
			timeFB = newGraph.testFB(newGraph);
		}
		cout << "Czas wykonania algorytmu Dijkstry dla " << testSize << " wierzcholkow i gestosci 20%: " << timeDijkstra / 100 << endl;
		cout << "Czas wykonania algorytmu Forda Bellmana dla " << testSize << " wierzcholkow i gestosci 20%: " << timeFB / 100 << endl;
		timeDijkstra = 0.0;
		timeFB = 0.0;

		newGraph = ListGraph::generateRandomMST(testSize, 60);
		for (int j = 0; j < 100; j++) {
			timeDijkstra = newGraph.testDijkstra(newGraph);
			timeFB = newGraph.testFB(newGraph);
		}
		cout << "Czas wykonania algorytmu Dijkstry dla " << testSize << " wierzcholkow i gestosci 60%: " << timeDijkstra / 100 << endl;
		cout << "Czas wykonania algorytmu Forda Bellmana dla " << testSize << " wierzcholkow i gestosci 60%: " << timeFB / 100 << endl;
		timeDijkstra = 0.0;
		timeFB = 0.0;

		newGraph = ListGraph::generateRandomMST(testSize, 99);
		for (int j = 0; j < 100; j++) {
			timeDijkstra = newGraph.testDijkstra(newGraph);
			timeFB = newGraph.testFB(newGraph);
		}
		cout << "Czas wykonania algorytmu Dijkstry dla " << testSize << " wierzcholkow i gestosci 99%: " << timeDijkstra / 100 << endl;
		cout << "Czas wykonania algorytmu Forda Bellmana dla " << testSize << " wierzcholkow i gestosci 99%: " << timeFB / 100 << endl;
		timeDijkstra = 0.0;
		timeFB = 0.0;
	}
}