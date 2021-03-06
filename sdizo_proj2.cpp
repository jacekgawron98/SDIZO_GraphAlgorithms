#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include "ListGraph.h"
#include "MatrixGraph.h"
using namespace std;

void displayMenu(string info, int problem) {

	cout << info << endl;
	cout << "1. Zbuduj z pliku" << endl;
	cout << "2. Utworz losowe" << endl;
	cout << "3. Wyswietl zawartosc" << endl;
	cout << "4. Testuj" << endl;
	switch (problem) {
	case 1: {
		cout << "5. Algorytm Prima" << endl;
		cout << "6. Algorytm Kruskala" << endl;
		break;
	}
	case 2: {
		cout << "5. Algorytm Dijkstry" << endl;
		cout << "6. Algorytm Forda-Bellmana" << endl;
		break;
	}
	case 3: {
		cout << "5. Algorytm Forda-Fulkersona" << endl;
		break;
	}
	}
	cout << "0. Powrot do menu" << endl;
}

void menuMST(ListGraph listGraph, MatrixGraph matrixGraph) {
	char option;
	do {
		displayMenu("=====WYZNACZANIE MINIMALNEGO DRZEWA ROZPINAJACEGO=====", 1);
		cin >> option;
		cin.clear();
		switch (option)
		{
		case '1': {
			string name;
			cout << "Podaj nazwe pliku: ";
			cin >> name;
			listGraph = ListGraph::readFromFileMST(listGraph, name);
			matrixGraph = MatrixGraph::readFromFileMST(matrixGraph, name);
			cout << endl << "Postac Listowa" << endl << endl;
			listGraph.displayGraph();
			listGraph.qsort();
			cout << endl << "Postac macierzowa" << endl << endl;
			matrixGraph.displayGraph();
			break;
		}
		case '2': {
			int den, ver;
			cout << "Podaj liczbe wierzcholkow: ";
			cin >> ver;
			cout << "Podaj gestosc grafu w %: ";
			cin >> den;
			matrixGraph = MatrixGraph::generateRandomMST(ver, den);
			listGraph = ListGraph::convertFromMatrix(matrixGraph);
			cout << endl << "Postac Listowa" << endl << endl;
			listGraph.displayGraph();
			listGraph.qsort();
			cout << endl << "Postac macierzowa" << endl << endl;
			matrixGraph.displayGraph();
			break;
		}
		case '3': {
			cout << endl << "Postac Listowa" << endl << endl;
			listGraph.displayGraph();
			cout << endl << "Postac macierzowa" << endl << endl;
			matrixGraph.displayGraph();
			break;
		}
		case '4': {
			cout << endl << "Postac Listowa" << endl << endl;
			ListGraph::testMST();
			cout << endl << "Postac macierzowa" << endl << endl;
			MatrixGraph::testMST();
			break;
		}
		case '5': {
			cout << endl << "Postac Listowa" << endl << endl;
			listGraph.displayMST(listGraph.prim());
			cout << endl << "Postac macierzowa" << endl << endl;
			matrixGraph.displayMST(matrixGraph.prim());
			break;
		}
		case '6': {
			cout << endl << "Postac Listowa" << endl << endl;
			listGraph.kruskal(false);
			cout << endl << "Postac macierzowa" << endl << endl;
			matrixGraph.kruskal(false);
			break;
		}
		default:
			break;
		}
		system("pause");
		system("cls");
	} while (option != '0');
}

void menuShortest(ListGraph listGraph, MatrixGraph matrixGraph) {
	char option;
	do {
		displayMenu("=====WYZNACZANIE NAJKROTSZEJ SCIEZKI W GRAFIE=====", 2);
		cin >> option;
		cin.clear();
		switch (option)
		{
		case '1': {
			string name;
			cout << "Podaj nazwe pliku: ";
			cin >> name;
			listGraph = ListGraph::readFromFileSP(listGraph, name);
			matrixGraph = MatrixGraph::readFromFileSP(matrixGraph, name);
			cout << endl << "Postac Listowa" << endl << endl;
			listGraph.displayGraph();
			cout << endl << "Postac macierzowa" << endl << endl;
			matrixGraph.displayGraph();
			break;
		}
		case '2': {
			int den, ver;
			cout << "Podaj liczbe wierzcholkow: ";
			cin >> ver;
			cout << "Podaj gestosc grafu w %: ";
			cin >> den;
			matrixGraph = MatrixGraph::generateRandomSP(ver, den);
			listGraph = ListGraph::convertFromMatrix(matrixGraph);
			cout << endl << "Postac Listowa" << endl << endl;
			listGraph.displayGraph();
			cout << endl << "Postac macierzowa" << endl << endl;
			matrixGraph.displayGraph();
			break;
		}
		case '3': {
			cout << endl << "Postac Listowa" << endl << endl;
			listGraph.displayGraph();
			cout << endl << "Postac macierzowa" << endl << endl;
			matrixGraph.displayGraph();
			break;
		}
		case '4': {
			cout << endl << "Postac Listowa" << endl << endl;
			ListGraph::testSP();
			cout << endl << "Postac macierzowa" << endl << endl;
			MatrixGraph::testSP();
			break;
		}
		case '5': {
			cout << endl << "Postac Listowa" << endl << endl;
			listGraph.displaySP(listGraph.dijkstra(),false);
			cout << endl << "Postac macierzowa" << endl << endl;
			matrixGraph.displaySP(matrixGraph.dijkstra(), false);
			break;
		}
		case '6': {
			cout << endl << "Postac Listowa" << endl << endl;
			listGraph.displaySP(listGraph.fordBellman(), true);
			cout << endl << "Postac macierzowa" << endl << endl;
			matrixGraph.displaySP(matrixGraph.fordBellman(), true);
			break;
		}
		default:
			break;
		}
		system("pause");
		system("cls");
	} while (option != '0');
}

int main()
{
	srand(time(NULL));

	ListGraph myListGraph = ListGraph(0);
	MatrixGraph myMatrixGraph = MatrixGraph(0);

	char option;
	do {
		cout << "MENU GLOWNE" << endl;
		cout << "1. Wyznaczanie minimalnego drzewa rozpinajacego" << endl;
		cout << "2. Wyznaczanie najkrotszej sciezki w grafie" << endl;
		cout << "0. Wyjscie" << endl;
		cin >> option;
		system("cls");
		switch (option)
		{
		case '1': {
			menuMST(myListGraph, myMatrixGraph);
			break;
		}
		case '2': {
			menuShortest(myListGraph, myMatrixGraph);
			break;
		}
		}
	} while (option != '0');
	return 0;
}

