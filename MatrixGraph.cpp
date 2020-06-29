#include "MatrixGraph.h"
#include "helperFunctions.h"
#include <cstdlib>

MatrixGraph::MatrixGraph(int size) {
	start = 0;
	this->size = size;
	matrix = new int*[size];
	for (int i = 0; i < size; i++) {
		matrix[i] = new int[size];
		for (int j = 0; j < size; j++) {
			matrix[i][j] = 0;
		}
	}
}

MatrixGraph MatrixGraph::readFromFileMST(MatrixGraph graph, string fileName) {
	ifstream file;
	file.open(fileName);
	if (!file) {
		cout << "Nie mozna wczytac z pliku" << endl;
		return graph;
	}
	int numberOfVertices, numberOfEdges;
	file >> numberOfEdges;
	file >> numberOfVertices;
	MatrixGraph newGraph = MatrixGraph(numberOfVertices);
	newGraph.edgesNumber = numberOfEdges;
	for (int i = 0; i < numberOfEdges; i++) {
		int start, end, weight;
		file >> start;
		file >> end;
		file >> weight;
		newGraph.matrix[start][end] = weight;
		newGraph.matrix[end][start] = weight;
	}
	return newGraph;
}

MatrixGraph MatrixGraph::readFromFileSP(MatrixGraph graph, string fileName) {
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
	MatrixGraph newGraph = MatrixGraph(numberOfVertices);
	newGraph.start = startPoint;
	for (int i = 0; i < numberOfEdges; i++) {
		int start, end, weight;
		file >> start;
		file >> end;
		file >> weight;
		newGraph.matrix[start][end] = weight;
	}
	return newGraph;
}

MatrixGraph MatrixGraph::generateRandomMST(int v, int density) {
	int numberOfEdges = (density * (v*(v - 1) / 2)/100);
	MatrixGraph newGraph = MatrixGraph(v);
	for (int i = 0; i < v - 1; i++) {
		newGraph.matrix[i][i + 1] = rand() % 10 + 1;
		newGraph.matrix[i + 1][i] = newGraph.matrix[i][i + 1];
	}
	numberOfEdges -= (v - 1);
	while (numberOfEdges > 0) {
		int x, y;
		x = rand() % v;
		y = rand() % v;
		if (x == y || newGraph.matrix[x][y] != 0) {
			continue;
		}
		newGraph.matrix[x][y] = rand() % 10 + 1;
		newGraph.matrix[y][x] = newGraph.matrix[x][y];
		numberOfEdges--;
	}
	return newGraph;
}

MatrixGraph MatrixGraph::generateRandomSP(int v, int density) {
	int numberOfEdges = (density * (v*(v - 1)) / 100);
	MatrixGraph newGraph = MatrixGraph(v);
	//newGraph.start = rand() % v;
	for (int i = 0; i < v - 1; i++) {
		newGraph.matrix[i][i + 1] = rand() % 10 + 1;
	}
	numberOfEdges -= (v - 1);
	while (numberOfEdges > 0) {
		int x, y;
		x = rand() % v;
		y = rand() % v;
		if (x == y || newGraph.matrix[x][y] != 0) {
			continue;
		}
		newGraph.matrix[x][y] = rand() % 10 + 1;
		numberOfEdges--;
	}
	return newGraph;
}

void MatrixGraph::displayGraph() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

void MatrixGraph::displayMST(int parent[]) {
	int total = 0;
	for (int i = 0; i < size; i++) {
		if (parent[i] < 0) continue;
		cout << parent[i] << " - " << i << " w: " << matrix[i][parent[i]] << endl;
		total += matrix[i][parent[i]];
	}
	cout << "Sumaryczny koszt: " << total << endl;
}

