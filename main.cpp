// Filename:	main.cpp
// Author:	EVGENI C. DOBRANOV
// Date:	4.19.2016

#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <string>
#include <set>
#include <vector>
#include <climits>

#include "GraphMatrix.h"

using namespace std;

void readfiles 		(int argc, char *argv[], GraphMatrix &incoming, GraphMatrix &outgoing);
void store 		(char* filename, GraphMatrix &matrix, int filenum);
void readcommands 	(istream &input, GraphMatrix &incoming, GraphMatrix &outgoing);

void list		(GraphMatrix &incoming, GraphMatrix &outgoing, string command);

void outofvertex 	(GraphMatrix &outgoing, GraphMatrix &incoming, string vertex);
void intovertex 	(GraphMatrix &outgoing, GraphMatrix &incoming, string vertex);

void receiveedge 	(GraphMatrix &incoming, GraphMatrix &outgoing, string edge);
void emitedge 		(GraphMatrix &incoming, GraphMatrix &outgoing, string edge);

void paths 		(GraphMatrix &incoming,	GraphMatrix &outgoing,
			string current,		string goal,
			string path, 		vector<string> &v,
			string command);
		
void outgoing_search 	(GraphMatrix &incoming,	GraphMatrix &outgoing,
			string current,		string goal,
			string path, 		vector<string> &v);
		
void incoming_search 	(GraphMatrix &incoming,	GraphMatrix &outgoing,
			string goal,		string path,
 			vector<string> &v,	string edge);
			
int count_arrows	(string path);

// ****************************************************************************
// * 				  MAIN FUNCTION 			      *
// ****************************************************************************
int main (int argc, char *argv[])
{
	GraphMatrix incoming;		// Matrix for vertices that receive edges
	GraphMatrix outgoing;		// Matrix for vertices that emit edges
	
	readfiles(argc, argv, incoming, outgoing);	// Read data into the matrices
	readcommands(cin, incoming, outgoing);		// Interpret the commands
	
	return 0;				// Return 0 as per convention
}

// ****************************************************************************
// * 			       INPUT FUNCTIONS 			      	      *
// ****************************************************************************

// Purpose: Makes sure there are 3 arguments and calls the store function
void readfiles (int argc, char *argv[], GraphMatrix &incoming, GraphMatrix &outgoing)
{
	assert(argc >= 3);	     	// Executable file, VertexIn, VertexOut
	store(argv[1], incoming, 1);	// Store the first file in "incoming" matrix
	store(argv[2], outgoing, 2);	// Store the second file in "outgoing" matrix
}

// Purpose: Stores all of the entries in the inputted file in the matrix
void store (char* filename, GraphMatrix &matrix, int filenum)
{
	ifstream infile;		// Declare an ifstream
	string vertex;			// Title of vertex
	
	infile.open(filename);		// Open the file

	// If file can't be opened, produce an error, otherwise open
	if (infile.fail()) {
		cerr << "Cannot open " << filename << endl;
		exit(EXIT_FAILURE);
	}
	else {
		char delimiter;
		cout << "Enter the delimiting character for file " << filenum << ": ";
		cin >> delimiter;
		cout << endl;
		
		string edge;
		
		// Read each line and delimit the vertex from the edge
		while (getline(infile, vertex, delimiter))
		{
			getline(infile, edge);
			edge.erase(edge.find_last_not_of("\n\r") + 1);
			
			matrix.add(vertex, edge);
		}
	}

	infile.close();		// Close the file when done
}

// Purpose: Reads in commands and calls corresponding functions
void readcommands (istream &input, GraphMatrix &incoming, GraphMatrix &outgoing)
{
	string command;			// Store the current command
	
	while(input >> command)		// Read in a command from input
	{
		if (command == "lv" || command == "le")
			list (incoming, outgoing, command);
		
		else if (command == "intovertex") {
			input >> command;
			intovertex (outgoing, incoming, command);
		}
		
		else if (command == "outofvertex") {
			input >> command;
			outofvertex (outgoing, incoming, command);
		}
		
		else if (command == "receiveedge") {
			input >> command;
			receiveedge (outgoing, incoming, command);
		}
		
		else if (command == "emitedge") {
			input >> command;
			emitedge (outgoing, incoming, command);
		}
		
		// For either all of the paths or the shortest path
		else if (command == "paths" || command == "shortestpath") {
			string current, goal;
			input >> current;	      // Starting vertex
			input >> goal;		      // End vertex
			vector<string> v;	      // Vector to hold paths
			paths (incoming, outgoing, current, goal, "", v, command);
		}
		
		else if (command == "q" || command == "quit" || command == "exit")
			break;
	}
}

// ****************************************************************************
// * 			         LIST FUNCTIONS 			      *
// ****************************************************************************

// Purpose: Lists all vertices or edges found in both matrices
void list (GraphMatrix &incoming, GraphMatrix &outgoing, string command)
{
	set<string> s;		// Set of vertices (to avoid duplicates)
	string *receive;	// Pointer to array of receiving vertices
	string *emit;		// Pointer to array of emitting vertices
	
	int incomingmax = command == "lv" ? incoming.getheight() : incoming.getwidth();
	int outgoingmax = command == "lv" ? outgoing.getheight() : outgoing.getwidth();
	
	if (command == "lv") {
		receive = incoming.lv();
		emit = outgoing.lv();
	}
	else {
		receive = incoming.le();
		emit = outgoing.le();
	}

	for (int i = 0; i < incomingmax; ++i)	// Insert receiving vertices into set
		s.insert(receive[i]);

	for (int i = 0; i < outgoingmax; ++i)	// Insert emitting vertices into set
		s.insert(emit[i]);

	delete[] receive;
	delete[] emit;
	
	while (!s.empty())
	{
		cout << *s.begin() << endl;	// Empty the set while printing it out
		s.erase(s.begin());
	}
}

