// Filename:	GraphMatrix.cpp
// Author:	EVGENI C. DOBRANOV
// Date:	4.19.2016

#include "GraphMatrix.h"
#include <cassert>

// Purpose: Constructor for a GraphMatrix
GraphMatrix::GraphMatrix()
{
	width = 0;				// Start with no edges
	height = 0;				// Start with no vertices
	
	widthcap = INITIAL_SIZE;		// Initial edge cap
	heightcap = INITIAL_SIZE;		// Initial vertex cap
	
	matrix = new string*[INITIAL_SIZE];	// Create new "vertical" row
	
	// For every entry, create the columns (how 2D matrices are done in C++)
	for (int i = 0; i < INITIAL_SIZE; ++i)
		matrix[i] = new string[INITIAL_SIZE];
	
	for (int i = 0; i < INITIAL_SIZE; ++i)	// Populate all entries with blanks
	{
		for (int j = 0; j < INITIAL_SIZE; ++j)
			matrix[i][j] = "";
	}
}

// Purpose: Destructor for a GraphMatrix
GraphMatrix::~GraphMatrix()
{
	for (int i = 0; i < heightcap; ++i)
		delete[] matrix[i];
	delete[] matrix;
}

// Purpose: Operator overload for () -> Returns the matrix value at indices
string &GraphMatrix::operator()(int a, int b) const
{
	assert(a >= 0 && a <= height && b >= 0 && b <= width);
	return matrix[a][b];
}

// Purpose: Adds new entry for vertex / edge, and adds a "T" in that position
void GraphMatrix::add(string vertex, string edge)
{
	if (height + 1 == heightcap)	// Expand the height if necessary
		expandheight();
		
	if (width + 1 == widthcap)	// Expand the width if necessary
		expandwidth();
	
	// Go through rows until the vertex or a blank space is found
	int row = 1;
	while (matrix[row][0] != "" and matrix[row][0] != vertex)
		row++;
	
	// If exit condition was because of a blank, add the vertex to the matrix
	if (matrix[row][0] == "")
	{
		matrix[row][0] = vertex;
		height++;
	}

	// Go through cols until the edge or a blank space is found
	int col = 1;
	while (matrix[0][col] != "" and matrix[0][col] != edge)
		col++;
	
	// If exit condition was because of a blank, add the edge to the matrix
	if (matrix[0][col] == "")
	{
		matrix[0][col] = edge;
		width++;
	}
	
	matrix[row][col] = "T";		// Mark the intersection with a "T"
}

// Purpose: Expand the height (capacity for the number of vertices)
void GraphMatrix::expandheight()
{
	string **temp = new string*[heightcap * 2 + 2];	// New 2D matrix
	
	// Go through all of the rows of the original matrix
	for (int row = 0; row < heightcap * 2 + 2; ++row)
	{
		// Create all of the new columns
		temp[row] = new string[widthcap];
		
		if (row <= height)	// Add all original "T" values
		{
			for (int col = 0; col < widthcap; ++col)
				temp[row][col] = matrix[row][col];
		}
		
		// Add blanks to the remainder
		else {
			for (int col = 0; col < widthcap; ++col)
				temp[row][col] = "";
		}
	}
	
	// Delete original values and reassign values
	for (int row = 0; row < heightcap; ++row)
		delete[] matrix[row];
	delete[] matrix;
	
	matrix = temp;
	heightcap = heightcap * 2 + 2;
}

// Purpose: Expand the width (capacity for the number of edges)
void GraphMatrix::expandwidth()
{
	// Go through all of the rows of the original matrix
	for (int row = 0; row < heightcap; ++row)
	{
		// Create all of the new columns
		string *temp = new string[widthcap * 2 + 2];
		
		// Copy all column values over
		for (int col = 0; col < widthcap * 2 + 2; ++col)
		{
			if (col <= width)
				temp[col] = matrix[row][col];
			else
				temp[col] = "";
		}
		
		delete[] matrix[row];	// Delete current row
		matrix[row] = temp;	// Update to new row
	}
	
	widthcap = widthcap * 2 + 2;	// Update width cap
}

// Purpose: List all of the vertices
string* GraphMatrix::lv()
{
	string *vertexs = new string[height + 1];
	
	for (int i = 1; i <= height; ++i)
		vertexs[i-1] = matrix[i][0];
	
	return vertexs;
}

// Purpose: List all of the edges
string* GraphMatrix::le()
{
	string *edges = new string[width + 1];
	
	for (int i = 1; i <= width; ++i)
		edges[i-1] = matrix[0][i];
	
	return edges;
}

// Purpose: Search through the rows of the matrix (i.e. - vertices)
void GraphMatrix::rowmajor(string vertex)
{
	int row = 1;
	while (matrix[row][0] != "" && matrix[row][0] != vertex && row <= height)
		row++;
	
	if (matrix[row][0] == vertex)
	{
		for (int col = 1; col <= width; ++col)
		{
			if (matrix[row][col] == "T")
				cout << matrix[0][col] << endl;
		}
	}
}

// Purpose: Search through the columns of the matrix (i.e. - edges)
void GraphMatrix::colmajor(string edge)
{
	int col = 1;
	while (matrix[0][col] != "" && matrix[0][col] != edge && col <= width)
		col++;
	
	if (matrix[0][col] == edge)
	{
		for (int row = 1; row <= height; ++row)
		{
			if (matrix[row][col] == "T")
				cout << matrix[row][0] << endl;
		}
	}
}

// Purpose: Searches through the rows and attempts to find a vertex or edge
bool GraphMatrix::exists(string vertex, string edge)
{
	// If vertex isn't blank, search for the vertex
	if (vertex != "") {
		for (int row = 1; row <= height; ++row) {
			if (matrix[row][0] == vertex)
				return true;
		}
	}

	else if (edge != "") {
		for (int col = 1; col <= width; ++col) {
			if (matrix[0][col] == edge)
				return true;
		}
	}
	
	return false;
}