void MatrixGraph::displaySP(int b[], bool isBF) {
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

int MatrixGraph::findMinKey(int key[], bool inMST[]) {
	int curMin = INT_MAX, index;
	for (int i = 0; i < size; i++) {
		if (inMST[i] == false && key[i] < curMin) {
			curMin = key[i];
			index = i;
		}
	}
	return index;
}

int* MatrixGraph::prim() {
	int* parent = new int[size];
	int* key = new int[size];
	bool* inMST = new bool[size];

	for (int i = 0; i < size; i++) {
		inMST[i] = false;
		key[i] = INT_MAX;
	}

	key[0] = 0;
	parent[0] = -1;
	for (int i = 0; i < size-1; i++) {
		int u = findMinKey(key, inMST);
		inMST[u] = true;
		for (int v = 0; v < size; v++) {
			if (inMST[v] == false && matrix[u][v] < key[v] && matrix[u][v]) {
				key[v] = matrix[u][v];
				parent[v] = u;
			}
		}
	}

	return parent;
}


void MatrixGraph::makeSet(int group[], int rank[]) {
	for (int i = 0; i < size; i++) {
		group[i] = i;
		rank[i] = 0;
	}
}

void MatrixGraph::Union(int x, int y,int group[],int rank[]) {
	int a = findSet(x,group);
	int b = findSet(y,group);
	if (rank[a] < rank[b]) {
		group[a] = b;
	}
	else {
		group[b] = a;
	}

	if (rank[a] == rank[b]) {
		rank[a]++;
	}
}

int MatrixGraph::findSet(int x, int group[]) {
	if (group[x] != x) {
		group[x] = findSet(group[x],group);
	}
	return group[x];
}


void MatrixGraph::kruskal(bool isTest) {
	int* group = new int[size];
	int* rank = new int[size];
	int total = 0;
	makeSet(group, rank);
	int edgeCount = 0;
	while(edgeCount < size-1){
		int min = INT_MAX, a = -1, b = -1;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (findSet(i,group) != findSet(j,group) && matrix[i][j] < min && matrix[i][j] != 0) {
					min = matrix[i][j];
					a = i;
					b = j;
				}
			}
		}
		Union(a, b, group,rank);
		edgeCount++;
		if (!isTest) {
			cout << a << " - " << b << " w: " << min << endl;
			total += min;
		}
	}
	if (!isTest) {
		cout << "Sumaryczny koszt: " << total << endl;
	}
}

int MatrixGraph::extractMin(int dist[], bool p[]) {
	int min = INT_MAX, index;
	for (int i = 0; i < size; i++) {
		if (p[i] == false && dist[i] <= min) {
			min = dist[i];
			index = i;
		}
	}
	return index;
}

int* MatrixGraph::dijkstra() {
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
		for (int v = 0; v < size; v++) {
			if (matrix[u][v] > 0 && p[v] == false && b[u] != INT_MAX && b[u] + matrix[u][v] < b[v]) {
				b[v] = b[u] + matrix[u][v];
			}
		}
	}

	return b;
}

int* MatrixGraph::fordBellman() {
	bool* p = new bool[size];
	int* b = new int[size + 1];

	for (int i = 0; i < size; i++) {
		b[i] = INT_MAX;
		p[i] = false;
	}

	b[start] = 0;
	b[size] = 0;
	for (int i = 0; i < size; i++) {
		for (int u = 0; u < size; u++) {
			for (int v = 0; v < size; v++) {
				if (matrix[u][v] != 0 && b[v] > b[u] + matrix[u][v]) {
					b[v] = b[u] + matrix[u][v];
					p[v] = true;
				}
			}
		}
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (matrix[i][j] != 0 && b[j] > b[i] + matrix[i][j]) {
				b[size] = 1;
				return b;
			}
		}
	}

	return b;
}

double MatrixGraph::testKruskal(MatrixGraph graph) {
	counterStart();
	graph.kruskal(true);
	return getCounter();
}

double MatrixGraph::testPrim(MatrixGraph graph) {
	counterStart();
	graph.prim();
	return getCounter();
}

double MatrixGraph::testDijkstra(MatrixGraph graph) {
	counterStart();
	graph.dijkstra();
	return getCounter();
}

double MatrixGraph::testFB(MatrixGraph graph) {
	counterStart();
	graph.fordBellman();
	return getCounter();
}

