# C++ Directed Graph Program

## Author

Evgeni Dobranov

## Description

This application takes in two input files and builds a directed graph out of the data there. There are certain commands that can be run on the imported data:

* paths <vertex a> <vertex b>		-> List all paths between two vertices
* shortestpath <vertex a> <vertex b>	-> List shortest path between two vertices
* lv					-> List all vertices
* le					-> List all edges
* intovertex <vertex>			-> List all edges directed towards a vertex
* outofvertex <vertex>			-> List all edges directed out of a vertex
* receiveedge <edge>			-> List all vertices receiving an edge
* emitedge <edge>			-> List all vertices emitting an edge
* q / quit / exit			-> Exit the program (^C as an alternative)

## Data Input & Running The Program

This application relies on two input files being in a specific format. Both files have the following format for each line: <vertex><delimiter><edge>

The first file contains all vertices that have edges directed towards them, and the second file contains all vertices that have edges directed out of them. Example:

If we wanted to represent the following graph:
![alt text](https:// "Example graph")

We would have two corresponding files like so:

VertexIn.txt
```
A:2
B:1
B:4
C:3
D:8
D:7
E:6
F:5
```

VertexOut.txt
```
A:1
A:3
A:8
B:2
C:4
C:5
E:7
F:6
```

The executable file `a.out` can be run directly from the Linux command line, or the program can be recompiled by using the included Makefile and typing `make` at the command line (note: this requires you to have the clang++ compiler installed). Of course, if you have other methods of compiling C++ programs - go for it!

In order to import the data correctly, the program should be executed like so on the command line: `me@linux:~$ ./a.out VertexIn.txt VertexOut.txt` and you can freely enter commands in after that.

One variation on the execution could be `me@linux:~$ ./a.out VertexIn.txt VertexOut.txt < Commands.txt > Output.txt` in order to execute all commands in `Commands.txt` and redirect the resulting terminal output to `Output.txt`. This can be useful for executing more commands on a larger data set.

NOTE: The program will ask for the delimiter in each file, so if you use `Commands.txt` make sure that the first two lines are the delimiters for each respective file (one character on each line).

## Inner Workings & Data Structure Design

#### Core Files

* GraphMatrix.h	- Contains the variables and function declarations required for a GraphMatrix object.
			  
* **GraphMatrix.cpp** - Defines the constructor, destructor and other functions for a GraphMatrix object (at heart, it's a 2D matrix).

* **main.cpp** - Contains main(), reads the input files and commands, and is responsible for the communication between the two matrices (explained below).

* **Makefile** - Contains the compiling commands.

#### Data Structure

The primary data structure is a GraphMatrix, which looks like this:

|     | 1 | 2 | 3 | 4 | ... |
|:---:|:-:|:-:|:-:|:-:|:---:|
|  A  |   | T |   |   |     |
|  B  | T |   |   | T |     |
|  C  |   |   | T |   |     |
|  D  |   |   |   |   |     |
| ... |   |   |   |   |     |

If this was VertexIn.txt, then it signifies that vertex A receives edge 2, vertex B receives edges 1 and 4, etc.

The main() function contains one matrix for each file, and ultimately communicates the two in order to retrieve paths. In order to search with a path, it looks at the data from VertexOut.txt (in row-major) and tries to find the edges being emitted from the first vertex. It will eventually span all of those edges, but when it finds just one, it starts to look at the data from VertexIn.txt (in column-major) and searches for vertices that might receive that edge.

The program "bounces" back and forth between the two matrices in a recursive fashion, marking paths that were already searched with an "F" instead of a "T". This prevents infinite loops.

The method for finding the shortest path is as simple as finding the path with the fewest number of connections.