#ifndef MATRIX_GRAPH_H
#define MATRIX_GRAPH_H
#include <string>
#include <iostream>
#include <fstream>
#include "helperFunctions.h"
using namespace std;


class MatrixGraph {
	int** matrix;
	int size;
	int edgesNumber;
	int start;

	double testPrim(MatrixGraph graph);
	double testKruskal(MatrixGraph graph);
	double testDijkstra(MatrixGraph graph);
	double testFB(MatrixGraph graph);

	void freeMemory();
	int findMinKey(int key[], bool inMST[]);

	void makeSet(int group[], int rank[]);
	int findSet(int x, int group[]);
	void Union(int x, int y, int group[],int rank[]);

	int extractMin(int dist[], bool p[]);
public:
	MatrixGraph(int size);
	static MatrixGraph readFromFileMST(MatrixGraph graph, string fileName);
	static MatrixGraph readFromFileSP(MatrixGraph graph, string fileName);
	static MatrixGraph generateRandomMST(int v, int density);
	static MatrixGraph generateRandomSP(int v, int density);

	void displayGraph();
	void displayMST(int parent[]);
	void displaySP(int b[],bool isBF);

	static void testMST();
	static void testSP();

	int* prim();
	void kruskal(bool isTest);
	int* dijkstra();
	int* fordBellman();
	
	int getSize() {
		return size;
	}

	int getEdgesNumber() {
		return edgesNumber;
	}

	int getStart() {
		return start;
	}
	int** getMatrix() {
		return matrix;
	}
};

#endif