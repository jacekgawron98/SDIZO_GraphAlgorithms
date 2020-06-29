#ifndef LIST_GRAPH_H
#define LIST_GRAPH_H

#include <string>
#include <iostream>
#include "MatrixGraph.h"
#include "helperFunctions.h"
using namespace std;

class ListElement {
public:
	int name;
	int start;
	int weight;
	ListElement *next;
	ListElement(int n, int w, int s) {
		name = n;
		start = s;
		weight = w;
		next = NULL;
	}
};

class List {
public:

	ListElement * head;
	ListElement* tail;
	int size;

	List() {
		head = NULL;
		tail = NULL;
		size = 0;
	}

	void pushBack(int name, int w, int s) {
		if (tail == NULL) {
			head = new ListElement(name, w, s);
			tail = head;
			size++;
			return;
		}
		size++;
		tail->next = new ListElement(name, w, s);
		tail = tail->next;
	}

	ListElement* find(int name) {
		ListElement* curElement = head;
		if (curElement == NULL) {
			return NULL;
		}
		while (curElement->name != name) {
			curElement = curElement->next;
			if (curElement == NULL)
				break;
		}
		return curElement;
	}

	void swapElements(ListElement* i, ListElement* j) {
		int nameBuff = i->name;
		int startBuff = i->start;
		int weightBuff = i->weight;
		i->name = j->name;
		i->start = j->start;
		i->weight = j->weight;
		j->name = nameBuff;
		j->start = startBuff;
		j->weight = weightBuff;
	}

	ListElement*& operator[](int index)
	{
		if (index >= size) {
			cout << "Przeroczono dozwolony indeks";
			cin.get();
			//exit(0);
		}
		ListElement *current = head;
		for (int i = 0; i <= index; i++) {
			if (i == index) {
				return current;
			}
			current = current->next;
		}
		current = NULL;
		return current;
	}
};

class ListGraph {
	List* list;
	List sE;
	int size;
	int edgesNumber;
	int start;

	double testPrim(ListGraph graph);
	double testKruskal(ListGraph graph);
	double testDijkstra(ListGraph graph);
	double testFB(ListGraph graph);

	int findSet(int x, int parent[]);
	void Union(int x, int y, int parent[], int rank[]);
	void makeSet(int parent[], int rank[]);
	int findMinKey(int key[], bool inMST[]);

	int extractMin(int dist[], bool p[]);
public:
	ListGraph(int size);

	static ListGraph readFromFileMST(ListGraph graph, std::string fileName);
	static ListGraph readFromFileSP(ListGraph graph, std::string fileName);
	static ListGraph convertFromMatrix(MatrixGraph graph);

	static ListGraph generateRandomMST(int v, int density);
	static ListGraph generateRandomSP(int v, int density);

	void qsort();

	void displayGraph();
	void displayMST(int parent[]);
	void displaySP(int b[], bool isBF);

	static void testMST();
	static void testSP();

	int* prim();
	void kruskal(bool isTest);
	int* dijkstra();
	int* fordBellman();
};

#endif