// Purpose: Lists the edges going into a desired vertex
void intovertex (GraphMatrix &outgoing, GraphMatrix &incoming, string vertex)
{
	if (outgoing.exists(vertex, "") or incoming.exists(vertex, ""))
		incoming.rowmajor(vertex);
	else
		cout << "Vertex not found" << endl;
}

// Purpose: Lists the edges coming out of a desired vertex
void outofvertex (GraphMatrix &outgoing, GraphMatrix &incoming, string vertex)
{
	if (outgoing.exists(vertex, "") or incoming.exists(vertex, ""))
		outgoing.rowmajor(vertex);
	else
		cout << "Vertex not found" << endl;
}

// Purpose: Lists the vertices that receive a desired edge
void receiveedge (GraphMatrix &outgoing, GraphMatrix &incoming, string edge)
{
	if (incoming.exists("", edge) or outgoing.exists("", edge))
		incoming.colmajor(edge);
	else
		cout << "Edge not found" << endl;
}

// Purpose: Lists the vertices that emit a desired edge
void emitedge (GraphMatrix &outgoing, GraphMatrix &incoming, string edge)
{
	if (incoming.exists("", edge) or outgoing.exists("", edge))
		outgoing.colmajor(edge);
	else
		cout << "Edge not found" << endl;
}

// ****************************************************************************
// * 			         PATH FUNCTIONS 			      *
// ****************************************************************************

// Purpose: Prints out either all paths, or shortest path b/w 2 vertices
void paths (	GraphMatrix &incoming, 	GraphMatrix &outgoing,
		string current, 	string goal,
		string path, 		vector<string> &v,
		string command)
{
	// If both vertices are defined, call outgoing_search()
	if ((outgoing.exists(current, "") or incoming.exists(current, "")) and
	    (outgoing.exists(goal, "") 	  or incoming.exists(goal, "")))
		outgoing_search(incoming, outgoing, current, goal, path, v);
		
	else {
		cout << "Vertex not found" << endl;
		return;
	}
	
	// If all paths are desired, loop through and print out each one
	if (command == "paths")
	{
		for (int i = 0; i < (int) v.size(); ++i)
			cout << v[i] << endl;
	}
	
	// Otherwise find only the shortest one (by counting the arrows)
	else {
		string shortest = "";
		int min_arrows = INT_MAX;
		
		for (int i = 0; i < (int) v.size(); ++i)
		{
			if (count_arrows(v[i]) <= min_arrows)
			{
				min_arrows = count_arrows(v[i]);
				shortest = v[i];
			}
		}
		
		if (shortest != "") cout << shortest << endl;
	}
}

// Purpose: Searches through outgoing matrix for possible leads into the incoming matrix
void outgoing_search (	GraphMatrix &incoming, 	GraphMatrix &outgoing,
			string current,		string goal,
			string path, 		vector<string> &v)
{
	// Loop through the emitting vertices
	for (int row = 1; row <= outgoing.getheight(); ++row)
	{
		// If desired vertex is found...
		if (outgoing(row, 0) == current)
		{
			// Loop through the edges for said vertex
			for (int col = 1; col <= outgoing.getwidth(); ++col)
			{
				// If the vertex emits an edge...
				if (outgoing(row, col) == "T")
				{
					// Mark path read
					outgoing(row, col) = "F";
					
					// Go searching through the other matrix
					incoming_search (incoming, outgoing, goal,
							path + outgoing(row, 0) + " (" + outgoing(0, col) + ") -> ",
							v, outgoing(0, col) );
					
					// Unmark path when above function returns (in case it's used again)
					outgoing(row,col) = "T";
				}
			}
			
			break;	// Break early since vertex can't exist elsewhere
		}
	}
}

// Purpose: Searches through incoming matrix for possible terminations of search
void incoming_search (	GraphMatrix &incoming, GraphMatrix &outgoing,
			string goal,		 string path,
 			vector<string> &v,	 string edge)
{
	// Loop through the receiving vertices' edges
	for (int col = 1; col <= incoming.getwidth(); ++col)
	{
		// If the edge is found...
		if (incoming(0, col) == edge)
		{
			// Loop through the vertices that receive said edge
			for (int row = 1; row <= incoming.getheight(); ++row)
			{
				// If the vertex receives the edge...
				if (incoming(row, col) == "T")
				{
					// Mark path read
					incoming(row, col) = "F";
					
					// If end vertex is reached, store the path and return
					if (incoming(row, 0) == goal)
					{
						incoming(row, col) = "T";
						path += goal;
						v.push_back(path);
						return;
					}
					
					// Otherwise search through the other matrix
					outgoing_search (incoming, outgoing, incoming(row, 0), goal, path, v);
					
					// Unmark path when above function returns (in case it's used again)
					incoming(row,col) = "T";
				}
			}
			
			break;	// Break early since edge can't exist elsewhere
		}
	}
}

// Purpose: Counts the number of arrows in a string (i.e. - the length of the path)
int count_arrows(string path)
{
	int count = 0;
	
	for (int i = 0; i < (int) path.length() - 1; i++)
	{
		// If two subsequent characters are an arrow -> ... count it
		if (path.substr(i, 2) == "->")
			count++;
	}
	
	return count;	// Return the number of arrows
}