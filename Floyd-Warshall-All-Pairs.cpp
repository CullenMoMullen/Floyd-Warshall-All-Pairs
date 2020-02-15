// Floyd-Warshall-All-Pairs.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <vector>
#include <list>
#include <cassert>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <limits>

#include "Floyd-Warshall-All-Pairs.h"
#include <random>

using namespace std;

#define MIN(x,y) (((x) < (y))? (x):(y))


typedef struct Edge_t_ {
    struct Node_t_* destNode;
    int weight;

    Edge_t_(struct Node_t_* dst, int Weight)
        : destNode(dst), weight(Weight)
    {
    
    }


} Edge_t;

typedef struct Node_t_ {
    list<Edge_t> list_EdgesOut;
    char name;
    
    Node_t_()
        :name('\0')
    {
    }
    ~Node_t_()
    {
        list_EdgesOut.clear();
    }

    void AddEdge(Node_t_* dstNode, int16_t weight) {
        assert(dstNode != NULL);
        this->list_EdgesOut.push_front(*(new Edge_t(dstNode,weight)));

        cout << "[" << this->name << "," << dstNode->name << "] => " << weight << endl;
    };

} Node_t;


vector<Node_t> nodes(NUM_NODES);

void printMatrix(string& str, int matrix[][NUM_NODES][NUM_NODES], int numRows, int numCols);


int main()
{
    int  w[13];
    
    //cout << "Hello World!\n";
    random_device rd;   // non-deterministic generator
    mt19937 gen(rd());  // to seed mersenne twister.
    uniform_int_distribution<> dist(1, 15); // distribute results between 1 and 6 inclusive.


    for (int n = 0; n < NUM_NODES; n++) {
        nodes[n].name = 'A' + n;    //A,B,C,D....
    }
    
    char s[] = { 'A','A','A','B','C','C','D','E','F','F','F','G','G' };
    char d[] = { 'B','C','D','F','B','G','E','A','A','C','G','D','E' };
    //int  w[] = { -6 , 18, -2, 9 , 5 , -2, -9, 7 , 13, 12, 1 , -4, -5 };

    for (int idx = 0; idx < 13; idx++) {
        w[idx] = dist(gen);
        NODE_BY_NAME(s[idx]).AddEdge(&NODE_BY_NAME(d[idx]), w[idx] -5);
    }

    //NODE_BY_NAME('A').AddEdge(&NODE_BY_NAME('B'), 3);
    //NODE_BY_NAME('A').AddEdge(&NODE_BY_NAME('C'), 8);
    //NODE_BY_NAME('A').AddEdge(&NODE_BY_NAME('E'), -4);

    //NODE_BY_NAME('B').AddEdge(&NODE_BY_NAME('D'), 1);
    //NODE_BY_NAME('B').AddEdge(&NODE_BY_NAME('E'), 7);
    //
    //NODE_BY_NAME('C').AddEdge(&NODE_BY_NAME('B'), 4);

    //NODE_BY_NAME('D').AddEdge(&NODE_BY_NAME('C'), -5);
    //NODE_BY_NAME('D').AddEdge(&NODE_BY_NAME('A'), 2);
    //NODE_BY_NAME('E').AddEdge(&NODE_BY_NAME('D'), 6);

    //NODE_BY_NAME('A').AddEdge(&NODE_BY_NAME('B'), 8);
    ////NODE_BY_NAME('A').AddEdge(&NODE_BY_NAME('C'), 6);
    //NODE_BY_NAME('A').AddEdge(&NODE_BY_NAME('D'), 1);


    //NODE_BY_NAME('B').AddEdge(&NODE_BY_NAME('C'), 1);
    ////NODE_BY_NAME('B').AddEdge(&NODE_BY_NAME('D'), 4);

    //NODE_BY_NAME('C').AddEdge(&NODE_BY_NAME('A'), 4);
    //NODE_BY_NAME('D').AddEdge(&NODE_BY_NAME('B'), 2);
    //NODE_BY_NAME('D').AddEdge(&NODE_BY_NAME('C'), 9);

                             //row      //col
    int d0Matrix[NUM_NODES+1][NUM_NODES][NUM_NODES];
    for (int i = 0; i < NUM_NODES; i++) {
        Node_t* src_node = &nodes[i];
        for (int j = 0; j < NUM_NODES; j++) {
            if (j == i) {
                d0Matrix[0][i][j] = 0;
            }
            else {
                d0Matrix[0][i][j] = INF;
            }
        }
        for (Edge_t t : src_node->list_EdgesOut) {
            d0Matrix[0][i][t.destNode->name - 'A'] = t.weight;
        }
    }
    string name = "D0";
    printMatrix(name, &d0Matrix[0], NUM_NODES, NUM_NODES);
    for (int k = 1; k <= NUM_NODES; k++) {
        for (int i = 0; i < NUM_NODES; i++) {
            for (int j = 0; j < NUM_NODES; j++) {
                if (i == j) {   //diagonals
                    d0Matrix[k][i][j] = 0;
                }
                else if ((j == (k - 1)) || (i == (k - 1))) {    //current row and column
                    d0Matrix[k][i][j] = d0Matrix[k - 1][i][j];
                }
                else {  // i != j fo sho
                    //Dk[i,j] = D(k-1)[i,j], D(k-1)[i,k] + D(k-1)[k,j]
                    int a = d0Matrix[k - 1][i][j];
                    int b = d0Matrix[k - 1][i][k-1];
                    int c = d0Matrix[k - 1][k-1][j];
 

                    //represent INF as '+'
                    if (a == INF) {   //is a INF?
                        if ((b != INF) && (c != INF)) {
                            d0Matrix[k][i][j] = b + c;

                        }
                        else {
                            d0Matrix[k][i][j] = INF;
                        }
                    }
                    else if ((b == INF) || (c == INF)) { //b or c component is inf
                        d0Matrix[k][i][j] = a;
                    }
                    else {
                        d0Matrix[k][i][j] = MIN(a, b+c);
                    }
               }
            }
        }
        name[1] = k + '0';
        printMatrix(name, &d0Matrix[k], NUM_NODES, NUM_NODES);
    }
}


void printMatrix(string& str, int matrix[][NUM_NODES][NUM_NODES] , int numRows, int numCols) {
    cout << endl;
    for (int a = 0; a < NUM_NODES; a++) {
        if (a == 0) {
            cout << "[" << " " << str.c_str() << endl;
        }
        for (int b = 0; b < NUM_NODES; b++) {
            if (b == 0) {
                cout << "  [";
            }
            int c = matrix[0][a][b];

            if (c == INF) {
                cout << setw(3) << "INF";
            }
            else {
                cout << setw(3) << c;
            }

            if (b != NUM_NODES - 1) {
                cout << ",";
            }
            else {
                cout << "]" << endl;
            }
        }
        if (a == NUM_NODES - 1) {
            cout << "]" << endl;
        }
    }
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