void MatrixGraph::testMST() {
	double timePrim = 0.0;
	double timeKruskal = 0.0;
	MatrixGraph newGraph = MatrixGraph(0);
	int testSize = 40;
	for (int i = 1; i < 8; i++) {
		testSize = 40 + (i - 1) * 40;
		newGraph = MatrixGraph::generateRandomMST(testSize,20);
		for (int j = 0; j < 100; j++) {
			timePrim = newGraph.testPrim(newGraph);
			timeKruskal = newGraph.testKruskal(newGraph);
		}
		cout << "Czas wykonania algorytmu Prima dla " << testSize << " wierzcholkow i gestosci 20%: " << timePrim / 100 << endl;
		cout << "Czas wykonania algorytmu Kruskala dla " << testSize << " wierzcholkow i gestosci 20%: " << timeKruskal / 100 << endl;
		timePrim = 0.0;
		timeKruskal = 0.0;
		newGraph.freeMemory();
		newGraph = MatrixGraph::generateRandomMST(testSize, 60);
		for (int j = 0; j < 100; j++) {
			timePrim = newGraph.testPrim(newGraph);
			timeKruskal = newGraph.testKruskal(newGraph);
		}
		cout << "Czas wykonania algorytmu Prima dla " << testSize << " wierzcholkow i gestosci 60%: " << timePrim / 100 << endl;
		cout << "Czas wykonania algorytmu Kruskala dla " << testSize << " wierzcholkow i gestosci 60%: " << timeKruskal / 100 << endl;
		timePrim = 0.0;
		timeKruskal = 0.0;
		newGraph.freeMemory();
		newGraph = MatrixGraph::generateRandomMST(testSize, 99);
		for (int j = 0; j < 100; j++) {
			timePrim = newGraph.testPrim(newGraph);
			timeKruskal = newGraph.testKruskal(newGraph);
		}
		cout << "Czas wykonania algorytmu Prima dla " << testSize << " wierzcholkow i gestosci 99%: " << timePrim / 100 << endl;
		cout << "Czas wykonania algorytmu Kruskala dla " << testSize << " wierzcholkow i gestosci 99%: " << timeKruskal / 100 << endl;
		timePrim = 0.0;
		timeKruskal = 0.0;
		newGraph.freeMemory();
	}
}

void MatrixGraph::testSP() {
	double timeDijkstra = 0.0;
	double timeFB = 0.0;
	MatrixGraph newGraph = MatrixGraph(0);
	int testSize = 40;
	for (int i = 1; i < 8; i++) {
		testSize = 40 + (i - 1) * 40;
		newGraph = MatrixGraph::generateRandomMST(testSize, 20);
		for (int j = 0; j < 100; j++) {
			timeDijkstra = newGraph.testDijkstra(newGraph);
			timeFB = newGraph.testFB(newGraph);
		}
		cout << "Czas wykonania algorytmu Dijkstry dla " << testSize << " wierzcholkow i gestosci 20%: " << timeDijkstra / 100 << endl;
		cout << "Czas wykonania algorytmu Forda Bellmana dla " << testSize << " wierzcholkow i gestosci 20%: " << timeFB / 100 << endl;
		timeDijkstra = 0.0;
		timeFB = 0.0;
		newGraph.freeMemory();
		newGraph = MatrixGraph::generateRandomMST(testSize, 60);
		for (int j = 0; j < 100; j++) {
			timeDijkstra = newGraph.testDijkstra(newGraph);
			timeFB = newGraph.testFB(newGraph);
		}
		cout << "Czas wykonania algorytmu Dijkstry dla " << testSize << " wierzcholkow i gestosci 60%: " << timeDijkstra / 100 << endl;
		cout << "Czas wykonania algorytmu Forda Bellmana dla " << testSize << " wierzcholkow i gestosci 60%: " << timeFB / 100 << endl;
		timeDijkstra = 0.0;
		timeFB = 0.0;
		newGraph.freeMemory();
		newGraph = MatrixGraph::generateRandomMST(testSize, 99);
		for (int j = 0; j < 100; j++) {
			timeDijkstra = newGraph.testDijkstra(newGraph);
			timeFB = newGraph.testFB(newGraph);
		}
		cout << "Czas wykonania algorytmu Dijkstry dla " << testSize << " wierzcholkow i gestosci 99%: " << timeDijkstra / 100 << endl;
		cout << "Czas wykonania algorytmu Forda Bellmana dla " << testSize << " wierzcholkow i gestosci 99%: " << timeFB / 100 << endl;
		timeDijkstra = 0.0;
		timeFB = 0.0;
		newGraph.freeMemory();
	}
}

void MatrixGraph::freeMemory() {
	for (int i = 0; i < size; i++) {
		delete[]matrix[i];
	}
	delete[]matrix;
}
